#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h> // библиотека "RFID".
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9
#define servo_pin 6
#define rled A0
#define gled A1
#define bled A2
#define id 

#define cards 2


#define Zumer_pin 5
#define button_pin 2
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,16,2);
Servo servo;
unsigned long uidDec, uidDecTemp; // для храниения номера метки в десятичном формате
long nuc[cards];
String nac[cards];
bool ui;



 String checkCard(long uid){
   switch (uid)
   {
   case 2161025113:
     return "Sasha";
     break;
   
   case 2789610488:
     return "Maksim";
     break;

   default:
    return "";
     break;
   }
 }



void setup() {
 Serial.begin(9600);
 SPI.begin(); // инициализация SPI / Init SPI bus.
 mfrc522.PCD_Init(); // инициализация MFRC522 / Init MFRC522 card.
 lcd.init();
 lcd.backlight();
 servo.attach(servo_pin);
 servo.write(0); // устанавливаем серву в закрытое сосотояние
 pinMode(Zumer_pin,OUTPUT);
 pinMode(button_pin,INPUT_PULLUP);
 led(200,255,0);
 Serial.println("Waiting for card...");
 
    lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Waiting for card");
 lcd.setCursor(6,1);
 lcd.print("....");
}
void loop() {
  ui = false;
  if(!digitalRead(button_pin))
    openh();
 // Поиск новой метки
 if ( ! mfrc522.PICC_IsNewCardPresent()) {
 return;
 }
 // Выбор метки
 if ( ! mfrc522.PICC_ReadCardSerial()) {
 return;
 }
 uidDec = 0;
 // Выдача серийного номера метки.
 for (byte i = 0; i < mfrc522.uid.size; i++)
 {
 uidDecTemp = mfrc522.uid.uidByte[i];
 uidDec = uidDec * 256 + uidDecTemp;
 }
 Serial.println("Card UID: ");
 Serial.println(uidDec); // Выводим UID метки в консоль.


  if(checkCard(uidDec) != ""){
    Serial.println(checkCard(uidDec));
    
    lcd.clear();
 lcd.setCursor(5,0);
 lcd.print("Hello,");
 lcd.setCursor(0,1);
 lcd.print(checkCard(uidDec)); 
    openh();
  }else{
    
    lcd.clear();
 lcd.setCursor(5,0);
 lcd.print("ERROR!");
 lcd.setCursor(2,1);
 lcd.print("Unikown card");
  led(255,0,0);
 zum(Zumer_pin,1000); // Делаем звуковой сигнал, Закрытие
  led(200,255,0);
  
    lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Waiting for card");
 lcd.setCursor(6,1);
 lcd.print("....");
 }

 
}
void zum(int n,int del){
  digitalWrite(n,HIGH);
  delay(del);
  digitalWrite(n,LOW);
}

void led(int r,int g,int b){
    analogWrite(rled,r);
    analogWrite(gled,g);
    analogWrite(bled,b);
}

void openh(){
  
  
  led(0,255,0);
 zum(Zumer_pin,200); // Делаем звуковой сигнал, Открытие
 servo.write(90); // Поворациваем серву на угол 90 градусов(Отпираем какой либо механизм: задвижку, поворациваем ключ и т.д.)
 delay(3000); // пауза 3 сек и механизм запирается.
 zum(Zumer_pin,200); // Делаем звуковой сигнал, Закрытие
 
  led(200,255,0);
  
 servo.write(0); // устанавливаем серву в закрытое сосотояние
 
    lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Waiting for card");
 lcd.setCursor(6,1);
 lcd.print("....");
}
