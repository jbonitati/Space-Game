#include "twowaymultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"


void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    		currentFrame = (currentFrame+1) % (numberOfFrames / 2);
    		if((velocityX() < 0 && accelerationX() <= 0) || accelerationX() < 0 ) currentFrame += (numberOfFrames / 2);
			timeSinceLastFrame = 0;
	}
}

void TwoWayMultiSprite::update(Uint32 ticks) {
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

  advanceFrame(ticks);
}
