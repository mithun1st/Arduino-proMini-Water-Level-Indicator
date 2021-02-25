#include<LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

//input encode (to know tslow n tsfull)
bool encode = true;

//input section- A to B height of tank
const byte tslow = 20, tsfull = 102;


//define state of tank level from 0 to 100
const byte empty = 9, low = 20, medium = 50, normal = 95;

//define motor controll
const byte mOn = 5, mOff = 97;

//how many num of beep sound & set refreshrate
const byte beep = 7, refreshRate = 3000;


int va;
bool fu = true, lo = true, motor = false, mx = true, my = true;
const byte t = 3, e = A0, m = 2, buz = 1, r = 12, g = 11, b = 10, au = A2, bl = 13;


byte batteryBody[8] = {
  0b1111,
  0b1111,
  0b1111,
  0b1111,
  0b1111,
  0b1111,
  0b1111,
  0b1111
};

byte batteryLow[8] = {
  0b1111,
  0b1111,
  0b1110,
  0b1110,
  0b1110,
  0b1110,
  0b1111,
  0b1111
};

byte batteryEmpty[8] = {
  0b1111,
  0b1111,
  0b0000,
  0b0000,
  0b0000,
  0b0000,
  0b1111,
  0b1111
};

byte batteryHead[8] = {
  0b1100,
  0b1100,
  0b0111,
  0b0111,
  0b0111,
  0b0111,
  0b1100,
  0b1100
};

byte batteryHalf[8] = {
  0b1111,
  0b1111,
  0b1000,
  0b1100,
  0b1110,
  0b1110,
  0b1111,
  0b1111
};

