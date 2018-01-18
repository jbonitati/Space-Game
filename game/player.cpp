#include "player.h"
#include <cmath>
#include <limits>
#include "gamedata.h"
#include <boost/lexical_cast.hpp>
#include "extractSurface.h"
#include "aaline.h"
#include "ioManager.h"

Player::Player(const std::string spriteName, std::string pName):
	TwoWayMultiSprite(spriteName,
			Vector2f(Gamedata::getInstance().getXmlInt(spriteName + "/startloc/x"),
			Gamedata::getInstance().getXmlInt(spriteName + "/startloc/y")),
			Vector2f(0,
			0)),
		playerName(pName), score(0),
  	maxSpeed(Gamedata::getInstance().getXmlInt("maxSpeed")),
  	acceleration(Gamedata::getInstance().getXmlInt("acceleration")),
  	friction(Gamedata::getInstance().getXmlInt("friction")),
		mass(Gamedata::getInstance().getXmlInt(std::string(spriteName + "/mass"))),
		health(Gamedata::getInstance().getXmlInt(std::string(spriteName + "/health"))),
		maxhealth(health),
		projs(),
	  chunks(),
	  freeList(),
	  surfaces(),
	  frames()
{
	//std::cout << "default" << std::endl;
}

Player::Player(const Player &p):
	TwoWayMultiSprite(p.getName()), playerName(p.playerName), score(p.score),
  	maxSpeed(p.maxSpeed),
  	acceleration(p.acceleration),
  	friction(p.friction),
		mass(p.mass),
		health(Gamedata::getInstance().getXmlInt(std::string(getName() + "/health"))),
		maxhealth(health),
		projs(p.projs),
	  chunks(),
	  freeList(),
	  surfaces(),
	  frames()
{
	setPosition(p.getPos());
	setVelocity(p.getVelocity());
	//std::cout << "copy" << std::endl;
}

Player& Player::operator=(const Player& p){
	TwoWayMultiSprite::operator=(p);
	//std::cout << "assign" << std::endl;
	playerName=(p.playerName);
	score=(p.score);
	maxSpeed=(p.maxSpeed);

	acceleration=(p.acceleration);
	friction=(p.friction);
	mass=(p.mass);
	health=(p.health);
	projs=(p.projs);
	return *this;
}

void Player::update(Uint32 ticks) {
	//check for not going over max speed
	if(chunkCount() == 0){
		if(isVisible()){
			advanceFrame(ticks);
		  Vector2f vincr = (getacceleration() - friction*getVelocity())* static_cast<float>(ticks) * 0.001;
		  setVelocity(getVelocity() + vincr);
			if(getVelocity().magnitude() >= maxSpeed)
				setVelocity(getVelocity()*((float)maxSpeed/getVelocity().magnitude()));
		  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
		  setPosition(getPosition() + incr);

		  if ( Y() < 0) {
				Y(0);
		    velocityY( abs( velocityY() ) );
		  }
		  if ( Y() > worldHeight-frameHeight) {
				Y(worldHeight-frameHeight);
		    velocityY( -abs( velocityY() ) );
		  }

		  if ( X() < 0) {
				X(0);
		    velocityX( abs( velocityX() ) );
		  }
		  if ( X() > worldWidth-frameWidth) {
				X(worldWidth-frameWidth);
		    velocityX( -abs( velocityX() ) );
		  }

		  advanceFrame(ticks);
		}
	}
	else  {
		std::list<Chunk>::iterator ptr = chunks.begin();
	  while (ptr != chunks.end()) {
	    ptr->update(ticks);
	    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
	      freeList.push_back(*ptr);
	      ptr = chunks.erase(ptr);
	    }
	    else ++ptr;
	  }
	}
}

