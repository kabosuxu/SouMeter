/*
 * 2021/11/26 オンライン温湿度データロガー
 * 【機能】部屋の温湿度をLEDおよびインターネットにて表示
 * 【原理】温湿度センサ (DHT22) で得た値を7segLEDで表示　
 * 　　　  また10分おきにインターネット上にデータを送信
 */
/*Define01:WiFi*/
#include <WiFi.h>
const char* ssid     = ""; //Fill out SSID of your Internet Rooter(2.5GHz) 
const char* password = ""; //Fill out Password of your Internet Rooter(2.5GHz)
WiFiClient client;

/*Define02:Ambient*/
#include <Ambient.h>
Ambient ambient;
unsigned int channelId = ;//Fill out ID of your Ambient channel 
const char* writeKey = "";//Fill out Writekey of your Ambient channel 

/*Define03:Sensor*/
#include<DHT.h>
#define DHTPIN 27
#define DHTTYPE DHT22
//create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

/*Define04:7segLED*/
#include "sevensegLED.h"
#define DATAPIN 13  // ESP32(13)⇔74LC595(14)SI
#define LATCHPIN 32 // ESP32(32)⇔74LC595(12)RCK
#define CLOCKPIN 33 // ESP32(33)⇔74LC595(11)SCK
//create an instance of 7segLED
sevensegLED seg(DATAPIN, LATCHPIN, CLOCKPIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  /*Setup01:WiFi*/
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  /*Setup02:ambient*/
  ambient.begin(channelId, writeKey, &client);
  /*SetUp03:Sensor*/
  Serial.println("DHT22 sensor!");
  dht.begin();
  /*SetUp04:7segLED*/
  seg.begin();
}

int count = 0;

void loop() {
   //01:Measure humdata & tmpdata
   float h = dht.readHumidity();
   // Read temperature as Celsius (the default)
   float t = dht.readTemperature();
   if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
   }
   int tmp = t*10;
   int hum = h*10;
   //03:Print tmp hum [tmp:mode=1]
   int printmode = 1;
   seg.Show_seg(0,tmp/100%10,printmode);  //Tmp 10th place (1ms)
   seg.Show_seg(1,tmp/10%10,printmode);   //Tmp 1th place  (1ms)
   seg.Show_seg(2,tmp/1%10,printmode);    //Tmp 0.1th place(1ms)
   //04:Print data hum [hum:mode=2]
   printmode = 2;
   seg.Show_seg(0,hum/100%10,printmode);  //Hum 10th place (1ms)
   seg.Show_seg(1,hum/10%10,printmode);   //Hum 1th place  (1ms)
   seg.Show_seg(2,hum/1%10,printmode);    //Hum 0.1th place(1ms)
   //05:Send to Ambient (every 10 minuites)
   count++;
   if(count > 100000){
    ambient.set(1, t);
    ambient.set(2, h);
    ambient.send();
    Serial.println(h);
    Serial.println(t);
    count = 0;
   }
}
