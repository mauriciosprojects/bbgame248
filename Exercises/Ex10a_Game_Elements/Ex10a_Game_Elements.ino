
//========== BBGame248 Electronics/Coding Workshop Exercise 10a ==========

//Purpose: Game spaceship element that moves with buttons
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


//---------- Loop routine that runs repeatedly ----------

void loop()
{
  display.Clear();
  DrawSpaceship();

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

