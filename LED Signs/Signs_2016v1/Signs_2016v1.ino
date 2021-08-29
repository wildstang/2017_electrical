/*
WildStang Signs 2013
By: Josh Smith and Steve Garward
*/


#include "LPD8806.h"
#include "SPI.h"
#include "Wire.h"
#include "patterns.h"
#include "EEPROM.h"

//The number of LEDs on the sign
#define NUM_PIXELS_TOTAL 52
#define HALF_LENGTH NUM_PIXELS_TOTAL / 2

#define MAX_HEIGHT 20

//#define WS_DEBUG

// This will use the following pins:
// #define Data (SDA):  2
// #define Clock (SCL): 3
LPD8806 strip = LPD8806(NUM_PIXELS_TOTAL, 2, 3);

#define PACKET_DATA_LENGTH 8

int packetData[PACKET_DATA_LENGTH] = {-1};

// This is the pattern that should be displayed
short pattern = 0;
boolean patternChanged = false;

#define SIGN_1 11
#define SIGN_2 13
#define SIGN_3 15

int start;

void setup()
{
//Each board now its sign address burned in
//   pinMode(4, INPUT);
//   pinMode(5, INPUT);
//   pinMode(6, INPUT);
//   digitalWrite(4, HIGH);
//   digitalWrite(5, HIGH);
//   digitalWrite(6, HIGH);

   // Determine the slave address
   // The pins are pulled high through the internal pullup, so we have
   // tied the hardware address pin low.  For the pin that is low, set the 
   // I2C address for this device

   Wire.begin(EEPROM.read(0));
   Wire.onReceive(receiveData);

   // Start up the LED strip
   strip.begin();

   // Update the strip, to start they are all 'off'
   strip.show();


#ifdef WS_DEBUG
   Serial.begin(9600);
#endif
}


void loop()
{

   if(hasPatternChanged ())
   {
      clearPatternChanged();
      switch (pattern)
      {
      case PATTERN_BLANK:
         blankStrip();
            break;
      case PATTERN_RAINBOW:
         while (!hasPatternChanged())
         {
            rainbowFromCenter(10);
         }
         break;
      case PATTERN_RED_FILL:
         colorFlowDown(127, 0, 0);
         // Set colour to stay on red
         solidColor(127, 0, 0);
         break;
      case PATTERN_BLUE_FILL:
         colorFlowDown(0, 0, 127);
         // Set colour to stay on blue
         solidColor(0, 0, 127);
         break;
      case PATTERN_RED_FILL_SHIMMER:
         colorFlowDownShimmer(127, 0, 0);
         // Set colour to stay on blue
         solidColor(127, 0, 0);
         break;
      case PATTERN_BLUE_FILL_SHIMMER:
         colorFlowDownShimmer(0, 0, 127);
         // Set colour to stay on blue
         solidColor(0, 0, 127);
         break;
// Not needed
//      case PATTERN_RED_FILL_TILT:
//         break;
//      case PATTERN_BLUE_FILL_TILT:
//         break;
      case PATTERN_TWINKLE:
         while (!hasPatternChanged())
         {
            twinkle();
         }
         break;
      case PATTERN_EQ_METER:
         while (!hasPatternChanged())
         {
            showLevel(averageSoundLevel());
         }
         break;
      case PATTERN_RAINBOW_PARTY:
         while (!hasPatternChanged())
         {
            rainbowPartyMode();
         }
         break;
      case PATTERN_EQ_EXPLODE:
         start = millis();
         
         // Glow orange and red
         while (millis() - start < 3000)
         {
            twinkle(50, 50, 0, 127, 127, 127, 40, 6);
         }
         
         // Glow bright red
         start = millis();
         while (millis() - start < 2000)
         {
            twinkle(50, 0, 0, 127, 127, 127, 20, 15);
         }
         
         // Break up pattern
         explode(127, 0, 0);
         break;

      default:
         blankStrip();
         break;
      }
   }
}



void clearPatternChanged()
{
   patternChanged = false;
}

boolean hasPatternChanged()
{
   return patternChanged;
}

