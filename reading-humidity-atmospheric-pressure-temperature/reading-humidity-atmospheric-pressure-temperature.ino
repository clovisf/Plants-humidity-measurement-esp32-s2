#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#include <WiFi.h>
#include "ThingSpeak.h"

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

const char* ssid = "";   // your network SSID (name) 
const char* password = "";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = ;
const char * myWriteAPIKey = "";

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

int stabilization= 0;
int iterationcounter= 0;
int oldtime= 0;
int humidityreadings;
int writetothingspeak= 0;
int temperature;
int pressure;

void setup() {
  // put your setup code here, to run once:
 
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  unsigned status;
  status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  //status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
                  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(micros()- oldtime > 10000){ //repeat every 10mS
    oldtime= micros();

    iterationcounter++;
    if(iterationcounter > 97){ //wait 970mS to do anything (in between readings)
      stabilization++;
      
      if(stabilization = 3){
        
        stabilization= 0;
        iterationcounter= 0;
        humidityreadings= analogRead(A0); //read analog value
        Serial.println(humidityreadings);

        Serial.print(F("Temperature = "));
        temperature= bmp.readTemperature();
        Serial.print(temperature);
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        pressure= bmp.readPressure();
        Serial.print(pressure);
        Serial.println(" Pa");
      }
    }
    writetothingspeak++;
    if(writetothingspeak == 3000){ //write to thingspeak iot server every 30 seconds
      writetothingspeak= 0;

      // Connect or reconnect to WiFi
      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Attempting to connect");
        while(WiFi.status() != WL_CONNECTED){
          WiFi.begin(ssid, password); 
          delay(5000);     
        } 
      Serial.println("\nConnected.");
      }

      // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
      // pieces of information in a channel.  Here, we write to field 1.
      
      ThingSpeak.setField(1, humidityreadings);
      ThingSpeak.setField(2, temperature);
      ThingSpeak.setField(3, pressure);
      

      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

      /*for(int j= 1; j < 9; j++){
        int x = ThingSpeak.writeField(myChannelNumber, j, humidityreadings[j], myWriteAPIKey);
    
        if(x == 200){
          Serial.println("Channel update successful.");
        }
        else{
          Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      delay(1000);
    }*/
    

    }
    
  }
  
}
