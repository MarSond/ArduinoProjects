#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>


#define DHT_PIN 5
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);
unsigned long lastMeasure = 0;
const long timestep = 2 * 1000; //seconds * milliseconds

#define WIFI_NAME "Westerbleichstr. 58"
#define WIFI_PASSWORD "WeBleStr58"
#define DEVICE_ID 2
#define DEVICE_NAME "espmega"
#define TOKEN "~1282595_nCVdBbs0mWWCU182f8WwGEAK"

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

void startWifi() {
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  int conntimer=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    conntimer++;
    Serial.print("... ");
    Serial.println(conntimer);
  }
  Serial.println("-----------------");
  Serial.println(WiFi.localIP());
  remoteMe.setConnector(new RemoteMeSocketConnector());
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}

void setup() {
  pinMode(DHT_PIN, INPUT);
  dht.begin();
  Serial.begin(115200);
  delay(1500);
  startWifi();
  Serial.print("setup done");
}

void loop() {
  remoteMe.loop();
  if (millis() - lastMeasure > timestep) {
    lastMeasure = millis();
    float hs = dht.readHumidity(); //Luftfeuchte auslesen
    float ts = dht.readTemperature(); //Temperatur auslesen

    displayValues(hs, ts);
  }

}

void displayValues(float hum, float temp) {
  if (isnan(hum)) {
    Serial.println("Fehler! NaN");
  } else {
    Serial.print(hum); Serial.print(" "); Serial.println(temp);
  }
}
