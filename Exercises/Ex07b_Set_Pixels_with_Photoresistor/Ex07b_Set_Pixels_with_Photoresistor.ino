
//========== BBGame248 Electronics/Coding Workshop Exercise 7b ==========

//Purpose: Rotating pattern with speed that varies according to sensed light
//Date: April 28, 2019


#include "BBGame248.h"
Display display;


//---------- Setup routine to run once ----------

void setup() 
{
  //Initialize LED matrix display
  display.Setup(8,8);
  pinMode(A0,INPUT);
  
  //Serial communication with computer over USB
  Serial.begin(115200);
  Serial.println("Hello");
}


//--- Global variables ---
int counter = 0;
int light = 0;


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

  //Read analog input A0 from photoresistor
  light = analogRead(A0);

  //Calculate delay by mapping A0 input range value 200-800 to delay of 50 to 250 ms
  int d = map(light,200,800,50,250);

  //Send variable values for plotting on Serial Plotter
  Serial.print(counter);
  Serial.print(" --- ");
  Serial.print(light);
  Serial.print(" --- ");
  Serial.println(d);

  //Delay that varies according to light
  delay(d);

  //Increment counter
  counter++;
  if (counter > 7) counter = 0;
}