void receiveData(int numBytes)
{
   byte temp;

   // Read the first byte
   if (Wire.available() > 0)
   {
      temp = Wire.read();
   }

   if (temp != pattern)
   {
      pattern = temp;
      patternChanged = true;
   }

      //      pattern = temp;
      // If this is not a pattern, read the data into the packet data array for the
      // function to process
      int i = 0;

      // We already have the first byte
//      packetData[0] = temp;

      // Now read the rest of the bytes
      while (Wire.available() > 0 && i < PACKET_DATA_LENGTH)
      {
         packetData[i++] = Wire.read();
      }

      // If we have not filled the buffer, mark the end of data with -1
      if (i < PACKET_DATA_LENGTH)
      {
         packetData[i] = -1;
      }
//      patternChanged = true;
   
}

boolean timedWait(int waitTime)
{
   unsigned long previousMillis = millis();
   unsigned long currentMillis = millis();
   for(previousMillis; (currentMillis - previousMillis) < waitTime; currentMillis = millis())
   {
      if (patternChanged == true)
      {
         return true;
      }
   }
   return false;
}

//Moves a set of LEDs across the strip and bounces them
void scanner(uint8_t r, uint8_t g, uint8_t b, int wait, boolean bounce)
{
  int h, i, j;

  int pos = 0;
  int dir = 1;

  // Erase the strip initially to be sure that we do not leave
  // LEDs on from previous functions
  for(h=0; h < strip.numPixels(); h++)
  {
    strip.setPixelColor(h, 0);
  }

  for(i=0; i<((strip.numPixels()-1) * 8); i++)
  {
    // Draw 5 pixels centered on pos.  setPixelColor() will clip
    // any pixels off the ends of the strip, no worries there.
    // we'll make the colors dimmer at the edges for a nice pulse
    // look
    strip.setPixelColor(pos - 2, strip.Color(r/4, g/4, b/4));
    strip.setPixelColor(pos - 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos, strip.Color(r, g, b));
    strip.setPixelColor(pos + 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos + 2, strip.Color(r/4, g/4, b/4));

    strip.show();
    //Wait function with interrupt
    if (true == timedWait(wait))
    {
      break;
    }

    // If we wanted to be sneaky we could erase just the tail end
    // pixel, but it's much easier just to erase the whole thing
    // and draw a new one next time.
    for(j=-2; j<= 2; j++) 
    strip.setPixelColor(pos+j, strip.Color(0,0,0));
    // Bounce off ends of strip
    pos += dir;
    if(pos < 0)
    {
      pos = 1;
      dir = -dir;
    }
    else if(pos >= strip.numPixels())
    {
      if(bounce == true)
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


void rainbowWheelStrobe(int onWait, int offWait)
{
  uint16_t i, j;

  for (j=0; j < 384 * 5; j++)
  {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++)
    {
      // tricky math! we use each pixel as a fraction of the full 384-color
      // wheel (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel(((i * 384 / strip.numPixels()) + j) % 384));
    }
    strip.show();

    if (true == timedWait(onWait))
    {
      break;
    }
    
    for(i=0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.show();

    if (true == timedWait(offWait))
    {
      break;  
    }
  }
}

/*
 * Sets all pixels off to blank the entire strip.
 */
void blankStrip()
{
   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      strip.setPixelColor(i, 0);
   }
   strip.show();
}


/**
 * Creates a rainbow that moves from the center out to both ends.  This does a single rainbow cycle,
 * and needs to be called multiple times to perform multiple rainbow cycles.
 *
 * This function will be interrupted by a pattern change.
 */
void rainbowFromCenter(uint8_t wait)
{
   int i, j;
   uint32_t color;

   int center = HALF_LENGTH / 2;

   //blankStrip();

   // Note: Due to the layout of the strip, if we count up with i, it goes towards the center,
   // but if we count down, we go from the center towards the end
   i = 384 - 1;

   // Iterate through all colours
   while(i >= 0 && !hasPatternChanged())
   {
      // Loop through the pixels, from the center out
      for (j=0; j < center; j++)
      {
         // Get the colour for this position
         color = Wheel( ((j * 384 / center / 2) + i) % 384);

         // Set the pixels on each side of the ceneter, on both strips
         // TODO: Change to use the height, not strip position
         strip.setPixelColor(center - 1 - j, color);
         strip.setPixelColor(center + j, color);
         strip.setPixelColor(NUM_PIXELS_TOTAL - center - 1 - j, color);
         strip.setPixelColor(NUM_PIXELS_TOTAL - center + j, color);
      }  

      strip.show();   // write all the pixels out
      // TODO: Change to timedWait()
      timedWait(wait);

      if (hasPatternChanged())
      {
         return;
      }
      i--;
   }
}


void solidColor(uint8_t red, uint8_t green, uint8_t blue)
{
   // Set all pixels to the same colour
   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      strip.setPixelColor(i, strip.Color(red, green, blue));
   }

   // Show the pixels
   strip.show();

   // Now use a timed wait that can be interrupted by a pattern change to keep the colour set
   while (!hasPatternChanged())
   {
      // Do nothing - use a timed wait to free up the processor
      timedWait(10);
   }
}

void doubleRainbow(uint8_t wait)
{
   blankStrip();
   int i, j;
   uint32_t color;

   for (i = (384 * 5) - 1; i >= 0 ; i--)
   {
      // 3 cycles of all 384 colors in the wheel
      for (j=0; j < HALF_LENGTH; j++)
      {
         color = Wheel( ((j * 384 / HALF_LENGTH) + i) % 384);   // Wheel( (i + j) % 384);
         strip.setPixelColor(j, color);
         strip.setPixelColor(NUM_PIXELS_TOTAL - 1 - j, color);
      }  

      strip.show();   // write all the pixels out
      if (true == timedWait(wait))
      {
        break;
      }
   }
}

/*
 * Chase the colour down the strip.  This leaves a diminishing trail of pixels behind it, the
 * length of which is `
 */
void colorChaseTrail(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait, uint8_t trailLength)
{
   // Clear the last pattern
   blankStrip();

   uint8_t dimRed = red / trailLength;
   uint8_t dimGreen = green / trailLength;
   uint8_t dimBlue = blue / trailLength;
   int currentPixel;

   uint32_t pixels[] = {0};
   uint32_t trailPattern[trailLength + 1];

   // Set up the trail pattern

   trailPattern[0] = 0;
   for (int i = 1; i <= trailLength; i++)
   {
      int index = trailLength + 1 - i;
      trailPattern[index] = strip.Color(max(red - (dimRed * (i - 1)), 0), max(green - (dimGreen * (i - 1)), 0), max(blue - (dimBlue * (i - 1)), 0));
   }

   // reset colour array
   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      pixels[i] = 0;
   }

   int lastStart = 0;

   // Fill in colours
   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      lastStart = i - trailLength;

      for (int j = 0; j <= trailLength; j++)
      {
         currentPixel = lastStart + j;

         if (currentPixel < 0)
         {
            // Work out position at end
            currentPixel = NUM_PIXELS_TOTAL + currentPixel;  // subtracts from length to get index
         }
         strip.setPixelColor(currentPixel, trailPattern[j]);
      }

      strip.show();
      if (true == timedWait(wait))
      {
        break;
      }
   }
}

