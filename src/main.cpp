#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.h>

// Define the IR receiver pin
const int IR_RECEIVE_PIN = 4;  
const int NUM_LEDS = 5;
const int LEDStrip_PIN = 6; 

int currentMode = 1; 

CRGB leds [NUM_LEDS];


void fadeInOutPerLED() {
    static uint8_t brightness = 0;
    static int8_t direction = 1;

    brightness += direction;
    if (brightness == 0 || brightness == 255) {
        direction = -direction;
    }

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(160, 255, brightness);
    }
    FastLED.show();
}

void ledSolid(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::White; 
  }
  FastLED.show();
}

void rainbowCycle() {
    static uint8_t hue = 0;  // 0-255 automatically cycles through colors
    
    hue += 2;  // Change speed here (higher = faster color change)
    
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue, 255, 255);  // All LEDs same color
    }
    FastLED.show();
}

void cylon() {
    static uint8_t pos = 0;
    static int8_t direction = 1;

    // Fade all LEDs
    fadeToBlackBy(leds, NUM_LEDS, 20);

    // Set current position
    leds[pos] = CRGB::Red;

    // Move position
    pos += direction;
    if (pos == 0 || pos == NUM_LEDS - 1) {
        direction = -direction;
    }
    FastLED.show();
}


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

    if(IrReceiver.decodedIRData.command == 0xC){
      currentMode = 1; 
      Serial.println("MODE: Fade");
    }


    if(IrReceiver.decodedIRData.command == 0x18) {  //Button 2 on Remote
      currentMode = 2;
      Serial.println("MODE: Rainbow");
    }

    if(IrReceiver.decodedIRData.command == 0x5E) {  //Button 3 on Remote
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

    switch (currentMode){
      case 1: 
        fadeInOutPerLED();
        break;
      case 2: 
        rainbowCycle();
        break;
      case 3:
        cylon();
        break;
    }  
}