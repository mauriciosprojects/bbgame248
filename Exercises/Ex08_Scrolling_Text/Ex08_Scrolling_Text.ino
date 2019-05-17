
//========== BBGame248 Electronics/Coding Workshop Exercise 8 ==========

//Purpose: Scrolling text on 11x4 LED matrix
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
String text = "Hello World";


//---------- Loop routine that runs repeatedly ----------

void loop() 
{
  //Draw text using scrolling text function and counter for position
  textDisplay.ScrollText(text, counter);
  counter++;
  delay(100);
}




