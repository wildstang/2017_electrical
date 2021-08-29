#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

#define BACKLENGTH 44
#define FRONTLENGTH 40
#define TOPLENGTH 54

#define BACK_STRIP_TOTAL BACKLENGTH + TOPLENGTH // Number of LEDs in strip

#define STRIP_1_DATAPIN    3
#define STRIP_1_CLOCKPIN   2
#define STRIP_2_DATAPIN    6
#define STRIP_2_CLOCKPIN   5
#define STRIP_3_DATAPIN    8
#define STRIP_3_CLOCKPIN   7
#define STRIP_4_DATAPIN    10
#define STRIP_4_CLOCKPIN   9
//#define STRIP_5_DATAPIN    12
//#define STRIP_5_CLOCKPIN   11

Adafruit_DotStar frontLeft = Adafruit_DotStar(FRONTLENGTH, STRIP_1_DATAPIN, STRIP_1_CLOCKPIN, DOTSTAR_BGR);
Adafruit_DotStar frontRight = Adafruit_DotStar(FRONTLENGTH, STRIP_2_DATAPIN, STRIP_2_CLOCKPIN, DOTSTAR_BGR);
Adafruit_DotStar backLeft = Adafruit_DotStar(BACK_STRIP_TOTAL, STRIP_3_DATAPIN, STRIP_3_CLOCKPIN, DOTSTAR_BGR);
Adafruit_DotStar backRight = Adafruit_DotStar(BACK_STRIP_TOTAL, STRIP_4_DATAPIN, STRIP_4_CLOCKPIN, DOTSTAR_BGR);

// Define pattern IDs here
#define DISABLED_PATTERN 1
#define AUTO_PATTERN 2
#define ALLIANCE_PATTERN 3
#define TURBO_PATTERN 4
#define SHOOTER_ON_PATTERN 5
#define SHOOTING_PATTERN 6
#define CLIMBING_PATTERN 7
#define CLIMB_COMPLETE_PATTERN 8
#define LEFT_FEED_JAMMED_PATTERN 9
#define RIGHT_FEED_JAMMED_PATTERN 10
#define INTAKE_PATTERN 11
#define OFF_PATTERN 12

static const byte i2cAddress = 0x10;

// This boolean gets set when we have new data that has been verified to be correct following our checks
boolean dataChanged = true;

// Create the variables that are used for raw data packets
// These should not be altered as they are handled internally by the recieveData function
unsigned char dataByte1 = 0;
unsigned char dataByte2 = 0;
unsigned char dataByte3 = 0;
unsigned char dataByte4 = 0;

// Create the variables that are actually used within the light functions.
// These are the actual variables that should be compared with but they should not be manually altered
unsigned int currentPattern = DISABLED_PATTERN;
unsigned int redValue = 0;
unsigned int greenValue = 0;
unsigned int blueValue = 0;

//color array
int redRGB[] = {255, 0, 0};
int orangeRGB[] = {255, 100, 0};
int yellowRGB[] = {255, 255, 0};
int greenRGB[] = {59, 255, 52};
int greenishBlueRGB[] = {0, 123, 167};
int purpleRGB[] = {70, 30, 70};
int pacmanFood[] = {255, 255, 255};

void setup()
{
  // Start the LED strip
  // Update the strip to ensure that all the LEDs are all off at the beginning
  backLeft.begin();
  backLeft.show();
  backRight.begin();
  backRight.show();
  frontLeft.begin();
  frontLeft.show();
  frontRight.begin();
  frontRight.show();

  // Begin I2C communications as a SLAVE. receiveData() will be called when new data arrives.
  // We call this last to avoid a nasty bug involving the LED initialization code
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveData);

}



