#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "vector2f.h"
#include "viewport.h"
#include <algorithm>

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}
MultiSprite & MultiSprite::operator=(const MultiSprite& m){
	Drawable::operator=(m);
	frames = m.getFrames();
	currentFrame = m.currentFrame;
	numberOfFrames = m.numberOfFrames;
	frameInterval = m.frameInterval;
	timeSinceLastFrame = m.timeSinceLastFrame;
	frameWidth = m.frameWidth;
	frameHeight = m.frameHeight;
	return *this;
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name,
           Vector2f(0,0),
           Vector2f(0,0)
           ),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

MultiSprite::MultiSprite(const std::string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel),
  frames( FrameFactory::getInstance().getFrames(n) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(n+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(n+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

void MultiSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
	if(isVisible())
  	frames[currentFrame]->draw(x, y);
}

void MultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }

}


bool MultiSprite::collidedWith(const Drawable* d) const{
	if(isVisible() && d->isVisible()){
		return execute(d);
	}else return false;
}

// The next function was contributed by Mike Kabbani.
// The function handles transparency for both sprite
// images that explicity assign a color key and for sprite
// images that use an alpha channel.
bool MultiSprite::isPixelVisible(Uint32 pixel, SDL_Surface *surface) const {
	Uint32 temp;
	SDL_PixelFormat* fmt = surface->format;
	if(fmt->BitsPerPixel == 32){
		temp=pixel&fmt->Amask; /* Isolate alpha component */
		temp=temp>>fmt->Ashift;/* Shift it down to 8-bit */
		temp=temp<<fmt->Aloss; /* Expand to a full 8-bit number */
		if(temp == 0) return false;
	}
	if(pixel == surface->format->colorkey) return false;
	return true;
}

bool MultiSprite::execute(const Drawable* obj) const {
  float left1 = X();
  float left2 = obj->X();
  float right1 = left1+getFrame()->getWidth();
  float right2 = left2+obj->getFrame()->getWidth();
  if ( right1 < left2 ) return false;
  if ( left1 > right2 ) return false;
  float top1 = Y();
  float top2 = obj->Y();
  float bottom1 = top1+getFrame()->getHeight();
  float bottom2 = top2+obj->getFrame()->getHeight();
  if ( bottom1 < top2 ) return false;
  if ( bottom2 < top1 ) return false;

  Vector2f p1 = getPosition() - Viewport::getInstance().getPosition();
  Vector2f p2 = obj->getPosition() - Viewport::getInstance().getPosition();

  const Frame * const frame1 = getFrame();
  const Frame * const frame2 = obj->getFrame();
  Uint16 width1 = frame1->getWidth();
  Uint16 width2 = frame2->getWidth();
  Uint16 height1 = frame1->getHeight();
  Uint16 height2 = frame2->getHeight();

  int o1Left = p1[0];
  int o1Right = o1Left+width1;

  int o2Left = p2[0];
  int o2Right = o2Left+width2;
  std::vector<int> sides;
  sides.reserve(4);
  sides.push_back( o1Left );
  sides.push_back( o1Right );
  sides.push_back( o2Left );
  sides.push_back( o2Right );
  std::sort( sides.begin(), sides.end() );


  int o1Up = p1[1];
  int o1Down = o1Up+height1;
  int o2Up = p2[1];
  int o2Down = o2Up+height2;
  std::vector<int> lids;
  lids.reserve(4);
  lids.push_back( o1Up );
  lids.push_back( o1Down );
  lids.push_back( o2Up );
  lids.push_back( o2Down );
  std::sort( lids.begin(), lids.end() );

  SDL_Surface* surface1 = frame1->getSurface();
  SDL_Surface* surface2 = frame2->getSurface();
  SDL_LockSurface(surface1);
  SDL_LockSurface(surface2);
  Uint32 *pixels1 = static_cast<Uint32 *>(surface1->pixels);
  Uint32 *pixels2 = static_cast<Uint32 *>(surface2->pixels);
  unsigned pixel1;
  unsigned pixel2;
  for (int x = sides[1]; x < sides[2]; ++x) {
    for (int y = lids[1]; y < lids[2]; ++y) {
      // check pixels in surface for obj1 and surface for obj2!
      int i = x - p1[0];
      int j = y - p1[1];
      pixel1 = pixels1[ (j * width1) + i ];
      i = x - p2[0];
      j = y - p2[1];
      pixel2 = pixels2[ (j * width2) + i ];

      if ( isPixelVisible(pixel1,surface1) && isPixelVisible(pixel2,surface2) ) {
        SDL_UnlockSurface(surface1);
        SDL_UnlockSurface(surface2);
        return true;
      }
    }
  }
  SDL_UnlockSurface(surface1);
  SDL_UnlockSurface(surface2);

  return false;
}
