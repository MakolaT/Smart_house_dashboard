#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <WiFiManager.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

// WiFi settings
#define WIFI_SSID "Thabiso"
#define WIFI_PASSWORD "tdzc3314"

// ESP8266WebServer server(80);

// Firebase settings
#define FIREBASE_HOST "smart-house-cbef7-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "QwfMBlApj2mgdCO05BYrVsyBwOhRzMKUYA0i63P6"

// DHT sensor settings
#define DHTPIN 14
#define DHTTYPE DHT11

// Relay control pins
int Relay1 = 12;    // D6
int Relay2 = 16;    // D0
int Relay3 = 13;    // D7
int Relay4 = 1;     // TX
int pirPin = 0;     // D3
int rainSensorPin = D8; // Rain sensor pin
int currentPin = A0; // SD2 ACS712 current sensor pin
int buzzerPin = 3; // Digital pin for the buzzer RX
int moi = D4;

#define I2C_ADDR    0x27
#define LCD_COLS    16
#define LCD_ROWS    2

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
Servo servo;

FirebaseData firebaseData;
void updateFirebaseData(const String& key);
// String firebasePath = "/data";

void handleFileRead(String path) {
  if (path.endsWith("/")) {
    path += "index.html";
  }

  String contentType = "text/plain";
  if (path.endsWith("index.html")) {
    contentType = "text/html";
  } else if (path.endsWith("style.css")) {
    contentType = "text/css";
  } else if (path.endsWith("smart.js")) {
    contentType = "application/javascript";
  } else if (path.endsWith(".ico")) {
    contentType = "image/x-icon";
  }

  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return;
  }

  server.send(404, "text/plain", "File Not Found");
}

void handleRoot() {
  handleFileRead("/index.html");
}

void handleRelay1On() {
  digitalWrite(Relay1, HIGH);
  server.send(200, "text/plain", "Relay 1 is ON");
}

void handleRelay1Off() {
  digitalWrite(Relay1, LOW);
  server.send(200, "text/plain", "Relay 1 is OFF");
}

void handleRelay2On() {
  digitalWrite(Relay2, HIGH);
  server.send(200, "text/plain", "Relay 2 is ON");
}

void handleRelay2Off() {
  digitalWrite(Relay2, LOW);
  server.send(200, "text/plain", "Relay 2 is OFF");
}

void handleRelay3On() {
  digitalWrite(Relay3, HIGH);
  server.send(200, "text/plain", "Relay 3 is ON");
}

void handleRelay3Off() {
  digitalWrite(Relay3, LOW);
  server.send(200, "text/plain", "Relay 3 is OFF");
}

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(pirPin, INPUT_PULLUP);
  pinMode(rainSensorPin, INPUT);
  pinMode(moi, INPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  digitalWrite(Relay3, LOW);
  digitalWrite(Relay4, LOW);

  Serial.begin(9600);

  WiFiManager wifiManager;
  wifiManager.autoConnect("MyWiFi");

  if (!LittleFS.begin()) {
    Serial.println("Error initializing LittleFS");
    while (1) {
      delay(1000);
    }
  }

  server.on("/", handleRoot);
  server.on("/relay1on", handleRelay1On);
  server.on("/relay1off", handleRelay1Off);
  server.on("/relay2on", handleRelay2On);
  server.on("/relay2off", handleRelay2Off);
  server.on("/relay3on", handleRelay3On);
  server.on("/relay3off", handleRelay3Off);

  server.onNotFound([]() {
    handleFileRead(server.uri());
  });

  server.begin();
  Serial.println("HTTP server started");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  dht.begin();

  lcd.begin();
  lcd.print("Hello");

  servo.attach(D8);
}

void loop() {
  float hum = dht.readHumidity();
  float tem = dht.readTemperature();
  int moiValue = digitalRead(moi);
  int pirState = digitalRead(pirPin);
  int rainSensorValue = digitalRead(rainSensorPin);
  float current = getCurrent();

  Serial.println(moiValue);
  delay(1000);

  Serial.println(hum);
  delay(1000);

  Serial.println(tem);
  delay(1000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moiValue);

  lcd.setCursor(0, 1);
  lcd.print("PIR: ");
  if (digitalRead(Relay3) == LOW) {
    if (pirState == HIGH) {
      lcd.print("motion detected");
      tone(buzzerPin, 1000);
      delay(1000);
      noTone(buzzerPin);
      delay(1000);
    } else {
      lcd.print("no motion");
      noTone(buzzerPin);
    }
  } else {
    lcd.print("alarm off");
    noTone(buzzerPin);
  }

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Curr: ");
  lcd.print(current);
  lcd.print("A");

  lcd.setCursor(0, 1);
  lcd.print("Rain: ");
  if (rainSensorValue > 500) {
    lcd.print("rain detected");
    servo.write(180);
    delay(2000);
  } else {
    lcd.print("no rain");
  }

  updateFirebaseData("humidity");
  updateFirebaseData("temperature");
  updateFirebaseData("moisture");
  updateFirebaseData("pirState");
  updateFirebaseData("rainSensorValue");
  updateFirebaseData("current");

  server.handleClient();

  WiFiClient client = server.client();
  if (!client)
    return;

  String data = checkClient(client);
  Serial.print(data);

  if (data == "Relay1ON") {
    digitalWrite(Relay1, LOW);
  } else if (data == "Relay1OFF") {
    digitalWrite(Relay1, HIGH);
  } else if (data == "Relay2ON") {
    digitalWrite(Relay2, LOW);
  } else if (data == "Relay2OFF") {
    digitalWrite(Relay2, HIGH);
  } else if (data == "Relay3ON") {
    digitalWrite(Relay3, LOW);
  } else if (data == "Relay3OFF") {
    digitalWrite(Relay3, HIGH);
  } else if (data == "AlarmOn" && pirState == HIGH) {
    lcd.print("motion detected");
    tone(buzzerPin, 1000);
    delay(100);
    noTone(buzzerPin);
    delay(100);
  } else if (data == "AlarmOff") {
    lcd.print("alarm off");
    noTone(buzzerPin);
  }
}

void updateFirebaseData(const String& key) {
  float value;

  if (key == "temperature")
    value = dht.readTemperature();
  else if (key == "humidity")
    value = dht.readHumidity();
  else if (key == "moisture")
    value = analogRead(A0);
  else if (key == "current")
    value = getCurrent();

  Firebase.setFloat(firebaseData, key, value);

  if (firebaseData.httpCode() == 200) {
    Serial.println(key + " updated successfully!");
  } else {
    Serial.println("Failed to update " + key + "!");
  }
}

String checkClient(WiFiClient& client) {
  while (!client.available())
    delay(1);

  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}

float getCurrent() {
  int sensorValue = analogRead(currentPin);
  float voltage = sensorValue * (3.3 / 1023.0);
  float current = (voltage - 1.65) / 0.066;
  return current;
}
