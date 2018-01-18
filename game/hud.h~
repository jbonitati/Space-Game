#include "aaline.h"
#include <string>

class Hud{
public:
	static Hud& getInstance();
	void draw (float f, unsigned int s, std::string curr);
	void update(unsigned int);
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

  	Hud();
	Hud(const Hud&);
  	Hud& operator=(const Hud&);
};
