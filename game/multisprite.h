#include <string>
#include <vector>
#include "drawable.h"
#pragma once

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const std::string& n, const Vector2f& pos, const Vector2f& vel);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite() { }
  MultiSprite & operator=(const MultiSprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }
  virtual std::vector<Frame *> getFrames() const{return frames;}
  virtual bool collidedWith(const Drawable*) const ;

  int getFrameWidth(){return frameWidth;}
  int getFrameHeight(){return frameHeight;}
  bool execute(const Drawable*) const;
  bool isPixelVisible(Uint32 pixel, SDL_Surface *surface) const;
  bool isOutofBounds()const{
    return (Y() < 0||Y() > worldHeight-frameHeight||X() < 0||X() > worldWidth-frameWidth);
  }

protected:
  std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  virtual void advanceFrame(Uint32 ticks);
};
