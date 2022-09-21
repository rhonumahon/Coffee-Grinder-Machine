#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int HX711_dout = 10; //mcu > HX711 dout pin
const int HX711_sck = 11; //mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int roaster = 2;
int heater = 3;
int heater_cylinder = 4;
int cooler = 5;
int cooler_cylinder = 6;
int grinder = 7;
int weigh_up = 8;
int weigh_down = 9;
int conveyor = 12;
int sealer = 13;
int upButton = A0;
int selectButton = A1;
int downButton = A2;
int menu = 1;
bool doneSelectType = false;


void setup() {
 //Serial.begin(9600);  
  pinMode(roaster, OUTPUT);
  pinMode(heater, OUTPUT);
  pinMode(heater_cylinder, OUTPUT);
  pinMode(cooler, OUTPUT);
  pinMode(cooler_cylinder, OUTPUT);
  pinMode(grinder, OUTPUT);
  pinMode(weigh_up, OUTPUT);
  pinMode(weigh_down, OUTPUT);
  pinMode(conveyor, OUTPUT);
  pinMode(sealer, OUTPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  LoadCell.begin();
  LoadCell.start(2000);
  LoadCell.setCalFactor(2000.0);
  
  digitalWrite(heater, HIGH);
  lcd.setCursor(2,0);
  lcd.print("Initializing");
  lcd.setCursor(4,1);
  lcd.print("heater...");
  delay(3000); //delay(300000);
  digitalWrite(heater, LOW);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Heater Ready!");
  delay(1000); //delay(3000);
  lcd.clear();
  updateMenuLCD();  
}

void loop() {
    if (!digitalRead(downButton)){
       if(doneSelectType == false){
          menu++;
          updateMenuLCD();
        }
    delay(200);
    while (!digitalRead(downButton));
    }
    if (!digitalRead(upButton)){
        if(doneSelectType == false){
        menu--;
        updateMenuLCD();
        }
    delay(200);
    while (!digitalRead(upButton));
    }
    if (!digitalRead(selectButton)){
       if(doneSelectType == false){
       updateMenu();
    }
    delay(200);
    while (!digitalRead(selectButton));
    }
}

void updateMenuLCD() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Excelsa");
      lcd.setCursor(0, 1);
      lcd.print(" Robusta");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Excelsa");
      lcd.setCursor(0, 1);
      lcd.print(">Robusta");
      break;
    case 3:
      lcd.clear();
      lcd.print(">Wonderbig");
      break;
    case 4:
      menu = 3;
      break;
  }
}

void updateMenu() {
  switch (menu) {
    case 1:
      doneSelectType = true;
      menu = 1;
      runExcelsa();
      break;
    case 2:
      doneSelectType = true;
      menu = 2;
      runRobusta();
      break;
    case 3:
      doneSelectType = true;
      menu = 3;
      runWonderbig();
      break;
  }
}

void runExcelsa() {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Processing");
      lcd.setCursor(2, 1);
      lcd.print("Excelsa...");
      digitalWrite(heater, HIGH);
      digitalWrite(roaster, HIGH);
      delay(2000);//delay(1200000);
      coolingOnwards();
}

void runRobusta() {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Processing");
      lcd.setCursor(2, 1);
      lcd.print("Robusta...");
      digitalWrite(heater, HIGH);
      digitalWrite(roaster, HIGH);
      delay(2000);//delay(1740000);
      coolingOnwards();
}

void runWonderbig() {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Processing");
      lcd.setCursor(2, 1);
      lcd.print("Wonderbig...");
      digitalWrite(heater, HIGH);
      digitalWrite(roaster, HIGH);
      delay(2000);//delay(1800000);
      coolingOnwards();
}

void coolingOnwards() {
      digitalWrite(heater, LOW);
      digitalWrite(heater_cylinder, HIGH);
      delay(2000);//delay(150000);
      digitalWrite(roaster, LOW);
      digitalWrite(heater_cylinder, LOW);
      digitalWrite(cooler, HIGH);
      delay(2000);//delay(240000);
      digitalWrite(cooler, LOW);
      digitalWrite(cooler_cylinder, HIGH);
      processing();  
}

void processing(){
  lcd.clear();
    String kg = "kg";
    float i = 0;
        //float result = i/1000;
    int weight = 17; //10=1kg
    digitalWrite(weigh_up, HIGH);
      while(i < weight){
      LoadCell.update();
      i = LoadCell.getData();
  
      lcd.setCursor(0, 0);
      lcd.print("Weight[kilo]:");
      lcd.setCursor(0, 1);
      lcd.print(i + kg);
      }
    digitalWrite(weigh_up, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight[kilo]:");
    lcd.setCursor(0, 1);
    lcd.print(i + kg);
    
    digitalWrite(weigh_down, HIGH);
    delay(300);
    digitalWrite(weigh_down, LOW);
    delay(300);
    digitalWrite(weigh_down, HIGH);
    delay(2000);
    digitalWrite(weigh_down, HIGH);
    delay(300);
    digitalWrite(weigh_down, LOW);
    delay(300);
    digitalWrite(weigh_down, HIGH);
    
    delay(4000);
    digitalWrite(weigh_down, LOW);
    digitalWrite(conveyor, HIGH);
    delay(1000);
    digitalWrite(conveyor, LOW);
    digitalWrite(sealer, HIGH);
    delay(500);
    digitalWrite(sealer, LOW);
    digitalWrite(conveyor, HIGH);
    delay(7000);
    digitalWrite(conveyor, LOW);
    doneSelectType = false;
    lcd.clear();
    lcd.print("Done Process!");
    delay(3000);
    menu = 1;
    LoadCell.tare();
    updateMenuLCD();
}
