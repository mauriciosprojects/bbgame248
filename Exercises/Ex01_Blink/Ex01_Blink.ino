#include "BBGame248.h"
Display display;

void setup()  
{ 
  display.Setup(8,8);
}

void loop()  
{
  //Turn one LED on
  display.SetPixel(2,2,ON);
  delay(200);

  //Turn one LED off
  display.SetPixel(2,2,OFF);
  delay(200);
}

