/*
Fil: Puls.ino
Författare: Hugo Cedervist
Datum 2023-12-13
Beskrivning: Det är en kod som använder sig av komponenten ad8232 för att a reda på nervsignalerna för att sedan rita ut dem på på en oled skärm. Den använder även
infon för att beräkna en persons puls och även då visa den på skärmen. 
*/

//här inkluderar jag biblioteken som krävs för att koden ska fungera:
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Här definerar jag konstanta värden:
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
const int analogInputPin = A0; // Analog pin where AD8232 output is connected

// Här definerar jag variabler: 
unsigned long previousMillis = 0;
unsigned long lastBeatTime = 0;
const int threshold = 550;  // Adjust this threshold based on your setup
const int samplingRate = 500;
int bpm = 0;
int i = 0;
int NewValue1 = 0;
int rawValue;
int lastRawValue = 0;

//här definerar jag skärmen som jag använder: 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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

//Här skulle jag gjort en toggle knapp som byten mellan att visa funktionerna ovanför och funktionen under.

  //bpm();

}

//Den här funktionen använder sig av ad8232 och ritar sedan ut värdena som skickas tillbaka i serial plottern.
void ecg() {
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println('!');
  }
  else {
    // send the value of analog input 0:
    Serial.println(analogRead(A0));
  }
}


//Den här funktionen använder sig av värdena som ecg funktionen ritar ut för att sedan rita ut dem på en skärm. 
void screen() {
  int NewValue = 64 - ((analogRead(A0)) / 8);
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

//Denna funktionen använder ecg värdena för att se hur många gånger dem värden går över en satt gräns och på så sätt kan mäta pulsen, sedan ritar den ut det på skärmen.
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
