#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "-"
#define WIFI_PASSWORD "-"
#define API_KEY "-"
#define DATABASE_URL "-"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

const int buzzer = 19;
const int LEDPIN = 5;
unsigned long previousMillis = 0;
const long interval = 3000;
bool switchval,lswitch,Sswitch;
void setup()
{
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(300);
  }

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    signupOK = true;
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{  
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval)
  {
   previousMillis = currentMillis;
   static int firebaseStep = 0; // Variable to keep track of the current Firebase code block

  switch (firebaseStep)
  {
    case 0:
      if (Firebase.RTDB.getBool(&fbdo, F("detected_classes/switch")))
      {
        switchval = fbdo.boolData();
      }
      break;

    case 1:
      if (Firebase.RTDB.getBool(&fbdo, F("detected_classes/lswitch")))
      {
        lswitch = fbdo.boolData();
      }
      break;

    case 2:
      if (Firebase.RTDB.getBool(&fbdo, F("detected_classes/Sswitch")))
      {
        Sswitch = fbdo.boolData();
      }
      break;
  }
  firebaseStep = (firebaseStep + 1) % 3;
}

  if (switchval)
  {
    digitalWrite(LEDPIN, LOW);
    
for (unsigned char i = 0; i < 80; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(1); 
  digitalWrite(buzzer, LOW);
  delay(1); 
}


for (unsigned char i = 0; i < 100; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(2); 
  digitalWrite(buzzer, LOW);
  delay(2); 
}

  }

else if(lswitch && Sswitch){
   digitalWrite(LEDPIN, LOW);
  
for (unsigned char i = 0; i < 80; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(1); 
  digitalWrite(buzzer, LOW);
  delay(1); 
}


for (unsigned char i = 0; i < 100; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(2); 
  digitalWrite(buzzer, LOW);
  delay(2); 
}

}


else if(lswitch){
  digitalWrite(LEDPIN, LOW);

}

else if(Sswitch){
  for (unsigned char i = 0; i < 80; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(1); 
  digitalWrite(buzzer, LOW);
  delay(1); 
}

for (unsigned char i = 0; i < 100; i++)
{
  digitalWrite(buzzer, HIGH);
  delay(2); 
  digitalWrite(buzzer, LOW);
  delay(2); 
}
}


  else
  {
    digitalWrite(LEDPIN, HIGH);
  }
  Serial.println(switchval);
  delay(10);
}
