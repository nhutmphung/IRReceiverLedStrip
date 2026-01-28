#include <Arduino.h>
#include <FastLED.h>
#include <IRremote.h>

// TODO 1/26/2026 add array with colors from ROYGBIV, white, and black and have it cycle through as a function

// Define the IR receiver pin
const uint8_t IR_RECEIVE_PIN = 4;  
const uint8_t NUM_LEDS = 5;
const uint8_t LEDStrip_PIN = 6; 

uint8_t currentMode = 1; 
int colorArrayCounter = 0; 

CRGB leds [NUM_LEDS];

CRGB colorArray [] = {
  CRGB::Red,
  CRGB::Orange, //TODO TS NOT ORANGE GANG
  CRGB::Yellow, //TODO TS NOT YELLOW IT LIKE PUKE YELLOW OR NEON YELLOW
  CRGB::Green,
  CRGB::Blue,
  CRGB::Indigo,
  CRGB::Violet,
  CRGB::Black,
  CRGB::White
};

const uint8_t NUM_COLORS = 9;
    

enum LedMode{
    MODE_FADE,
    MODE_COLORCHANGE,
    MODE_CYLON,
    MODE_RAINBOW,
    MODE_SKIP,
    MODE_FAIRY
};

void colorChange() { 

  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = colorArray[colorArrayCounter]; 
  }

  FastLED.show();

}

void lightSkip() {  //pretty sure this is in documentation for cylon or Chase/Scanners

}

void fairyLights() {    //TODO ADD dimming to fairy? 
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(45, 255, 125);
    }
    FastLED.show(); 
}

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

    if(IrReceiver.decodedIRData.command == 0xC){  //Button 1 on Remote
      currentMode = MODE_FADE; 
      Serial.println("CURRENT MODE: FADE");
    }

    if(IrReceiver.decodedIRData.command == 0x18) {  //Button 2 on Remote
      currentMode = MODE_RAINBOW;
      Serial.println("MODE: Rainbow");
    }

    if(IrReceiver.decodedIRData.command == 0x5E) {  //Button 3 on Remote
      currentMode = MODE_COLORCHANGE; 
      Serial.println("MODE: Color Chnage");

      //resets colorArrayCounter back to red
      colorArrayCounter++; 
      if(colorArrayCounter >= NUM_COLORS) {
        colorArrayCounter = 0;
      }
    }
    
    if(IrReceiver.decodedIRData.command == 0x8) {  //Button 4 on Remote
      currentMode = MODE_FAIRY; 
      Serial.println("MODE: Color Chnage");    
    }



    // Print the raw code (useful for troubleshooting)
    Serial.print("Raw Code: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    Serial.println( colorArrayCounter);
    
    Serial.println("----------------------------------------");
    
    // Ready to receive the next signal
    IrReceiver.resume();

  }

    switch (currentMode)  
    { 
      case MODE_FADE: 
        fadeInOutPerLED();
        break;
      case MODE_RAINBOW: 
        rainbowCycle();
        break;
      case MODE_CYLON:
        cylon();
        break;
      case MODE_COLORCHANGE: 
        colorChange();
        break;
      case MODE_FAIRY:
        fairyLights(); 
        break;

    }  
}