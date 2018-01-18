#include "ioManager.h"
#include "objecthud.h"
#include "projectileManager.h"


ObjectHud& ObjectHud::getInstance() {
  static ObjectHud ObjectHud;
  return ObjectHud;
}

ObjectHud::ObjectHud() :
  gdata(Gamedata::getInstance()),
  io( IOManager::getInstance()),
  position(Gamedata::getInstance().getXmlInt("ObjectHud/loc/x"), Gamedata::getInstance().getXmlInt("ObjectHud/loc/y")),
  width(gdata.getXmlInt("ObjectHud/width")),
  height(gdata.getXmlInt("ObjectHud/height")),
  thickness(gdata.getXmlInt("ObjectHud/thickness")),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  visible(false),
  color(),
  lineColor()
{
  color = SDL_MapRGB(io.getScreen()->format, gdata.getXmlInt("ObjectHud/red"), gdata.getXmlInt("ObjectHud/green"), gdata.getXmlInt("ObjectHud/blue"));
  lineColor = SDL_MapRGB(io.getScreen()->format, 255 - gdata.getXmlInt("ObjectHud/red")/10, 255 - gdata.getXmlInt("ObjectHud/green")/10, 255 - gdata.getXmlInt("ObjectHud/blue")/10);
}

void ObjectHud::draw() {
  SDL_Surface * const screen = io.getScreen();
  if(visible){
	Draw_AALine(screen, X(), Y() + height/2, X() + width, Y() + height/2, height, color);

	Draw_AALine(screen, X(),Y(), X() + width,Y(), thickness, lineColor);
	Draw_AALine(screen, X(),Y() + height, X() + width,Y() + height, thickness, lineColor);
	Draw_AALine(screen, X(),Y(), X(),Y() + height, thickness, lineColor);
	Draw_AALine(screen, X() + width,Y(), X() + width,Y() + height, thickness, lineColor);

	io.printMessageValueAt("Moons Free: ", ProjectileManager::getInstance().numFree(),X()+ thickness, (int)(Y() + 1*height/5.0));
  io.printMessageValueAt("Moons Active: ", ProjectileManager::getInstance().numActive(),X()+ thickness, (int)(Y() + 2*height/5.0));
	io.printMessageValueAt("Explosions Free: ", ProjectileManager::getInstance().numFreeExplosions(),X()+ thickness, (int)(Y() + 3*height/5.0));
	io.printMessageValueAt("Explosions Active: ", ProjectileManager::getInstance().numActiveExplosions(),X()+ thickness, (int)(Y() + 4*height/5.0));

  }
}

void ObjectHud::update() {
  //if(secs == 2) setVisible(false);
}