void loop()
{
  if (dataChanged)
  {
    if (currentPattern == DISABLED_PATTERN)
    {
      dataChanged = false;
      disabled();
    }
    else if (currentPattern == AUTO_PATTERN)
    {
      dataChanged = false;
      autonomous();
    }
    else if (currentPattern == ALLIANCE_PATTERN)
    {
      dataChanged = false;
      alliance();
    }
    else if (currentPattern == TURBO_PATTERN)
    {
      dataChanged = false;
      turbo();
    }
    else if (currentPattern == SHOOTER_ON_PATTERN)
    {
      dataChanged = false;
      shooterOn();
    }
    else if (currentPattern == SHOOTING_PATTERN)
    {
      dataChanged = false;
      shooting();
    }
    else if (currentPattern == CLIMBING_PATTERN)
    {
      dataChanged = false;
      climbing();
    }
    else if (currentPattern == CLIMB_COMPLETE_PATTERN)
    {
      dataChanged = false;
      climbOver();
    }
    else if (currentPattern == LEFT_FEED_JAMMED_PATTERN)
    {
      dataChanged = false;
      leftJammed();
    }
    else if (currentPattern == RIGHT_FEED_JAMMED_PATTERN)
    {
      dataChanged = false;
      rightJammed();
    }
    else if (currentPattern == INTAKE_PATTERN)
    {
      dataChanged = false;
      intake();
    }
    else if (currentPattern == OFF_PATTERN)
    {
      dataChanged = false;
      allOff();
    }
  }
}

void intake()
{
  blankBackLeft();
  blankBackRight();
  blankFrontLeft();
  blankFrontRight();
}


void allOff()
{
  blankBackLeft();
  blankBackRight();
  blankFrontLeft();
  blankFrontRight();
}

void disabled()
{
    fillBackLeft(redValue, greenValue, blueValue);
    fillFrontLeft(redValue, greenValue, blueValue);
    fillBackRight(redValue, greenValue, blueValue);
    fillFrontRight(redValue, greenValue, blueValue);
  
//  unsigned int i, j;
//
//  for (j = 0; j < 768 * 1; j++)
//  { // 5 cycles of all 384 colors in the wheel
//    for (i = 0; i < backLeft.numPixels(); i++)
//    {
//      // tricky math! we use each pixel as a fraction of the full 384-color
//      // wheel (thats the i / strip.numPixels() part)
//      // Then add in j which makes the colors go around per pixel
//      // the % 768 is to make the wheel cycle around
//      backLeft.setPixelColor(i, Wheel(backLeft, ((i * 768 / backLeft.numPixels()) + j) % 768));
//      backRight.setPixelColor(i, Wheel(backRight, ((i * 768 / backRight.numPixels()) + j) % 768));
//    }
//    backLeft.show();   // write all the pixels out
//    backRight.show();   // write all the pixels out
//
//    if (timedWait(20))
//    {
//      break;
//    }
//  }
}



void alliance()
{
    fillBackLeft(redValue, greenValue, blueValue);
    fillFrontLeft(redValue, greenValue, blueValue);
    fillBackRight(redValue, greenValue, blueValue);
    fillFrontRight(redValue, greenValue, blueValue);

//  int waitTime = 300;
//
//
//  for (int i = 0; i < BACK_STRIP_TOTAL / 2; i++)
//  {
//    float blue = (i * (240 / ((BACK_STRIP_TOTAL - 2) / 2))) + 15;
//    //   int blue = (i * 80) + 15;
//    // strip.Color(blue, green, red);  ==> max 255
//    backLeft.setPixelColor(i, backLeft.Color(0, blue, 0));
//    backLeft.setPixelColor(BACK_STRIP_TOTAL - i - 1, backLeft.Color( 0, blue, 0));
//    backLeft.show();
//    delay(350);
//  }
//  delay(waitTime * 5);
//  for (int i = 0; i < BACK_STRIP_TOTAL; i++)
//  {
//    backLeft.setPixelColor(i, 0, 0, 0);
//  }
//  backLeft.show();
//  backRight.show();
//  frontLeft.show();
//  frontRight.show();
//
//  delay(waitTime);
//
}

void turbo()
{
}

