
#include <Wire.h>

enum IOcommand {
  I2_WHO_AM_I //Отдали UID
  , I2_RESET_ME // сброс
  , I2_CHANGE_I2C_ADDR // сменить I2C-адрес вручную
  , I2_SAVE_I2C_ADDR // Сохранить текущий адрес во флэш, чтобы стартовать при последующих включениях с него
  , I2_PIN_MODE_INPUT // настроили пин на вход
  , I2_PIN_MODE_OUTPUT // .. на выход
  , I2_PIN_MODE_PULLUP // .. вход с поддтяжкой вверх (подумать PullDown, OpenCollector - надо/нет? Скорее нет)
  , I2_DIGITAL_READ    // считали состояние пина
  , I2_DIGITAL_WRITE_HIGH // Выставили пин в высокий уровень
  , I2_DIGITAL_WRITE_LOW // .. в низкий уровень
  , I2_PWM_FREQ // установка частоты (общая для всех PWM-пинов)
  , I2_ANALOG_WRITE // Запустить ШИМ
  , I2_ANALOG_READ // Считать значениие с АЦП
};



byte addr = 42;
uint8_t pwm = 0;
byte cmd[3];

uint16_t val=0;

void pinToCmd(uint8_t pin) {
  uint16_t preCmd  = (uint16_t)1 << pin;
  cmd[2] = preCmd & (byte)0xff;
  cmd[1] = (preCmd >> 8) & (byte)0xff;
}

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  // while (!Serial);             // Leonardo: wait for serial monitor
  /*
    cmd[0] = I2_PIN_MODE_OUTPUT;
    pinToCmd(9);
    Wire.beginTransmission(addr);
    Wire.write(cmd, 3);
    Wire.endTransmission();
  */
}


void loop()
{
  /*  for (int y = 0; y < 9; y++) {
      cmd[1] = y;
        Serial.println(y);

      for (int i = 0; i < 255; ++i) {
        cmd[2] = i;
  */
  for (pwm = 0; pwm < 255; ++pwm) {
    cmd[0] = I2_ANALOG_WRITE;
    cmd[1] = 9;
    cmd[2] = val&0xff;

    Wire.beginTransmission(addr);
    Wire.write(cmd, 3);
    Wire.endTransmission();
    delay(10);

    cmd[0] = I2_ANALOG_READ;
    cmd[1] = 0;
    cmd[2] = 4;
    Wire.beginTransmission(addr);
    Wire.write(cmd, 3);
    Wire.endTransmission();
    Wire.requestFrom(addr, 2);
    val = Wire.read();
    val <<= 8;
    val |= Wire.read();
    Serial.println(val>>7);
    val >>=7;

  }

  /*
    cmd[0] = I2_DIGITAL_WRITE_LOW;
    Wire.beginTransmission(addr);
    Wire.write(cmd, 3);
    Wire.endTransmission();
    delay(1000);           // wait 5 seconds for next scan
  */

  /*    }
    }
  */
}
