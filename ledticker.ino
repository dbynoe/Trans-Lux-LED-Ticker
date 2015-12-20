
// This #include statement was automatically added by the Particle IDE.
#include "Fonthelper.h"

// This #include statement was automatically added by the Particle IDE.
#include "Font6x10.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


int colour = 6; //global variable for colour - was trying the variable to cloud thing

int goodColors[]={6,7,8};
/*valid colours are:
6 red pixel
7 is green
8 is orange
9 seems to start interlacing red and green (in the tables below assume X is the first color and - is the other)
10 red and orange
11 green and red
12 green and orange
13 orange and red
14  orange and green
*/

int offset = 6;
int updateTime = 30000;
float lastMillis = millis() + updateTime;


void setup() {
    
    Time.zone(-7);
    Serial1.begin(9600); //serial communicatin to the sign
   Particle.function("text", recieveText); //external function to recieve text in. 

}

void loop() {

if ((millis()-lastMillis)>=updateTime){
parseString ("   ",colour, offset);
psLogo(6,3);
parseString (" protospace.ca/sign ",colour, offset);
psLogo(6,3);
parseString ("   ",colour, offset);
colour++;
 lastMillis=millis();
if (colour >=9) {colour=6;}
}
    
}

void parseString(String incStr, int txtColour, int txtOffset) //function to extract characters from strings
{
  for (int i=0; i <incStr.length(); i++){
    printChar (incStr.charAt(i), txtColour, txtOffset);}
}

int GetBitmapLocation (char letter) { //look up the location of the character in the bitmap file
  int result =0;
  for(int i=0;i<arraySize(__apple6x10_index__);i++){

  if(__apple6x10_index__[i]== letter) {

  result=i;
  }}
  return result;
} 

void printChar(char letter, int txtColour, int txtOffset) { //function to draw the character to the screen, extracts one vertical line at a time
int lineBuffer =0;
  int charWidth = 6; //hardcoded for now, to be fixed once I add multiple fonts
  int charHeight = 10;
  int index = GetBitmapLocation(letter);
  for (int x = 1; x <= charWidth; x++) {
  for (int y = 1; y <= charHeight; y++) {
bitWrite(lineBuffer,y+txtOffset, bitRead(__apple6x10_bitmap__[(y+(index*charHeight))-1],charWidth-x+(8-charWidth)));

  }

sendData(lineBuffer, txtColour, txtOffset);

  }
  
}

void fillSign(int fillColour)
{
    for (int i =0; i <240; i++){
        sendData (16777215,fillColour,0);
    }
}

void sendData(int incData, int txtColour, int txtOffset) { //sends a line to the screen

//Inc data is a 25 bit value starting at the bottom row and being 0 for off and 1 for on 
    byte tmpByt = 0;

if (txtColour== 19)
{txtColour = int(random(6,9));}

 Serial1.write(txtColour); //colour first

    
  for (int block = 0; block <= 4; block++) {
 for (int row =1; row <=5; row++) {
     
     bitWrite (tmpByt, 5-row, bitRead(incData, (row+(block*5))-1) ); 
//the sign is divided up into 5 5 led sections but only has 24 pixels
//each section is treated as its own 5 bit binary, simply convert your 0's and 1's to an int and add 32 for the first row, 64 for the second etc. 

 }
Serial1.write(byte(tmpByt+(32+block*32)));

      tmpByt = 0;
      
  }
    
Serial1.write(5);
  
}

int recieveText (String incStr) //incoming message from the cloud
{  
  



    parseString (" ",colour,offset);
    int rndColour = goodColors[random(arraySize(goodColors))];
    parseString (incStr,rndColour,random(0,14));
    parseString (" ",colour,offset);
lastMillis = millis();

      return 1;
}

uint8_t psLogoBMP[] = {
    _____XXX, XXX_____,
    ___XXXXX, XXXXX___,
    __XXXX__, __XXXX__,
    _XXX____, ____XXX_,
    _XX_____, _____XX_,
    _XX_____, _____XX_,
    _XXXXXXX, XXXXXXX_,
    _XXXXXXX, XXXXXXX_,
    _XX_____, ________,
    _XXXXXXX, XXXXXXX_,
    _XXXXXXX, XXXXXXX_,
    ________, _____XX_,
    _XXXXXXX, XXXXXXX_,
    _XXXXXXX, XXXXXXX_,
    ___XX___, ___XX___,
    ___XX___, ___XX___,
  };

void psLogo (int logoColour, int curOffset){//show a ps logo
int lineBuffer =0;
  int charWidth = 16; 
  int charHeight = 16;

  for (int x = 1; x <= charWidth; x++) {
  for (int y = 1; y <= charHeight; y++) {
      if (x > 8){
bitWrite(lineBuffer,y+curOffset, bitRead(psLogoBMP[((y-1)*2)+1],8-(x-8)));
}
else {
    bitWrite(lineBuffer,y+curOffset, bitRead(psLogoBMP[(y-1)*2],8-x));
}

  }

sendData(lineBuffer, logoColour, curOffset);

  }

}

