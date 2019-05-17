
//========== BBGAME248 HARDWARE LIBARY ==========

//Date: May 14, 2019

#include <EEPROM.h>
#include "BBGame248.h"



//---------- GAME BOARD CLASS ----------

// Purpose: To abstract the BBGame248 device as a device that
// can identify itself. By storing the identity of the device
// in EEPROM memory, then we can safely store and retrieve
// other information in the EEPROM memory. This other information
// will be settings and other things that will be remembered after
// power down for the next power up.


void GameBoard::CheckID()
{
  if (!IsCorrectGameID()) WriteGameID();
}


bool GameBoard::IsCorrectGameID()
{
  // Return true if EEPROM contains "BBG248A" signature
  return ((EEPROM.read(EEPROM_ID+0) == 'B') && 
          (EEPROM.read(EEPROM_ID+1) == 'B') && 
          (EEPROM.read(EEPROM_ID+2) == 'G') && 
          (EEPROM.read(EEPROM_ID+3) == '2') && 
          (EEPROM.read(EEPROM_ID+4) == '4') && 
          (EEPROM.read(EEPROM_ID+5) == '8') && 
          (EEPROM.read(EEPROM_ID+6) == 'A'));
}


void GameBoard::WriteGameID()
{
  // Write "BBG248A" signature into EEPROM to stand for "BBGame248 Aliens"
  EEPROM.write(EEPROM_ID+0, 'B');
  EEPROM.write(EEPROM_ID+1, 'B');
  EEPROM.write(EEPROM_ID+2, 'G'); 
  EEPROM.write(EEPROM_ID+3, '2');
  EEPROM.write(EEPROM_ID+4, '4');
  EEPROM.write(EEPROM_ID+5, '8');
  EEPROM.write(EEPROM_ID+6, 'A');

  // Initialize various settings to default values
  EEPROM.write(EEPROM_RNDSEED,  0);
  EEPROM.write(EEPROM_GAMEMODE, 0);
  EEPROM.write(EEPROM_MSGINDEX, 2);
  EEPROM.write(EEPROM_MSGSPEED, 0);
}


//---------- MAX7219 CONTROLLER CLASS ----------

// Purpose: To abstract the MAX7219 controller that is used to
// drive the chain of LED matrices.


class MAX7219Controller
{
public:
  MAX7219Controller(byte n): numModules(n) {};
  void Initialize();
  void SendToAll(byte address, byte data);
  void SendDisplayBytes(byte *displayBuffer, int numBytes);
  void ClockInByte(byte data);

private:
  byte numModules;
  static const byte OUTPUT_PIN_CS     = 4;
  static const byte OUTPUT_PIN_CLK    = 3;
  static const byte OUTPUT_PIN_DATAIN = 2;
  static const byte REGISTER_NOOP        = 0x00;
  static const byte REGISTER_DIGIT0      = 0x01;
  static const byte REGISTER_DIGIT1      = 0x02;
  static const byte REGISTER_DIGIT2      = 0x03;
  static const byte REGISTER_DIGIT3      = 0x04;
  static const byte REGISTER_DIGIT4      = 0x05;
  static const byte REGISTER_DIGIT5      = 0x06;
  static const byte REGISTER_DIGIT6      = 0x07;
  static const byte REGISTER_DIGIT7      = 0x08;
  static const byte REGISTER_DECODEMODE  = 0x09;
  static const byte REGISTER_INTENSITY   = 0x0A;
  static const byte REGISTER_SCANLIMIT   = 0x0B;
  static const byte REGISTER_SHUTDOWN    = 0x0C;
  static const byte REGISTER_DISPLAYTEST = 0x0F;
};


