// Libraries for LoRa 
#include <SPI.h>
#include <LoRa.h>

// Libraries for DHT 11
#include <DHT.h>
#include <DHT_U.h>

//define the pins used by the LoRa transceiver module
#define NSS 5     // CS pin
#define RESET 14  // Reset pin
#define DIO0 2    // DIO0 pin
#define SCK 18
#define MISO 19
#define MOSI 23
#define DIO0 2

//DHT SENSOR
#define DHTPIN 35
#define DHTTYPE DHT11

//868E6 for Europe
#define BAND 868E6
//GLOBAL VARIABLES
float temperature=0;
float humidity = 0;

//initilize packet counter
int readingID = 0;
String LoRaMessage = "";

uint32_t delayMS=2000;

//define DHT instance
DHT_Unified dht(DHTPIN, DHTTYPE);


//INITIALIZE DHT
void initDHT(){
  //initializing DHT device
  Serial.println(F("Initializing DHT Sensor"));
  dht.begin();
  Serial.println(F("DHT Sensor is OK!"));
}
//GET DHT READINGS
void getDHTreadings(){
  //Delay between measurements
  delay(delayMS);
  //Get temperature event print its value
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature)){
    Serial.println(F("Error reading temperature!"));
  }
  else{
    Serial.print(F("Temperature: "));
    temperature = event.temperature;
    Serial.print(temperature);
    Serial.println(F("°C"));
  }

  //get humidity event and print its value
  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity)){
    Serial.println(F("Error reading humidity!"));
  }else{
    Serial.print(F("Humidity: "));
    humidity = event.relative_humidity;
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

}

//----------------------Initialize LoRa Module-----------------------------------------//
void initLoRa()
{
  Serial.println("LoRa Sender Test");
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, NSS);
  //setup LoRa transceiver module
  LoRa.setPins(NSS, RESET, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa başlatılırken bir hata oluştu!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  delay(2000);
}

//------------------Send data to receiver node using LoRa-------------------------//
void sendReadings() {
  LoRaMessage = String(readingID) + "/" + String(temperature) + "&" + String(humidity);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();

  Serial.println("Packet Sent!");

  delay(10000);
}
void getReadings()
{
  getDHTreadings();
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  initLoRa();
  initDHT();
}

void loop() {
  sendReadings();
}

