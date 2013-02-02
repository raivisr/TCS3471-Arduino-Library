TCS3471-Arduino-Library
=======================

TCS3471 Color and light sensor IC by AMS Arduino library

Link to manufacturer site:

&nbsp;&nbsp;&nbsp;&nbsp;http://www.ams.com/ger/Products/Light-Sensors/Color-Sensor/TCS34717

Link to TAUTIC ELECTRONICS LLC TCS34717 break-out board on tindie.com:

&nbsp;&nbsp;&nbsp;&nbsp;https://www.tindie.com/shops/TAUTIC/tcs34717-color-sensor-breakout/

As always with break-outs and cool chips, please read the datasheet. Names of functions and
literals reflect terminology in datasheet.

Tested with Arduino 1.0.3 on Arduino Uno, should work on any Arduino and Arduino compatible platform
that supports Wire library. Tweaks might apply and your mileage will vary.

Connections between break-out and Arduino Uno to run provided example:

	break-out pin   Arduino Uno pin
	GND             GND
	VCC             5V
	SDA             A4
	INT             2
	SCL             A5
	OE              unconnected
	EN              unconnected

For SDA and SCL pins on other Arduino boards, please refer to:

&nbsp;&nbsp;&nbsp;&nbsp;http://arduino.cc/en/Reference/Wire

If you connect break-out board to a 3.3V Arduino, please connect VCC of break-out to 3.3V power supply.

Short function reference:

	TCS3471(void (*i2cWriteFunc)(byte,byte,byte*),void (*i2cReadFunc)(byte,byte,byte*))
&nbsp;&nbsp;&nbsp;&nbsp;constructor of TCS3471 object, please refer to example for implementation details

	bool detect()
&nbsp;&nbsp;&nbsp;&nbsp;function returns true, if TCS3471 chip was found on i2c bus, false if not

	bool enable()
&nbsp;&nbsp;&nbsp;&nbsp;function enables color and light detection on TCS3471 and returns true if chip is
&nbsp;&nbsp;&nbsp;&nbsp;present or false if it is not

	void disable()
&nbsp;&nbsp;&nbsp;&nbsp;disables color and light detection on TCS3471 and puts the chip in powersaving mode

	void setIntegrationTime(float integrationTime)
&nbsp;&nbsp;&nbsp;&nbsp;sets color and light detection cycle length (please read datasheet for details on
&nbsp;&nbsp;&nbsp;&nbsp;what impact this parameter has on detection). Parameter is in milliseconds,
&nbsp;&nbsp;&nbsp;&nbsp;meaningful parameter values are from 2.4ms to 614.4ms in 2.4ms steps.
&nbsp;&nbsp;&nbsp;&nbsp;Function will find closest allowed value to the one that is supplied.

	void setWaitTime(float waitTime)
&nbsp;&nbsp;&nbsp;&nbsp;sets wait cycle length. Parameter in milliseconds, meaningful values from 0
&nbsp;&nbsp;&nbsp;&nbsp;to 7219ms. If set to less than 2.4ms, wait cycle is disabled. From 2.4ms to
&nbsp;&nbsp;&nbsp;&nbsp;614.4ms step size is 2.4ms, from 614.4 and up, step size is 28.8ms

	void setGain(tcs3471Gain_t gain)
&nbsp;&nbsp;&nbsp;&nbsp;sets gain of color and light detector amplifier. possible gain values are
&nbsp;&nbsp;&nbsp;&nbsp;TCS3471_GAIN_1x
&nbsp;&nbsp;&nbsp;&nbsp;TCS3471_GAIN_4x
&nbsp;&nbsp;&nbsp;&nbsp;TCS3471_GAIN_16x
&nbsp;&nbsp;&nbsp;&nbsp;TCS3471_GAIN_60x

	void enableInterrupt()
&nbsp;&nbsp;&nbsp;&nbsp;enables interrupt generation on TCS3471, please read datasheet and look at example
&nbsp;&nbsp;&nbsp;&nbsp;for information on how it is used

	void disableInterrupt()
&nbsp;&nbsp;&nbsp;&nbsp;disables interrupt generation on TCS3471

	void clearInterrupt()
&nbsp;&nbsp;&nbsp;&nbsp;clears interrupt bit in TCS3471 and resets interrupt pin. Interrupt on TCS3471
&nbsp;&nbsp;&nbsp;&nbsp;has to be cleared by master device once it has been handled.
&nbsp;&nbsp;&nbsp;&nbsp;Please note, since TCS3471 has open drain outputs, high voltage means 0 and
&nbsp;&nbsp;&nbsp;&nbsp;low voltage means 1

	void interruptHighThreshold(word highThreshold)
&nbsp;&nbsp;&nbsp;&nbsp;set high threshold for interrupt generation

	void interruptLowThreshold(word lowThreshold)
&nbsp;&nbsp;&nbsp;&nbsp;set low threshold for interrupt generation

	void interruptPersistence(byte persistence)
&nbsp;&nbsp;&nbsp;&nbsp;set number of consecutive times interrupt condition must occur, before interrupt
&nbsp;&nbsp;&nbsp;&nbsp;is generated and interrupt pin asserted

	byte getChipID()
&nbsp;&nbsp;&nbsp;&nbsp;returns chip ID, please refer to datasheet for possible values

	bool rgbcValid()
&nbsp;&nbsp;&nbsp;&nbsp;returns true if RGBC data is valid and can be read, false otherwise

	word readCData()
&nbsp;&nbsp;&nbsp;&nbsp;returns last detected value of C(lear) channel

	word readRData()
&nbsp;&nbsp;&nbsp;&nbsp;returns last detected value of R(ed) channel

	word readGData()
&nbsp;&nbsp;&nbsp;&nbsp;returns last detected value of G(reen) channel

	word readBData()
&nbsp;&nbsp;&nbsp;&nbsp;returns last detected value of B(lue) channel
