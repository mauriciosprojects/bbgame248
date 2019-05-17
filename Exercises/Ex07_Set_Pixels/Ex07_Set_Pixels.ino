
//========== BBGame248 Electronics/Coding Workshop Exercise 7 ==========

//Purpose: Rotating pattern created by setting individual pixels
//Date: April 28, 2019


#include "BBGame248.h"
Display display;


//---------- Setup routine to run once ----------

void setup() 
{
  //Initialize LED matrix display
  display.Setup(8,8);
}


//--- Global variable ---
int counter = 0;


//---------- Loop routine that runs repeatedly ----------

void loop() 
{
  display.Clear();

  //Draw one pixel in the center
  display.SetPixel(2,2);

  //Based on the counter value, draw pixels to create rotating pattern
  if (counter == 0) { display.SetPixel(2,1); display.SetPixel(2,0); }
  if (counter == 1) { display.SetPixel(1,1); display.SetPixel(0,0); }
  if (counter == 2) { display.SetPixel(1,2); display.SetPixel(0,2); }
  if (counter == 3) { display.SetPixel(1,3); display.SetPixel(0,4); }
  if (counter == 4) { display.SetPixel(2,3); display.SetPixel(2,4); }
  if (counter == 5) { display.SetPixel(3,3); display.SetPixel(4,4); }
  if (counter == 6) { display.SetPixel(3,2); display.SetPixel(4,2); }
  if (counter == 7) { display.SetPixel(3,1); display.SetPixel(4,0); }
  delay(100);

  //Increment counter
  counter++;
  if (counter > 7) counter = 0;
}

