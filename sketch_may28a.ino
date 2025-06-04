#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// pin na katerega je priključen fotoupor
#define photopin A0

// stikalo za preklop prikaza
#define SW1 5

// transistor control pin
#define TRANSISTOR_PIN 6

// pini za Ledice
#define redLED 11
#define yellowLED 9
#define greenLED 7

LiquidCrystal_I2C lcd(0x27, 16, 2);

float Ua, Um, R_LDR, lux;
float Ucc = 5.0;
const float R1_10k = 10000.0;
const float R1_1k = 1000.0;
float R1_effective;  // effective pull-up resistance

const int numPoints = 49;
const int lux_kvadrat = 500;
int kvadrati;

float R_ref[numPoints] = {
  670000, 148900, 79900, 22340, 14410, 7820, 5713, 4532, 3751, 3279, 2838, 2551, 2346,
  2167, 2045, 1833, 1719, 1671, 1602, 1528, 1494, 1405, 1392, 1324, 1000, 732,
  670, 597, 523, 469, 458, 420, 374, 355, 337, 330, 311, 306, 290,
  285, 277, 264, 252, 244, 233, 223, 206, 174
};

float lux_ref[numPoints] = {
  0, 5, 10, 30, 50, 100, 150, 200, 250, 300, 350, 400, 450,
  500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1500, 2000,
  2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500,
  9000, 9500, 10000, 11000, 12000, 13000, 14000, 15000, 20000
};

// Compute lux based on LDR resistance
float getLuxFromResistance(float R) {
  for (int i = 0; i < numPoints - 1; i++) {
    if (R <= R_ref[i] && R >= R_ref[i + 1]) {
      float r1 = R_ref[i];
      float r2 = R_ref[i + 1];
      float l1 = lux_ref[i];
      float l2 = lux_ref[i + 1];
      return l1 + (R - r1) * (l2 - l1) / (r2 - r1);
    }
  }
  return -1;
}

// Control LEDs based on lux level
void fromLuxToLed (float lux){
  digitalWrite(greenLED, lux <= 500.0 ? HIGH : LOW);
  digitalWrite(yellowLED, (lux > 500.0 && lux < 2000.0) ? HIGH : LOW);
  digitalWrite(redLED, lux >= 2000.0 ? HIGH : LOW);
}

void setup() {
  pinMode(photopin, INPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(TRANSISTOR_PIN, OUTPUT);

  digitalWrite(TRANSISTOR_PIN, LOW); // Start with transistor OFF (10k)

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);

  byte smiley[8] = {
    B00000,
    B01010,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000,
  };
  lcd.createChar(0, smiley);

  byte kvadrat[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  lcd.createChar(1, kvadrat);

  lcd.setCursor(0, 0);
  lcd.print("Pozdravljeni ");
  lcd.write(byte(0));
  delay(2000);
  lcd.clear();
}

void loop() {
  // Measure average over 1 second (5 samples)
  float R_total = 0;
  for (int i = 0; i < 5; i++) {
    R_total += readLDRResistance();
    delay(200); // 5 samples in 1 second
  }
  R_LDR = R_total / 5.0;
  lux = getLuxFromResistance(R_LDR);
  fromLuxToLed(lux);

  Serial.print("Upornost LDR = ");
  Serial.print(R_LDR);
  Serial.print(" Ohm. Svetilnost = ");
  Serial.print(lux);
  Serial.println(" lux.");

  if (digitalRead(SW1) == HIGH) {
    lcd.setCursor(0, 0);
    lcd.print("Osvetljenost");
    lcd.setCursor(2, 1);
    lcd.print(lux);
    lcd.print(" lux.");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Osvetljenost");
    kvadrati = lux / lux_kvadrat;
    for (int i = 0; i < kvadrati && i < 16; i++) {
      lcd.setCursor(i, 1);
      lcd.write(byte(1));
    }
  }

  delay(500);
  lcd.clear();
}

// This function handles resistor switching and calculates LDR resistance
float readLDRResistance() {
  digitalWrite(TRANSISTOR_PIN, LOW); // Transistor OFF → 10kΩ only
  delay(5);  // allow voltage to settle
  int adcValue = analogRead(photopin);
  float voltage = adcValue * Ucc / 1023.0;

  // Check if voltage is too low (LDR too small); switch transistor ON to lower pull-up
  if (voltage < 0.5) {
    digitalWrite(TRANSISTOR_PIN, HIGH); // Transistor ON → 10k||1k
    delay(5);
    adcValue = analogRead(photopin);
    voltage = adcValue * Ucc / 1023.0;
    R1_effective = (R1_10k * R1_1k) / (R1_10k + R1_1k); // ~909Ω
  } else {
    R1_effective = R1_10k;
  }
  
  return R1_effective * ((Ucc - voltage) / voltage);
}