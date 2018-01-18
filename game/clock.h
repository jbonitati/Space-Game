#include <SDL.h>
#include <string>

class Manager;

class Clock {
public:
  static Clock getInstance();

private:
  friend class Manager;
  static Clock* instance;

  bool started;
  bool paused;
  bool sloMo;
  const bool framesAreCapped;
  const int frameCap;

  unsigned int frames;

  unsigned int tickSum;
  unsigned int sumOfAllTicks;
  unsigned int timeAtStart;
  unsigned int timeAtPause;
  unsigned int currTicks;
  unsigned int prevTicks;
  unsigned int ticks;
  float frameRate;
  unsigned int * frameRates;

  unsigned int getTicks() const;
  unsigned int getTotalTicks() const { return sumOfAllTicks; }

  unsigned int getElapsedTicks();
  Clock& operator++();
  void toggleSloMo();
  float getFrameRate() const {return frameRate;}
  unsigned int getSeconds() const { return getTotalTicks()/1000;  }

  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  unsigned int getFrames() const  { return frames;  }
  unsigned int capFrameRate() const;

  void start();
  void pause();
  void unpause();
  void display() const;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};
