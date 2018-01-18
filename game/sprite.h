#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  Sprite(const std::string&,
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Sprite(const Sprite& s);
  virtual ~Sprite() { }

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual bool collidedWith(const Drawable*) const ;
  virtual void update(Uint32 ticks);
  virtual Sprite& operator=(const Sprite &rhs);
  bool execute(const Drawable*) const;
  bool isOutofBounds()const;
  bool isPixelVisible(Uint32 pixel, SDL_Surface *surface) const;

  int getframeWidth()const {return frameWidth;  }
  int getframeHeight()const {return frameHeight;}

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
};
#endif