void MAX7219Controller::Initialize()
{
  // Set GPIO output pin to control MAX7219 chips
  pinMode(OUTPUT_PIN_DATAIN, OUTPUT);
  pinMode(OUTPUT_PIN_CS, OUTPUT);
  pinMode(OUTPUT_PIN_CLK, OUTPUT);

  // Initialize for LED 8x8 matrix operation
  SendToAll(REGISTER_SHUTDOWN,    0x01);  // Not shutdown mode
  SendToAll(REGISTER_DISPLAYTEST, 0x00);  // No display test
  SendToAll(REGISTER_DECODEMODE,  0x00);  // No decode to 7-segment display
  SendToAll(REGISTER_SCANLIMIT,   0x07);  // No scan limit

  // Set brightness between 0x00 and 0x0F
  SendToAll(REGISTER_INTENSITY, 0x0F);

  // Initialize all display data registers
  SendToAll(REGISTER_DIGIT0, 0x00);
  SendToAll(REGISTER_DIGIT1, 0x00);
  SendToAll(REGISTER_DIGIT2, 0x00);
  SendToAll(REGISTER_DIGIT3, 0x00);
  SendToAll(REGISTER_DIGIT4, 0x00);
  SendToAll(REGISTER_DIGIT5, 0x00);
  SendToAll(REGISTER_DIGIT6, 0x00);
  SendToAll(REGISTER_DIGIT7, 0x00);
}


void MAX7219Controller::SendToAll(byte address, byte data) 
{
  digitalWrite(OUTPUT_PIN_CS, LOW);

  for ( byte c = 0; c < numModules; c++)
  {
    ClockInByte(address);
    ClockInByte(data);
  }

  digitalWrite(OUTPUT_PIN_CS, LOW);
  digitalWrite(OUTPUT_PIN_CS, HIGH);
}


void MAX7219Controller::SendDisplayBytes(byte *displayBuffer, int numBytes)
{
  byte i = 0;
  for (int iRegister = 1; iRegister <= 8; iRegister++)
  {
    digitalWrite(OUTPUT_PIN_CS, LOW);
    for (int iModule = 1; iModule <= numModules; iModule++)
    {
      ClockInByte(iRegister);
      ClockInByte(displayBuffer[i]);
      i++;
    }
    digitalWrite(OUTPUT_PIN_CS, HIGH);
  }
}

void MAX7219Controller::ClockInByte(byte data)
{
  noInterrupts();

  // Loop 1-bit mask starting at MSB bit position 8 down to LSB bit position 1
  for (byte bitPositionMask = 0x80; bitPositionMask != 0x00; bitPositionMask >>= 1)
  {
    byte bitData = (data & bitPositionMask) ? HIGH : LOW;
    digitalWrite(OUTPUT_PIN_CLK, LOW);        // Clock signal edge down
    digitalWrite(OUTPUT_PIN_DATAIN, bitData); // Data bit
    digitalWrite(OUTPUT_PIN_CLK, HIGH);       // Clock signal edge up
  }

  interrupts();
}


//---------- DISPLAY CLASS ----------

// Purpose: To abstract the BBGame114 LED matrix as a display,
// with methods/functions to clear the display, draw a dot,
// draw horizontal or vertical lines and draw text.


MAX7219Controller ledMatrixController(3);
byte displayBuffer[24];
int  displayCounter = 0;


void Display::Setup(byte _numCols, byte _numRows)
{
  numCols = _numCols;
  numRows = _numRows;
  if (numCols > 24) numCols = 24;
  if (numRows > 8)  numRows = 8;

  ledMatrixController.Initialize();
  Clear();
  SetupTimedRefresh();
}


void Display::Clear()
{
  byte *p = displayBuffer;
  for (byte i = 0; i < 24; i++)
    (*(p++)) = 0x0;
}


byte Display::MakeBit(byte y)
{
  return (1 << (7 - y));
}


byte Display::SetPixel(byte x, byte y, byte c)
{
  if ((x < 0) || (x >= numCols)) return 0;
  if ((y < 0) || (y >= numRows)) return 0;

  byte iModule = 2;
  if (x >= 8)  iModule = 1;
  if (x >= 16) iModule = 0;
  byte shift = 7 - (x % 8);
  byte mask = 1 << shift;
  byte iRegister = y + y + y;
  byte bits = c << shift;
  byte i = iModule + iRegister;
  noInterrupts();
  displayBuffer[i] &= ~mask;
  displayBuffer[i] |= bits;
  interrupts();

  return MakeBit(y);
}


void Display::SetColumnByte(byte x, byte columnBits)
{
  if ((x < 0) || (x >= numCols)) return;

  byte iModule = 2;
  if (x >= 8)  iModule = 1;
  if (x >= 16) iModule = 0;
  byte shift = 7 - (x % 8);
  byte maskDestByte = 1 << shift;
  byte maskSourceByte = 0x80;
  for (byte iRegister = 0; iRegister < 24; iRegister += 3)
  {
    byte c = (columnBits & maskSourceByte) ? 1 : 0;
    byte bits = c << shift;
    byte i = iModule + iRegister;
    noInterrupts();
    displayBuffer[i] &= ~maskDestByte;
    displayBuffer[i] |= bits;
    interrupts();
    maskSourceByte >>= 1;
  }
}


