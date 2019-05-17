
//========== BBGAME114 SPACE INVADERS GAME LIBARY ==========

//Date: May 30, 2018


#ifndef LIBGAME_H
#define LIBGAME_H


#include "BBGame248.h"


typedef struct
{
    int start_delay;
    int duration;
    int update_interval;
    int add_interval;
    byte type;
    byte min;
    byte max;
    byte points;
    int start;
    int end;
} 
GameLevel;

typedef enum 
{ 
    ALIENWAVE_RANDOM = 0,
    ALIENWAVE_MARCH = 1,
    ALIENWAVE_END = 2      
} 
AlienWaveType;


byte NewAlienLevel1(int counter);
byte NewAlienLevel2(int counter);
byte NewAlienLevel3(int counter);
byte NewAlienLevelEmpty(int counter);


class Aliens
{
public:
    Aliens();
    void AddAlien(int counter);
    void Draw(Display &display);
    void Update(int counter);
    int NextAlienInterval();
    bool Finished();
    void SetGameLevels(GameLevel *newLevels);

    byte aliens[25];
    byte pos[25];
    byte state[25];
    byte num;
    byte next;

    byte numLevels;
    byte level;

private:
    GameLevel *levels;
    void AdjustGameLevelGaps(GameLevel *newLevels);
};


#define ALIEN_HIT 2


class Spaceship
{
public:
    Spaceship();
    void Draw(Display &display);
    void Move(int8_t dir);
    void CheckHit(Aliens &aliens);
    void SetPos(byte externalPos);

    byte pos;
    byte bitImage;
    byte state;
};


class Laser
{
public:
    Laser();
    void Draw(Display &display);
    void Update(int counter);
    void Fire(int shipPos);
    void CheckHit(Aliens &aliens);
    void SetPos(bool externalFired, byte externalPos, byte externalDistance);
    void SetCounter(int externalCounter);
    bool IsStillFired();
    int GetNumHits();

    bool fired;
    int8_t pos;
    int8_t distance;
    byte bitImage; //not used yet
    int counter;
};


class ModeMenu 
{
public:
  ModeMenu();
  void StartupCheck();
  void DoSelectionMenu();
private: 
  int gameMode;
};

class ScrollingMessage 
{
public:
  ScrollingMessage();
  void AddMessage(const char* message);
  void ScrollMessage();
private: 
  byte numMessages;
  static const byte maxNumMessages = 10;
  const char* displayMessages[maxNumMessages];
  char messageBuffer[101];
  byte messageIndex;
  int messageScrollDelay;
  int messageScrollDelayIndex;
  static const int numDelays = 3;
  int messageScrollDelays[numDelays] = { 60, 25, 12 };
};


void AddMessage(const char *message);


#endif //LIBGAME_H
