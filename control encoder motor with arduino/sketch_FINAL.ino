#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int T_min = 60 ;
int T_max = 90 ;
int T_alarm = 125 ;
int fan_rate ;
int T_Pin = A0;   // the output of LM35
int T; float t1 = 0 ; float t2 =0 ; float t3=0 ;
int enA = 5;
int in1 = 7;
int in2 = 6;
void setup() {
  pinMode(5 , OUTPUT);
  pinMode(6 , OUTPUT);
  pinMode(7 , OUTPUT);
  pinMode(A0, INPUT );
  lcd.begin(16, 2);
  lcd.setCursor(5, 0); //(0 ,0 ) means top left of LCD
  lcd.print("salam");
  lcd.setCursor(1, 1);
  lcd.print("sal no mobarak");
  delay(1000);
  lcd.clear();
}
void ISRHandler() {
  t1 = millis();
  t3 = t1 - t2 ;
  t2 = t1 ;
}
int readtemperature()
{
  T = analogRead(T_Pin);
  return T  ;
}
void loop()
{
  attachInterrupt(0, ISRHandler , RISING );
  lcd.setCursor(9,1);
  lcd.print(t3);
  T = readtemperature();
  lcd.setCursor(3, 0);
  lcd.print("T=");
  lcd.setCursor(6, 0);
  // ADC =>  V ref / 1024 , so  (5/1024)=4.8828
  // and then because of sensor which is 10mv/c so we divide 4.8828 to 10 => (4.8828 / 10) = 0.48828 
  lcd.print(T * 0.48828);
  lcd.setCursor(12, 0);
  lcd.write(223);
  lcd.print("C");
  if (T * 0.48828 < 60)
  {
    fan_rate = 0 ;
    lcd.setCursor(3, 0);
    lcd.print("T=");
    lcd.setCursor(6, 0);
    lcd.print(T * 0.48828);
    lcd.setCursor(0, 1);
    lcd.print("pwm=");
    lcd.setCursor(5,1);
    lcd.print(fan_rate);

  }
  else if ((T * 0.48828 <= T_max) && (T * 0.48828 >= T_min))
  { fan_rate = map(T * 0.48828, 60, 90, 0, 255);
    lcd.setCursor(3, 0);
    lcd.print("T=");
    lcd.setCursor(6, 0);
    lcd.print(T * 0.48828);
    lcd.setCursor(0, 1);
    lcd.print("pwm=");
    lcd.setCursor(5,1);
    lcd.print(fan_rate);
  }
  else if ((T * 0.48828 > T_max ) && (T * 0.48828 < T_alarm))
  {
    fan_rate = 255 ;
    lcd.setCursor(3, 0);
    lcd.print("T=");
    lcd.setCursor(6, 0);
    lcd.print(T * 0.48828);
    lcd.setCursor(0, 1);
    lcd.print("pwm=");
    lcd.setCursor(5,1);
    lcd.print(fan_rate);
  }
  else if (T * 0.48828 > T_alarm)
  {
    lcd.setCursor(3, 0);
    lcd.print("T=");
    lcd.setCursor(6, 0);
    lcd.print(T * 0.48828);
    lcd.setCursor(0, 1);
    lcd.print("Alarm:over 125");
    lcd.setCursor(14, 1);
    lcd.write(223);
    lcd.print("C");
    fan_rate = 255 ;
  }
  digitalWrite(7 , HIGH   );
  digitalWrite(6 , LOW    );
  analogWrite (5 , fan_rate);
  delay(1000);
}
