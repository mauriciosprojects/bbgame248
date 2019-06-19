   
//========== BBGame248 Electronics/Coding Workshop Exercise 10d ==========

//Purpose: Full game with score display
//Date: May 17, 2019  

#include "BBGame248.h"
Display display;
Buttons buttons;
TextDisplay textDisplay(display);

#include "BBGame248SpaceInv.h"
Spaceship ship;
Laser laser;
Aliens aliens;

ModeMenu menu;


//---------- Setup routine to run once ----------

void setup()
{
  display.Setup(24,8); 
  display.Clear();
  buttons.Setup();

  AddMessage("HELLO");
  menu.StartupCheck();
  
  textDisplay.DisplayTextCentered("INVADR");  
  int btnPressed = buttons.WaitAnyBtnPressed();
  
  if ((btnPressed == BTN3) || (btnPressed == BTN4))
    menu.DoSelectionMenu();
}


//--- Global variables ---

int counter = 0;
int d = 7;

int shipPos = 4;
int laserPos = 16;
int laserDistance = 0;
bool laserFired = false;

int numShips = 5;
int score = 0;
bool scoreDisplayed = false;

void ScrollMessage(String s)
{
  int n = textDisplay.getScrollWidth(s);
  for(int i = 0; i < n; i++)
  {
    textDisplay.ScrollText(s,i);
    delay(35);
  }
}

//---------- Loop routine that runs repeatedly ----------

void loop()
{
  if ((numShips > 0) && !aliens.Finished())
  {
    display.Clear();
    DrawSpaceship();
    aliens.Draw(display);
    DrawLaser();
    DrawSpaceship();

    UpdateLaser();
    laser.CheckHit(aliens);
    laserFired = laser.IsStillFired();
    ship.CheckHit(aliens);
    aliens.Update(counter);
    
    if ((counter % aliens.NextAlienInterval()) == 0) 
      aliens.AddAlien(counter);

    if(buttons.BtnPressed(BTN1)) FireLaser(shipPos); 
    if(buttons.BtnPressed(BTN4)) MoveSpaceship(+1);
    if(buttons.BtnPressed(BTN3)) MoveSpaceship(-1);

    if(buttons.BtnDown(BTN2))
    {
      score = laser.GetNumHits();
      textDisplay.DisplayTextCentered(String(score));
    }
  }
  else if (scoreDisplayed == false)
  {
    display.Clear();
    score = laser.GetNumHits();
    textDisplay.DisplayTextCentered(String(score));
    scoreDisplayed = true;
    counter = 0;
  }
  else if (scoreDisplayed == true)
  {
    if (counter > 32000)
      display.Clear();
  }

  counter++;
  delay(d);
}


void DrawSpaceship()
{
  display.SetPixel(0,shipPos);
  display.SetPixel(1,shipPos);
  ship.SetPos(shipPos);
}

void MoveSpaceship(int dir)
{
  if ((dir > 0) && (shipPos < 7)) shipPos++;
  if ((dir < 0) && (shipPos > 0)) shipPos--;
}

void DrawLaser()
{
  if (laserFired == true)
    display.SetPixel(laserDistance,laserPos);

  laser.SetPos(laserFired,laserPos,laserDistance);
  laser.SetCounter(counter);
}

void FireLaser(int pos)
{
  laserFired = true;
  laserPos = pos;
  laserDistance = 0;
}

void UpdateLaser()
{
  if (laserDistance > 23)
    laserFired = false;
  else
  {
    if (laserFired == true)
      laserDistance++;
  }
}

