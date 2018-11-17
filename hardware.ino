#include <dht11.h>
#include <ArduinoJson.h>

// PINS
#define dustPin 7
#define dht11Pin 5
#define ledPowerPin 2
#define breathPin 4
#define basePin 7


// DelayTimes
#define delayTime 280
#define delayTime2 40
#define offTime 9680


int getDust()
{
  float dustVal = 0;
  // ledPowerPin is any digital pin on the arduino connected to Pin 3 on the sensor
  digitalWrite(ledPowerPin, LOW);
  delayMicroseconds(delayTime);
  dustVal = analogRead(dustPin);
  delayMicroseconds(delayTime2);
  digitalWrite(ledPowerPin, HIGH);
  delayMicroseconds(offTime);

  delay(500);
  return dustVal;
}

dht11 getDHT11()
{
  dht11 DHT11;

  DHT11.read(dht11Pin);
  
  delay(500);
  return DHT11;
}

int getBreath()
{
  return digitalRead(breathPin);
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledPowerPin, OUTPUT);
  pinMode(dustPin, INPUT);
  pinMode(dht11Pin, INPUT);
  pinMode(breathPin, INPUT);
  pinMode(basePin, OUTPUT);
  digitalWrite(basePin, LOW);
}

void loop()
{
  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& data = jsonBuffer.createObject();
  data["dust"] = getDust();
  data["humidity"] = (float)getDHT11().humidity;
  data["temperature"] = (float)getDHT11().temperature;
  data["breath"] = getBreath();

  data.printTo(Serial);
  Serial.println();
}
