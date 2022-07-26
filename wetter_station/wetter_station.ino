#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>


#define DHT_PIN 5
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);
unsigned long lastMeasure = 0;
long timestep = 10 * 1000; //seconds * milliseconds

#define WIFI_NAME "Westerbleichstr. 58"
#define WIFI_PASSWORD "WeBleStr58"
#define DEVICE_ID 2
#define DEVICE_NAME "espmega"
#define TOKEN "~1282595_nCVdBbs0mWWCU182f8WwGEAK"

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

inline void setHumm(double d) {remoteMe.getVariables()->setDouble("humidity", d); }
inline void setTemp(double d) {remoteMe.getVariables()->setDouble("temperatur", d); }

void onSleeptimeChange(int sleeptime){
  timestep=sleeptime;
  Serial.print("New sleeptime:");
  Serial.println(sleeptime);
}

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
  remoteMe.getVariables()->observeInteger("weather_sleeptime", onSleeptimeChange);
  remoteMe.setConnector(new RemoteMeSocketConnector());
	remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);
}

void setup() {
  pinMode(DHT_PIN, INPUT);
  dht.begin();
  Serial.begin(115200);
  delay(1500);
  startWifi();
  Serial.println("setup done");
}

void loop() {
  if (millis() - lastMeasure > timestep) {
    lastMeasure = millis();
    double hs = dht.readHumidity(); //Luftfeuchte auslesen
    double ts = dht.readTemperature(); //Temperatur auslesen
    displayValues(hs, ts);

    if(!remoteMe.loop()){//no connection established
        return;
    }
    setHumm(hs);
    setTemp(ts);
  }
}

void displayValues(float hum, float temp) {
  if(isnan(hum)) {
    Serial.println("Fehler! NaN");
  } else {
    Serial.print(hum); Serial.print(" "); Serial.println(temp);
  }

}
