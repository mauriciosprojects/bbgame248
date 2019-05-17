
//========== BBGame248 Electronics/Coding Workshop Exercise 2b ==========

//Purpose: LEDs move according to one button input
//Date: April 22, 2019


#include "BBGame248.h"
Display display;
Buttons buttons;


//---------- Setup routine to run once ----------

void setup()  
{ 
  //Initialize LED matrix display
  display.Setup(8,8);
}


//--- Global variable ---
int counter = 1;


//---------- Loop routine that runs repeatedly ----------

void loop()  
{
  //Turn on one LED according to counter
  if(counter==1) display.SetPixel(1,2,ON);
  if(counter==2) display.SetPixel(2,2,ON);
  if(counter==3) display.SetPixel(3,2,ON);
  if(counter==4) display.SetPixel(4,2,ON);
  if(counter==5) display.SetPixel(5,2,ON);
  delay(200);

  //Check button and change counter accordingly
  if(buttons.BtnDown(BTN1))
  {
      //If button is pressed, decrement counter variable and wrap
      counter = counter - 1;
      if(counter < 1) counter = 5;
  }
  else
  {
      //Otherwise if not pressed, increment counter variable and wrap
      counter = counter + 1;
      if(counter > 5) counter = 1;
  }

  //Turn off all LEDs for an instant (no delay)
  display.SetPixel(1,2,OFF);
  display.SetPixel(2,2,OFF);
  display.SetPixel(3,2,OFF);
  display.SetPixel(4,2,OFF);
  display.SetPixel(5,2,OFF);
}

