
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <avr/pgmspace.h> //allows use of progmem

//define ports for LED panel
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A4
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define WHITE 0xFFFF

//define bitmap for outline of tarheel
static const unsigned char tarheel  [] PROGMEM{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000011, B11111100, B01110000,
B00111111, B11111110, B00000011, B00000000, 
B00100000, B00000000, B00000000, B10111000, 
B01000000, B00000000, B00000000, B10111000, 
B10000000, B00000000, B00000000, B10000000, 
B10000000, B00000000, B00000000, B01011100,
B10000000, B00000000, B00000000, B01011100,
B10000000, B00000000, B00000000, B00100000,
B10000001, B11111100, B00000000, B00101110,
B10001110, B00000011, B00000000, B00101110,
B01110000, B00000000, B11000000, B00100000,
B00000000, B00000000, B00111000, B01011110,
B00000000, B00000000, B00000111, B10010010, 
B00000000, B00000000, B00000000, B00001110,
};

//vars for volume detection
const int sampleWindow = 400; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
unsigned int previousVolMeasure = 0;
unsigned int currentVolMeasure = 0;

void setup() {
 Serial.begin(9600); 
 matrix.begin(); 
 matrix.drawBitmap(0, 0, tarheel, 32, 15, BLUE); 
}

void loop() {
  matrix.drawBitmap(0, 0, tarheel, 32, 15, BLUE);

  //matrix.fillScreen(0x0000);
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
 
  while (millis() - startMillis < sampleWindow)
  {
      sample = analogRead(11); 
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;
   Serial.println(peakToPeak);

   /*These two variables may need to be modified if the project is placed in an environment with different levels of
   background noise/voltage. It assumes readings will be between 50 and 200 (readings were consistent with a human voice 
   speaking at normal volume in an otherwise quiet room.) */

   unsigned int maxSignal = 200;
   unsigned int minSignal = 50; 

   
   if(peakToPeak < minSignal + 30){
    peakToPeak = minSignal;
   }
   if(peakToPeak > maxSignal){
    peakToPeak = maxSignal;
   }
   previousVolMeasure = currentVolMeasure;
   currentVolMeasure = map(peakToPeak, minSignal, maxSignal, 0, 27); //convert range of signal (0 to 1024) to range of display (0 to 27)


  //fill in outline based on volume.
  //if readings has increased since the last measurement, draw more lines to fill in shape of foot.
  if(currentVolMeasure > previousVolMeasure){
  switch(currentVolMeasure){
    case 27:
      matrix.drawPixel(29,13,BLUE);
    case 26:
      matrix.drawPixel(28,13,BLUE);
    case 25:
      matrix.drawLine(25,8,25,11,BLUE);
    case 24:
      matrix.drawLine(24,6,24,12,BLUE);
    case 23:
      matrix.drawLine(23,2,23,12,BLUE);
    case 22:
      matrix.drawLine(22,2,22,12,BLUE);
    case 21:
      matrix.drawLine(21,2,21,12,BLUE);
    case 20:
      matrix.drawLine(20,2,20,11,BLUE);
    case 19:
      matrix.drawLine(19,2,19,11,BLUE);
    case 18:
      matrix.drawLine(18,2,18,11,BLUE);
    case 17:
      matrix.drawLine(17,2,17,10,BLUE);
    case 16:
      matrix.drawLine(16,2,16,10,BLUE);
    case 15:
      matrix.drawLine(15,2,15,9,BLUE);
    case 14:
      matrix.drawLine(14,3,14,9,BLUE);
    case 13:
      matrix.drawLine(13,3,13,8,BLUE);
    case 12:
      matrix.drawLine(12,3,12,8,BLUE);
    case 11:
      matrix.drawLine(11,3,11,8,BLUE);
    case 10:
      matrix.drawLine(10,3,10,8,BLUE);
    case 9:
      matrix.drawLine(9,3,9,8,BLUE);
    case 8:
      matrix.drawLine(8,3,8,8,BLUE);
    case 7: 
      matrix.drawLine(7,3,7,8,BLUE);
      matrix.drawPixel(7, 6, WHITE);
    case 6:
      matrix.drawLine(6,3,6,9,BLUE);
      matrix.drawLine(6,5,6,6,WHITE); 
    case 5:
      matrix.drawLine(5, 3, 5, 9,BLUE);
      matrix.drawLine(5,5,5,7,WHITE); 
    case 4:
      matrix.drawLine(4, 3, 4, 9, BLUE);
      matrix.drawLine(4, 5, 4, 8, WHITE);
    case 3: 
      matrix.drawLine(3, 3, 3, 10, BLUE);
      matrix.drawLine(3, 6, 3, 8, WHITE);
    case 2: 
      matrix.drawLine(2, 4, 2, 10, BLUE); 
      matrix.drawLine(2, 6, 2, 8, WHITE);
    case 1:
      matrix.drawLine(1, 5, 1, 10, BLUE);
  }
  }
  else{ //if reading has decreased from the last measurement, erase lines that were drawn based on previous readings
      switch(currentVolMeasure){
        case 0:
          matrix.drawLine(1, 5, 1, 10, BLACK);
        case 1: 
          matrix.drawLine(2, 4, 2, 10, BLACK); 
          matrix.drawLine(2, 6, 2, 8, BLACK);
        case 2: 
          matrix.drawLine(3, 3, 3, 10, BLACK);
          matrix.drawLine(3, 6, 3, 8, BLACK);
        case 3:
          matrix.drawLine(4, 3, 4, 9, BLACK);
          matrix.drawLine(4, 5, 4, 8, BLACK);
        case 4:
          matrix.drawLine(5, 3, 5, 9,BLACK);
          matrix.drawLine(5,5,5,7,BLACK);
        case 5:
          matrix.drawLine(6,3,6,9,BLACK);
          matrix.drawLine(6,5,6,6,BLACK); 
        case 6: 
          matrix.drawLine(7,3,7,8,BLACK);
          matrix.drawPixel(7, 6, BLACK); 
        case 7:
          matrix.drawLine(8,3,8,8,BLACK);  
        case 8:
          matrix.drawLine(9,3,9,8,BLACK); 
        case 9:
          matrix.drawLine(10,3,10,8,BLACK);  
        case 10:
          matrix.drawLine(11,3,11,8,BLACK);   
        case 11:
          matrix.drawLine(12,3,12,8,BLACK); 
        case 12:
          matrix.drawLine(13,3,13,8,BLACK);
        case 13:
          matrix.drawLine(14,3,14,9,BLACK); 
        case 14:
          matrix.drawLine(15,2,15,9,BLACK); 
        case 15:
          matrix.drawLine(16,2,16,10,BLACK); 
        case 16:
          matrix.drawLine(17,2,17,10,BLACK);
        case 17:
          matrix.drawLine(18,2,18,11,BLACK);  
        case 18:
          matrix.drawLine(19,2,19,11,BLACK);  
        case 19:
          matrix.drawLine(20,2,20,11,BLACK);   
        case 20:
          matrix.drawLine(21,2,21,12,BLACK); 
        case 21:
          matrix.drawLine(22,2,22,12,BLACK); 
        case 22:
          matrix.drawLine(23,2,23,12,BLACK);
        case 23:
          matrix.drawLine(24,6,24,12,BLACK);  
        case 24:
          matrix.drawLine(25,8,25,11,BLACK); 
        case 25:
          matrix.drawPixel(28,13,BLACK);           
        case 26:
          matrix.drawPixel(29,13,BLACK);   
       }    
  }
}