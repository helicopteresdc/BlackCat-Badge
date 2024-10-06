#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "secrets.h"  // WiFi Configuration (WiFi name and Password)
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#define PIN 7
#define NUMPIXELS 2
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#include "pitches.h"
int melody[] = {
  NOTE_E4,
  NOTE_G4,
  NOTE_A4,
  NOTE_A4,
  0,
};

int noteDurations[] = {
  125, 125, 250, 125, 125
};
const char* ssid = SECRET_SSID;
const char* password = SECRET_WIFI_PASSWORD;

const int httpsPort = 443;
const String url = "http://api.coindesk.com/v1/bpi/currentprice/BTC.json";
const String historyURL = "http://api.coindesk.com/v1/bpi/historical/close.json";
const String cryptoCode = "BTC";

WiFiClient client;
HTTPClient http;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
const int songspeed = 1.5;
double prevpercentChange;
double prevBTCUSDPrice;
void setup() {
  Serial.begin(115200);
  pixels.begin();
  // Initialize display
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);  // Set font

  // Display WiFi connection status
  display.drawStr(0, 10, "Connecting to WiFi...");
  display.sendBuffer();

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  display.clearBuffer();
  display.drawStr(0, 10, "Connected to: ");
  display.drawStr(0, 20, ssid);
  display.sendBuffer();
  delay(1500);
  display.clearBuffer();
  display.sendBuffer();
}

void loop() {
  Serial.print("Connecting to ");
  Serial.println(url);

  http.begin(url);
  int httpCode = http.GET();
  StaticJsonDocument<2000> doc;
  DeserializationError error = deserializeJson(doc, http.getString());

  if (error) {
    Serial.print(F("deserializeJson Failed"));
    Serial.println(error.f_str());
    delay(2500);
    return;
  }

  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);

  String BTCUSDPrice = doc["bpi"]["USD"]["rate_float"].as<String>();
  String lastUpdated = doc["time"]["updated"].as<String>();
  http.end();

  Serial.print("Getting history...");
  StaticJsonDocument<2000> historyDoc;
  http.begin(historyURL);
  int historyHttpCode = http.GET();
  DeserializationError historyError = deserializeJson(historyDoc, http.getString());

  if (historyError) {
    Serial.print(F("deserializeJson(History) failed"));
    Serial.println(historyError.f_str());
    delay(2500);
    return;
  }

  Serial.print("History HTTP Status Code: ");
  Serial.println(historyHttpCode);
  JsonObject bpi = historyDoc["bpi"].as<JsonObject>();
  double yesterdayPrice;
  for (JsonPair kv : bpi) {
    yesterdayPrice = kv.value().as<double>();
  }

  Serial.print("BTCUSD Price: ");
  Serial.println(BTCUSDPrice.toDouble());

  Serial.print("Yesterday's Price: ");
  Serial.println(yesterdayPrice);

  bool isUp = BTCUSDPrice.toDouble() > yesterdayPrice;
  double percentChange;

  if (prevBTCUSDPrice - BTCUSDPrice.toDouble() < 0) {
    pixels.clear();
    pixels.setPixelColor(1, pixels.Color(0, 150, 0));
    pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    pixels.show();
    for (int thisNote = 0; thisNote < 5; thisNote++) {
      int wait = noteDurations[thisNote] * songspeed;
      tone(1, melody[thisNote], wait);
      delay(wait);
      noTone(1);
    }
    pixels.clear();
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }
  if (prevBTCUSDPrice - BTCUSDPrice.toDouble() > 0) {
    pixels.clear();
    pixels.setPixelColor(1, pixels.Color(150, 0, 0));
    pixels.setPixelColor(0, pixels.Color(150, 0, 0));
    pixels.show();
    for (int thisNote = 0; thisNote < 5; thisNote++) {
      int wait = noteDurations[thisNote] * songspeed;
      tone(1, melody[thisNote], wait);
      delay(wait);
      noTone(1);
    }
    pixels.clear();
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }
  prevBTCUSDPrice = BTCUSDPrice.toDouble();

    String dayChangeString = "24hr. Change: ";
  if (isUp) {
    percentChange = ((BTCUSDPrice.toDouble() - yesterdayPrice) / yesterdayPrice) * 100;
  } else {
    percentChange = ((yesterdayPrice - BTCUSDPrice.toDouble()) / yesterdayPrice) * 100;
    dayChangeString = dayChangeString + "-";
  }

  // Display Header
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);  // Set font
  printCenter("BTC/USD", 0, 10);

  // Display BTC Price
  display.setFont(u8g2_font_ncenB14_tr);  // Set larger font for price
  printCenter("$" + BTCUSDPrice, 0, 30);
  display.setFont(u8g2_font_ncenB08_tr);  // Set smaller font
  dayChangeString = dayChangeString + String(percentChange, 2) + "%";
  printCenter(dayChangeString, 0, 55);
  display.sendBuffer();

  http.end();
  delay(5000);
}

// Function to center text
void printCenter(const String buf, int x, int y) {
  int16_t w = display.getStrWidth(buf.c_str());
  display.drawStr((128 - w) / 2, y, buf.c_str());
}
