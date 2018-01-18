#include <string>
#include <list>
#include <vector>
#include "twowaymultisprite.h"
#include "chunk.h"
#include "projectile.h"

class Player : public TwoWayMultiSprite{
public:
	Player(const std::string spriteName, std::string playerName);
	Player(const Player& p);
	virtual ~Player() {
		for ( unsigned int i = 0; i < frames.size(); ++i ) {
	    SDL_FreeSurface( surfaces[i] );
	    delete frames[i]; // ExplodingSprite made them, so it deletes them
	  }
	}
 	virtual void update(Uint32 ticks);
	virtual void draw() const;
	void move(Vector2f);
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void stopVertical();
	void stopHorizontal();
	std::string getPlayerName()const{return playerName;}
	int getMass(){return mass;}
	void increaseScore(int points){score += points;}
	void takeDamage(int d);
  	int getmaxSpeed()const{return maxSpeed;}
		int getHealth()const{return health;}
		int getMaxHealth()const{return maxhealth;}
	void addProjectile(Projectile *p){
		score += 50;
		projs.push_back(p);
	}
	void shootFirst(){projs.erase(projs.begin());}
	std::vector<Projectile *> getProjs(){
		return projs;
	}
	void clearProjs(){
		for(std::vector<Projectile *>::iterator it = projs.begin(); it != projs.end(); it++){
			(*it)->reset();
		}
		projs.clear();
	}
	void playerCollide(Player *p);
	virtual bool collidedWith(const Drawable *d) const;
		Player& operator=(const Player& p);

		void makeChunks(unsigned int);
	  unsigned int chunkCount() const { return chunks.size(); }
	  unsigned int freeCount()  const { return freeList.size(); }

		int getScore()const{return score;}

	void moveAway(Vector2f v);
	void moveTowards(Vector2f v);

private:
	std::string playerName;
	int score;
  	int maxSpeed;
  	int acceleration;
		int friction;
		int mass;
		int health;
		int maxhealth;
		std::vector<Projectile *> projs;

		std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
	  std::list<Chunk> freeList; // When a chunk gets out of range it goes here

	  std::vector<SDL_Surface*> surfaces; // Each chunk has a Surface
	  std::vector<Frame*> frames; // Each chunk has a Frame
};