void colorChase(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait)
{
   // Clear the last pattern
   blankStrip();

   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      // Set the pixel colour
      strip.setPixelColor(i, strip.Color(red, green, blue));

      // If we are the start of the strip, loop around to the end of the strip
      if (i == 0)
      {
         strip.setPixelColor(NUM_PIXELS_TOTAL - 1, 0);
      }
      else
      {
         strip.setPixelColor(i - 1, 0);
      }

      strip.show();
      if (true == timedWait(wait))
      {
        break;
      }
   }
}

void twinkle()
{
   int numLit = 6;  // The number of LEDs lit at any time

   // Repeat until there is a requested pattern change
   while (!hasPatternChanged())
   {
      // Initialise an array of flags to 0
      int pixels[NUM_PIXELS_TOTAL] = {0};
      randomSeed(micros());

      // Pick random pixels (a total of numLit) and set their flag to 1.
      // These are the pixels we light up
      for (int i = 0; i < numLit; i++)
      {
         pixels[random(NUM_PIXELS_TOTAL)] = 1;
      }

      // Now light up the pixels.  If the flag is a 1, we set it to white, otherwise
      // we turn it off.
      for (int i=0; i < strip.numPixels(); i++)
      {
         if (pixels[i])
         {
            strip.setPixelColor(i, strip.Color(127, 127, 127));
         }
         else
         {
            strip.setPixelColor(i, strip.Color(0, 0, 0));
         }
      }

      // Show the pattern
      strip.show();   // write all the pixels out
      if (true == timedWait(20))
      {
        break;
      }
   }
}


