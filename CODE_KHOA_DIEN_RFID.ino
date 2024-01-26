#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 2 // led 1 gắn chân 4
#define BUZZER 3 // coi găn chân 3
#define KHOA 4 // khoa dien
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

//..... Thu vien LCD I2C.....//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(KHOA, OUTPUT);
  noTone(BUZZER);
  digitalWrite(KHOA, LOW);
  Serial.println("Dua the vao...");
  Serial.println();


  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("KHOA DIEN");
  lcd.setCursor(3, 1);
  lcd.print("RFID RC522");
  delay(2000);
  lcd.clear();

}
void loop()
{

  lcd.setCursor(2, 0);
  lcd.print("MOI QUET THE ");
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "E9 E8 9C 8D" || content.substring(1) == "E9 E7 9C 8D"  ) // dán ID của thẻ vừa đọc được từ trước
  {
    Serial.println("DUNG THE");
    Serial.println();
    lcd.setCursor(0, 1);
    lcd.print("***  MO CUA  ***");
    right();
    digitalWrite(LED_G, HIGH);
    digitalWrite(KHOA, HIGH);
    delay(5000);                   // Thời gian mở cửa
    digitalWrite(KHOA, LOW);
    digitalWrite(LED_G, LOW);
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  else
  {
    Serial.println("SAI THE");
    Serial.println();
    lcd.setCursor(0, 1);
    lcd.print("***  SAI THE ***");
    wrong();
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
}
void right()
{
  
  tone(BUZZER, 700);
  delay(500);
  noTone(BUZZER);
  
}
void wrong()
{
  tone(BUZZER, 300);
  delay(100);
  noTone(BUZZER);
  delay(50);
  tone(BUZZER, 300);
  delay(350);
  noTone(BUZZER);
  delay(1000);
}
