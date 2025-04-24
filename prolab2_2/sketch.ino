#include <LiquidCrystal.h>

// Pin tanımlamaları
const int MOTOR_BUTTON = 2;    // Motor başlatma butonu
const int SEATBELT_BUTTON = 3;  // Emniyet kemeri butonu
const int DOOR_SWITCH = 4;      // Kapı anahtarı
const int BUZZER = 11;          // Buzzer
const int RED_LED = 5;          // Kemer LED
const int BLUE_LED = 6;         // Far LED
const int YELLOW_LED = 7;       // Yakıt LED
const int DOOR_LED_R = 8;       // Kapı LED (Kırmızı)
const int DOOR_LED_G = 9;       // Kapı LED (Yeşil)
const int DOOR_LED_B = 10;      // Kapı LED (Mavi)
const int LDR = A1;             // Işık sensörü
const int TEMP_SENSOR = A0;     // Sıcaklık sensörü
const int FUEL_SENSOR = A2;     // Yakıt sensörü
const int MAIN_MOTOR_LED = 12;  // Ana motor LED
const int FAN_MOTOR_LED = 13;   // Klima LED

// LCD pin tanımlamaları
const int RS = 22;
const int EN = 23;
const int D4 = 24;
const int D5 = 25;
const int D6 = 26;
const int D7 = 27;

// Değişkenler
bool motorStatus = false;
bool seatbeltStatus = false;
bool doorStatus = false;
int fuelLevel = 100;
float temperature = 0;
int lightLevel = 0;
int testScenario = 0;  // Seçilen test senaryosu
bool inTestMode = false; // Test modunda mı?
String serialInput = ""; // Serial girişi için değişken

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  // Pin modları
  pinMode(MOTOR_BUTTON, INPUT_PULLUP);
  pinMode(SEATBELT_BUTTON, INPUT_PULLUP);
  pinMode(DOOR_SWITCH, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW); // Buzzer'ı başlangıçta kapat
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(DOOR_LED_R, OUTPUT);
  pinMode(DOOR_LED_G, OUTPUT);
  pinMode(DOOR_LED_B, OUTPUT);
  pinMode(MAIN_MOTOR_LED, OUTPUT);
  pinMode(FAN_MOTOR_LED, OUTPUT);
  
  // LCD başlatma
  lcd.begin(16, 2);
  showTestMenu();
  
  Serial.begin(9600);
  Serial.println("Araç Kontrol Sistemi");
  Serial.println("Test Senaryoları:");
  Serial.println("1: Kemer Testi");
  Serial.println("2: Sıcaklık Testi");
  Serial.println("3: Far Testi");
  Serial.println("4: Yakıt Testi");
  Serial.println("5: Kapı Testi");
  Serial.println("6: Tüm Sistem");
  Serial.println("Lütfen bir senaryo seçin (1-6):");
}

void loop() {
  // Serial monitörden giriş kontrolü
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      if (serialInput.length() > 0) {
        int selectedScenario = serialInput.toInt();
        if (selectedScenario >= 1 && selectedScenario <= 6) {
          testScenario = selectedScenario - 1;
          inTestMode = true;
          startSelectedTest();
          Serial.print("Seçilen Senaryo: ");
          Serial.println(selectedScenario);
        } else {
          Serial.println("Geçersiz seçim! Lütfen 1-6 arası bir sayı girin.");
        }
        serialInput = "";
      }
    } else {
      serialInput += c;
    }
  }

  if (inTestMode) {
    runSelectedTest();
  }
  
  delay(100);
}

void showTestMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEST MENUSU");
  lcd.setCursor(0, 1);
  
  switch(testScenario) {
    case 0:
      lcd.print("1: KEMER TESTI");
      break;
    case 1:
      lcd.print("2: SICAKLIK TESTI");
      break;
    case 2:
      lcd.print("3: FAR TESTI");
      break;
    case 3:
      lcd.print("4: YAKIT TESTI");
      break;
    case 4:
      lcd.print("5: KAPI TESTI");
      break;
    case 5:
      lcd.print("6: TUM SISTEM");
      break;
  }
}

void startSelectedTest() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEST BASLIYOR");
  lcd.setCursor(0, 1);
  lcd.print("Cikis icin tusla");
  delay(2000);
}

