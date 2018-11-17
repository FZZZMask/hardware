#include <dht11.h>

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

long getDust()
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
  if (dustVal != 0)
    if (((float)abs(value - dustVal) / (float)value < 2.0) || value == 0)
      value = (value * 99 + dustVal) / 100;
    else
      value = (value * 199 + dustVal) / 200;

  return value;
}

int getHumidity()
{
  static int value = 0;

  dht11 DHT11;

  DHT11.read(dht11Pin);

  //滤波
  if (DHT11.humidity > 20 && DHT11.humidity < 100)
  {
    if (value == 0)
    {
      value = DHT11.humidity;
      return value;
    }
    if ((float)abs(value - DHT11.humidity) / (float)value < 0.1)
      value = (value * 3 + DHT11.humidity) / 4;
    else
      value = (value * 9 + DHT11.humidity) / 10;
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
      value = (value * 9 + DHT11.temperature) / 10;
  }
  return value;
}

// 表示前100次检测中的呼气频率
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
  int count = 0;
  int dust, humidity, temperature;
  float breath;
  String toSend;
  

  while (1)
  {
    dust = getDust();
    delay(intervalTime);
    humidity = getHumidity();
    delay(intervalTime);
    temperature = getTemperature();
    delay(intervalTime);
    breath = getBreath();
    delay(intervalTime);

    count ++;

    if (count == 30)
    {
      toSend = "{\"dust\":";
      toSend+=dust;
      toSend+=",\"humidity\":";
      toSend+=humidity;
      toSend+=",\"temperature\":";
      toSend+=temperature;
      toSend+=",\"breath\":";
      toSend+=breath;
      toSend+="}";
      Serial.println(toSend);

      count = 0;
    }
  }


}