byte smi[8] = {
  0b00000,
  0b00000,
  0b10001,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte sad[8] = {
  0b00000,
  0b00000,
  0b10001,
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b00000
};

byte shu[8] = {
  0b00000,
  0b00000,
  0b10001,
  0b00000,
  0b00000,
  0b01110,
  0b00000,
  0b00000
};

void setup() {
  pinMode(t, OUTPUT);
  pinMode(e, INPUT);
  pinMode(buz, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(m, OUTPUT);
  pinMode(bl, OUTPUT);
  pinMode(au, INPUT);

  digitalWrite(m, LOW);

  motor = EEPROM.read(0);
  digitalWrite(bl, motor);

  //
  pinMode(A3, INPUT);
  //

  lcd.begin(16, 2);
  //Serial.begin(9600);

  lcd.createChar(8, batteryLow);
  lcd.createChar(1, batteryBody);
  lcd.createChar(2, batteryEmpty);
  lcd.createChar(3, batteryHead);
  lcd.createChar(4, batteryHalf);
  lcd.createChar(5, smi);
  lcd.createChar(6, shu);
  lcd.createChar(7, sad);


  digitalWrite(buz, 1);
  lcd.setCursor(1, 0);
  lcd.print("Mahadi Hassan");
  lcd.setCursor(4, 1);
  lcd.print("MITHUN");
  rgb(1, 0, 0);
  delay(400);
  rgb(0, 0, 1);
  delay(400);
  rgb(0, 1, 0);
  delay(400);
  rgb(0, 0, 0);
  digitalWrite(buz, 0);

  lcd.noDisplay();
  delay(500);
  lcd.display();

}

void loop() {

  //testing
  //va = map(analogRead(A3), 0, 1018, 0, 100);
  //


  va = sonic();


  if (!encode) {
    //<encode>
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(va);
    Serial.println(va);
    delay(1000);
    // </encode>

  }
  else {
    va = map(va, tsfull, tslow, 0, 100);
    if (va > 100) {
      va = 100;
    }
    else if (va < 0) {
      va = 0;
    }


    //print with lcd
    lcd.clear();

    if (va >= normal) {
      refreshRate = 5000;
      
      rgb(0, 1, 1);
      lcd.setCursor(0, 0);
      lcd.print(" Tank: Full !!!");
    }
    else if (va > medium) {
      rgb(0, 1, 0);
      lcd.setCursor(0, 0);
      lcd.print(" Tank: Normal ");
      lcd.write(5);
    }
    else if (va > low) {
      rgb(0, 0, 1);
      lcd.setCursor(0, 0);
      lcd.print(" Tank: Medium ");
      lcd.write(6);
    }
    else if (va > empty) {
      refreshRate = 2000;
      
      rgb(1, 1, 0);
      lcd.setCursor(0, 0);
      lcd.print(" Tank:  Low   ");
      lcd.write(7);
    }
    else if (va >= 0) {
      rgb(1, 0, 0);
      lcd.setCursor(0, 0);
      lcd.print(" Tank: Empty !!!");
    }


    //motor section
    if (digitalRead(au)) {
      delay(700);
      if (digitalRead(au)) {
        motor = !motor;
        EEPROM.update(0, motor);
        buzzer(200);
        lcd.setCursor(1, 2);
        if (motor) {
          lcd.print("Auto Mode: ON");
        }
        else {
          lcd.print("Auto Mode: OFF");
          digitalWrite(m, LOW);
        }
        digitalWrite(bl, motor);
        delay(3000);
      }

    }

    if (va <= mOn && motor && mx) {
      digitalWrite(m, HIGH);
      buzzer(100);
      mx = false;
      my = true;
    }
    else if (va >= mOff && motor && my) {
      digitalWrite(m, LOW);
      buzzer(100);
      my = false;
      mx = true;
    }


    //bATTERY sECTION
    lcd.setCursor(0, 1);
    lcd.write(1);

    lcd.setCursor(1, 1);
    for (int i = 0; i <= 10; i++) {
      lcd.write(2);
    }
    lcd.setCursor(0, 1);
    for (int i = 0; i <= map(va, 0, 100, 0, 10); i++) {
      lcd.write(1);
      if (va <= 1) {
        lcd.setCursor(0, 1);
        lcd.write(8);
      }
      else if (i == map(va, 0, 100, 0, 10)) {
        lcd.write(4);
      }
    }

    lcd.setCursor(11, 1);
    lcd.write(3);
    if (va < 100) {
      lcd.print(" ");
    }
    lcd.print(va);
    lcd.print("%");


    //tank full
    if (!motor) {
      if (va >= normal && fu) {
        warf(beep);
        fu = false;
      }
      if (!fu && va < normal - 3) {
        fu = true;
      }

      //tank low
      if (va <= empty && lo) {
        warl(beep);
        lo = false;
      }
      if (!lo && va > empty + 3) {
        lo = true;
      }
    }


    delay(refreshRate);


    //print with serial communication
    //Serial.print(sonic());
    //  Serial.print(sonic());
    //  Serial.print("\t");
    //  Serial.println (va);
    //  Serial.print("\t");
    //  Serial.println (motor);
  }


}



//function define

void warf(int d) {
  for (int i = 0; i < d; i++) {
    rgb(0, 1, 1);
    digitalWrite(buz, 1);
    delay(300);
    rgb(0, 0, 0);
    digitalWrite(buz, 0);
    lcd.noDisplay();
    delay(300);
    lcd.display();
  }
}

void warl(int d) {
  for (int i = 0; i < d; i++) {
    rgb(1, 0, 0);
    digitalWrite(buz, 1);
    delay(500);
    rgb(0, 0, 0);
    digitalWrite(buz, 0);
    lcd.noDisplay();
    delay(500);
    lcd.display();
  }
}

void buzzer(int i) {
  digitalWrite(buz, HIGH);
  delay(i);
  digitalWrite(buz, LOW);
}

int sonic() {
  digitalWrite(t, 0);
  delay(2);

  digitalWrite(t, 1);
  delay(10);
  digitalWrite(t, 0);

  int d = pulseIn(e, 1);
  int inc = d * 0.034 / 2;
  return inc;
}

void rgb(int aa, int bb, int cc) {
  digitalWrite(r, aa);
  digitalWrite(g, bb);
  digitalWrite(b, cc);
}
