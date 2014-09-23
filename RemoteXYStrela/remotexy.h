#ifndef _REMOTEXY_H_
#define _REMOTEXY_H_


/* укажите используемый Serial */
#define REMOTEXY_SERIAL Serial1


/* структура определяет все переменные вашего интерфейса управления */
typedef struct {
  
    /* input variable */
  signed char joystick_1_x; /* =-100..100 координата x положения джойстика */
  signed char joystick_1_y; /* =-100..100 координата y положения джойстика */

    /* output variable */
  unsigned char level_1; /* =0..100 положение уровня */

    /* other variable */
  unsigned char connect_flag;  /* =1 if wire connected, else =0 */

} RemoteXY_TypeDef;

/* эту переменную необходимо использовать для передачи данных */
extern RemoteXY_TypeDef RemoteXY;


void RemoteXY_Init (void);
void RemoteXY_Handler (void);

#endif //_REMOTEXY_H_
