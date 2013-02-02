/*
  TCS3471.h - TCS3471 Color light-to-digital converter IC by TAOS/AMS library
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

#ifndef TCS3471_h
#define TCS3471_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// slave address
#define TCS3471_ADDRESS_1   0x29
#define TCS3471_ADDRESS_2   0x39

// registers
#define TCS3471_ENABLE_REG  0x00
#define TCS3471_ATIME_REG   0x01
#define TCS3471_WTIME_REG   0x03
#define TCS3471_AILTL_REG   0x04
#define TCS3471_AILTH_REG   0x05
#define TCS3471_AIHTL_REG   0x06
#define TCS3471_AIHTH_REG   0x07
#define TCS3471_PERS_REG    0x0C
#define TCS3471_CONFIG_REG  0x0D
#define TCS3471_CONTROL_REG 0x0F
#define TCS3471_ID_REG      0x12
#define TCS3471_STATUS_REG  0x13
#define TCS3471_CDATA_REG   0x14
#define TCS3471_CDATAH_REG  0x15
#define TCS3471_RDATA_REG   0x16
#define TCS3471_RDATAH_REG  0x17
#define TCS3471_GDATA_REG   0x18
#define TCS3471_GDATAH_REG  0x19
#define TCS3471_BDATA_REG   0x1A
#define TCS3471_BDATAH_REG  0x1B

// command register bits
#define TCS3471_COMMAND_BIT  0x80
#define TCS3471_AUTOINCR_BIT 0x20
#define TCS3471_SPECIAL_BIT  0x60
#define TCS3471_INTCLEAR_BIT 0x06

// enable register bits
#define TCS3471_AIEN_BIT     0x10
#define TCS3471_WEN_BIT      0x08
#define TCS3471_AEN_BIT      0x02
#define TCS3471_PON_BIT      0x01

// configuration register bits
#define TCS3471_WLONG_BIT    0x02

// control register bits
typedef enum
{
    TCS3471_GAIN_1X = 0x00,
    TCS3471_GAIN_4X = 0x01,
    TCS3471_GAIN_16X = 0x02,
    TCS3471_GAIN_60X = 0x03,
}
tcs3471Gain_t;

// ID register values
#define TCS3471_1_5_VALUE    0x14
#define TCS3471_3_7_VALUE    0x1D

// status register bits
#define TCS3471_AINT_BIT     0x10
#define TCS3471_AVALID_BIT   0x01

class TCS3471
{
public:
    TCS3471(void (*i2cWriteFunc)(byte,byte,byte*),void (*i2cReadFunc)(byte,byte,byte*));
    bool detect();
    bool enable();
    void disable();
    void setIntegrationTime(float integrationTime);
    void setWaitTime(float waitTime);
    void setGain(tcs3471Gain_t gain);
    void enableInterrupt();
    void disableInterrupt();
    void clearInterrupt();
    void interruptHighThreshold(word highThreshold);
    void interruptLowThreshold(word lowThreshold);
    void interruptPersistence(byte persistence);
    byte getChipID();
    bool rgbcValid();
    word readCData();
    word readRData();
    word readGData();
    word readBData();
    
    void write8(byte reg, byte val);
    void write16(byte reg, word val);
    byte read8(byte reg);
    word read16(byte reg);
private:
    byte _i2cBuffer[3];
    bool _detected;
    byte _i2cAddress;

    void (*_i2cWrite)(byte,byte,byte*);
    void (*_i2cRead)(byte,byte,byte*);
};

#endif
