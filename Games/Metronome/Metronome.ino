#include "BBGame248.h"
Display display;
Buttons buttons;
TextDisplay textDisplay(display);
Sound sound2;

float bpm = 100.0;

void setup()
{
  display.Setup(24,8); 
  display.Clear();
  buttons.Setup();
  sound2.Setup();

  textDisplay.DisplayTextCentered("Metro");  
  delay(1000);  
}


//--- Global variables ---

int counter = 0;
int timer = 0;
int d = 7;
int needlePos = 0;
int needlePosi = 0;


void loop()
{
  int d = (60000 / bpm);
  int needleIncr = d / 23;
  int ibpm = bpm;

  if (needlePos < timer)
  {
    needlePos += needleIncr;
    needlePosi++;
    needlePosi %= 47;
  }

  display.Clear();
  textDisplay.DisplayTextCentered(String(ibpm));
  display.vLine(needlePosi <= 23 ? needlePosi : 47 - needlePosi);

  if (counter <= 0)
  {
    counter = d;
    timer = 0;
    needlePos = 0;
    sound2.StartSound(50,3,0);
  }  

  counter -= 10;
  timer += 10;
  sound2.Update(0);
  delay(10);

  if (buttons.BtnPressed(BTN4)) bpm -= 10;
  if (buttons.BtnPressed(BTN3)) bpm += 10;
  if (bpm < 20) bpm = 20;
  if (bpm > 200) bpm = 200;
}

