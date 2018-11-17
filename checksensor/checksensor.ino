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
  static long value = 200;

  int dustVal = 0;
  // ledPowerPin is any digital pin on the arduino connected to Pin 3 on the sensor
  digitalWrite(ledPowerPin, LOW);
  delayMicroseconds(delayTime);
  dustVal = analogRead(dustPin);
  delayMicroseconds(delayTime2);
  digitalWrite(ledPowerPin, HIGH);
  delayMicroseconds(offTime);


  // 因为这个传感器貌似有问题，所以滤波只能这样

  return dustVal;
}

int getHumidity()
{

  dht11 DHT11;

  DHT11.read(dht11Pin);

  //滤波
  return DHT11.humidity;
}

int getTemperature()
{

  dht11 DHT11;

  DHT11.read(dht11Pin);

  return DHT11.temperature;
}

// 表示前100次检测中的呼气频率
float getBreath()
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
  int count = 0;
  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& data = jsonBuffer.createObject();

  while (1)
  {
    data["dust"] = getDust();
    delay(intervalTime);
    data["humidity"] = getHumidity();
    delay(intervalTime);
    data["temperature"] = getTemperature();
    delay(intervalTime);
    data["breath"] = getBreath();
    delay(intervalTime);

    data.printTo(Serial);
    Serial.println();
    delay(500);
  }
}
