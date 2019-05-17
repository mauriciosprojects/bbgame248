
//========== BBGame248 Electronics/Coding Workshop Exercise 8 ==========

//Purpose: Numeric counter up to 999 on 11x4 LED matrix
//Date: April 24, 2019


#include "BBGame248.h"
Display display;
TextDisplay textDisplay(display);


//---------- Setup routine to run once ----------

void setup() 
{
  //Initialize LED matrix display
  display.Setup(24,8);
}


//--- Global variable ---
int counter = 0;


//---------- Loop routine that runs repeatedly ----------

void loop() 
{
  //Create string variable out of counter value
  String text(counter);

  //Draw text string
  display.Clear();
  textDisplay.DisplayText(text);

  //Increment counter up to 999
  if (counter < 999) counter++;

  delay(100);
}
