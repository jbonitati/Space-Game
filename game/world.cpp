#include <iostream>
#include "world.h"
#include "frameFactory.h"

World::World(const std::string& name, int fact) :
  io( IOManager::getInstance() ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  factor(fact),
    frameWidth( frame->getWidth() ),
    worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
    viewX(0.0), viewY(0.0),
    view(Viewport::getInstance()),
    distance(0)
{}

void World::update(Uint32 ticks) {
  distance = (int)(distance + factor*static_cast<float>(ticks)*.001) % frameWidth;
  viewX = static_cast<int>(view.X() + distance) % frameWidth;
  viewY = view.Y();
}

void World::draw() const {
  frame->draw(viewX, viewY, 0, 0);
  frame->draw(0, viewY, frameWidth-viewX, 0);
}

/*void Frame::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  SDL_Rect src = { sx, sy, width, height };
  SDL_Rect dest = {dx, dy, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}*/