void Player::draw() const{
	if(chunkCount() > 0){
		// Override draw; use the draw in Chunk, which derives from Sprite.
	  // So the draw we're using is actually in Sprite
	  std::list<Chunk>::const_iterator ptr = chunks.begin();
	  while (ptr != chunks.end()) {
	    ptr->draw();
	    ++ptr;
	  }
	}else{
		//draw health bar
		if(isVisible()){
			Draw_AALine(IOManager::getInstance().getScreen(),
				X() + frameWidth/2 - maxhealth/2, Y(),
				X() + frameWidth/2 - maxhealth/2 + health, Y(), 2,
				SDL_MapRGB(IOManager::getInstance().getScreen()->format,0,255,0)
			);
			Draw_AALine(IOManager::getInstance().getScreen(),
				X() + frameWidth/2 - maxhealth/2 + health, Y(),
				X() + frameWidth/2 + maxhealth/2, Y(), 2,
				SDL_MapRGB(IOManager::getInstance().getScreen()->format,255,0,0)
			);
		}
		TwoWayMultiSprite::draw();
	}
}

void Player::moveAway(Vector2f r){
	//there's danger at position r! run!
	move(getPos() - r);
}

void Player::moveTowards(Vector2f r){
	//ooh, an item! Let's pick it up!
	move(r-getPos());
}

void Player::takeDamage(int d){
	health -= d;
	if(health <= 0){
		makeChunks(Gamedata::getInstance().getXmlInt("chunkSize"));
		health = maxhealth;
	}
}

bool Player::collidedWith(const Drawable *d) const{
	if(chunkCount() > 0) return false;
	else return TwoWayMultiSprite::collidedWith(d);
}

void Player::playerCollide(Player *p){
	Vector2f direction((p->getPos() - getPos()).normalize());
	setPosition(getPos() - direction);
	p->setPosition(p->getPos() + direction);
	setVelocity(-1*getVelocity()*p->getMass() / mass);
	p->setVelocity(-1*getVelocity()*mass / p->getMass());
}

void Player::move(Vector2f direction){
	try{
		setacceleration(direction.normalize()*maxSpeed/mass);
	}catch(...){
		setacceleration(direction);
	}
}

void Player::moveUp(){
	accelerationY(-1.0*acceleration/(mass));
}
void Player::moveDown(){
	accelerationY(1.0*acceleration/(mass));
}
void Player::moveLeft(){
	accelerationX(-1.0*acceleration/(mass));
}
void Player::moveRight(){
	accelerationX(1.0*acceleration/(mass));
}
void Player::stopVertical(){
	accelerationY(0);
}
void Player::stopHorizontal(){
	accelerationX(0);
}

void Player::makeChunks(unsigned int n) {
  // Break the SDL_Surface into n*n squares; where each square
  // has width and height of frameWidth/n and frameHeight/n
  unsigned int chunk_width = std::max(1u, getFrame()->getWidth()/n);
  unsigned int chunk_height = std::max(1u, getFrame()->getHeight()/n);
  int speedx = static_cast<int>(velocityX()); // Wanna test for zero...
  int speedy = static_cast<int>(velocityY()); // Make sure it's an int.
  if (speedx == 0) speedx = 1; // Make sure it's not 0;
  if (speedy == 0) speedy = 1; // Make sure it's not 0;
  int i = 0;
  // Get the SDL_Surface so we can chunk it:
  SDL_Surface* spriteSurface = getFrame()->getSurface();
  Sint16 source_y = 0;
  while  (source_y < getFrame()->getHeight() ) {
    Sint16 source_x = 0;
    while ( source_x < getFrame()->getWidth() ) {
      // Give each chunk it's own speed/direction:
      float sx = (rand() % speedx + 40) * (rand()%2?-1:1); // 'cause %0 is
      float sy = (rand() % speedy + 40) * (rand()%2?-1:1); // float except
      SDL_Surface* surf = ExtractSurface::getInstance().
         get(spriteSurface, chunk_width, chunk_height, source_x, source_y);
      Frame* frame = new Frame(surf);
      Chunk chunk(
                Vector2f(X()+source_x,   // x coord of destination
                         Y()+source_y), // y coord of destination
                Vector2f(sx, sy),
                std::string(getName()+"chunk"+boost::lexical_cast<std::string>(i++)),
                frame);
      // chunks uses value semantics, as does frames, but there's
      // a big difference:
      chunks.push_back(chunk);
      frames.push_back(frame);
      surfaces.push_back(surf);
      source_x += chunk_width;
    }
    source_y += chunk_height;
  }
}
