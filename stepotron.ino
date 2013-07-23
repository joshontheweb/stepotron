//Mux_Shield_AnalogIn_Example
//http://mayhewlabs.com/arduino-mux-shield

/*
This example shows how to read and store all 48 pins as analog inputs into arrays and print the results over serial.
Multiplexer pin inputs that do not have a voltage reading (i.e nothing connected) will have erratic values.

To simplify this code further, one might use nested for loops or function calls.
*/

#include <toneAC.h>



//Give convenient names to the control pins
#define CONTROL0 5    
#define CONTROL1 4
#define CONTROL2 3
#define CONTROL3 2

//Create arrays for data from the the MUXs
//See the Arduino Array Reference: http://www.arduino.cc/en/Reference/Array
int mux0array[16];
int mux1array[16];
int mux2array[16];

int steps = 8;
int thisStep = 1;

// Stepped Pentatonic mapping
//
uint16_t pentatonicTable[54] = {
  0,19,22,26,29,32,38,43,51,58,65,77,86,103,115,129,154,173,206,231,259,308,346,
  4461,518,616,691,822,923,1036,1232,1383,1644,1845,2071,2463,2765,3288,
  3691,4143,4927,5530,6577,7382,8286,9854,11060,13153,14764,16572,19708,22121,26306
};

uint16_t mapPentatonic(uint16_t input) {
  uint8_t value = (input) / (1024/53);
  return (pentatonicTable[value]);
}

void setup()
{
  //Set MUX control pins to output
  pinMode(CONTROL0, OUTPUT);
  pinMode(CONTROL1, OUTPUT);
  pinMode(CONTROL2, OUTPUT);
  pinMode(CONTROL3, OUTPUT);
  
  //Open the serial port at 28800 bps
  Serial.begin(28800);
}
  

void loop()
{
  
  thisStep++;
  if (thisStep >= steps) {
    thisStep = 0;
  }
  //This for loop is used to scroll through and store the 16 inputs on the FIRST multiplexer
//  for (int i=0; i<16; i++)
//  {
//    //The following 4 commands set the correct logic for the control pins to select the desired input
//    //See the Arduino Bitwise AND Reference: http://www.arduino.cc/en/Reference/BitwiseAnd
//    //See the Aruino Bitshift Reference: http://www.arduino.cc/en/Reference/Bitshift
//    digitalWrite(CONTROL0, (i&15)>>3); 
//    digitalWrite(CONTROL1, (i&7)>>2);  
//    digitalWrite(CONTROL2, (i&3)>>1);  
//    digitalWrite(CONTROL3, (i&1));     
//    
//    //Read and store the input value at a location in the array
//    mux0array[i] = analogRead(0);
//  }
  
  //This for loop is used to scroll through the SECOND multiplexer
  for (int i=0; i<16; i++)
  {
    digitalWrite(CONTROL0, (i&15)>>3); 
    digitalWrite(CONTROL1, (i&7)>>2);  
    digitalWrite(CONTROL2, (i&3)>>1);  
    digitalWrite(CONTROL3, (i&1));     
    mux1array[i] = analogRead(1);
  }
  
  //This for loop is used to scroll through the THIRD multiplexer
//  for (int i=0; i<16; i++)
//  {
//    digitalWrite(CONTROL0, (i&15)>>3); 
//    digitalWrite(CONTROL1, (i&7)>>2);  
//    digitalWrite(CONTROL2, (i&3)>>1);  
//    digitalWrite(CONTROL3, (i&1));     
//    mux2array[i] = analogRead(2);
//  }    
//  
  //The following lines are for printing out results of array0
//  Serial.print("mux0array: ");
//  for (int i=0; i<16; i++)
//  {
//    Serial.print(mux0array[i]);
//    Serial.print("-");
//  }
//  Serial.println();  //line feed
  
  //The following lines are for printing out results of array1
  Serial.print("mux1array: ");
  for (int i=0; i<8; i++)
  {
    Serial.print(mux1array[i]);
    Serial.print("-");
  }
  Serial.println();
  
  //The following lines are for printing out results of array2
//  Serial.print("mux2array: ");
//  for (int i=0; i<16; i++)
//  {
//    Serial.print(mux2array[i]);
//    Serial.print("-");
//  }
//  Serial.println();


  //Since all 3 multiplexers have the same control pins, the one multiplexer data line we want to 
  //talk to should be set to output and the other two multiplexer lines should be be 'bypassed' by 
  //setting the pins to input
    
  //Turn on output to digital pin 14 (MUX 0) and turn off the other 2 multiplexer data pins
  pinMode(14, OUTPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
    
  //This for loop is used to scroll through the FIRST multiplexer
  
  for (int i=0; i<8; i++) {
    if (i == thisStep) {
      Serial.println(mux1array[i]);
      Serial.println(mapPentatonic(mux1array[i]));
      
      
      toneAC(mapPentatonic(mux1array[i]), 10, 500, true);
    }
  }
  
  for (int i=0; i<8; i++) {    
    //The following 4 commands set the correct logic for the control pins to select the desired input
    //See the Arduino Bitwise AND Reference: http://www.arduino.cc/en/Reference/BitwiseAnd
    //See the Aruino Bitshift Reference: http://www.arduino.cc/en/Reference/Bitshift
    digitalWrite(CONTROL0, (i&15)>>3); //S3
    digitalWrite(CONTROL1, (i&7)>>2);  //S2
    digitalWrite(CONTROL2, (i&3)>>1);  //S1
    digitalWrite(CONTROL3, (i&1));     //S0
    
    if (i == thisStep) {
      digitalWrite(14, HIGH);
      delay(map(mux1array[8], 0, 1023, 0, 300));
//      digitalWrite(14, LOW);
//      delay(100);
      
    }
  }


//  delay(500);
}
