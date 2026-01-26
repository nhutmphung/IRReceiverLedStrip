#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.h>

// Define the IR receiver pin
const int IR_RECEIVE_PIN = 4;  
const int NUM_LEDS = 20;
const int LEDStrip_PIN = 6; 

CRGB leds [NUM_LEDS];


void setup() {
  Serial.begin(9600);
  
  // Start the IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  FastLED.addLeds<WS2812B, LEDStrip_PIN, GRB>(leds, NUM_LEDS); 
  FastLED.setBrightness(50); 
  delay(2000);

 
}

void loop() {
  // Check if IR signal received
  if (IrReceiver.decode()) {
    
    // Print the protocol type
    Serial.print("Protocol: ");
    Serial.println(IrReceiver.decodedIRData.protocol);
    
    // Print the hex code
    Serial.print("Hex Code: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    if (IrReceiver.decodedIRData.command == 0xC){
      leds[0] = CRGB::White;
      leds[1] = CRGB::White;
      leds[2] = CRGB::White;
      FastLED.show();
      delay(250); 
    
    }

    if(IrReceiver.decodedIRData.command == 0x18) {
      leds[0] = CRGB::Red;
      leds[1] = CRGB::Red;
      leds[2] = CRGB::Red;
      FastLED.show();
      delay(250); 
    
    }

    if(IrReceiver.decodedIRData.command == 0x18) {
      leds[0] = CRGB::Blue;
      leds[1] = CRGB::Blue;
      leds[2] = CRGB::Blue;
      FastLED.show();
      delay(250); 
    
    }


    // Print the raw code (useful for troubleshooting)
    Serial.print("Raw Code: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    
    Serial.println("----------------------------------------");
    
    // Ready to receive the next signal
    IrReceiver.resume();
  }
}