void Display::vLine(byte x, byte y1, byte y2)
{
  if (y1 == 255) y1 = 0;
  if (y2 == 255) y2 = numRows;
  for (byte i = y1; i <= y2; i++)
    SetPixel(x, i, 1);
}


void Display::hLine(byte y, byte x1, byte x2)
{
  if (x1 == 255) x1 = 0;
  if (x2 == 255) x2 = numCols;
  for (byte i = x1; i <= x2; i++)
    SetPixel(i, y, 1);
}


void Display::hShift(byte dir, bool wrap)
{
  /*
    signed char incr = (dir == 0) ? +1 : -1;
    byte *p1 = (dir == 0) ? (displayBuffer+1) : (displayBuffer+numCols-2);
    byte *p2 = p1 - incr;
    byte *p0 = p2;
    byte v0 = (wrap == false) ? 0 : (*p2);

    for(byte i = 0; i < (numCols-1); i++)
    {
    (*p2) = (*p1);
    p1 += incr;
    p2 += incr;
    }
    (*p2) = v0;
  */
}


void Display::vShift(byte dir, bool wrap)
{
  /*
    byte shiftedOutBits;

    for(byte i = 0; i < numCols; i++)
    {
    if (dir == 0)
    {
      shiftedOutBits = (displayBuffer[i] & 0x8) >> 3;
      displayBuffer[i] <<= 1;
    }
    else
    {
      shiftedOutBits = (displayBuffer[i] & 0x1) << 3;
      displayBuffer[i] >>= 1;
    }

    if (wrap == true)
      displayBuffer[i] |= shiftedOutBits;
    }
  */
}


void Display::SetupTimedRefresh()
{
  //Stop interrupts
  cli();

  //Clear timer2 interrupt registers
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0; //Initialize counter value to 0

  //Turn on timer2 CTC mode
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS20); //100 - 64 scaler, 0.004 ms increment
  TCCR2B |= (1 << CS21); //110 - 256 scaler, 0.016 ms increment
  TCCR2B |= (1 << CS22); //111 - 1024 scaler, 0.064 ms increment

  //Set compare match register for 8khz increments
  OCR2A = 120; //At 16 MHz, t = (255 + 1) * 0.064 ms = 16.384 ms

  //Enable timer interrupt on compare with OCR2A
  TIMSK2 |= (1 << OCIE2A);

  //Enable interrupts
  sei();
}

byte s_i = 0;
byte s_iRegister = 0;
byte s_iModule = 0;

SIGNAL(TIMER2_COMPA_vect)
{
  ledMatrixController.SendDisplayBytes(displayBuffer, 24);
}


//---------- BUTTONS CLASS ----------

// Purpose: To simplify the use of the pushbuttons on the
// BBGame114 board. The BtnPressed() method/function only
// returns true one time after a buttons is pressed, and it
// is not reset until the button is released.


static const byte buttonPins[4] = { A0, 6, 8, 7 };


