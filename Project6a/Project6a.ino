#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT22 Settings
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Initialize DHT22
  dht.begin();

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (1)
      ;
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();
}

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check sensor reading
  if (isnan(humidity) || isnan(temperature)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println("Sensor Error!");
    display.display();

    Serial.println("DHT22 Read Failed!");
    delay(2000);
    return;
  }

  // Comfort Status
  String comfort;

  if (temperature > 28) {
    comfort = "TOO HOT";
  } else if (temperature < 20) {
    comfort = "TOO COLD";
  } else if (humidity > 70) {
    comfort = "HUMID";
  } else if (humidity < 40) {
    comfort = "DRY";
  } else {
    comfort = "COMFORT";
  }

  // Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C   ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %   ");

  Serial.print("Status: ");
  Serial.println(comfort);

  // OLED Display
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Temp & Humidity");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Hum : ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 50);
  display.print("Status:");
  display.print(comfort);

  display.display();

  delay(2000);
}