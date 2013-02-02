/*
  TCS3471.h - TCS3471 Color light-to-digital converter IC by TAOS/AMS library demo code
  Copyright (c) 2012, 2013 Raivis Rengelis (raivis [at] rrkb.lv). All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include <TCS3471.h>

// Since library stays away from hardware, user sketch has to take care about
// actual communications. Please note that functions need to be exactly like this,
// return void and accept 3 parameters, 1st - i2c address of slave chip,
// 2nd number of bytes to be written or read and 3rd - pointer to a byte array
// from where to read or where to write respective bytes
// Since TCS3471 chips have 12c fixed address, this can come in handy if you want to
// put multiple chips behind multiplexer or do something like that
void i2cWrite(byte address, byte count, byte* buffer);
void i2cRead(byte address, byte count, byte* buffer);

// This is where TCS3471 object gets created
// See how functions declared get passed to constructor as parameters
TCS3471 TCS3471(i2cWrite, i2cRead);

// Just a variable to tell main loop that interrupt was triggered
volatile bool colorAvailable = false;

// counter to check when to finish printing
int printXtimes = 0;

void setup()
{
    Serial.begin(9600);
    // starting i2c interface in master mode
    Wire.begin();
    // TCS3471 can generate interrupt when color/light detection cycle finishes and/or is over or under set threshold
    // To free resources of Arduino connect INT pin of TCS3471 to pin 2 of Arduino and use interrupts!
    // NB! Open drain logic in use, which means high voltage = 0, low voltage = 1, thus FALLING edge trigger
    attachInterrupt(0,TCS3471Int,FALLING);
    // No need to pound the chip if there is no chip, detect() function checks if chip responds and returns true
    // if it is and false if it does not
    if(!TCS3471.detect())
        {
            Serial.println("Something is not right. Check your wiring, sensor needs at least VCC, GND, SCL and SDA.");
            while (1);
        }
    // tell TCS3471 to generate interrupts on INT pin
    TCS3471.enableInterrupt();
    // integration time is time chip takes to measure RGBC values, the longer, the higher precision
    // range is from 2.4ms up to 614.4ms, parameter is float and in milliseconds,
    // step is 2.4ms, but you can put in any number you like, library will take care of rounding
    TCS3471.setIntegrationTime(700.0);
    // set wait time between measurements, chip just sits there and waits for approximately that many
    // milliseconds and conserves power
    // range is from 2.4ms up to 7400ms, from 2.4ms up to 614.4ms step is 2.4ms, from 614.4ms up step is  28.8ms
    // library takes care of rounding in this case too
    // if set to anything less than 2.4ms, wait time is disabled
    TCS3471.setWaitTime(2000.0);
    // chip has 4 different analog gain settings - TCS3471_GAIN_1X, TCS3471_GAIN_4X, TCS3471_GAIN_16X and TCS3471_GAIN_60X
    // naked chip under regular ambient lighting works ok with 1x gain
    TCS3471.setGain(TCS3471_GAIN_1X);
    // if C(lear) channel goes above this value, interrupt will be generated
    // range is from 0-65535, 16 full bits
    // just for demonstration purposes, setting it right above half of full range
    TCS3471.interruptHighThreshold(32768);
    // similar as above, only C channel has to go below this value
    // again, range is from 0-65535
    // setting it right below half of full range
    // this will ensure that interrupt is generated all the time
    TCS3471.interruptLowThreshold(32767);
    // interrupt persistence determines how many times in row C channel has to go above high threshold
    // or below low threshold for interrupt to be generated
    // range is from 1 to 60, from 1-3 step is 1, from 5-60 step is 5
    // library takes care of finding closest valid value
    TCS3471.interruptPersistence(2);
    // once interrupt flag is set in chip, it must be cleared from host
    // in case it was left on from previous runs, we clear it here
    // otherwise we are not going to get new interrupts
    TCS3471.clearInterrupt();
    // and finally, when all the parameters are set, let's start measuring color and light
    TCS3471.enable();
}

void loop()
{
    if (colorAvailable)
    {
    	// check if valid RGBC data is available
    	// in this scenario this call is redundant
    	// but for demonstration purposes, let's have it here
    	if (TCS3471.rgbcValid())
    	{
    		// as mentioned previously, interrupt flag has to be cleared from host
    		TCS3471.clearInterrupt();
    		// reset Arduino flag too
    		colorAvailable = false;
    		// read C(lear) channel data
    		// range from 0-65535
        	word clearVal = TCS3471.readCData();
        	// read R(ed) channel data
        	// range 0-65535
        	word redVal   = TCS3471.readRData();
        	// read G(reen) channel data
        	// range 0-65535
        	word greenVal = TCS3471.readGData();
        	// read B(lue) channel data
        	// range 0-65535
        	word blueVal  = TCS3471.readBData();
        	// and print it all out
        	Serial.print("Light is ");
        	Serial.print(clearVal,HEX);
        	Serial.print(" overall and red is ");
        	Serial.print(redVal, HEX);
        	Serial.print(" while green is ");
        	Serial.print(greenVal, HEX);
        	Serial.print(" and blue is ");
        	Serial.println(blueVal, HEX);
        	printXtimes++;
        }
    }
    if (printXtimes > 9)
    {
    	// disable interrupt on Arduino
    	detachInterrupt(0);
    	// disable interrupt generation on TCS3471
    	TCS3471.disableInterrupt();
    	// and finally disable the chip itself, saves power!
    	TCS3471.disable();
    }
}

// this is function that gets called on interrupt from TCS3471
void TCS3471Int()
{
	colorAvailable = true;
}

// implementation of i2cWrite and i2cRead functions for simplest case when there is only one
// TCS2471 chip attached to Arduino's two wire bus
void i2cWrite(byte address, byte count, byte* buffer)
{
    Wire.beginTransmission(address);
    byte i;
    for (i = 0; i < count; i++)
    {
#if ARDUINO >= 100
        Wire.write(buffer[i]);
#else
        Wire.send(buffer[i]);
#endif
    }
    Wire.endTransmission();
}

void i2cRead(byte address, byte count, byte* buffer)
{
    Wire.requestFrom(address, count);
    byte i;
    for (i = 0; i < count; i++)
    {
#if ARDUINO >= 100
        buffer[i] = Wire.read();
#else
        buffer[i] = Wire.receive();
#endif
    }
}
