#include "WiFi.h"
#include <DHT.h>
#include <FirebaseESP32.h>



#define FIREBASE_HOST "https://smartfarm-1a6b8-default-rtdb.firebaseio.com/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "e1ihC8M0zpVzKuTqj9UMBEzz2dxsCWf5A5ypCJxU"

// #define WIFI_SSID "project"
// #define WIFI_PASSWORD "12345678"

#define WIFI_SSID "OPPO A33"
#define WIFI_PASSWORD "00181014"
// #define WIFI_SSID "¥¥Anonymous¥¥"
// #define WIFI_PASSWORD "$$yournotsafe$$"

#define DHTPIN 27		// Connect Data pin of DHT to D27
int pump = 5;			// Connect relay to D5  
int _moisture,sensor_analog;
const int sensor_pin = A0;        //pin = vp
float temp = 30.60,hum= 66.20,fr= 84.95;
int mois;

#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData moisData;

FirebaseJson json;



void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(pump,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C  ,"));
  Serial.print(fr);
  Serial.println(F("°F  "));

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
  {
    temp = t;
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", h))
  { 
    hum = h;
    fr = f;
    Serial.println();
  }
  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C  ,"));
  Serial.print(fr);
  Serial.println(F("°F  "));

}

void pumpSensor(){
  int m;
  sensor_analog = analogRead(sensor_pin);
  m = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  delay(100);
  
  if (Firebase.setInt(firebaseData, "/FirebaseIOT/mois", m))
  {
    mois = m;
    Serial.println();
  }

  Serial.print(F("Moisture: "));
  Serial.print(mois);
  Serial.println(F("%  "));

  int intValue = m;


  if (intValue <= 50) {
      digitalWrite(pump, HIGH); // Set the pump pin HIGH
      Serial.println("Pump Start");
  } else if (intValue >= 51) {
      digitalWrite(pump, LOW); // Set the pump pin LOW
  }
  
}

void loop() {
  sensorUpdate();
  delay(100);
  pumpSensor();
  delay(100);

}
