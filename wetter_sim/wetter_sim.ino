

#include <DHT.h>

#define DHTPIN 16
#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);


#define TRUNCATE(val, numDecimalPlaces) \
  (float)(((long)((double)(val) * pow(10, (numDecimalPlaces) * 1.0f))) / (pow(10, (numDecimalPlaces)* 1.0f)))



void setup() {
  dht.begin();
  Serial.begin(19200);
}

void loop() {
  float hs = dht.readHumidity(); //Luftfeuchte auslesen
  float ts = dht.readTemperature(); //Temperatur auslesen
  int   t = ts * 10.0f;
  int   h = hs * 10.0f;


  int num3T = (t) % 10;
  int num2T = (t / 10) % 10;
  int num1T = (t / 100) % 10;

  int num3H = (h) % 10;
  int num2H = (h / 10) % 10;
  int num1H = (h / 100) % 10;

  Serial.print(num1T);
  Serial.print(num2T);
  Serial.println(num3T);

  Serial.print(num1H);
  Serial.print(num2H);
  Serial.println(num3H);

  // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
  if (isnan(ts) || isnan(hs))
  {
    Serial.println("DHT22 konnte nicht ausgelesen werden");
  }
  else
  {
        Serial.print("Luftfeuchte: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperatur: ");
        Serial.print(t);
        Serial.println(" C");


   
    delay(1000);

  }
}
