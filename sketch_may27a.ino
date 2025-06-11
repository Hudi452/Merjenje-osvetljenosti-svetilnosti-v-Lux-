#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// pin na katerega je priključen fotoupor
#define photopin A0

// stikalo za preklop prikaza
#define SW1 5

// pin za bazo tranzistorja
#define TRANSISTOR_PIN 2

// pini za Ledice
#define redLED 11
#define yellowLED 9
#define greenLED 7

LiquidCrystal_I2C lcd(0x27, 16, 2);

// spremenljivke za napetosti, upornosti in svetilnost
float Ua, Um, R_LDR, lux;
float Ucc = 5.0;
const float R1_10k = 10000.0;
const float R1_1k = 1000.0;
float R1_effective;  

const int numPoints = 49; // število meritev upornosti in svetilnosti za kalibracijo
const int lux_kvadrat = 200; // število lux-ov, ki jih predstavlja 1 kvadratek
int kvadrati;

// refernčna razpredelnica upornosti za kalibracijo
float R_ref[numPoints] = {
  670000, 148900, 79900, 22340, 14410, 7820, 5713, 4532, 3751, 3279, 2838, 2551, 2346,
  2167, 2045, 1833, 1719, 1671, 1602, 1528, 1494, 1405, 1392, 1324, 1000, 732,
  670, 597, 523, 469, 458, 420, 374, 355, 337, 330, 311, 306, 290,
  285, 277, 264, 252, 244, 233, 223, 206, 174
};

// referenčna razpredelnica svetilnosti za kalibracijo
float lux_ref[numPoints] = {
  0, 5, 10, 30, 50, 100, 150, 200, 250, 300, 350, 400, 450,
  500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1500, 2000,
  2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500,
  9000, 9500, 10000, 11000, 12000, 13000, 14000, 15000, 20000
};

// Izračunaj upornost LDR
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

// Prižig LED-ic glede na osvetljenost prostora
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

  digitalWrite(TRANSISTOR_PIN, LOW); // tranzistor je od začetka zaprt

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);

// kreiranje smiley
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

// kreiranje kvadrata
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

// začetni izpis na LCD-ju
  lcd.setCursor(0, 0);
  lcd.print("Pozdravljeni ");
  lcd.write(byte(0));
  delay(2000);
  lcd.clear();
}

void loop() {
// izmeri povprečno vrednost na 200 ms
  float R_total = 0;
  for (int i = 0; i < 5; i++) {
    R_total += readLDRResistance(); 
    delay(200); // 5 vzorcev se vzame 
  }
  R_LDR = R_total / 5.0;
  lux = getLuxFromResistance(R_LDR);
  fromLuxToLed(lux);

// Izpis meritve na serijskem zaslonu
  Serial.print("Upornost LDR = ");
  Serial.print(R_LDR);
  Serial.print(" Ohm. Svetilnost = ");
  Serial.print(lux);
  Serial.println(" lux.");

// preklop med izpisom osvetljenosti v lux-ih in grafičnim prikazom s kvadratki
  if (digitalRead(SW1) == LOW) {
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

// Ta funkcija vklaplja in izklaplja tranzistor (dva merilna območja)
float readLDRResistance() {
  digitalWrite(TRANSISTOR_PIN, LOW); // Tranzistor izklopljen → 10kΩ upor
  delay(5);
  int adcValue = analogRead(photopin);
  float voltage = adcValue * Ucc / 1023.0;

  // Preveri, če je napetost preveč nizka in preklopi na 2. merilno območje
  if (voltage < 0.5) {
    digitalWrite(TRANSISTOR_PIN, HIGH); // Transistor vklopljen → 10k||1k
    delay(5);
    adcValue = analogRead(photopin);
    voltage = adcValue * Ucc / 1023.0;
    R1_effective = (R1_10k * R1_1k) / (R1_10k + R1_1k); // ~909Ω
  } else {
    R1_effective = R1_10k;
  }
  return R1_effective * ((Ucc - voltage) / voltage);
}