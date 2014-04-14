/*
 * Strela.h
 *
 *
 */
 
#ifndef STRELA_H
#define STRELA_H

#include <inttypes.h> 
 
/* 
class Strela
{
    public:


}
*/

bool buttonRead(uint8_t buttonNum);

ledWrite(uint8_t ledNum);


class Motor
{
    public:
        Motor(
            int motorSpeed_1,
            int motorSpeed_2);
        
        connection(
            bool direction_1 = 0,
            bool direction_2 = 0);
            
    private:
        init();
           

}