void twinkleFade(uint8_t red, uint8_t green, uint8_t blue, int fadeFactor, int wait, int numLit, int numTimes)
{
   uint8_t redFade = red / fadeFactor;
   uint8_t greenFade = green / fadeFactor;
   uint8_t blueFade = blue / fadeFactor;
   
   uint8_t currentRed = red;
   uint8_t currentGreen = green;
   uint8_t currentBlue = blue;
   
   uint32_t currentColor = strip.Color(currentRed, currentGreen, currentBlue);
   uint32_t black = strip.Color(0, 0, 0);
   
   // While we are not total black...
   while (currentRed || currentGreen || currentBlue)
   {
      // Repeat until there is a requested pattern change
      for (int count = 0; count < numTimes; count++)
      {
         // Initialise an array of flags to 0
         int pixels[NUM_PIXELS_TOTAL] = { 0 };
         randomSeed(micros());
   
         // Pick random pixels (a total of numLit) and set their flag to 1.
         // These are the pixels we light up
         for (int i = 0; i < numLit; i++)
         {
            pixels[random(NUM_PIXELS_TOTAL)] = 1;
         }
   
         // Now light up the pixels.  If the flag is a 1, we set it to white, otherwise
         // we turn it off.
         for (int i=0; i < strip.numPixels(); i++)
         {
            if (pixels[i])
            {
               strip.setPixelColor(i, currentColor);
            }
            else
            {
               strip.setPixelColor(i, black);
            }
         }
   
         // Show the pattern
         strip.show();   // write all the pixels out
   
         if (timedWait(wait))
         {
            break;
         }
      }
      currentRed -= redFade;
      if (currentRed < 0)
      {
         currentRed = 0;
      }
      
      currentGreen -= greenFade;
      if (currentGreen < 0)
      {
         currentGreen = 0;
      }
      currentBlue -= blueFade;
      if (currentBlue < 0)
      {
         currentBlue = 0;
      }
      currentColor = strip.Color(currentRed, currentGreen, currentBlue);
   }
}


void twinkle(uint8_t bgred, uint8_t bggreen, uint8_t bgblue, uint8_t fgred, uint8_t fggreen, uint8_t fgblue, int wait, int numLit)
{
   int pixels[NUM_PIXELS_TOTAL] = {0};
   randomSeed(micros());

   for (int i = 0; i < numLit; i++)
   {
      pixels[random(NUM_PIXELS_TOTAL)] = 1;
   }

   for (int i=0; i < strip.numPixels(); i++)
   {
      if (pixels[i])
      {
         strip.setPixelColor(i, strip.Color(fgred, fggreen, fgblue));
      }
      else
      {
         strip.setPixelColor(i, strip.Color(bgred, bggreen, bgblue));
      }
   }  
   strip.show();   // write all the pixels out

   timedWait(wait);
}

void rainbowTwinkle(int times, int wait)
{
   int numLit = 15;
   uint16_t j = 0;
   
   for (int i = 0; i < times; i++)
   {  
      int pixels[NUM_PIXELS_TOTAL] = {0};
      randomSeed(micros());
   
      for (int i = 0; i < numLit; i++)
      {
         pixels[random(NUM_PIXELS_TOTAL)] = 1;
      }
      
      if (j >= 384)
      {
        j = 0;
      }

      for (int i=0; i < strip.numPixels(); i++)
      {
         if (pixels[i])
         {
           strip.setPixelColor(i, Wheel(((i * 384 / strip.numPixels()) + j) % 384));
         }
         else
         {
           strip.setPixelColor(i, strip.Color(0, 0, 0));
         }
      }  
      strip.show();   // write all the pixels out
      j = j+10;
      if (true == timedWait(wait))
      {
        break;
      }
   }
}


