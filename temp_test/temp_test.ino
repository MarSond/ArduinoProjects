#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
  Serial.begin(19200);
}

void loop() {
  float h = dht.readHumidity(); //Luftfeuchte auslesen
  float t = dht.readTemperature(); //Temperatur auslesen

  // Prüfen ob eine gültige Zahl zurückgegeben wird. Wenn NaN (not a number) zurückgegeben wird, dann Fehler ausgeben.
  if (isnan(t) || isnan(h))
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
    delay(2000);
  }
}
