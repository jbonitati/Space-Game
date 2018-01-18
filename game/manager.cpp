#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "manager.h"
#include "time.h"
#include "frameFactory.h"
#include "scaledSprite.h"
#include "projectileManager.h"

Manager::~Manager() {
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}

Manager::Manager(int num) :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor")),
  world3("back3", Gamedata::getInstance().getXmlInt("back3/factor")),
  viewport( Viewport::getInstance() ),
  hud( Hud::getInstance() ),
  objhud( ObjectHud::getInstance()),
  sprites(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  numPlayers(num),
  sound()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
	srand(time(NULL));
	for(int i = 0; i < Gamedata::getInstance().getXmlInt("painters/num"); i++){
		float scale = (Gamedata::getInstance().getXmlInt("painters/minScale") + rand()%Gamedata::getInstance().getXmlInt("painters/maxScale"))*0.1;
    Frame *f = FrameFactory::getInstance().getFrames(Gamedata::getInstance().getXmlStr("painters/object"))[0];
		sprites.push_back(new ScaledSprite(Gamedata::getInstance().getXmlStr("painters/object"),
			f->getSurface(), scale));
	}
  	viewport.setObjectToTrack(ProjectileManager::getInstance().getPlayer(currentSprite));
}

void Manager::draw() const {
  world.draw();
  world2.draw();
  world3.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  //io.printMessageValueAt("Seconds: ", clock.getSeconds(), 10, 20);
  //io.printMessageAt("Press T to switch sprites", 10, 90);
  //io.printMessageValueAt("Sun x: ",TwoWayMultiSprite::getTrackedX(), 10, 110);
  if(ProjectileManager::getInstance().win(currentSprite) ||
    (Gamedata::getInstance().getXmlInt("numPlayers") > 1 && ProjectileManager::getInstance().win(1)))
    io.printMessageCenteredAt("Congratulations! You are the best Planet! (r to restart)",180);
  if(ProjectileManager::getInstance().lose(currentSprite))io.printMessageCenteredAt("Player 1 Game Over! (r to restart)", 200);
  if(Gamedata::getInstance().getXmlInt("numPlayers") > 1 && ProjectileManager::getInstance().lose(1))io.printMessageCenteredAt("Player 2 Game Over! (r to restart)", 220);
  io.printMessageValueAt("Score: ",ProjectileManager::getInstance().getPlayer(currentSprite)->getScore()
    + Gamedata::getInstance().getXmlInt("numPlayers") > 1 ? ProjectileManager::getInstance().getPlayer(1)->getScore() : 0, 400, 10);
  io.printMessageAt(title, 10, 450);
  ProjectileManager::getInstance().drawAll();
  hud.draw(clock.getFrameRate(), clock.getSeconds(), ProjectileManager::getInstance().getPlayer(currentSprite)->getPlayerName());
  objhud.draw();
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.'
       << std::setfill('0') << std::setw(4)
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  //Player *temp = ProjectileManager::getInstance().getPlayer(currentSprite);
  currentSprite = (currentSprite + 1)%(ProjectileManager::getInstance().numPlayers());
  //characters[currentSprite]->setPosition(temp->getPosition());
  //characters[currentSprite]->setVelocity(temp->getVelocity());
  viewport.setObjectToTrack(ProjectileManager::getInstance().getPlayer(currentSprite));
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( (int)(clock.getSeconds() - lastSeconds) >= Gamedata::getInstance().getXmlInt("spawnRate") ) {
    ProjectileManager::getInstance().spawnProjectile();
    lastSeconds = clock.getSeconds();
  }

  if ( (int)(clock.getSeconds()) > 0 && (int)(clock.getSeconds()) % 307 == 0) {
    sound.startMusic();
  }

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  ProjectileManager::getInstance().updateAll(ticks, sound);
  world.update(ticks);
  world2.update(ticks);
  world3.update(ticks);
  hud.update(clock.getSeconds());
  objhud.update();
  viewport.update(); // always update viewport last
}

void Manager::restart(){
   ProjectileManager::getInstance().wipe();
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  sound.startMusic();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
        Player *p = ProjectileManager::getInstance().getPlayer(currentSprite);
        Player *p2 = Gamedata::getInstance().getXmlInt("numPlayers") > 1 ? ProjectileManager::getInstance().getPlayer(1) : NULL;
        Uint8 *keystate = SDL_GetKeyState(NULL);
        if (event.type ==  SDL_QUIT) { done = true; break; }
        if(event.type == SDL_KEYDOWN) {
          if(keystate[SDLK_r]){
            restart();
            //switchSprite();
          }
          if(keystate[SDLK_t]){
            //restart();
            //switchSprite();
          }
          if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
            done = true;
            break;
          }
          if ( keystate[SDLK_SPACE] ) {
            ProjectileManager::getInstance().shoot(currentSprite);
          }
          if ( keystate[SDLK_RETURN] ) {
            if(p2)ProjectileManager::getInstance().shoot(1);
          }
        	if ( keystate[SDLK_w]){
        		p->moveUp();
        	}
        	if ( keystate[SDLK_s]){
        		p->moveDown();
        	}
        	if ( keystate[SDLK_a]){
        		p->moveLeft();
        	}
        	if ( keystate[SDLK_d]){
        		p->moveRight();
        	}
          if ( keystate[SDLK_UP]){
        		if(p2)p2->moveUp();
            else p->moveUp();
        	}
        	if ( keystate[SDLK_DOWN]){
        		if(p2)p2->moveDown();
            else p->moveDown();
        	}
        	if ( keystate[SDLK_LEFT]){
        		if(p2)p2->moveLeft();
            else p->moveLeft();
        	}
        	if ( keystate[SDLK_RIGHT]){
        		if(p2)p2->moveRight();
            else p->moveRight();
        	}
          if ( keystate[SDLK_p] ) {
              if ( clock.isPaused() ) clock.unpause();
              else clock.pause();
          }
        	if ( keystate[SDLK_F1]){
        		Hud::getInstance().setVisible(!hud.isVisible());
        	}
        	if ( keystate[SDLK_F2]){
        		ObjectHud::getInstance().setVisible(!objhud.isVisible());
        	}
                if (keystate[SDLK_F4] && !makeVideo) {
                  std::cout << "Making video frames" << std::endl;
                  makeVideo = true;
                }
      }

      if(event.type == SDL_KEYUP){

      	if ( ! (keystate[SDLK_w] ||  keystate[SDLK_s])){
      		p->stopVertical();
      	}
      	if ( ! (keystate[SDLK_a]||keystate[SDLK_d])){
      		p->stopHorizontal();
      	}if ( ! (keystate[SDLK_UP] ||  keystate[SDLK_DOWN])){
      		if(p2)p2->stopVertical();
      	}
      	if ( ! (keystate[SDLK_LEFT]||keystate[SDLK_RIGHT])){
      		if(p2)p2->stopHorizontal();
      	}
      }
    }
      draw();
      update();
  }
}