void Buttons::Setup()
{
  pinMode(buttonPins[0], INPUT);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(buttonPins[2], INPUT_PULLUP);
  pinMode(buttonPins[3], INPUT_PULLUP);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  pinMode(A7, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  for (byte i = 0; i < 4; i++)
    btnWasNotPressed[i] = true;
}


void Buttons::ReadButtons()
{
  btnDown[0] = (digitalRead(buttonPins[0]) == 1);
  btnDown[1] = (digitalRead(buttonPins[1]) == 0);
  btnDown[2] = (digitalRead(buttonPins[2]) == 0);
  btnDown[3] = (digitalRead(buttonPins[3]) == 0);
}


bool Buttons::BtnPressed(byte i)
{
  bool BtnPressedNow = BtnDown(i);

  if (btnWasNotPressed[i] && BtnPressedNow)
  {
    btnWasNotPressed[i] = false;
    return true;
  }
  else
  {
    if (!BtnPressedNow)
      btnWasNotPressed[i] = true;
    return false;
  }
}


bool Buttons::BtnDown(byte i)
{
  ReadButtons();
  return btnDown[i];
}


bool Buttons::AnyBtnDown()
{
  ReadButtons();
  return (btnDown[0] || btnDown[1] || btnDown[2] || btnDown[3]);
}


byte Buttons::WaitAnyBtnPressed()
{
  while ((btnDown[0] || btnDown[1] || btnDown[2] || btnDown[3]) == 0)
  {
    ReadButtons();
    delay(50);
  }

  byte btnPressed = -1;
  if (btnDown[0]) btnPressed = BTN1;
  if (btnDown[1]) btnPressed = BTN2;
  if (btnDown[2]) btnPressed = BTN3;
  if (btnDown[3]) btnPressed = BTN4;

  while ((btnDown[0] || btnDown[1] || btnDown[2] || btnDown[3]) != 0)
  {
    ReadButtons();
    delay(50);
  }
  return btnPressed;
}


//---------- SOUND CLASS ----------

// Purpose: To output sound tones through the piezoelectric
// speaker on the the BBGame114 board.


void Sound::Setup()
{
  pinMode(SNDPIN, OUTPUT);
  tone = 0;
  duration = 0;
  noisy = false;
}


void Sound::Update(int counter)
{
  if (duration > 0)
  {
    duration--;
    analogWrite(SNDPIN, tone);
    if (noisy && (counter % 2 == 0))
      analogWrite(SNDPIN, 0);
  }
  else
    analogWrite(SNDPIN, 0);
}


void Sound::StartSound(byte _tone, byte _duration, bool _noisy)
{
  tone = _tone;
  duration = _duration;
  noisy = _noisy;
}


//---------- TEXTDISPLAY CLASS ----------

// Purpose: To draw alphanumeric characters and punctuation on the
// BBGame114 display.


TextDisplay::TextDisplay(Display& _display):
  display(_display)
{
}


void TextDisplay::DisplayText(String s, int pos)
{
  byte n = s.length();
  byte np;
  byte const *p;

  for (byte i = 0; i < n; i++)
  {
    GetCharPixelData(s[i], &np, &p);
    for (byte j = 0; j < np; j++)
    {
      display.SetColumnByte(pos + j, pgm_read_byte_near(p + j));
    }
    pos += np;
    if (i < (n - 1)) pos += 1;
  }
}


void TextDisplay::DisplayTextCentered(String s)
{
  int np = GetWidth(s);
  int pos = (display.numCols - np) / 2;
  if (pos < 0) pos = 0;
  DisplayText(s, pos);
}


int TextDisplay::GetWidth(String s)
{
  byte n = s.length();
  byte np;
  byte const *p;

  int pos = 0;
  for (int i = 0; i < n; i++)
  {
    GetCharPixelData(s[i], &np, &p);
    pos += np;
    if (i < (n - 1)) pos += 1;
  }

  return pos;
}


void TextDisplay::ScrollText(String s, int counter)
{
  int pos = display.numCols - (counter % getScrollWidth(s));
  display.Clear();
  DisplayText(s, pos);
}


int TextDisplay::getScrollWidth(String s)
{
  int width = display.numCols + GetWidth(s) + display.numCols;
  return width;
}


const byte PROGMEM font3x4[] = 
{
//  ...  
//  ###  
//  #.#  
//  #.#  
//  #.#  
//  ###  
//  ...  
//  ...  
3, 0x7C, 0x44, 0x7c, 0x00, 0x00, // block for invalid character

//        !    "    #      $    %    &    '    (    )    *    +    ,    -    .    /
//  ...  ...  ...  ...    ...  ...  ....  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  ...  #..  #.#  .#.#   .##  #.#  .#..  #..  .#.  #..  ...  ...  ...  ...  ...  ..#
//  ...  #..  #.#  #####  ##.  ..#  #.#.  #..  #..  .#.  #.#  .#.  ...  ...  ...  ..#
//  ...  #..  ...  .#,#.  ###  .#.  .#..  ...  #..  .#.  .#.  ###  ...  ###  ...  .#.
//  ...  ...  ...  #####  .##  #..  #.#.  ...  #..  .#.  #.#  .#.  ...  ...  ...  #..
//  ...  #..  ...  .#.#.  ##.  #.#  .#.#  ...  .#.  #..  . .  ...  .#.  ...  #..  #..
//  ...  ...  ...  ...    ...  ...  ....  ...  ...  ...  ...  ...  #..  ...  ...  ...
//  ...  ...  ...  ...    ...  ...  ....  ...  ...  ...  ...  ...  ...  ...  ...  ...
2, 0x00, 0x00, 0x00, 0x00, 0x00, // (space)
1, 0x74, 0x00, 0x00, 0x00, 0x00, // !
3, 0x60, 0x00, 0x60, 0x00, 0x00, // "
5, 0x28, 0x7C, 0x28, 0x7C, 0x28, // #
3, 0x34, 0x7C, 0x58, 0x00, 0x00, // $
3, 0x4C, 0x10, 0x64, 0x00, 0x00, // %
4, 0x28, 0x54, 0x28, 0x04, 0x00, // &
1, 0x60, 0x00, 0x00, 0x00, 0x00, // '
2, 0x38, 0x44, 0x00, 0x00, 0x00, // (
2, 0x44, 0x38, 0x00, 0x00, 0x00, // )
3, 0x28, 0x10, 0x28, 0x00, 0x00, // *
3, 0x10, 0x38, 0x10, 0x00, 0x00, // +
2, 0x02, 0x04, 0x00, 0x00, 0x00, // ,
3, 0x10, 0x10, 0x10, 0x00, 0x00, // -
1, 0x04, 0x00, 0x00, 0x00, 0x00, // .
3, 0x0C, 0x10, 0x60, 0x00, 0x00, // /

//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  .X.  .X.  XX.  XX.  X.X  XXX  .X.  XXX  .X.  .X.
//  x.x  xx.  ..x  ..x  x.x  x..  x..  ..x  X.X  X.X
//  X.X  .X.  .XX  .X.  XXX  XX.  XX.  .X.  .X.  .XX
//  X.X  .X.  X..  ..X  ..X  ..X  X.X  .X.  X.X  ..X
//  .X.  .X.  XXX  XX.  ..X  XX.  .X.  .X.  .X.  .x.
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
3, 0x38, 0x44, 0x38, 0x00, 0x00,
2, 0x20, 0x7C, 0x00, 0x00, 0x00,
3, 0x4C, 0x54, 0x34, 0x00, 0x00,
3, 0x44, 0x54, 0x28, 0x00, 0x00,
3, 0x70, 0x10, 0x7C, 0x00, 0x00,
3, 0x74, 0x54, 0x48, 0x00, 0x00,
3, 0x38, 0x54, 0x08, 0x00, 0x00,
3, 0x40, 0x5C, 0x60, 0x00, 0x00,
3, 0x28, 0x54, 0x28, 0x00, 0x00,
3, 0x20, 0x54, 0x38, 0x00, 0x00,

//  ...  ...  ...  ...  ...  ...  ....
//  ...  ...  ..#  ...  #..  ##.  .##.
//  #..  .#.  .#.  ###  .#.  ..#  #..#
//  ...  ...  #..  ...  ..#  .#.  #.##
//  #..  ...  .#.  ###  .#.  ...  #...
//  ...  .#.  ..#  ...  #..  .#.  .##.
//  ...  #..  ...  ...  ...  ...  ....
//  ...  ...  ...  ...  ...  ...  ....
1, 0x28, 0x00, 0x00, 0x00, 0x00, // :
2, 0x02, 0x24, 0x00, 0x00, 0x00, // ;
3, 0x10, 0x28, 0x44, 0x00, 0x00, // <
3, 0x28, 0x28, 0x28, 0x00, 0x00, // =
3, 0x44, 0x28, 0x10, 0x00, 0x00, // >
3, 0x40, 0x54, 0x20, 0x00, 0x00, // ?
4, 0x38, 0x44, 0x54, 0x30, 0x00, // @

//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  .X.  XX.  .xx  XX.  XXX  XXX  .xx  X.X  x..  ..X  X.X  X..  X.x 
//  x.x  x.x  X..  X.X  X..  X..  x..  X.X  x..  ..X  X.X  X..  XxX
//  XxX  XX.  X..  X.X  XXX  XXX  X.x  XXX  x..  ..X  XX.  X..  X.X
//  X.X  X.X  X..  X.X  X..  X..  X.X  X.X  x..  ..X  X.X  X..  X.X
//  X.X  XX.  .XX  XX.  XXX  X..  .XX  X.X  x..  XX.  X.X  XXX  X.X
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ..
//  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...  ...
3, 0x3C, 0x50, 0x3C, 0x00, 0x00,
3, 0x7C, 0x54, 0x28, 0x00, 0x00,
3, 0x38, 0x44, 0x44, 0x00, 0x00,
3, 0x7C, 0x44, 0x38, 0x00, 0x00,
3, 0x7C, 0x54, 0x54, 0x00, 0x00,
3, 0x7C, 0x50, 0x50, 0x00, 0x00,
3, 0x38, 0x44, 0x5C, 0x00, 0x00,
3, 0x7C, 0x10, 0x7C, 0x00, 0x00,
1, 0x7C, 0x00, 0x00, 0x00, 0x00,
3, 0x04, 0x04, 0x78, 0x00, 0x00,
3, 0x7C, 0x10, 0x6C, 0x00, 0x00,
3, 0x7C, 0x04, 0x04, 0x00, 0x00,
3, 0x7C, 0x20, 0x7C, 0x00, 0x00,
 
//  ...  ...  ...  ...   ...  ...  ...  ...  ...  ...  ...  ...  ...
//  XX.  .X.  XX.  .X..  XX.  .XX  XXX  X.X  X.X  X.X  X.X  X.X  XXX
//  X.X  x.X  X.X  X.X.  X.X  X..  .x.  X.X  X.x  X.X  x.x  x.X  ..X
//  X.X  X.X  XX.  X.X.  XX.  .X.  .X.  X.X  X.X  XXX  .X.  .x.  .x.
//  X.X  X.X  X..  X.X.  X X  ..X  .X.  X.X  X.X  XXX  X.X  .X.  X..
//  X.X  .X.  X..  .X.X  X.X  XX.  .X.  .XX  .X.  X.X  X.X  .X.  XXX
//  ...  ...  ...  ....  ...  ...  ...  ...  ...  ...  ...  ...  ...
//  ...  ...  ...  ....  ...  ...  ...  ...  ...  ...  ...  ...  ...
3, 0x7C, 0x40, 0x3C, 0x00, 0x00,
3, 0x38, 0x44, 0x38, 0x00, 0x00,
3, 0x7C, 0x50, 0x20, 0x00, 0x00,
4, 0x38, 0x44, 0x38, 0x04, 0x00,
3, 0x7C, 0x50, 0x2C, 0x00, 0x00,
3, 0x24, 0x54, 0x48, 0x00, 0x00,
3, 0x40, 0x7C, 0x40, 0x00, 0x00,
3, 0x78, 0x04, 0x7C, 0x00, 0x00,
3, 0x78, 0x04, 0x78, 0x00, 0x00,
3, 0x7C, 0x18, 0x7C, 0x00, 0x00,
3, 0x6C, 0x10, 0x6C, 0x00, 0x00,
3, 0x60, 0x1C, 0x60, 0x00, 0x00,
3, 0x4C, 0x54, 0x64, 0x00, 0x00,

//  ...  ...  ...  ...  ...
//  ##.  #..  ##.  .#.  ...
//  #..  #..  .#.  #.#  ...
//  #..  #..  .#.  ...  ...
//  #..  #..  .#.  ...  ...
//  ##.  #..  ##.  ...  ###
//  ...  ...  ...  ...  ...
//  ...  ...  ...  ...  ...
2, 0x7C, 0x44, 0x00, 0x00, 0x00, // [
1, 0x7C, 0x00, 0x00, 0x00, 0x00, // |
2, 0x44, 0x7C, 0x00, 0x00, 0x00, // ]
3, 0x20, 0x40, 0x20, 0x00, 0x00, // ^
3, 0x04, 0x04, 0x04, 0x00, 0x00, // _
};


void TextDisplay::GetCharPixelData(char c, byte *np, byte const **p)
{
  int index = 0;
  if((c >= ' ') && (c <= '_'))
    index = c - ' ' + 1;
  else if((c >= 'a') && (c <= '~'))
    index = c - 'a' + 33 + 1;
  else 
    index = 0;
  index *= 6;
  (*np) = pgm_read_byte_near(font3x4+index);
  (*p) = font3x4 + index + 1;
}
