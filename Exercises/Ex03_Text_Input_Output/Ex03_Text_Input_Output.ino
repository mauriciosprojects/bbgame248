
//========== BBGame248 Electronics/Coding Workshop Exercise 3 ==========

//Purpose: Serial text communication between Arduino and computer
//Date: April 28, 2019


//---------- Setup routine to run once ----------

void setup() 
{
  //Serial communication with computer over USB
  Serial.begin(115200);
  
  //Print to computer on Arduino Serial Monitor
  Serial.println();
  Serial.println();
  Serial.println("Hello!");
  Serial.println("Let's calculate the area of a triangle.");

  //Get length value input on computer as floating point number
  Serial.print("Enter length: ");
  float length = getFloatInput();
  
  //Get width value input on computer as floating point number
  Serial.print("Enter width: ");
  float width = getFloatInput();

  //Calculate area of triangle and print to computer
  float area = (length * width) / 2.0;
  Serial.print("The area is ");
  Serial.print(area);
  Serial.println(" square units.");
}


//---------- Loop routine that runs repeatedly ----------

void loop() 
{
  //Do nothing here
}


//---------- Function getFloatInput  ----------

//Function to check if bytes are coming in through serial connection, 
//then read those bytes in as a floating point number 

float getFloatInput()
{
  //Wait until bytes come in through serial connection
  while (Serial.available() == 0) delay(100);

  //Get incoming bytes and interpret as floating point number  
  float x = Serial.parseFloat();
  Serial.println(x);

  //Read incoming bytes until end of line Enter character is read
  while (Serial.available() > 0) Serial.read();

  //Return to floating point value to the calling function
  return x;
}
