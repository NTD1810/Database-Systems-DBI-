#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
#include <DHT.h>

//cảm biến nhiệt độ
const int DHTPIN = 2;//Chân Out của cảm biến nối chân số 2 Arduino
const int DHTTYPE = DHT11;   // Khai báo kiểu cảm biến là DHT11
DHT dht(DHTPIN, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến

int gtri = 4; int gt;
int menu = 5; int gtmenu; 
int demmenu = 0; 

// cảm biến đất
int relay=6;
int cb=A0;

void setup() 
{
  Serial.begin(9600);

  dht.begin(); //Khởi động cảm biến
  pinMode(gtri, INPUT_PULLUP);
  pinMode(menu, INPUT_PULLUP);


  lcd.init(); //Khởi động LCD                    
  lcd.backlight(); //Mở đèn

  //cảm biến nhiệt độ
  pinMode(LED_BUILTIN, OUTPUT);

  // cảm biến đất
  pinMode(cb,INPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);

  menucapnhat();
}

void loop() 
{
   
  gt = digitalRead(gtri);
  Serial.print("Lên: "); Serial.print(gt); Serial.print("    ");
  gtmenu = digitalRead(menu);
  if (gt == 0)
  {
    if (demmenu >= 1)
    { demmenu = 0;  }
    else
    { demmenu++;  }   
    menucapnhat();
    delay(200);
  }

  //cảm biến nhiệt độ
  float doam = dht.readHumidity(); //Đọc độ ẩm
  float doC = dht.readTemperature(); //Đọc nhiệt độ C
  float doF = dht.readTemperature(true); //Đọc nhiệt độ F
  if(doC>31){
        Serial.println(" quạt quay");
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else{
        digitalWrite(LED_BUILTIN, LOW);
      }
  // cảm biến đất
  int digital=analogRead(cb);
  digital = map(digital, 0, 1023, 100, 0);
  if(digital>=50){
    Serial.println(" máy bơm");
    digitalWrite(relay,LOW);
  }
  else{
    digitalWrite(relay,HIGH);
  }
  
  if (gtmenu == 0)
  {
    chonmenu(digital, doam, doC, doF);
    menucapnhat();
    delay(200);
  }
  Serial.print("Đếm Menu: "); Serial.print(demmenu); Serial.println("    ");

}
  
  void menucapnhat() 
{
  if (demmenu == 0)
  {
    lcd.clear();
    lcd.print(">MENU 1");
    lcd.setCursor(0, 1);
    lcd.print(" MENU 2");  
  }
  else if (demmenu == 1)
  {
    lcd.clear();
    lcd.print(" MENU 1");
    lcd.setCursor(0, 1);
    lcd.print(">MENU 2");    
  }
}
void chonmenu(int digital,float doam,float doC,float doF) 
{
  if (demmenu == 0){
      lcd.clear();
      // Kiểm tra cảm biến có hoạt động hay không
      if (isnan(doam) || isnan(doC) || isnan(doF)) {
         Serial.println("Không có giá trị trả về từ cảm biến DHT");
         return;
      }

      Serial.print("Độ ẩm: ");
      Serial.print(doam);
      
      lcd.setCursor(0,0);
      lcd.print("DO AM:");
      lcd.setCursor(0,1);
      lcd.print("C|F:");
      lcd.setCursor(7,0); //con trỏ vị trí số 7, hiện ô số 8
      lcd.print(doam);
      lcd.setCursor(12,0); //Con trở ở vị trí 12, hiện ô 13
      lcd.print("%");
  
      Serial.print("%  Nhiệt độ: ");
      Serial.print(doC);
      Serial.print("°C | ");
      Serial.print(doF);
      Serial.println("°F");

      lcd.setCursor(5,1);
      lcd.print(doC);
      lcd.setCursor(10,1);
      lcd.print("|");
      lcd.setCursor(11,1);
      lcd.print(doF);
      delay(5000);
  }
  else if (demmenu == 1)
  {
  lcd.clear();
  Serial.print("Humidity:");
  Serial.print(digital);
  Serial.println("%");
  
  lcd.setCursor(3, 0);
  lcd.print("Humidity:");
  lcd.print(digital);
  lcd.print("%");
  delay(5000);

  }
}
