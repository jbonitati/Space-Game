#include "projectile.h"
#include <cmath>
#include "gamedata.h"
#include <boost/lexical_cast.hpp>
#ifndef M_Pi
    #define M_Pi 3.14159265358979323846
#endif

Projectile::Projectile(int i)
  : MultiSprite(Gamedata::getInstance().getXmlStr("projectiles/moon" + boost::lexical_cast<std::string>(i) + "/object"),
      Vector2f(0,0), Vector2f(0,0)),
  index(i),
  orbiting(false),
  dangerous(false),
  angle(fmod(rand(), 2.0*M_Pi)),
  mass(Gamedata::getInstance().getXmlInt(
    std::string(Gamedata::getInstance().getXmlStr(
      std::string("projectiles/moon" + boost::lexical_cast<std::string>(i) + "/object")) + "/mass"))),
  damage(Gamedata::getInstance().getXmlInt(std::string("projectiles/moon" + boost::lexical_cast<std::string>(i) + "/damage"))),
  orbitpos(Vector2f(0,0)),
  oradius(1)
  {}

Projectile::Projectile(const Projectile& p) :
  MultiSprite(p.getName(), p.getPos(), p.getVelocity()),
  index(p.index), orbiting(p.orbiting), dangerous(p.dangerous),
  angle(p.angle), mass(p.mass), damage(p.damage), orbitpos(p.orbitpos), oradius(p.oradius)
{ }

Projectile::Projectile(int i, const Vector2f& pos, const Vector2f& vel)
  : MultiSprite(Gamedata::getInstance().getXmlStr("projectiles/moon" + boost::lexical_cast<std::string>(i) + "/object"), pos, vel),
    index(i),
    orbiting(),
    dangerous(false),
    angle(fmod(rand(), 2*M_Pi)),
    mass(Gamedata::getInstance().getXmlInt(
      std::string(Gamedata::getInstance().getXmlStr(
        std::string("projectiles/moon" + boost::lexical_cast<std::string>(i) + "/object")) + "/mass"))),
    damage(Gamedata::getInstance().getXmlInt(std::string("projectiles/moon" + boost::lexical_cast<std::string>(i) + "/damage"))),
    orbitpos(Vector2f(0,0)),
    oradius(1)
    {}

void Projectile::update(Uint32 ticks){
  if(orbiting){
    float angularVelocity = (-.40 / mass)*2*M_Pi;
    angle = fmod((angle - angularVelocity*static_cast<float>(ticks)*.001) , 2*M_Pi);
    Vector2f prev = getPosition();
    setPosition(orbitpos + Vector2f(oradius +  oradius*cos(angle) - getFrameWidth()/2.0,oradius + oradius * sin(angle) - getFrameHeight()/2.0));
    setVelocity((getPosition() - prev).normalize()*-1*angularVelocity*oradius);
  }else{
    MultiSprite::update(ticks);
  }
}

void Projectile::draw() const{
  MultiSprite::draw();
}

void Projectile::orbit(Vector2f o, int r){
  dangerous = false;
  orbiting = true;
  orbitpos = o;
  oradius = r;
  setFrame(NORMAL);
}

void Projectile::shoot(){
  orbiting = false;
  dangerous = true;
  setFrame(DANGEROUS);
  if(getVelocity().magnitude() == 0)setVelocity((getPosition() + Vector2f(getFrameWidth()/2.0, getFrameHeight()/2.0) - orbitpos).normalize() * oradius*(1.0 / mass)*2*M_Pi);
}

bool Projectile::collidedWith(const Drawable *d) const {
  return MultiSprite::collidedWith(d);
}
