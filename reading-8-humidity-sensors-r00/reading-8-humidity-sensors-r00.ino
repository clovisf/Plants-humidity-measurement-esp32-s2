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
int humidityreadings[8];
byte selectchannel= 0b00000000;
int i= 0;
int writetothingspeak= 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
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
      if(iterationcounter == 98){ //here I select which input will be read
        digitalWrite(2, bitRead(selectchannel, 0));
        digitalWrite(3, bitRead(selectchannel, 1));
        digitalWrite(4, bitRead(selectchannel, 2));
        digitalWrite(5, bitRead(selectchannel, 3));
        selectchannel= selectchannel + 0b0001;
      }
      if(stabilization = 3){
        
        if(i==9){
          i=0;
        }else{
          i++; //wait 30mS to stabilize readings
        }
        stabilization= 0;
        iterationcounter= 0;
        humidityreadings[i]= analogRead(A0); //read analog value
        Serial.print("leitura sensor [");
        Serial.print(i);
        Serial.print("]= ");
        Serial.println(humidityreadings[i]);

        Serial.print(F("Temperature = "));
        Serial.print(bmp.readTemperature());
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        Serial.print(bmp.readPressure());
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
      
      ThingSpeak.setField(1, humidityreadings[0]);
      ThingSpeak.setField(2, humidityreadings[1]);
      ThingSpeak.setField(3, humidityreadings[2]);
      ThingSpeak.setField(4, humidityreadings[3]);
      ThingSpeak.setField(5, humidityreadings[4]);
      ThingSpeak.setField(6, humidityreadings[5]);
      ThingSpeak.setField(7, humidityreadings[6]);
      ThingSpeak.setField(8, humidityreadings[7]);

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
