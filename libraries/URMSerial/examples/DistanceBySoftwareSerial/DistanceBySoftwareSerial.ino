/*
   DistanceBySoftwareSerial.pde - URM 37 Control Library Version 2.5.0
Author: Miles Burton, www.milesburton.com
Copyright (c) 2009 Miles Burton All Rights Reserved

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include <SoftwareSerial.h>
#include <URMSerial.h>

URMSerial urm;

int value; // This value will be populated
int getMeasurement()
{
    // Request a distance reading from the URM37
    switch(urm.requestMeasurementOrTimeout(DISTANCE, value)) // Find out the type of request
    {
        case DISTANCE: // Double check the reading we recieve is of DISTANCE type
            //    Serial.println(value); // Fetch the distance in centimeters from the URM37
            return value;
            break;
        case TEMPERATURE:
            return value;
            break;
        case ERROR:
            Serial.println("Error");
            break;
        case NOTREADY:
            Serial.println("Not Ready");
            break;
        case TIMEOUT:
            Serial.println("Timeout");
            break;
    } 

    return -1;
}

void setup() {
    Serial.begin(9600);                  // Sets the baud rate to 9600
    urm.begin(7,6,9600);                 // RX Pin, TX Pin, Baud Rate
    Serial.println("URM37 Library by Miles Burton - Distance. Version 2.0");   // Shameless plug 
}

void loop()
{
    Serial.println("Measurement: ");
    Serial.println(getMeasurement());  // Output measurement
    delay(500);
}
