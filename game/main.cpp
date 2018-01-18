// Brian Malloy        Game Construction
//Joey Bonitati
#include "manager.h"
#include <SDL/SDL_mixer.h>

Clock* Clock::instance = NULL;

int main(int, char*[]) {
   try {
      Manager manager(Gamedata::getInstance().getXmlInt("numPlayers"));
      manager.play();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
