// Моторы подключаются к клеммам M1+,M1-,M2+,M2-
// Если полюса моторов перепутаны, 
// изменим соответствующие константы CON_MOTOR с 0 на 1
#define CON_MOTOR1     0
#define CON_MOTOR2     0

// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами 
// 4 и 7 для направления, 5 и 6 для скорости
#define SPEED_1      5 
#define DIR_1        4

#define SPEED_2      6
#define DIR_2        7

// Определим направления движения
#define FORWARD   0
#define BACKWARD  1
#define LEFT      2
#define RIGHT     3

// В функции go() мы будем управлять направлением движения и скоростью

void go(char newDirection, char speed) 
{
  boolean motorDirection_1, motorDirection_2;
  
  switch ( newDirection ) {
    
    case FORWARD:
        motorDirection_1 = true;
        motorDirection_2 = true;
        break;
    case BACKWARD:
        motorDirection_1 = false;
        motorDirection_2 = false;
        break;        
    case LEFT:
        motorDirection_1 = true;
        motorDirection_2 = false;
        break;
    case RIGHT:
        motorDirection_1 = false;
        motorDirection_2 = true;
        break;    
  }
  // Если мы ошиблись с подключением - меняем направление на обратное
  motorDirection_1 = CON_MOTOR1 ^ motorDirection_1;
  motorDirection_2 = CON_MOTOR2 ^ motorDirection_2;
  
  // Поехали! Скорость может меняться в пределах от 0 до 255.
  analogWrite(SPEED_1, speed);
  analogWrite(SPEED_2, speed);
  
  digitalWrite(DIR_1, motorDirection_1);
  digitalWrite(DIR_2, motorDirection_2);  
}
  
void setup() 
{
    // Настраивает выводы платы 4,5,6,7 на вывод сигналов 
    for(int i = 4; i <= 7; i++)     
        pinMode(i, OUTPUT);  
} 

void loop() 
{ 
    // Задержка 5 секунд после включения питания 
    delay(5000); 
    
    // Медленный разгон до максимальной скорсти  
    for (int i=50; i<=250; ++i)
    {
      go(FORWARD, i);
      delay(30);
    }
    
    // Едем секунду вперёд на максимальной скорости
    go(FORWARD, 255);
    delay(1000);
    
    // быстро крутимся влево полторы секунды
    go(LEFT, 250);
    delay(1500);
    
    // медленно едем назад полторы секунды
    go(BACKWARD, 70);
    delay(1500);
    
    // медленно крутимся вправо полторы секунды
    go(RIGHT, 80);
    delay(1500);
    
    // Остановка. Скорость равна нулю
    go(FORWARD, 0);

    // Всё, приехали. Стоим до нажатия Reset или отключения питания
    while (true);
    
}
