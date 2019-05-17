
//========== BBGame248 Electronics/Coding Workshop Exercise 9b ==========

//Purpose: Numerical counter up to 999 with button control
//Date: April 28, 2019


#include "BBGame248.h"
Display display;
Buttons buttons;
TextDisplay textDisplay(display);


//---------- Setup routine to run once ----------

void setup() 
{
  //Initialize LED matrix display
  display.Setup(24,8);
  buttons.Setup();
}


//--- Global variables ---
int counter = 0;
bool running = false;


//---------- Loop routine that runs repeatedly ----------

void loop() 
{
  //Create string variable out of counter value
  String text(counter);

  //Draw text string
  display.Clear();
  textDisplay.DisplayText(text);

  //Only increment counter if running variable is true
  if (running)
  {
    if (counter < 999) counter++;
  }

  //Check BTN1
  if (buttons.BtnPressed(BTN1))
  {
    //If running is true, set to false
    if (running == true) 
      running = false;
    //If running is false, set to true
    else
      running = true;
  }

  delay(100);  
}