void colorFlowDown(uint8_t red, uint8_t green, uint8_t blue)
{
   blankStrip();

   int height;
   int pixels[4];

   for (int count = 19; count >= 0; count--)
   {
#ifdef WS_DEBUG
      Serial.print("count = ");
      Serial.println(count);
#endif
      for (height = count; height <= 19; height++)
      {
#ifdef WS_DEBUG
         Serial.print("height = ");
         Serial.println(height);
#endif
         heightToPixels(height, pixels);
         // Loop through pixels returned
         for (int i = 0; i < 4 && pixels[i] > -1; i++)
         {
#ifdef WS_DEBUG
            Serial.print("pixels[i] = ");
            Serial.println(pixels[i]);
#endif
            strip.setPixelColor(pixels[i], strip.Color(red, green, blue));
         }
      }
      strip.show();
      // TODO: Change to timedWait()
      timedWait(100);
   }   
}


void colorFlowDownShimmer(uint8_t red, uint8_t green, uint8_t blue)
{
   blankStrip();

   int height;
   int pixels[4];
   int shimmerRow1[4];
   int shimmerRow2[4];

   for (int count = 19; count >= 0; count--)
   {
      for (height = count; height <= 19; height++)
      {
         heightToPixels(height, pixels);
         // Loop through pixels returned
         for (int i = 0; i < 4 && pixels[i] > -1; i++)
         {
            strip.setPixelColor(pixels[i], strip.Color(red, green, blue));
         }
         strip.show();
         if (height == count || height == count + 1)
         {
            heightToPixels(count, shimmerRow1);
            heightToPixels(count + 1, shimmerRow2);

            for (int j = 1; j < 5; j++)
            {
               // Shimmer
               for (int i = 0; i < 4; i++)
               {
                  if (shimmerRow1[i] > -1)
                  {
                     if (random(50) > 25)
                     {
                        strip.setPixelColor(shimmerRow1[i], strip.Color(127, 127, 127));
                     }
                     else
                     {
                        strip.setPixelColor(shimmerRow1[i], strip.Color(0, 0, 0));
                     }
                  }
                  if (shimmerRow2[i] > -1)
                  {
                     if (random(50) > 25)
                     {
                        strip.setPixelColor(shimmerRow2[i], strip.Color(127, 127, 127));
                     }
                     else
                     {
                        strip.setPixelColor(shimmerRow2[i], strip.Color(0, 0, 0));
                     }
                  }
               }
               strip.show();
               timedWait(5);
            }
         }
      }
      //// TODO: Change to timedWait()
      timedWait(50);

   }

}


void heightToPixels(int height, int *pixels)
{
   pixels[0] = pixels[1] = pixels[2] = pixels[3] = -1;

#ifdef WS_DEBUG
   Serial.println("In heightToPixels()");
   Serial.print("height = ");
   Serial.println(height);
#endif

   if (height >= 0 && height < 20)
   {
      pixels[0] = height;
      pixels[1] = (NUM_PIXELS_TOTAL - 1 - height);
#ifdef WS_DEBUG
      Serial.print("pixels[0] = ");
      Serial.println(pixels[0]);
      Serial.print("pixels[1] = ");
      Serial.println(pixels[1]);
#endif
   }

   // Special cases
   if (height == 13)
   {
#ifdef WS_DEBUG
      Serial.println("height = 13");
      Serial.println("pixels[2] = 26");
#endif
      pixels[2] = 26;
   }
   else if (height == 19)
   {
#ifdef WS_DEBUG
      Serial.println("height = 19");
      Serial.println("pixels[2] = 20");
#endif
      pixels[2] = 20;
   }
   else if (height > 13 && height < 19)
   {
      // Top row: max height + first position we care about - height = position
      pixels[2] = 18 + 21 - height;

      // Bottom row: first pixel we care about - max height + height = position
      pixels[3] = 31 - 18 + height;
#ifdef WS_DEBUG
      Serial.println("height > 13 && height < 19");
      Serial.print("pixels[2] = ");
      Serial.println(pixels[2]);
      Serial.print("pixels[3] = ");
      Serial.println(pixels[3]);
#endif
   }
}