void shooterOn()
{
  int backInterval = BACKLENGTH / 8;
  int frontInterval = FRONTLENGTH / 8;
  
  blankBackLeft();
  blankBackRight();
  blankFrontLeft();
  blankFrontRight();
  
  for (int interval = 0; interval < 4; interval++)
  {
    for (int i = 0; i < backInterval; i++)
    {
      backLeft.setPixelColor((interval * backInterval) + i, 255, 255, 0);
      backLeft.setPixelColor(BACKLENGTH - ((interval * backInterval) + i) - 1, 255, 255, 0);

      backRight.setPixelColor((interval * backInterval) + i, 255, 255, 0);
      backRight.setPixelColor(BACKLENGTH - ((interval * backInterval) + i) - 1, 255, 255, 0);
    }
    for (int i = 0; i < frontInterval; i++)
    {
      frontLeft.setPixelColor((interval * frontInterval) + i, 255, 255, 0);
      frontLeft.setPixelColor(FRONTLENGTH - ((interval * frontInterval) + i) - 1, 255, 255, 0);

      frontRight.setPixelColor((interval * frontInterval) + i, 255, 255, 0);
      frontRight.setPixelColor(FRONTLENGTH - ((interval * frontInterval) + i) - 1, 255, 255, 0);
    }
    
    backLeft.show();
    backRight.show();
    frontLeft.show();
    frontRight.show();
    
    if (timedWait(50))
    {
      return;
    }
  }
}

void shooting() {

  int currentPixel;

  // Attempt to move the RED forward
  for (int i = 0; i < BACKLENGTH / 2; i++) {

    for (int offset = 0; offset < 2; offset++)

    {
      currentPixel = i + offset;
      if (currentPixel < BACKLENGTH / 2)
      {
        backLeft.setPixelColor(currentPixel, i * 12, 255 - 9 * i, i * 12);
        backRight.setPixelColor(currentPixel, i * 15, 255, i * 15);
        // Other side
        backLeft.setPixelColor(BACKLENGTH - currentPixel - 1, i * 12, 255 - 3 * i, i * 12);
        backRight.setPixelColor(BACKLENGTH - currentPixel - 1, i * 15, 255, i * 15);
      }
      if (currentPixel < FRONTLENGTH / 2)
      {
        frontLeft.setPixelColor(currentPixel, 0, 255, 0);
        frontRight.setPixelColor(currentPixel, 0, 255, 0);
        frontLeft.setPixelColor(FRONTLENGTH - currentPixel - 1, 0, 255, 0);
        frontRight.setPixelColor(FRONTLENGTH - currentPixel - 1, 0, 255, 0);
      }
    }


    backLeft.show();
    backRight.show();
    frontLeft.show();
    frontRight.show();


    for (int offset = 0; offset < 2; offset++)
    {
      currentPixel = i + offset;
      if (currentPixel < BACKLENGTH / 2)
      {
        backLeft.setPixelColor(currentPixel, 0, 0, 0);
        backRight.setPixelColor(currentPixel, 0, 0, 0);
        // Other side
        backLeft.setPixelColor(BACKLENGTH - currentPixel - 1, 0, 0, 0);
        backRight.setPixelColor(BACKLENGTH - currentPixel - 1, 0, 0, 0);
      }
      if (currentPixel < FRONTLENGTH / 2)
      {
        frontLeft.setPixelColor(currentPixel, 0, 0, 0);
        frontRight.setPixelColor(currentPixel, 0, 0, 0);
        frontLeft.setPixelColor(FRONTLENGTH - currentPixel - 1, 0, 0, 0);
        frontRight.setPixelColor(FRONTLENGTH - currentPixel - 1, 0, 0, 0);
        delay(10);
      }
    }

    //    fillRange(backLeft, BACKLENGTH, BACK_STRIP_TOTAL, 0, 0, 0);
    //    fillRange(backRight, BACKLENGTH, BACK_STRIP_TOTAL, 0, 0, 0);

    backLeft.show();
    backRight.show();
    frontLeft.show();
    frontRight.show();
    delay(2);
  }
}

void climbing()
{
  {
    // put your main code here, to run repeatedly:
    for (int j = 0; j < 4; j++) //do 2 cycles of chasing
    {
      for (int q = 0; q < 4; q++)
      {
        for (int i = 0; i < BACKLENGTH * 3; i = i + 4)
        {
          frontLeft.setPixelColor(q + i, 0, 255, 255);  //turn every fourth pixel on
          frontRight.setPixelColor(q + i, 0, 255, 255);  //turn every fourth pixel on
          backLeft.setPixelColor(q + i, 0, 255, 255);  //turn every fourth pixel on
          backRight.setPixelColor(q + i, 0, 255, 255);  //turn every fourth pixel on
          //bgr
        }
        frontLeft.show();
        frontRight.show();
        backLeft.show();
        backRight.show();
        delay(100);
        for (int i = 0; i < BACKLENGTH * 3; i = i + 4)
        {
          frontLeft.setPixelColor(q + i, 0);      //turn every fourth pixel off
          frontRight.setPixelColor(q + i, 0);      //turn every fourth pixel off
          backLeft.setPixelColor(q + i, 0 );      //turn every fourth pixel off
          backRight.setPixelColor(q + i, 0);      //turn every fourth pixel off
        }
      }
    }
  }
}

