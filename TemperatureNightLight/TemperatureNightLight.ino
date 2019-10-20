/*******************************************************************
 *  A project for chaning the colour of a kids light based on the
 *  temperature of the room by emulating the IR remote that comes
 *  with the light
 *  
 *  Parts:
 *  D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe
 *  Pack of 10 IR LEDs (Good ones)* - http://s.click.aliexpress.com/e/s3yNpaC
 *  BME280 Temperature & Humidity Sensor (4 pin)* - http://s.click.aliexpress.com/e/l4xWrpck
 *  Ir Reciever Board - https://www.aliexpress.com/item/32723068169.html
 *  
 *  * = Affilate
 *  
 *  If you find what I do usefuland would like to support me,
 *  please consider becoming a sponsor on Github
 *  https://github.com/sponsors/witnessmenow/
 *  
 *                                                                 
 *  Written by Brian Lough
 *  YouTube: https://www.youtube.com/brianlough
 *  Tindie: https://www.tindie.com/stores/brianlough/
 *  Twitter: https://twitter.com/witnessmenow                                 
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <Arduino.h>
#include <Wire.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <IRremoteESP8266.h>
#include <IRsend.h>

// This library is for sending the IR Commands
// 
// Can be installed from the library manager, search for "IRremoteESP8266"
// https://github.com/crankyoldgit/IRremoteESP8266

#include <Adafruit_BME280.h>
// This library for interfacing with the temp sensor
// 
// Can be installed from the library manager, search for "bme280"
// https://github.com/adafruit/Adafruit_BME280_Library
// 
// You also will need to install the Adafruit Unified Sesnor library
// Can be installed from the library manager, search for "unified sesnor"
// https://github.com/adafruit/Adafruit_Sensor


// ----------------------------
// Pin Defintions
// ----------------------------

#define IR_SEND_PIN D6

//Not needed for this sketch
#define IR_RECV_PIN D5

// Using the default i2c pins of the ESP8266
// for the BME280 so definition needed.
// SDA -> D1
// SCL -> D2

// ----------------------------

// All commands that are my remote control.
// I used the IRrecDumpV2 example from the 
// IRremoteESP8266 library to read these

// Power - irsend.sendNEC(0xFFE21D, 32);
// Bright Down - irsend.sendNEC(0xFF629D, 32);
// Bright Down - irsend.sendNEC(0xFFA25D, 32);

// RED - irsend.sendNEC(0xFF22DD, 32);
// Yellow - irsend.sendNEC(0xFF02FD, 32);
// Purple - irsend.sendNEC(0xFFC23D, 32);

// GREEN - irsend.sendNEC(0xFFE01F, 32);
// Light Blue - irsend.sendNEC(0xFFA857, 32);
// Orange - irsend.sendNEC(0xFF906F, 32);

// BLUE - irsend.sendNEC(0xFF6897, 32);
// Magenta - irsend.sendNEC(0xFF9867, 32);
// WHITE - irsend.sendNEC(0xFFB04F, 32);

// TIME1 - irsend.sendNEC(0xFF30CF, 32);
// TIME2 - irsend.sendNEC(0xFF18E7, 32);
// TIME3 - irsend.sendNEC(0xFF7A85, 32);

// HEARBEAT - irsend.sendNEC(0xFF10EF, 32);
// TREE - irsend.sendNEC(0xFF38C7, 32);

IRsend irsend(IR_SEND_PIN);
#define SEALEVELPRESSURE_HPA (1013.25)

// Temperatures (in celcius) that the light will change colour,
// change these as required
float minTemp = 18;
float maxTemp = 26;

Adafruit_BME280 bme;

void setup() {
  irsend.begin();
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(100000);

  unsigned status;

  // by default the library uses an i2c address of 0x77,
  // but mine had an address of 0x76 (most of the cheapers ones do)
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1);
  }

}

float temp = 0;

void loop() {
  temp = bme.readTemperature();
  Serial.print(F("Temperature = "));
  Serial.print(temp);
  Serial.println(" *C");

  if (temp > maxTemp)
  {
    // Temp is above the max, set light to RED
    irsend.sendNEC(0xFF22DD, 32);
  } else if (temp < minTemp) {
    // Temp is below the min, set light to BLUE
    irsend.sendNEC(0xFF6897, 32);
  } else {
    // deafult the light to GREEN
    irsend.sendNEC(0xFFE01F, 32);
  }

  delay(2000);

}
