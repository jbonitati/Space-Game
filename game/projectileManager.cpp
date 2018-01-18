#include "projectileManager.h"
#include "vector2f.h"
#include "gamedata.h"
#include <list>
#include <cmath>
#include "ioManager.h"
#include "sound.h"

ProjectileManager::~ProjectileManager()
{
  std::cout << "Deleting ProjectileManager" << std::endl;
  for(std::list<Projectile *>::iterator it = freeList.begin(); it!= freeList.end(); it++){
    delete *it;
  }
  for(std::list<Projectile *>::iterator it = activeList.begin(); it!= activeList.end(); it++){
    delete *it;
  }
  for(std::list<Sprite *>::iterator it = freeExplosions.begin(); it!= freeExplosions.end(); it++){
    delete *it;
  }
  for(std::list<Sprite *>::iterator it = activeExplosions.begin(); it!= activeExplosions.end(); it++){
    delete *it;
  }
  characters.clear();
  characterList.clear();
}

void ProjectileManager::wipe(){
  freeList.splice(freeList.end(), activeList);
  activeList.clear();
  freeExplosions.splice(freeExplosions.end(), activeExplosions);
  activeExplosions.clear();
  characters.clear();
  addcharacters();
  resetPlayers();
  for(int i = 0; i < (int)(characters.size()); i++)
    characters[i].clearProjs();
}

ProjectileManager& ProjectileManager::getInstance() {
  static ProjectileManager factory;
  return factory;
}

void ProjectileManager::updateAll(Uint32 ticks, SDLSound &sound){
  std::list<Projectile *>::iterator tit;
  for(std::list<Projectile *>::iterator it = activeList.begin();
    it != activeList.end(); ++it){
    for(unsigned int n = 0; n < characters.size(); n++){
      Player *p = &characters[n];
        Projectile *t = (*it);
        //check if players are hit
        if(p->collidedWith(t)){
          if(t->isDangerous() && !(Gamedata::getInstance().getXmlInt("godMode") == 17 && n < (twoPlayers ? 2 : 1))){
              p->takeDamage(t->getDamage());
              if(p->getHealth() == p->getMaxHealth()){
                while((int)(p->getProjs().size()) > 0){
                  p->getProjs()[0]->shoot();
                  p->shootFirst();
                }
                p->setVisible(false);
              }
              for(unsigned int temp = 0; temp < characters.size();temp++){
                if(temp != n) characters[temp].increaseScore(250);
              }
              t->setDangerous(false);
              it = explode(it);
              sound[0];

          }else if(!t->inOrbit())
            p->addProjectile(t);
          break;
        }
    }
  for(int i = 0; i < (int)(characters.size()); i++){

    //check if players hit each other
    Player *play = &characters[i];
    for(int j = i+1;j < (int)(characters.size()); j++){
      if((play->getPos() - characters[j].getPos()).magnitude() < Gamedata::getInstance().getXmlInt("AISmartness")){
        //shoot at the player
        if(!(twoPlayers && j ==1) && characters[j].getProjs().size() > 0 && !characters[j].collidedWith(characters[j].getProjs()[0])){
          characters[j].getProjs()[0]->shoot();
          characters[j].shootFirst();
        }
      }
      if(play->collidedWith(&characters[j])){
        play->playerCollide(&characters[j]);
        if(i <= (twoPlayers ? 1 : 0))
          sound[1];
      }
    }
    //update AI
    if(i > twoPlayers ? 1 : 0){
      if(activeList.size() > 0){
        std::list<Projectile *>::iterator closest = activeList.begin();

        for(std::list<Projectile *>::iterator it = ++(activeList.begin());
          it != activeList.end(); ++it){
            if((play->getPos() - (*closest)->getPos()).magnitude() > (play->getPos() - (*it)->getPos()).magnitude()){
              closest = it;
            }
        }
        if((play->getPos() - (*closest)->getPos()).magnitude() < Gamedata::getInstance().getXmlInt("AISmartness")){
          if((*closest)->isDangerous()){
            play->moveAway((*closest)->getPos());
          }
          else if(!(*closest)->inOrbit()){
            play->moveTowards((*closest)->getPos());
          }
        }else{
          if(play->getVelocity().magnitude() <=10) play->setVelocity(
            Vector2f(rand()%play->getmaxSpeed() - play->getmaxSpeed()/2, rand()%play->getmaxSpeed()- play->getmaxSpeed()/2));
          else play->moveTowards(play->getPos() + play->getVelocity());
        }
      }else{
        if(play->getVelocity().magnitude() <=10) play->setVelocity(
          Vector2f(rand()%play->getmaxSpeed()- play->getmaxSpeed()/2, rand()%play->getmaxSpeed()- play->getmaxSpeed()/2));
        else play->moveTowards(play->getPos() + play->getVelocity());
      }
    }

    //update orbits
    int rad = (int)(hypot( play->getFrameWidth(), play->getFrameHeight()));
    for(int ject = 0; ject < (int)(characters[i].getProjs().size()); ject++){
          if(ject == 0) play->getProjs()[ject]->setFrame(Projectile::ACTIVE);
          play->getProjs()[ject]->orbit(play->getPos()-Vector2f(play->getFrameWidth(), play->getFrameHeight()), rad);
    }
  }

  //update projectiles and characters
  for(std::list<Projectile *>::iterator it = activeList.begin();
    it != activeList.end(); ++it) (*it)->update(ticks);
  for(int i = 0; i < (int)(characters.size()); i++) characters[i].update(ticks);

  }
}

