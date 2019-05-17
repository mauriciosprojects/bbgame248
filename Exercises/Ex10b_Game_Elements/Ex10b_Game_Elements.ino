
//========== BBGame248 Electronics/Coding Workshop Exercise 10b ==========

//Purpose: Game spaceship and firing laser elements
//Date: May 2, 2019  


#include "BBGame248.h"
Display display;
Buttons buttons;


//---------- Setup routine to run once ----------

void setup()
{
  display.Setup(24,8);
  buttons.Setup();
}


//--- Global variables ---

int counter = 0;
int d = 10;

int shipPos = 0;
int laserPos = 0;
int laserDistance = 0;
bool laserFired = false;


//---------- Loop routine that runs repeatedly ----------

void loop()
{
  display.Clear();
  DrawSpaceship();
  DrawLaser();
  UpdateLaser();

  if(buttons.BtnPressed(BTN1)) FireLaser(shipPos); 
  if(buttons.BtnPressed(BTN3)) MoveSpaceship(-1);
  if(buttons.BtnPressed(BTN4)) MoveSpaceship(+1);

  counter++;
  delay(d);
}

void DrawSpaceship()
{
  display.SetPixel(0,shipPos);
  display.SetPixel(1,shipPos);
}

void MoveSpaceship(int dir)
{
  if (dir == +1) shipPos++;
  if (dir == -1) shipPos--;

  if (shipPos < 0) shipPos = 0;
  if (shipPos > 7) shipPos = 7;
}


void FireLaser(int pos)
{
  laserFired = true;
  laserPos = pos;
  laserDistance = 0;
}


void DrawLaser()
{
  if (laserFired == true)
    display.SetPixel(laserDistance,laserPos);
}


void UpdateLaser()
{
  if (laserFired == true) laserDistance++;
  if (laserDistance > 23) laserFired = false;
}
