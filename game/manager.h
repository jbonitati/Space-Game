#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "objecthud.h"
#include "sound.h"

class Manager {
public:
  Manager (int num);
  ~Manager ();
  void play();
  void switchSprite();
  void restart();
private:
  const bool env;
  const IOManager& io;
  Clock clock;

  SDL_Surface * const screen;
  World world;
  World world2; //parallax scrolling
  World world3;
  Viewport& viewport;
  Hud& hud;
  ObjectHud& objhud;

  std::vector<Drawable*> sprites;
  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  int numPlayers;
  SDLSound sound;


  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
