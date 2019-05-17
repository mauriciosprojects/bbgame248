
//========== BBGame248 Electronics/Coding Workshop Exercise 5 ==========

//Purpose: Test 8x8 LED matrix
//Date: April 22, 2019


#include "BBGame248.h"
Display display;
int d = 50; //ms



//---------- Setup routine to run once ----------

void setup() 
{
  //Initialize LED matrix display
  display.Setup(8,8);
}


//---------- Loop routine that runs repeatedly ----------

void loop() 
{
  //Flash sequence of horizontal lines
  for(int y = 0; y < 8; y++)
  {
    display.Clear();
    display.hLine(y);
    delay(d);
  }

  //Flash sequence of vertical lines
  for(int x = 0; x < 8; x++)
  {
    display.Clear();
    display.vLine(x);
    delay(d);
  }
}