int stripPositionToHeight(int pos)
{
   int height;

   // Right side of vertical stroke - LEDs 0-19
   if (pos < 20)
   {
      height = pos;
   }
   // Left side of vertical stroke - LEDs 32-51
   else if (pos > 31)
   {
      height = 19 - (pos - 32);
   }
   // Top of top stroke - LEDs 20-25
   else if (pos >= 20 && pos <= 25)
   {
      height = 19 - (pos - 20);
   }
   // Bottom of top stroke - LEDs 26-31
   else if (pos >= 26 && pos <= 31)
   {
      // 18 is not a mistake
      // we also want to work up, not down
      height = 18 - (31 - pos);
   }

   return height;
}



uint32_t Wheel(uint16_t WheelPos)
{
   byte r, g, b;
   switch(WheelPos / 128)
   {
   case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
   case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
   case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
   }
   return(strip.Color(r,g,b));
}

void showLevel(int p_level)
{
   int height;
   int pixels[4];
   
   for (int i=0; i < strip.numPixels(); i++)
   {
      strip.setPixelColor(i, 0);  // turn all pixels off
   }
   
   for (height = 0; height < p_level; height++)
   {
      heightToPixels(height, pixels);
      
      for (int i=0; i < 4 && pixels[i] > -1; i++)
      {
         strip.setPixelColor(pixels[i], getLevelColor(height));
      }
   }
   strip.show();
}

uint32_t getLevelColor(int level)
{
   if (level < 10)
   {
      // Green
      return strip.Color(0,127,0);
   }
   else if (level >= 10 && level < 16)
   {
      // Orange
      return strip.Color(127,127,0);
   }
   else if (level > 16)
   {
      // Red
      return strip.Color(127,0,0);
   }
}

int averageSoundLevel()
{
   int average = 0;
   
   average = (packetData[0] + packetData[1]) / 2;
   
   if (average < 0)
   {
      average = 0;
   }
   else if (average > 20)
   {
      average = 20;
   }
   
   return average;
}

void rainbowPartyMode()
{
   int p_level;
   int height;
   int pixels[4];
   int i, j;
   uint32_t color;

   int center = HALF_LENGTH / 2;

   for (int i=0; i < strip.numPixels(); i++)
   {
      strip.setPixelColor(i, 0);  // turn all pixels off
   }

   // Note: Due to the layout of the strip, if we count up with i, it goes towards the center,
   // but if we count down, we go from the center towards the end
   i = 384 - 1;

   // Iterate through all colours
   while(i >= 0 && !hasPatternChanged())
   {
      // Loop through the pixels, from the center out
      for (j=0; j < center; j++)
      {
         // Get the colour for this position
         color = Wheel( ((j * 384 / center / 2) + i) % 384);

         // Set the pixels on each side of the ceneter, on both strips
         // TODO: Change to use the height, not strip position
         strip.setPixelColor(center - 1 - j, color);
         strip.setPixelColor(center + j, color);
         strip.setPixelColor(NUM_PIXELS_TOTAL - center - 1 - j, color);
         strip.setPixelColor(NUM_PIXELS_TOTAL - center + j, color);
      }
      
      p_level = averageSoundLevel();
      
      for (height = 20; height >= p_level*2; height--)
      {
         heightToPixels(height, pixels);
      
         for (int i=0; i < 4 && pixels[i] > -1; i++)
         {
            strip.setPixelColor(pixels[i], 0);
         }
      }

      strip.show();   // write all the pixels out

      if (hasPatternChanged())
      {
         return;
      }
      i--;
   }
}