void runSelectedTest() {
  switch(testScenario) {
    case 0:
      testMotorAndSeatbelt();
      break;
    case 1:
      testTemperature();
      break;
    case 2:
      testHeadlights();
      break;
    case 3:
      testFuel();
      break;
    case 4:
      testDoor();
      break;
    case 5:
      testAllSystems();
      break;
  }
}

void testAllSystems() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TUM SISTEM TESTI");
  lcd.setCursor(0, 1);
  lcd.print("CALISIYOR...");
  
  readSensors();
  checkMotor();
  checkSeatbelt();
  checkTemperature();
  checkLight();
  
  // Kapı durumu kontrolü
  doorStatus = digitalRead(DOOR_SWITCH);
  if (doorStatus) {
    // Kapı açık
    digitalWrite(DOOR_LED_R, HIGH);
    digitalWrite(DOOR_LED_G, LOW);
    digitalWrite(DOOR_LED_B, HIGH);
    
    motorStatus = false;
    digitalWrite(MAIN_MOTOR_LED, LOW);
  } else {
    // Kapı kapalı
    digitalWrite(DOOR_LED_R, LOW);
    digitalWrite(DOOR_LED_G, LOW);
    digitalWrite(DOOR_LED_B, LOW);
  }
  
  // Far durumunu göster
  if (lightLevel <= 250) {
    digitalWrite(BLUE_LED, HIGH);
  } else {
    digitalWrite(BLUE_LED, LOW);
  }
  
  // Yakıt seviyesi kontrolü
  if (fuelLevel == 0) {
    // Yakıt bitti
    motorStatus = false;
    digitalWrite(MAIN_MOTOR_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(DOOR_LED_R, LOW);
    digitalWrite(DOOR_LED_G, LOW);
    digitalWrite(DOOR_LED_B, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("YAKIT BITTI");
    lcd.setCursor(0, 1);
    lcd.print("MOTOR DURDU");
  } else if (fuelLevel < 5) {
    // Kritik seviye
    static unsigned long lastBlinkTime = 0;
    static bool yellowLedState = false;
    
    if (millis() - lastBlinkTime > 500) {
      yellowLedState = !yellowLedState;
      digitalWrite(YELLOW_LED, yellowLedState);
      lastBlinkTime = millis();
    }
    
    lcd.setCursor(0, 0);
    lcd.print("KRITIK: YAKIT AZ");
    lcd.setCursor(0, 1);
    lcd.print("SEVIYE: %");
    lcd.print(fuelLevel);
  } else if (fuelLevel < 10) {
    // Düşük seviye
    digitalWrite(YELLOW_LED, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("UYARI: YAKIT DUSUK");
    lcd.setCursor(0, 1);
    lcd.print("SEVIYE: %");
    lcd.print(fuelLevel);
  } else {
    // Normal seviye
    digitalWrite(YELLOW_LED, LOW);
    lcd.setCursor(0, 0);
    lcd.print("YAKIT NORMAL");
    lcd.setCursor(0, 1);
    lcd.print("SEVIYE: %");
    lcd.print(fuelLevel);
  }
}

void testMotorAndSeatbelt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KEMER TESTI");
  
  // Diğer sistemleri devre dışı bırak
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(DOOR_LED_R, LOW);
  digitalWrite(DOOR_LED_G, LOW);
  digitalWrite(DOOR_LED_B, LOW);
  digitalWrite(FAN_MOTOR_LED, LOW);
  
  // Kemer durumu kontrolü
  checkSeatbelt();
  
  // Motor butonu kontrolü
  static bool lastMotorButtonState = HIGH;
  bool currentMotorButtonState = digitalRead(MOTOR_BUTTON);
  
  if (currentMotorButtonState == LOW && lastMotorButtonState == HIGH) {
    delay(20); // Debounce için kısa bekleme
    currentMotorButtonState = digitalRead(MOTOR_BUTTON);
    
    if (currentMotorButtonState == LOW) { // Hala basılıysa
      if (!seatbeltStatus) {
        // Kemer takılı değilse
        motorStatus = false;
        digitalWrite(MAIN_MOTOR_LED, LOW);
        tone(BUZZER, 1000); // Buzzer'ı çalıştır
        digitalWrite(RED_LED, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("KEMER TAKILI DEGIL!");
      } else {
        // Kemer takılıysa ve motor kapalıysa
        if (!motorStatus) {
          motorStatus = true;
          digitalWrite(MAIN_MOTOR_LED, HIGH);
          lcd.setCursor(0, 1);
          lcd.print("MOTOR: CALISIYOR");
        }
      }
    }
  }
  lastMotorButtonState = currentMotorButtonState;
}

void testTemperature() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SICAKLIK TESTI");
  
  // Diğer sistemleri devre dışı bırak
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(DOOR_LED_R, LOW);
  digitalWrite(DOOR_LED_G, LOW);
  digitalWrite(DOOR_LED_B, LOW);
  digitalWrite(MAIN_MOTOR_LED, LOW);
  
  readSensors();
  checkTemperature();
  
  // Far durumunu kontrol et ve göster
  if (lightLevel <= 250) {
    digitalWrite(BLUE_LED, HIGH);
  } else {
    digitalWrite(BLUE_LED, LOW);
  }
  
  lcd.setCursor(0, 1);
  lcd.print("SICAKLIK: ");
  lcd.print(temperature, 1);
  lcd.print("C");
}

void testHeadlights() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FAR TESTI");
  
  // Diğer sistemleri devre dışı bırak
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(DOOR_LED_R, LOW);
  digitalWrite(DOOR_LED_G, LOW);
  digitalWrite(DOOR_LED_B, LOW);
  digitalWrite(MAIN_MOTOR_LED, LOW);
  digitalWrite(FAN_MOTOR_LED, LOW);
  
  readSensors();
  checkLight();
  
  lcd.setCursor(0, 1);
  if (lightLevel <= 250) {
    digitalWrite(BLUE_LED, HIGH);
    lcd.print("FARLAR: ACIK");
  } else {
    digitalWrite(BLUE_LED, LOW);
    lcd.print("FARLAR: KAPALI");
  }
}