std::list<Projectile *>::iterator ProjectileManager::explode(std::list<Projectile *>::iterator it){
    (*it)->setDangerous(false);
    if((int)(freeExplosions.size()) > 0){
      Sprite *e = *freeExplosions.begin();
      e->setPosition((*it)->getPos());
      activeExplosions.push_back(e);
      freeExplosions.erase(freeExplosions.begin());
    }else{
      activeExplosions.push_back(new Sprite(Gamedata::getInstance().getXmlStr("projectiles/moon1/explosionSprite"),
        (*it)->getPos(), Vector2f(0,0)));
    }
    freeList.push_back(*it);
    return activeList.erase(it);
}

void ProjectileManager::drawAll() const{
  for(std::list<Projectile *>::const_iterator it = activeList.begin(); it != activeList.end(); ++it){
    (*it)->draw();
  }
  for(std::list<Sprite *>::const_iterator spit = activeExplosions.begin(); spit != activeExplosions.end(); ++spit){
    (*spit)->draw();
  }
  for(std::vector<Player>::const_iterator it = characters.begin(); it != characters.end(); ++it){
    (*it).draw();
  }
}

void ProjectileManager::shoot(int index){
  Player *p = &characters[index];
  if(p->getProjs().size() > 0 && p->isVisible()){
    p->getProjs()[0]->shoot();
    p->shootFirst();
  }
}

void ProjectileManager::spawnProjectile(){
  freeExplosions.splice(freeExplosions.end(), activeExplosions);
  activeExplosions.clear();
  if(freeList.size() > 0){
    Projectile *p = freeList.front();
    p->setPosition(Vector2f(rand()%Gamedata::getInstance().getXmlInt("world/width"),
      rand()%Gamedata::getInstance().getXmlInt("world/height")));
    p->reset();
    p->setVelocity(Vector2f(0,0));
    p->setDangerous(false);
    p->setFrame(Projectile::NORMAL);
    activeList.push_back(p);
    freeList.erase(freeList.begin());
  }else{
    if((int)(activeList.size()) < Gamedata::getInstance().getXmlInt("maxProjectiles"))
      activeList.push_back(new Projectile(1,Vector2f(rand()%Gamedata::getInstance().getXmlInt("world/width"),
        rand()%Gamedata::getInstance().getXmlInt("world/height")),Vector2f(0,0)));
  }
}