void setStripColor(uint8_t red, uint8_t green, uint8_t blue)
{
   uint32_t color = strip.Color(red, green, blue);

   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      strip.setPixelColor(i, color);
   }
}

void explode(uint8_t red, uint8_t green, uint8_t blue)
{
   uint32_t white = strip.Color(127, 127, 127);
   uint32_t dark = strip.Color(30, 30, 0);
   
   int center = MAX_HEIGHT / 2;
   int pixelsUpper[4];
   int pixelsLower[4];
   
   solidColor(red, green, blue);

   // Reverse flow fill from the center out, with a white inside line
   // fill the center with a twinkle fill. Good Lord...
   for (int count = center - 1; count >= 0; count--)
   {
      // Don't black out the center row
      if (count != center - 1)
      {
         // Black out the previous rows
         heightToPixels(count - 1, pixelsLower);
         heightToPixels(count, pixelsUpper);
         for (int i = 0; i < 4 && pixelsLower[i] > -1; i++)
         {
            strip.setPixelColor(pixelsLower[i], dark);
         }
         for (int i = 0; i < 4 && pixelsUpper[i] > -1; i++)
         {
            strip.setPixelColor(pixelsUpper[i], dark);
         }
      }

      // Make the center line white
      heightToPixels(count, pixelsLower);
      heightToPixels(count + 1, pixelsUpper);
      for (int i = 0; i < 4 && pixelsLower[i] > -1; i++)
      {
         strip.setPixelColor(pixelsLower[i], white);
      }
      for (int i = 0; i < 4 && pixelsUpper[i] > -1; i++)
      {
         strip.setPixelColor(pixelsUpper[i], white);
      }
   }
   
   // Now do a slow twinkle flow down effect
   for (int i = MAX_HEIGHT; i >= 0; i--)
   {
      // Fill with a low pixel count twinkle
      twinkleFade(127, 0, 0, 10, 70, 3, 4);
   }
}




void explode(int red, int green, int blue)
{
   int white = strip.Color(127, 127, 127);
   int dark = strip.Color(30, 0, 0);

   int center = 20 / 2;
   int pixelsUpper[4];
   int pixelsLower[4];

   solidColor(127, 127, 127);

   timedWait(100);

   // Set all pixels to the same colour
   for (int i = 0; i < NUM_PIXELS_TOTAL; i++)
   {
      strip.setPixelColor(i, strip.Color(red, green, blue));
   }

   int lower;
   int upper;
   // Reverse flow fill from the center out, with a white inside line
   // fill the center with a twinkle fill. Good Lord...
   for (int count = 1; count < center; count++)
   {
      randomSeed(micros());
      
      lower = center - count;
      upper = center + count - 1;

      // Black out the previous rows
      heightToPixels(lower + 1, pixelsLower);  // account for center
      heightToPixels(upper - 1, pixelsUpper);

      for (int i = 0; i < 4 && pixelsLower[i] > -1; i++)
      {
         if (random(30) > 15)
         {
            strip.setPixelColor(pixelsLower[i], white);
         }
         else
         {
            strip.setPixelColor(pixelsLower[i], dark);
         }
      }

      for (int i = 0; i < 4 && pixelsUpper[i] > -1; i++)
      {
         if (random(30) > 15)
         {
            strip.setPixelColor(pixelsUpper[i], white);
         }
         else
         {
            strip.setPixelColor(pixelsUpper[i], dark);
         }
      }

      // Make the center line white
      heightToPixels(lower, pixelsLower);
      heightToPixels(upper, pixelsUpper);
      for (int i = 0; i < 4 && pixelsLower[i] > -1; i++)
      {
         strip.setPixelColor(pixelsLower[i], white);
      }
      for (int i = 0; i < 4 && pixelsUpper[i] > -1; i++)
      {
         strip.setPixelColor(pixelsUpper[i], white);
      }

      strip.show();
      timedWait(30);
   }

   // Now do a slow twinkle fade out effect
   for (int i = 0; i < 30; i++)
   {
      twinkle(40, 0, 0, 127, 80, 80, 50, 6);
   }
   twinkleFade(127, 0, 0, 5, 70, 6, 8);
}