void testFuel() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YAKIT TESTI");
  
  // Diğer sistemleri devre dışı bırak
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(DOOR_LED_R, LOW);
  digitalWrite(DOOR_LED_G, LOW);
  digitalWrite(DOOR_LED_B, LOW);
  digitalWrite(MAIN_MOTOR_LED, LOW);
  digitalWrite(FAN_MOTOR_LED, LOW);
  
  readSensors();
  
  // Yakıt seviyesi kontrolü
  if (fuelLevel == 0) {
    // Yakıt bitti
    motorStatus = false;
    digitalWrite(MAIN_MOTOR_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(DOOR_LED_R, LOW);
    digitalWrite(DOOR_LED_G, LOW);
    digitalWrite(DOOR_LED_B, LOW);
    lcd.setCursor(0, 1);
    lcd.print("YAKIT BITTI");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOTOR DURDU");
  } else if (fuelLevel < 5) {
    // Kritik seviye
    static unsigned long lastBlinkTime = 0;
    static bool yellowLedState = false;
    
    if (millis() - lastBlinkTime > 500) {
      yellowLedState = !yellowLedState;
      digitalWrite(YELLOW_LED, yellowLedState);
      lastBlinkTime = millis();
    }
    
    lcd.setCursor(0, 1);
    lcd.print("KRITIK: YAKIT AZ");
    lcd.setCursor(0, 0);
    lcd.print("SEVIYE: %");
    lcd.print(fuelLevel);
  } else if (fuelLevel < 10) {
    // Düşük seviye
    digitalWrite(YELLOW_LED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("UYARI: YAKIT DUSUK");
    lcd.setCursor(0, 0);
    lcd.print("SEVIYE: %");
    lcd.print(fuelLevel);
  } else {
    // Normal seviye
    digitalWrite(YELLOW_LED, LOW);
    lcd.setCursor(0, 1);
    lcd.print("YAKIT NORMAL");
    lcd.setCursor(0, 0);
    lcd.print("SEVIYE: %");
    lcd.print(fuelLevel);
  }
}

void testDoor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KAPI TESTI");
  
  // Diğer sistemleri devre dışı bırak
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(FAN_MOTOR_LED, LOW);
  
  // Kapı durumu kontrolü
  doorStatus = digitalRead(DOOR_SWITCH);
  
  if (doorStatus) {
    // Kapı açık
    digitalWrite(DOOR_LED_R, HIGH);
    digitalWrite(DOOR_LED_G, LOW);
    digitalWrite(DOOR_LED_B, HIGH);
    
    motorStatus = false;
    digitalWrite(MAIN_MOTOR_LED, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("UYARI: KAPI ACIK");
    lcd.setCursor(0, 0);
    lcd.print("MOTOR CALISMAZ");
  } else {
    // Kapı kapalı
    digitalWrite(DOOR_LED_R, LOW);
    digitalWrite(DOOR_LED_G, LOW);
    digitalWrite(DOOR_LED_B, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("KAPI: KAPALI");
    lcd.setCursor(0, 0);
    lcd.print("MOTOR HAZIR");
    
    // Motor butonu kontrolü
    static bool lastMotorButtonState = HIGH;
    bool currentMotorButtonState = digitalRead(MOTOR_BUTTON);
    
    if (currentMotorButtonState == LOW && lastMotorButtonState == HIGH) {
      motorStatus = !motorStatus;
      if (motorStatus) {
        digitalWrite(MAIN_MOTOR_LED, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("MOTOR: CALISIYOR");
      } else {
        digitalWrite(MAIN_MOTOR_LED, LOW);
        lcd.setCursor(0, 0);
        lcd.print("MOTOR: KAPALI");
      }
      delay(50); // Debounce için kısa bekleme
    }
    lastMotorButtonState = currentMotorButtonState;
  }
}

void readSensors() {
  // Sıcaklık okuma
  int tempValue = analogRead(TEMP_SENSOR);
  temperature = map(tempValue, 0, 1023, 0, 50);
  
  // Işık seviyesi okuma
  lightLevel = analogRead(LDR);
  
  // Yakıt seviyesi okuma
  int fuelValue = analogRead(FUEL_SENSOR);
  fuelLevel = map(fuelValue, 0, 1023, 0, 100);
}

void checkMotor() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(MOTOR_BUTTON);
  
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    if (!doorStatus && fuelLevel > 0 && seatbeltStatus) {
      motorStatus = !motorStatus;
      if (motorStatus) {
        digitalWrite(MAIN_MOTOR_LED, HIGH);
      } else {
        digitalWrite(MAIN_MOTOR_LED, LOW);
      }
    }
  }
  lastButtonState = currentButtonState;
}

void checkSeatbelt() {
  static bool lastSeatbeltButtonState = HIGH;
  bool currentSeatbeltButtonState = digitalRead(SEATBELT_BUTTON);
  
  // Buton basıldığında
  if (currentSeatbeltButtonState == LOW && lastSeatbeltButtonState == HIGH) {
    delay(20); // Debounce için kısa bekleme
    currentSeatbeltButtonState = digitalRead(SEATBELT_BUTTON);
    
    if (currentSeatbeltButtonState == LOW) { // Hala basılıysa
      seatbeltStatus = !seatbeltStatus; // Kemer durumunu değiştir
      
      if (seatbeltStatus) {
        // Kemer takıldığında
        noTone(BUZZER); // Buzzer'ı kapat
        digitalWrite(BUZZER, LOW);
        digitalWrite(RED_LED, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("KEMER TESTI");
      } else {
        // Kemer çıkarıldığında
        motorStatus = false;
        digitalWrite(MAIN_MOTOR_LED, LOW);
        tone(BUZZER, 1000); // Buzzer'ı çalıştır
        digitalWrite(RED_LED, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("KEMER TAKILI DEGIL!");
      }
    }
  }
  
  lastSeatbeltButtonState = currentSeatbeltButtonState;
}

void checkTemperature() {
  static float lastTemperature = 0;
  static bool climateStatus = false;
  
  if (temperature != lastTemperature || climateStatus != (temperature > 25)) {
    if (temperature > 25) {
      digitalWrite(FAN_MOTOR_LED, HIGH);
      climateStatus = true;
    } else {
      digitalWrite(FAN_MOTOR_LED, LOW);
      climateStatus = false;
    }
    lastTemperature = temperature;
  }
}

void checkLight() {
  static int lastLightLevel = 0;
  static bool headlightStatus = false;
  
  if (lightLevel != lastLightLevel || headlightStatus != (lightLevel <= 250)) {
    if (lightLevel <= 250) {
      digitalWrite(BLUE_LED, HIGH);
      headlightStatus = true;
    } else {
      digitalWrite(BLUE_LED, LOW);
      headlightStatus = false;
    }
    lastLightLevel = lightLevel;
  }
} 