void climbOver()
{
  {
    // put your main code here, to run repeatedly:
    for (int j = 0; j < 4; j++) //do 2 cycles of chasing
    {
      for (int q = 0; q < 4; q++)
      {
        for (int i = 0; i < BACKLENGTH * 3; i = i + 4)
        {
          frontLeft.setPixelColor(q + i, 255, 255, 255);  //turn every fourth pixel on
          frontRight.setPixelColor(q + i, 255, 255, 255);  //turn every fourth pixel on
          backLeft.setPixelColor(q + i, 255, 255, 255);  //turn every fourth pixel on
          backRight.setPixelColor(q + i, 255, 255, 255);  //turn every fourth pixel on
          //bgr
        }
        frontLeft.show();
        frontRight.show();
        backLeft.show();
        backRight.show();
        delay(100);
        for (int i = 0; i < BACKLENGTH * 3; i = i + 4)
        {
          frontLeft.setPixelColor(q + i, 0);      //turn every fourth pixel off
          frontRight.setPixelColor(q + i, 0);      //turn every fourth pixel off
          backLeft.setPixelColor(q + i, 0 );      //turn every fourth pixel off
          backRight.setPixelColor(q + i, 0);      //turn every fourth pixel off
        }
      }
    }
  }
  twinkle(backLeft);
}


void leftJammed()
{
  for (int i = 0; i < 5; i++)
  {
    fillBackLeft(255, 0, 0);
    fillFrontLeft(255, 0, 0);
  
    if (timedWait(200))
    {
      return;
    }
    
    blankBackLeft();
    blankFrontLeft();
  
    if(timedWait(200))
    {
      return;
    }
  }
}

void rightJammed()
{
  for (int i = 0; i < 5; i++)
  {
    fillBackRight(255, 0, 0);
    fillFrontRight(255, 0, 0);
  
    if (timedWait(200))
    {
      return;
    }
    
    blankBackRight();
    blankFrontRight();
  
    if(timedWait(200))
    {
      return;
    }
  }
}


void autonomous()
{
  fillFrontLeft(255, 255, 0);
  fillFrontRight(255, 255, 0);
  fillBackLeft(255, 255, 0);
  fillBackRight(255, 255, 0);
}

// Send a small block of lights down the strip and optionally bounce them back
void scanner(Adafruit_DotStar strip, int r, int g, int b, unsigned int wait, boolean bounce)
{
  unsigned int h = 0, i = 0;
  char j = 0;
  int pos = 0, dir = 1;

  // Erase the strip initially to be sure that we do not leave
  // LEDs on from previous functions
  for (h = 0; h < strip.numPixels(); h++)
  {
    strip.setPixelColor(h, 0);
  }

  for (i = 0; i < ((strip.numPixels() - 1) * 8); i++)
  {
    // Draw 5 pixels centered on pos.  setPixelColor() will clip
    // any pixels off the ends of the strip, no worries there.
    // we'll make the colors dimmer at the edges for a nice pulse
    // look
    strip.setPixelColor(pos - 2, strip.Color(r / 4, g / 4, b / 4));
    strip.setPixelColor(pos - 1, strip.Color(r / 2, g / 2, b / 2));
    strip.setPixelColor(pos, strip.Color(r, g, b));
    strip.setPixelColor(pos + 1, strip.Color(r / 2, g / 2, b / 2));
    strip.setPixelColor(pos + 2, strip.Color(r / 4, g / 4, b / 4));

    strip.show();
    // Wait function with interrupt
    if (true == timedWait(wait))
    {
      break;
    }

    // If we wanted to be sneaky we could erase just the tail end
    // pixel, but it's much easier just to erase the whole thing
    // and draw a new one next time.
    for (j = -2; j <= 2; j++)
    {
      strip.setPixelColor(pos + j, strip.Color(0, 0, 0));
    }
    // Bounce off ends of strip
    pos += dir;
    if (pos < 0)
    {
      pos = 1;
      dir = -dir;
    }
    else if (pos >= strip.numPixels())
    {
      if (bounce == true)
      {
        pos = strip.numPixels() - 2;
        dir = -dir;
      }
      else
      {
        pos = 0;
      }
    }
  }
}


