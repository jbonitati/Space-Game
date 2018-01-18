#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "multisprite.h"
#include "gamedata.h"



class Projectile : public MultiSprite {
public:
  static const unsigned int NORMAL = 0;
  static const unsigned int DANGEROUS = 1;
  static const unsigned int ACTIVE = 2;

  Projectile(int index);
  Projectile(const Projectile&);
  Projectile(int index, const Vector2f& pos, const Vector2f& vel);
  virtual ~Projectile() { }

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  virtual void setFrame(unsigned int i){currentFrame = i % numberOfFrames;}

  virtual void shoot();
  virtual void orbit(Vector2f pos, int radius);
  virtual bool collidedWith(const Drawable *) const;
  bool isDangerous() const{return dangerous;}
  void setDangerous(bool d){dangerous = d;}
  int getDamage() const{
    return getVelocity().magnitude() >= Gamedata::getInstance().getXmlInt("maxSpeed") ? 2*damage : damage;
  }
  int getMass() const{return mass; }
  bool inOrbit()const{return orbiting;}
  void reset(){
    orbiting = false;
    dangerous = false;
  }

protected:
  int index;
  bool orbiting;
  bool dangerous;
  float angle;
  int mass;
  int damage;
  Vector2f orbitpos;
  int oradius;
  Projectile *operator=(const Projectile&);
};
