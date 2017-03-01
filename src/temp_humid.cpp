#include <Arduino.h>
#include <Wire.h>
#include <gradient.h>
#include <rgb_lcd.h>
#include <windmill_encoding.h>
#include <x10rf.h>
#include <TH02_dev.h>

//Minimum and maximum temperature (C) used for calculating the background color gradient.
const int MIN_TEMP = 16;
const int MAX_TEMP = 40;

//Gradient colors
const RGB START_COLOR(0,0,255); //Blue
const RGB BETWEEN_COLOR(0,255,0); //Green
const RGB END_COLOR(255,0,0); //Red
const int NUMBER_OF_COLORS = 3;

//Pin definitions
const int RF_TX_PIN = 3;      //433 mhz transmitter
const int RF_TXLED_PIN = 7;  //Optional LED that blinks when a signal is transmitted
const int BUTTON_PIN = -1;   //optional button for switching the backlight on and off (for example if you use battery power, you can conserve power this way). -1 is always on.

//RF parameters
const int RF_REPEATS = 4;               //Number of times the value is sent in one go
const long SEND_INTERVAL = 5000;        //Send interval in milliseconds

//Sensor related
const int SENSOR_ID_TEMP = 0xFF;
const int SENSOR_ID_HUMIDITY = 0xFE;
const int TEMP_QTY = 0x00;              //Quantity type as specified per the Windmill encoding
const int HUMIDITY_QTY = 0x01;

Gradient gradient;
rgb_lcd lcd;
x10rf rftrx = x10rf(RF_TX_PIN,RF_TXLED_PIN,RF_REPEATS);

unsigned long previousMillis;

// int value;
// int min;
// int max;

void setupGradient() {
  RGB colors[3];
  colors[0] = START_COLOR;
  colors[1] = BETWEEN_COLOR;
  colors[2] = END_COLOR;

  gradient.init(MIN_TEMP, MAX_TEMP, colors, NUMBER_OF_COLORS);
}

void setupRF() {
  rftrx.begin();
}

void setupLcd() {
  lcd.begin(16, 2);
  lcd.clear();
}

void setupTH02() {
  delay(150);
  TH02.begin();
  delay(100);
}

void setup() {
  setupGradient();
  setupRF();
  setupLcd();
  setupTH02();
}

float getTemperature() {
  return TH02.ReadTemperature();
}

float getHumidity() {
  return TH02.ReadHumidity();
}

void printTempAndHumidity(float temp, float humidity, int buttonState) {
  lcd.setCursor(0,0);
  lcd.print("Temp=");
  lcd.print(temp);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Humidity=");
  lcd.print(humidity);
  lcd.print(" %");

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn backlight on:
    RGB color = gradient.getColorFromValue((int)temp);
    lcd.setRGB(color.r, color.g, color.b);
  }
  else
  {
    lcd.setRGB(0,0,0);
  }
  delay(500);
}

void sendValue(float value, int sensorId, int type) {

  long norm_value = (long)(value * 100);
  WindmillValue wmValue;
  wmValue.value = norm_value;
  wmValue.quantity = type;

  uint32_t encoded;
  encode(wmValue, &encoded);

  //Send it according to the RFXMeter protocol with value encoded according to the Windmill encoding
  rftrx.RFXmeter(sensorId,0,encoded);
}



void loop() {

  float temp = getTemperature();
  float humidity = getHumidity();

  int buttonState = HIGH;

  if (BUTTON_PIN != -1) {
    buttonState = digitalRead(BUTTON_PIN);
  }

  printTempAndHumidity(temp, humidity, buttonState);
  unsigned long currentMillis = millis();

  //When interval has elapsed send value
  if (currentMillis - previousMillis >= SEND_INTERVAL) {
     lcd.setCursor(14,0);
     lcd.print("*");
     previousMillis = currentMillis;
     sendValue(temp, SENSOR_ID_TEMP, TEMP_QTY);
     delay(500);
     sendValue(humidity, SENSOR_ID_HUMIDITY, HUMIDITY_QTY);
     lcd.setCursor(14,0);
     lcd.print(" ");
  }
}
