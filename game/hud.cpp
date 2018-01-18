#include "ioManager.h"
#include "hud.h"
#include "projectileManager.h"
#include "gamedata.h"

Hud& Hud::getInstance() {
  static Hud hud;
  return hud;
}

Hud::Hud() :
  gdata(Gamedata::getInstance()),
  io( IOManager::getInstance()),
  position(Gamedata::getInstance().getXmlInt("hud/loc/x"), Gamedata::getInstance().getXmlInt("hud/loc/y")),
  width(gdata.getXmlInt("hud/width")),
  height(gdata.getXmlInt("hud/height")),
  thickness(gdata.getXmlInt("hud/thickness")),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  visible(true),
  color(),
  lineColor()
{
  color = SDL_MapRGB(io.getScreen()->format, gdata.getXmlInt("hud/red"), gdata.getXmlInt("hud/green"), gdata.getXmlInt("hud/blue"));
  lineColor = SDL_MapRGB(io.getScreen()->format, 255 - gdata.getXmlInt("hud/red")/10, 255 - gdata.getXmlInt("hud/green")/10, 255 - gdata.getXmlInt("hud/blue")/10);
}

void Hud::draw(float rate, unsigned int secs, std::string current) {
  SDL_Surface * const screen = io.getScreen();
  if(visible){
	Draw_AALine(screen, X(), Y() + height/2, width, Y() + height/2, height, color);

	Draw_AALine(screen, X(),Y(), width,Y(), thickness, lineColor);
	Draw_AALine(screen, X(),height, width,height, thickness, lineColor);
	Draw_AALine(screen, X(),Y(), X(),height, thickness, lineColor);
	Draw_AALine(screen, width,Y(), width,height, thickness, lineColor);

	io.printMessageValueAt("fps: ", rate, X() + thickness, (int)(Y() + height/9.0));
	io.printMessageValueAt("Seconds elapsed: ", secs, X()+ thickness, (int)(Y() + 2*height/9.0));
	io.printMessageAt("       Player 1 \t Player 2 ", X()+ thickness, (int)(Y() + 3*height/9.0));
	io.printMessageAt("Move: WASD \t Up,Down,Left,Right ", X()+ thickness, (int)(Y() + 4*height/9.0));
	io.printMessageAt("Shoot: Space \t Enter", X()+ thickness, (int)(Y() + 5*height/9.0));
	io.printMessageAt("Kill the other planets to become the best!", X()+ thickness, (int)(Y() + 6*height/9.0));
	io.printMessageAt(std::string("Player 1: ") + current, X()+ thickness, (int)(Y() + 7*height/9.0));
	if(Gamedata::getInstance().getXmlInt("numPlayers") > 1) io.printMessageAt(std::string("Player 2: ") + "venus", X()+ thickness, (int)(Y() + 8*height/9.0));

  }else{
		io.printMessageAt("Press F1 to display the HUD", X(), Y());
  }
}

void Hud::update(unsigned int secs) {
  if(secs == 3) setVisible(false);
}
