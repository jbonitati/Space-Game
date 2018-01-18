#include "aaline.h"
#include <string>
#pragma once

class ObjectHud{
public:
	static ObjectHud& getInstance();
	void draw ();
	void update();
	void setVisible(bool v){visible = v;}
	bool isVisible() {return visible;}

 Vector2f getPosition() const { return position; }
  float X() const { return position[0]; }
  void X(float x) { position[0] = x; }
  float Y() const { return position[1]; }
  void Y(float y) { position[1] = y; }
private:
	const Gamedata& gdata;
	const IOManager& io;
  	Vector2f position;
  	unsigned width;
  	unsigned height;
	unsigned thickness;
  	unsigned worldWidth;
 	unsigned worldHeight;
	bool visible;
	uint32_t color;
	uint32_t lineColor;

  	ObjectHud();
	ObjectHud(const ObjectHud&);
  	ObjectHud& operator=(const ObjectHud&);
};