void twinkle(Adafruit_DotStar strip)
{
  int numLit = 6;  // The number of LEDs lit at any time

  // Repeat until there is a requested pattern change
  while (!dataChanged)
  {
    // Initialise an array of flags to 0
    int pixels[40] = {0};
    randomSeed(micros());

    // Pick random pixels (a total of numLit) and set their flag to 1.
    // These are the pixels we light up
    for (int i = 0; i < numLit; i++)
    {
      pixels[random(strip.numPixels())] = 1;
    }

    // Now light up the pixels.  If the flag is a 1, we set it to white, otherwise
    // we turn it off.
    for (int i = 0; i < strip.numPixels(); i++)
    {
      if (pixels[i])
      {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
      }
      else
      {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }

    // Show the pattern
    strip.show();   // write all the pixels out

    if (timedWait(20))
    {
      break;
    }
  }
}

// Sets all pixels off to blank the entire strip.
void blankFrontLeft()
{
  fillFrontLeft(0, 0, 0);
}

void blankFrontRight()
{
  fillFrontRight(0, 0, 0);
}

void blankBackLeft()
{
  fillBackLeft(0, 0, 0);
}

void blankBackRight()
{
  fillBackRight(0, 0, 0);
}

void fillFrontLeft(unsigned int red, unsigned int green, unsigned int blue)
{
  for (unsigned int i = 0; i < frontLeft.numPixels(); i++)
  {
    frontLeft.setPixelColor(i, red, green, blue);
  }
  frontLeft.show();
}

void fillBackLeft(unsigned int red, unsigned int green, unsigned int blue)
{
  for (unsigned int i = 0; i < backLeft.numPixels(); i++)
  {
    backLeft.setPixelColor(i, red, green, blue);
  }
  backLeft.show();
}

void fillFrontRight(unsigned int red, unsigned int green, unsigned int blue)
{
  for (unsigned int i = 0; i < frontRight.numPixels(); i++)
  {
    frontRight.setPixelColor(i, red, green, blue);
  }
  frontRight.show();
}

void fillBackRight(unsigned int red, unsigned int green, unsigned int blue)
{
  for (unsigned int i = 0; i < backRight.numPixels(); i++)
  {
    backRight.setPixelColor(i, red, green, blue);
  }
  backRight.show();
}

// Turns off a specified range of pixels
void blankRange(Adafruit_DotStar strip, unsigned int p_start, unsigned int p_end)
{
  for (unsigned int i = p_start; i < p_end; i++)
  {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

// Turns off a specified range of pixels
void fillRange(Adafruit_DotStar strip, unsigned int p_start, unsigned int p_end, unsigned int red, unsigned int green, unsigned int blue)
{
  for (unsigned int i = p_start; i < p_end; i++)
  {
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}

// This gets called every time we receive new data over the I2C lines
// See I2Cdesign.md for a complete explanation of our utilization
void receiveData(int byteCount)
{
  // Check the byte count to ensure that we are recieving a 5 byte packet
  if (byteCount == 4)
  {
    // Strip off the last byte and read the value
    dataByte1 = (0x000000FF & Wire.read()); // Pattern
    dataByte2 = (0x000000FF & Wire.read()); // Red
    dataByte3 = (0x000000FF & Wire.read()); // Green
    dataByte4 = (0x000000FF & Wire.read()); // Blue

    // Finally set the data to the variables we actually use in loop()
    currentPattern = dataByte1;
    redValue = dataByte2;
    greenValue = dataByte3;
    blueValue = dataByte4;

    // Set the flag to say that we have new data
    dataChanged = true;
  }
  // This should clear out any packets that are bigger than the required 5 bytes
  else if (byteCount > 4)
  {
    // Keep on reading the bytes from the buffer until they are gone. They are simply not used and thus
    // will be thrown away.
    while (Wire.available() > 0)
    {
      Wire.read();
    }
  }
}

// Wait function (Specified Time)
// This is called when we wait to wait in between events that are occuring in our functions.
// Much better than using delay() because we can interrupt the parent function when new data is received.
boolean timedWait(unsigned int waitTime)
{
  unsigned long previousMillis = millis();
  unsigned long currentMillis = millis();

  for (previousMillis; (currentMillis - previousMillis) < waitTime; currentMillis = millis())
  {
    // This may appear to have to effect and the compiler will even throw a warning about it.
    // However, dataChanged is set even when in this loop by the receiveData() function
    if (dataChanged)
    {
      return true;
    }
  }
  return false;
}

// Wait function (infinite)
// This is called when we wait to wait in between events that are occuring in our functions.
// Much better than using delay() because we can interrupt the parent function when new data is received.
// We sit in this function until dataChanged becomes true.
boolean infiniteWaitFunction()
{
  while (!dataChanged)
  {
    // Do nothing. We just sit in here until new data is recieved.
  }
  // If we break out of the while loop, then dataChanged must be true so we can return true
  return true;
}


// Cycle through a rainbow of colors throughout the whole strip
void rainbowWheel(Adafruit_DotStar strip, unsigned int wait)
{
  unsigned int i, j;

  for (j = 0; j < 768 * 1; j++)
  { // 5 cycles of all 384 colors in the wheel
    for (i = 0; i < strip.numPixels(); i++)
    {
      // tricky math! we use each pixel as a fraction of the full 384-color
      // wheel (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 768 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel(strip, ((i * 768 / strip.numPixels()) + j) % 768));
    }
    strip.show();   // write all the pixels out

    if (true == timedWait(wait))
    {
      break;
    }
  }
}

// Credit to adafruit for this function. This is used in calculating a
// "color wheel" for the rainbow function
unsigned long Wheel(Adafruit_DotStar strip, unsigned int WheelPos)
{
  byte r = 0, g = 0, b = 0;

  switch (WheelPos / 256)
  {
    case 0:
      r = 255 - WheelPos % 256;   // Red down
      g = WheelPos % 256;      // Green up
      b = 0;                  // Blue off
      break;
    case 1:
      g = 255 - WheelPos % 256;  // Green down
      b = WheelPos % 256;      // Blue up
      r = 0;                  // Red off
      break;
    case 2:
      b = 255 - WheelPos % 256;  // Blue down
      r = WheelPos % 256;      // Red up
      g = 0;                  // Green off
      break;
  }
  return (strip.Color(r, g, b));
}

//SOME NEW FUNCTIONS (rainbow theater chase with the theater chase function)
void rainbowTheaterChase()
{
  theaterChase(backLeft.Color(0, 255, 255), 50); //folly: 255, 0, 79 (red)
  theaterChase(backLeft.Color(orangeRGB[2], orangeRGB[1], orangeRGB[0]), 50); //amber: 255, 191, 0 (orange)
  theaterChase(backLeft.Color(yellowRGB[0], yellowRGB[1], yellowRGB[2]), 50); //gold: 255, 215, 0 (yellow)
  theaterChase(backLeft.Color(greenRGB[0], greenRGB[1], greenRGB[2]), 50); //screamin' green: 118, 255, 122 (light green)
  theaterChase(backLeft.Color(greenishBlueRGB[0], greenishBlueRGB[1], greenishBlueRGB[2]), 50); //cerulean: 0, 123, 167 (greenish blue / teal)
  theaterChase(backLeft.Color(purpleRGB[0], purpleRGB[1], purpleRGB[2]), 50); //plum: 221, 160, 221 (purple)
}

void theaterChase(int color, int wait)
{
  for (int j = 0; j < 2; j++) //do 2 cycles of chasing
  {
    for (int q = 0; q < 4; q++)
    {
      for (int i = 0; i < backLeft.numPixels(); i = i + 4)
      {
        backLeft.setPixelColor(q + i, color);  //turn every fourth pixel on
      }
      backLeft.show();
      delay(wait);
      for (int i = 0; i < backLeft.numPixels(); i = i + 4)
      {
        backLeft.setPixelColor(q + i, 0);      //turn every fourth pixel off
      }
    }
  }
}


