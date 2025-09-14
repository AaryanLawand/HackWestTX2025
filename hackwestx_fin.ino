#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_CCS811.h>
#include <DHT11.h>

// ---------- WiFi ----------
const char* ssid     = "TTUguest";
const char* password = "maskedraiders";

// ---------- ThingSpeak ----------
const char* host = "api.thingspeak.com";
String writeAPIKey = "ONUHN6P1IG7KG91P";

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Sensors ----------
DHT11 dht11(2);                // DHT11 on GPIO2
Adafruit_CCS811 ccs;
Adafruit_MPL3115A2 baro;

WiFiClient client;

void setup() {
  Serial.begin(9600);

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Booting...");
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  display.println("Connecting WiFi...");
  display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: "); Serial.println(WiFi.localIP());

  display.println("WiFi connected");
  display.display();
  delay(1000);

  // Init sensors
  if (!baro.begin()) {
    Serial.println("MPL3115A2 not found");
    display.println("Baro error");
    display.display();
    while (1);
  }
  baro.setSeaPressure(1013.26);

  if (!ccs.begin()) {
    Serial.println("CCS811 not found");
    display.println("CCS811 error");
    display.display();
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Ready");
  display.display();
  delay(500);
}

void loop() {
  // --- Read sensors ---
  int humidity = dht11.readHumidity();
  float pressure = baro.getPressure();
  float altitude = baro.getAltitude();
  float temperature = baro.getTemperature();

  uint16_t eco2 = 0;
  uint16_t tvoc = 0;

  if (ccs.available() && !ccs.readData()) {
    eco2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();
  }
  

  // Debug to Serial
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print("C, Hum: "); Serial.print(humidity);
  Serial.print("%, Pressure: "); Serial.print(pressure);
  Serial.print("hPa, Alt: "); Serial.print(altitude);
  Serial.print("m, eCO2: "); Serial.print(eco2);
  Serial.print("ppm, TVOC: "); Serial.println(tvoc);

  // --- Display on OLED ---
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Temp: "); display.print(temperature); display.println("C");
  display.print("Hum: "); display.print(humidity); display.println("%");
  display.print("Press: "); display.print(pressure); display.println("hPa");
  display.print("Alt: "); display.print(altitude); display.println("m");
  display.print("eCO2: "); display.print(eco2); display.println("ppm");
  display.print("TVOC: "); display.print(tvoc); display.println("ppb");
  display.display();

  // --- Send to ThingSpeak ---
  String url = "/update?api_key=" + writeAPIKey;
  url += "&field1=" + String(temperature);
  url += "&field2=" + String(pressure);
  url += "&field3=" + String(altitude);
  url += "&field4=" + String(humidity);
  url += "&field5=" + String(eco2);
  url += "&field6=" + String(tvoc);

  Serial.print("Connecting to ThingSpeak...");
  if (client.connect(host, 80)) {
    Serial.println("connected");
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
  }

  // Optional: read server reply
  while (client.connected() || client.available()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }
  client.stop();

  // ThingSpeak limit: 15 seconds minimum
  delay(15000); // 20 seconds between updates
}
