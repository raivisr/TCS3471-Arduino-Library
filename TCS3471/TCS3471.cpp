/*
  TCS3471.cpp - TCS3471 Color light-to-digital converter IC by TAOS/AMS library
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

#include "TCS3471.h"

TCS3471::TCS3471(void (*i2cWriteFunc)(byte,byte,byte*),void (*i2cReadFunc)(byte,byte,byte*))
{
    _i2cWrite = i2cWriteFunc;
    _i2cRead = i2cReadFunc;
    _detected = false;
    _i2cAddress = 0;
}

bool TCS3471::detect()
{
    if (_detected)
        return true;
    else
    {
        _i2cAddress = TCS3471_ADDRESS_1;
        byte tmpaddr = read8(TCS3471_ID_REG);
        if (tmpaddr == 0x14 || tmpaddr == 0x1D)
            {
                _detected = true;
            }
        else
            {
                _i2cAddress = TCS3471_ADDRESS_2;
                tmpaddr = read8(TCS3471_ID_REG);
                if (tmpaddr == 0x14 || tmpaddr == 0x1D)
                    {
                        _detected = true;
                    }
                else
                    {
                        _i2cAddress = 0;
                    }
            }
    }
    return _detected;
}

bool TCS3471::enable()
{
    detect();
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_PON_BIT);
        delay(3);
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_AEN_BIT);
        return true;
    }
    else
        return false;
}

void TCS3471::disable()
{
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) & ~(TCS3471_PON_BIT | TCS3471_AEN_BIT));
    }
}

void TCS3471::setIntegrationTime(float integrationTime)
{
    if (_detected)
    {
        word aTime = (word)(integrationTime * 10);
        aTime = aTime / 24;
        if (aTime > 256)
            aTime = 256;
        aTime = aTime - 256;
        write8(TCS3471_ATIME_REG, (byte)aTime);
    }
}

void TCS3471::setWaitTime(float waitTime)
{
    if (_detected)
    {
        int32_t wTime = (int32_t)(waitTime * 10);
        if (wTime < 24)
        {
            write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) & ~TCS3471_WEN_BIT);
            return;
        }
        else if (wTime > 6144)
        {
            write8(TCS3471_CONFIG_REG, TCS3471_WLONG_BIT);
            if (wTime > 73728)
                wTime = 73728;
            wTime = wTime / 288;
        }
        else
        {
            write8(TCS3471_CONFIG_REG, 0x00);
            wTime = wTime / 24;
        }
        wTime = 256 - wTime;
        write8(TCS3471_WTIME_REG, (byte)wTime);
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_WEN_BIT);
    }
}

void TCS3471::setGain(tcs3471Gain_t gain)
{
    if (_detected)
    {
        write8(TCS3471_CONTROL_REG, gain);
    }
}

void TCS3471::enableInterrupt()
{
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) | TCS3471_AIEN_BIT);
    }
}

void TCS3471::disableInterrupt()
{
    if (_detected)
    {
        write8(TCS3471_ENABLE_REG, read8(TCS3471_ENABLE_REG) & ~TCS3471_AIEN_BIT);
    }
}

void TCS3471::clearInterrupt()
{
    if (_detected)
    {
        _i2cBuffer[0] = TCS3471_COMMAND_BIT | TCS3471_SPECIAL_BIT | TCS3471_INTCLEAR_BIT;
        _i2cWrite(_i2cAddress,1,_i2cBuffer);
    }
}

void TCS3471::interruptHighThreshold(word highThreshold)
{
    if (_detected)
    {
        write16(TCS3471_AIHTL_REG,highThreshold);
    }
}

void TCS3471::interruptLowThreshold(word lowThreshold)
{
    if (_detected)
    {
        write16(TCS3471_AILTL_REG,lowThreshold);
    }
}

void TCS3471::interruptPersistence(byte persistence)
{
    if (_detected)
    {
        byte valueToWrite = persistence;
        if (valueToWrite > 60)
            valueToWrite = 60;
        if (valueToWrite > 3)
            valueToWrite = valueToWrite / 5 + 3;
        write8(TCS3471_PERS_REG,valueToWrite & 0x0F);
    }
}

byte TCS3471::getChipID()
{
    detect();
    if (_detected)
    {
        return read8(TCS3471_ID_REG);
    }
    else
        return 0;
}

bool TCS3471::rgbcValid()
{
    if (_detected)
    {
        return (read8(TCS3471_STATUS_REG) & TCS3471_AVALID_BIT) == TCS3471_AVALID_BIT;
    }
    else
        return false;
}

word TCS3471::readCData()
{
    if (_detected)
    {
        return read16(TCS3471_CDATA_REG);
    }
    return 0;
}

word TCS3471::readRData()
{
    if (_detected)
    {
        return read16(TCS3471_RDATA_REG);
    }
    return 0;
}

word TCS3471::readGData()
{
    if (_detected)
    {
        return read16(TCS3471_GDATA_REG);
    }
    return 0;
}

word TCS3471::readBData()
{
    if (_detected)
    {
        return read16(TCS3471_BDATA_REG);
    }
    return 0;
}

void TCS3471::write8(byte reg, byte val)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | reg;
    _i2cBuffer[1] = val;
    _i2cWrite(_i2cAddress,2,_i2cBuffer);
}

void TCS3471::write16(byte reg, word val)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | TCS3471_AUTOINCR_BIT | reg;
    _i2cBuffer[1] = val & 0xFF;
    _i2cBuffer[2] = (val >> 8) & 0xFF;
    _i2cWrite(_i2cAddress,3,_i2cBuffer);
}

byte TCS3471::read8(byte reg)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | reg;
    _i2cWrite(_i2cAddress, 1, _i2cBuffer);
    _i2cRead(_i2cAddress, 1, _i2cBuffer);
    return _i2cBuffer[0];
}

word TCS3471::read16(byte reg)
{
    _i2cBuffer[0] = TCS3471_COMMAND_BIT | TCS3471_AUTOINCR_BIT | reg;
    _i2cWrite(_i2cAddress, 1, _i2cBuffer);
    _i2cRead(_i2cAddress, 2, _i2cBuffer);
    word ret = _i2cBuffer[1] << 8;
	ret |= _i2cBuffer[0];
    return ret;
}
