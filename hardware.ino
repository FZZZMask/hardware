#include <dht11.h>
#include <ArduinoJson.h>

// Pins
#define dustPin 7
#define dht11Pin 5
#define ledPowerPin 2
#define breathPin 4
#define basePin 7


// DelayTimes
#define delayTime 280
#define delayTime2 40
#define offTime 9680
#define intervalTime 10

int getDust()
{
  static int value = 0;

  int dustVal = 0;
  // ledPowerPin is any digital pin on the arduino connected to Pin 3 on the sensor
  digitalWrite(ledPowerPin, LOW);
  delayMicroseconds(delayTime);
  dustVal = analogRead(dustPin);
  delayMicroseconds(delayTime2);
  digitalWrite(ledPowerPin, HIGH);
  delayMicroseconds(offTime);


  // 因为这个传感器貌似有问题，所以滤波只能这样
  if (dustVal != 0)
    if (((float)abs(value - dustVal) / (float)value < 2.0) || value == 0)
      value = (value * 5 + dustVal) / 6;
    else
      value = (value * 19 + dustVal) / 20;

  return value;
}

int getHumidity()
{
  static int value = 0;

  dht11 DHT11;

  DHT11.read(dht11Pin);

  //滤波  
  if (DHT11.humidity > 20 && DHT11.humidity < 85)
  {
    if (value == 0)
    {
      value = DHT11.humidity;
      return value;
    }
    if ((float)abs(value - DHT11.humidity) / (float)value < 0.1)
      value = (value * 3 + DHT11.humidity) / 4;
    else
      value = (value * 5 + DHT11.humidity) / 6;
  }

  return value;
}

int getTemperature()
{
  static int value = 0;

  dht11 DHT11;

  DHT11.read(dht11Pin);

  //滤波
  if (DHT11.temperature > 5 && DHT11.temperature < 35)
  {
    if (value == 0)
    {
      value = DHT11.temperature;
      return value;
    }
    if ((float)abs(value - DHT11.temperature) / (float)value < 0.1)
      value = (value * 3 + DHT11.temperature) / 4;
    else
      value = (value * 5 + DHT11.temperature) / 6;
  }
  return value;
}

float getBreath()
{
  static float value = 0.5;

  value = (value * 99.0 + digitalRead(breathPin)) / 100.0;
  
  return value;
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
  delay(intervalTime);
  data["humidity"] = getHumidity();
  delay(intervalTime);
  data["temperature"] = getTemperature();
  delay(intervalTime);
  data["breath"] = getBreath();

  data.printTo(Serial);
  Serial.println();
  delay(500);
}
