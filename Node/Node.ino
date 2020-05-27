#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson 5.13.2
#include <NTPClient.h>
#include <WiFiUdp.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""


const int TouchPin=D1;
const char* name="Sentra 01";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  timeClient.begin();
  // GMT -6 = -21600
  timeClient.setTimeOffset(-21600);
  
  const int TouchPin=D1;
  Serial.begin(9600);
  delay(7000);
  Serial.println(" -- ITCC --");
  Serial.println("");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  Serial.println("Capacitive Sensor");
  Serial.println(name);
  

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.print(WIFI_SSID);
  Serial.println("");

  Serial.print("Ip: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Firebase connection: Ok");
}

  boolean flag=true;
void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay =ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  String formattedTime = timeClient.getFormattedTime();
  int currentYear = ptm->tm_year+1900;


  
  if (flag){
    Serial.print("Date: ");
    Serial.print(monthDay);
    Serial.print("/");
    Serial.print(currentMonth);
    Serial.print("/");
    Serial.println(currentYear);

    Serial.print("Hour: ");
    Serial.println(formattedTime); 
    Serial.println("");
    Serial.println("-Logs-");
    Serial.println("");
    
    flag=false;
  }



  Serial.print(formattedTime);  

   int sensorValue = digitalRead(TouchPin);

   Serial.print(" - Status: ");
   if(sensorValue==0){
       sendData( currentYear, currentMonth,  monthDay,  formattedTime);
    }else{
       Serial.print("Ok");
       
    }

  Serial.println("");
  delay(2000);
}


void sendData(int currentYear, int currentMonth, int monthDay, String formattedTime){
   Serial.print("Alert- HTTP POST REQUEST: "); 
   StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
   JsonObject& JSONencoder = JSONbuffer.createObject();

   JSONencoder["date"] = String(currentYear)+"-"+String(currentMonth)+"-"+String(monthDay)+" "+formattedTime;
   JSONencoder["car"] = name;
   JSONencoder["status"] = "Alert";
   

   char JSONmessageBuffer[300];
   JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

   //Serial.println(JSONmessageBuffer);
   
  String name = Firebase.push("logs", JSONencoder);
  // handle error
  if (Firebase.failed()) {
      Serial.print("Firebase Error:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("HTTP 200 Ok ");
  Serial.print(name);


  }
  
  
