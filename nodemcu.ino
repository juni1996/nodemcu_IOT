#include <ESP8266WiFi.h>

#define LM35  A0

const char* ssid     = "Nama Wifi Kamu";
const char* password = "Password Wifi Kamu";

const char* host = "appjuni.xyz";

WiFiClient client;
const int httpPort = 80;
String url, urlmonitor;

float data, suhu;
unsigned long timeout;
  
void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(LM35, INPUT);
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void baca_suhu(){
  data = analogRead(LM35);
  suhu = (data * 500) / 1024;
  Serial.print(suhu);
  Serial.println(" derajat");

    Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
  }

// We now create a URI for the request
  urlmonitor = "/nodemcu/Get/getSensor?nilai_sensor=";
  urlmonitor += suhu;
  
  Serial.print("Requesting URL: ");
  Serial.println(urlmonitor);

// This will send the request to the server
  client.print(String("GET ") + urlmonitor + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

// Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  Serial.println();
  delay(1000);
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    //return;
  }

// We now create a URI for the request
  url = "/nodemcu/Baca/baca";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);

// This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

// Read all the lines of the reply from server and print them to Serial
  while(client.available()){
//    String line = client.readStringUntil('\r');
//    Serial.print(line);
    if(client.find("ON")){
      digitalWrite(D0,HIGH);     //lampu on
      Serial.println("Lampu ON");
    }else{  
      digitalWrite(D0,LOW);    //lampu off
      Serial.println("Lampu OFF");
    }
  }

  Serial.println();
  Serial.println("closing connection");
  Serial.println();

  baca_suhu();
  
}
