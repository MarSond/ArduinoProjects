#include <TM1637.h>
#include <TM1637Display.h>
#include <DHT.h>

#include <ESP8266WiFi.h>


#define DHTPIN 12 //8
#define DHTTYPE DHT22

#define CLK 4 //PIN 2 für den CLK PIN
#define DIO 5 //PIN3 für den DIO 
//erzeugen des Display Objektes mit den Parametern für die PINs
TM1637Display display(CLK, DIO);

DHT dht(DHTPIN, DHTTYPE);

const uint8_t SEG_NUM[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // 0
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // 1
  SEG_C | SEG_E | SEG_G,                           // 2
  SEG_C | SEG_E | SEG_G,
  SEG_C | SEG_E | SEG_G,
  SEG_C | SEG_E | SEG_G,
  SEG_C | SEG_E | SEG_G,
  SEG_C | SEG_E | SEG_G,
  SEG_C | SEG_E | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
};

#define TRUNCATE(val, numDecimalPlaces) \
  (float)(((long)((double)(val) * pow(10, (numDecimalPlaces) * 1.0f))) / (pow(10, (numDecimalPlaces)* 1.0f)))

void blank() {
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  //Setzt die Anzahl der möglichen Segmente.
  display.setSegments(data);
}

void connectWifi(){
  WiFi.begin("TP-LINK_2DCF2C", "63179026");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  display.setBrightness(8);
  blank();
  connectWifi();
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
    //    Serial.print("Luftfeuchte: ");
    //    Serial.print(h);
    //    Serial.print(" %\t");
    //    Serial.print("Temperatur: ");
    //    Serial.print(t);
    //    Serial.println(" C");


    uint8_t dataT[] = {0x0, 0x0, 0x0, 0b01100011};
    dataT[0] = display.encodeDigit(num1T);
    dataT[1] = display.encodeDigit(num2T);
    dataT[2] = display.encodeDigit(num3T);
    //dataT[3] = display.encodeDigit(12);

    uint8_t dataH[] = {0x0, 0x0, 0x0, 0b01110110};
    dataH[0] = display.encodeDigit(num1H);
    dataH[1] = display.encodeDigit(num2H);
    dataH[2] = display.encodeDigit(num3H);
    //dataH[3] = display.encodeDigit(10);


    display.setSegments(dataT);
    delay(3000);
    display.setSegments(dataH);
    //display.showNumberDecEx(t,0, false, 3);

    // display.showNumberDecEx(h, 80,false, 3);
    delay(3000);

  }
}
