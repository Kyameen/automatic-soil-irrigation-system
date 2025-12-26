#include <DHT.h>
#include <SoftwareSerial.h>


#define SOIL_SENSOR_PIN  7
#define RAIN_SENSOR_PIN  8
#define RELAY_PIN        5

#define PUMP_ON  LOW     
#define PUMP_OFF HIGH

#define DHTPIN   4
#define DHTTYPE  DHT11


#define BT_RX 10   
#define BT_TX 11   



DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial bluetooth(BT_RX, BT_TX);

unsigned long lastDHTRead   = 0;
unsigned long lastStatusOut = 0;

const unsigned long DHT_INTERVAL    = 3000; 
const unsigned long STATUS_INTERVAL = 1000; 


void setup() {

  pinMode(SOIL_SENSOR_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, PUMP_OFF);

  Serial.begin(9600);
  bluetooth.begin(9600);

  dht.begin();

  Serial.println("=== SMART IRRIGATION SYSTEM STARTED ===");
  bluetooth.println("=== SMART IRRIGATION SYSTEM STARTED ===");
}


void loop() {

  unsigned long currentMillis = millis();


  if (currentMillis - lastStatusOut >= STATUS_INTERVAL) {
    lastStatusOut = currentMillis;

    int soilState = digitalRead(SOIL_SENSOR_PIN);
    int rainState = digitalRead(RAIN_SENSOR_PIN);

    Serial.println("------------------------------------");
    bluetooth.println("------------------------------------");

    Serial.print("Soil State : ");
    bluetooth.print("Soil State : ");
    Serial.println(soilState);
    bluetooth.println(soilState);

    Serial.print("Rain State : ");
    bluetooth.print("Rain State : ");
    Serial.println(rainState);
    bluetooth.println(rainState);

    if (rainState == 1) {
      digitalWrite(RELAY_PIN, PUMP_OFF);

      Serial.println("Rain detected → Pump OFF");
      bluetooth.println("Rain detected → Pump OFF");
    }
    else {
      if (soilState == 1) {
        digitalWrite(RELAY_PIN, PUMP_ON);

        Serial.println("Soil dry → Pump ON");
        bluetooth.println("Soil dry → Pump ON");
      }
      else {
        digitalWrite(RELAY_PIN, PUMP_OFF);

        Serial.println("Soil wet → Pump OFF");
        bluetooth.println("Soil wet → Pump OFF");
      }
    }
  }

  if (currentMillis - lastDHTRead >= DHT_INTERVAL) {
    lastDHTRead = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("DHT READ ERROR");
      bluetooth.println("DHT READ ERROR");
      return;
    }

    Serial.print("Humidity    : ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" C");

    bluetooth.print("Humidity    : ");
    bluetooth.print(humidity);
    bluetooth.println(" %");

    bluetooth.print("Temperature : ");
    bluetooth.print(temperature);
    bluetooth.println(" C");
  }
}
