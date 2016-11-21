#define REMOTEXY_MODE__HC05_HARDSERIAL

#include <RemoteXY.h>

/* RemoteXY connection settings */
#define REMOTEXY_SERIAL Serial1
#define REMOTEXY_SERIAL_SPEED 9600


/* RemoteXY configurate  */
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
{ 2, 0, 21, 0, 6, 5, 0, 1, 0, 0
  , 0, 5, 5, 2, 88, 0, 1, 0, 0, 5
  , 5, 5, 2, 88, 0
};

/* this structure defines all the variables of your control interface */
struct {

  /* input variable */
  uint8_t button_1; /* =1 if button pressed, else =0 */
  uint8_t button_2; /* =1 if button pressed, else =0 */

  /* other variable */
  uint8_t connect_flag;  /* =1 if wire connected, else =0 */

} RemoteXY;

#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_BUTTON_1 13


void setup()
{
  RemoteXY_Init ();

  pinMode (PIN_BUTTON_1, OUTPUT);

  // TODO you setup code

}

void loop()
{
  RemoteXY_Handler ();

  digitalWrite(PIN_BUTTON_1, (RemoteXY.button_1 == 0) ? LOW : HIGH);

  // TODO you loop code
  // use the RemoteXY structure for data transfer
}
