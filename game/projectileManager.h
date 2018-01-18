#include <list>
#include <vector>
#include "projectile.h"
#include "player.h"
#include "sound.h"

class ProjectileManager {
public:
  static ProjectileManager& getInstance();
  ~ProjectileManager();
  void wipe();
  void updateAll(Uint32, SDLSound&);
  void drawAll()const;
  void spawnProjectile();

  int numFree() const{return freeList.size();}
  int numActive() const{return activeList.size();}
  int numFreeExplosions() const{return freeExplosions.size();}
  int numActiveExplosions() const{return activeExplosions.size();}

  Player *getPlayer(int index){
    return &characters[index];
  }
  int numPlayers()const{
    return characters.size();
  }

  void shoot(int index);

  void addcharacter(std::string n){
  	characterList.push_back(  Player("characters/" + n, n));
  }

  void addcharacters(){
    addcharacter("earth");
    addcharacter("venus");
    addcharacter("mercury");
    addcharacter("mars");
    addcharacter("jupiter");
    addcharacter("saturn");
    addcharacter("uranus");
    addcharacter("neptune");
    addcharacter("pluto");
  }

  void resetPlayers(){
    characters.clear();
    int i = 0;
    characters.push_back(characterList[i++]);
    if( twoPlayers) characters.push_back(characterList[i++]);
    while(i < Gamedata::getInstance().getXmlInt("numEnemies") + 1){
      characters.push_back(characterList[i++%(int)(characterList.size())]);
    }
  }

  bool win(int index)const{
      bool alldestroyed = true;
      int i= 0;
      while(alldestroyed && i < (int)(characters.size())){
        if(i != index) alldestroyed = !(characters[i].isVisible());
        else alldestroyed = characters[i].isVisible();
        i++;
      }
      return alldestroyed;
  }

  bool lose(int index)const{
    return !characters[index%(int)(characters.size())].isVisible();
  }

private:
  std::list<Projectile *> freeList;
  std::list<Projectile *> activeList;
  std::list<Sprite *> freeExplosions;
  std::list<Sprite *> activeExplosions;
  std::vector<Player> characters;
  std::vector<Player> characterList;
  bool twoPlayers;

  std::list<Projectile *>::iterator explode(std::list<Projectile *>::iterator it);

  ProjectileManager()
    : freeList(), activeList(), freeExplosions(), activeExplosions(), characters(), characterList(), twoPlayers(Gamedata::getInstance().getXmlInt("numPlayers") > 1){
      addcharacters();
      resetPlayers();
}
  ProjectileManager(const ProjectileManager&);
  ProjectileManager& operator=(const ProjectileManager&);
};
