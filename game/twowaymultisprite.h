#include <string>
#include <vector>
#include "multisprite.h"


class TwoWayMultiSprite : public MultiSprite {
public:
  TwoWayMultiSprite(const std::string& name) :
    MultiSprite(name)
    { }
  TwoWayMultiSprite(const std::string& n, const Vector2f& pos, const Vector2f& vel) :
	  MultiSprite(n,pos,vel){}
  TwoWayMultiSprite(const TwoWayMultiSprite& s):
    MultiSprite(s)
    { }
  virtual ~TwoWayMultiSprite() { }
  virtual void update(Uint32 ticks);
    //TwoWayMultiSprite& operator=(const TwoWayMultiSprite &rhs);

private:

protected:
  void advanceFrame(Uint32 ticks);
};
