/* Arduino-Sketch zur Bestimmung der Schwerttiefe der Eule
 *  
 * Basis ist ein ARDUINO Nano, ein Drehencoders Az-Delivery KY-040 
 * und ein Display Nokia 5110
 * 
 * Anschluss des Drehencoder PIN CLK an Arduino Pin Int0 
 * und Drehencoder PIN DT an Arduino Pin Int1
 * 
 * Encoder        note                           Arduino
 * -----------------------------------------------------------------
 * 1-GND ......   ground ....................... GND
 * 2-VCC ......   +3.3 V ....................... 3V3
 * 3-SW  ......   Switch not connected ......... NC
 * 4-DT  ......   Signal B ..................... INT1
 * 5-CLK ......   Signal A ..................... INT0
 * 
 * 
 * Die Daten werden auf ein Display Nokia 5110 ausgegeben.
 * (c) 2013 +++ Filip Stoklas, aka FipS, http://www.4FipS.com +++
 * ARTICLE URL: http://forums.4fips.com/viewtopic.php?f=3&t=1086
 * Arduino Nano SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
 * 
 * LCD            note                            Arduino
* ------------------------------------------------------------------
* 1-VCC ........  +3.3 V ........................ 3V3  
* 2-GND ........  ground ........................ GND  
* 3-SCE ........  chip select (CS) .............. D10  
* 4-RST ........  reset ......................... D8   
* 5-D/C ........  data/command selector (A0) .... D9   
* 6-DN<MOSI> ...  serial data in (MOSI, SDIN) ... D11  
* 7-SCLK .......  clock (SCK, CLK) .............. D13  
* 8-LED ........  backlight, +3.3 V ............. 3V3  
* 
*/

#define encoderPinA 2  // Port for encoder
#define encoderPinB 3  // Port for encoder
#define switchPin 4    // Port for resetswitch to reset counter
#include "U8glib.h"

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management

// interrupt service routine vars
boolean A_set = false;            
boolean B_set = false;

//Display Nokia 5110
U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8

// Print values
void draw(int prozent)
{
    u8g.setFont(u8g_font_gdr25r);
    //u8g.setScale2x2();
    enum {BufSize=5}; // If a is short use a smaller number, eg 5 or 6 
    char buf[BufSize];
    snprintf (buf, BufSize, "%d", prozent);
    u8g.drawStr(20, 34, buf);
    //u8g.drawFrame(0,0,45,20);
    //u8g.drawStr(0, 40, "2022");
}


void setup() {

  u8g.setColorIndex(1); // pixel on
  
  pinMode(encoderPinA, INPUT); 
  pinMode(encoderPinB, INPUT); 

  digitalWrite(encoderPinA, HIGH);  // turn on internal pullup resistors
  digitalWrite(encoderPinB, HIGH);  // turn on internal pullup resistors
  
  pinMode(switchPin, INPUT_PULLUP);    // turn on internal pullup resistors and configure input

  attachInterrupt(0, doEncoderA, CHANGE); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, doEncoderB, CHANGE); // encoder pin on interrupt 1 (pin 3)

  Serial.begin(9600);  // output
}

void loop()
 
 { 
  u8g.firstPage();
  rotating = true;  // reset the debouncer

  if (lastReportedPos != encoderPos)
  {
    // Print value on serialterminal
    Serial.print("Index:");
    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos; 
  }
  
  bool resetButtonPressed = digitalRead(switchPin);
  if (!resetButtonPressed){
      Serial.println("Resetbutton pressed");
      encoderPos = 0;
      Serial.print("Index:");
      Serial.println(encoderPos, DEC);
      }
      
  do
   {
    draw(encoderPos); 
   }
   
   while(u8g.nextPage());
   //delay(500);
}

// Interrupt on A changing state
void doEncoderA()
{
  if ( rotating ) delay (1);  // wait a little until the bouncing is done
  if( digitalRead(encoderPinA) != A_set ) {  // debounce once more
    A_set = !A_set;
    // adjust counter + if A leads B
    if ( A_set && !B_set ) 
      encoderPos += 1;
    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if ( rotating ) delay (1);
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set ) 
      encoderPos -= 1;
    rotating = false;
  }
}

// Calculate depth of sword
void calculateSword()
{
  // Hier kommt die Berechnung der Schwerttiefe rein
  // Die Drehencoderposition wird per 3-Satz in eine
  // Laenge umgerechnet und als Wert zurueckgegeben.
  
}
