/*********
  Complete project details at https://randomnerdtutorials.com

  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are required to interface (two I2C and one reset).
  Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries, with contributions from the open source community. BSD license, check license.txt for more information All text above, and the splash screen below must be included in any redistribution.
*********/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int analogInputPin = A0;  // Analog pin where AD8232 output is connected
int rawValue;
int lastRawValue = 0;
unsigned long lastBeatTime = 0;
const int threshold = 550;  // Adjust this threshold based on your setup
int bpm = 0;

unsigned long previousMillis = 0;
const int samplingRate = 500;

int i = 0;
int NewValue1 = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -



  display.display();
  delay(2000); // Pause for 2 seconds


  display.clearDisplay();

  display.display();

  Serial.begin(9600);

}

void loop() {
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  ecg ();
  delay(1);
  screen();
  delay(1);

}





void ecg() {
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println('!');
  }
  else {
    // send the value of analog input 0:
    Serial.println(analogRead(A0));
  }
  //Wait for a bit to keep serial data from saturating

  

}

void screen() {
  int NewValue = 128 - ((analogRead(A0)) / 4);
  if (i < 129) {
    display.drawPixel(i, NewValue , WHITE);
    display.display();
    i = i + 1;
    if (i == 128) {
      display.clearDisplay();
      display.display();

      i = 1;
    }
  }
}

void bpm1() {
  rawValue = analogRead(analogInputPin);

  // Check for a heartbeat
  if (rawValue > threshold && lastRawValue <= threshold) {
    unsigned long beatTime = millis();
    bpm = int(60000 / (beatTime - lastBeatTime));  // Calculate beats per minute
    lastBeatTime = beatTime;

    Serial.print("Heartbeat detected! BPM: ");
    Serial.println(bpm);
  }

  // Display BPM on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.print("BPM: ");
  display.print(bpm);
  display.display();

  // Print BPM to Serial Monitor
  Serial.print("BPM: ");
  Serial.println(bpm);

  // Calculate BPM every 'samplingRate' milliseconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= samplingRate) {
    previousMillis = currentMillis;
    // You can use bpm as needed in your additional code here
  }

  lastRawValue = rawValue;

}
