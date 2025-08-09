#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  if (!bmp.begin(0x77)) {
    Serial.println("BMP280 not found!");
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,   // temperature
                  Adafruit_BMP280::SAMPLING_X16,  // pressure
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  Serial.println("Collecting baseline pressure...");
}

void loop() {
  static float sum = 0;
  static int count = 0;

  float pressure_hPa = bmp.readPressure() / 100.0F; // Pa → hPa
  sum += pressure_hPa;
  count++;

  Serial.print("Instant: "); Serial.print(pressure_hPa, 2); Serial.print(" hPa   ");
  Serial.print("Avg: "); Serial.print(sum / count, 2); Serial.println(" hPa");

  delay(500); // adjust sample rate
}
