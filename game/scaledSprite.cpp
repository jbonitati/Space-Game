#include <iostream>
#include <cmath>
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"

ScaledSprite::ScaledSprite(const std::string& name,
                           SDL_Surface* surface, float s) :
  Drawable(name,
           Vector2f(rand()%Gamedata::getInstance().getXmlInt("world/width"),
                    rand()%Gamedata::getInstance().getXmlInt("world/height")/1.2),
           Vector2f(-1*Gamedata::getInstance().getXmlInt("painters/maxSpeed") * s,0)
  ),
  scale(s),
  scaledSurface( rotozoomSurface(surface, 0, scale, SMOOTHING_ON) ),
  frame( new Frame(scaledSurface)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()),
  scale(s.scale),
  scaledSurface(s.scaledSurface),
  frame(s.frame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  scale = rhs.scale;
  scaledSurface = rhs.scaledSurface;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

ScaledSprite::~ScaledSprite() {
  SDL_FreeSurface( scaledSurface );
  delete frame;
}

void ScaledSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

unsigned ScaledSprite::getPixel(Uint32 i, Uint32 j) const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frame->getSurface()->pixels);
  return pixels[ ( y * frame->getWidth() ) + x ];
}


void ScaledSprite::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() + frameHeight < 0) {
    Y(worldHeight);
  }
  if ( Y()  > worldHeight) {
    Y(0 - frameHeight);
  }

  if ( X() + frameWidth< 0) {
    X(worldWidth);
  }
  if ( X() > worldWidth) {
    X(0 - frameWidth);
  }
}
