#ifndef __MACHINE_CONTROL_H__
#define __MACHINE_CONTROL_H__

#include "utility/MAX31865/MAX31865.h"
#include "utility/THERMOCOUPLE/MAX31855.h"
#include "utility/RS485/RS485.h"
#include "utility/QEI/QEI.h"
#include "utility/ioexpander/ArduinoIOExpander.h"
#include "utility/RTC/PCF8563T.h"
#include "utility/RTC/PCF8563T.h"

#include "Arduino.h"
#include "mbed.h"

#include "USBHost.h"

namespace machinecontrol {

class RTDClass {
public:
	void selectChannel(int channel) {
		for (int i=0; i<3; i++) {
			ch_sel[i] = (i == channel ? 1 : 0);
		}
		delay(150);
	}
	void enableTC() {
		rtd_th = 0;
		digitalWrite(PI_0, LOW);
		digitalWrite(PA_6, HIGH);
	}
	void enableRTD() {
		rtd_th = 1;
		digitalWrite(PI_0, HIGH);
		digitalWrite(PA_6, LOW);

	}
	void disableCS() {
		digitalWrite(PI_0, HIGH);
		digitalWrite(PA_6, HIGH);
	}
	MAX31865Class rtd = MAX31865Class(PA_6);
	MAX31855Class tc = MAX31855Class(7);

private:
	mbed::DigitalOut ch_sel[3] = { mbed::DigitalOut(PD_6), mbed::DigitalOut(PI_4), mbed::DigitalOut(PG_10)};
	mbed::DigitalOut rtd_th = mbed::DigitalOut(PC_15);

};

extern RTDClass temp_probes;

static 	mbed::CAN   _can(PB_8, PH_13);

class COMMClass {
public:
	// to be tested: check if can be made a big pin initialization
	void init() {
		//SHUTDOWN OF RS485 LEDS
		digitalWrite(PA_0, LOW);
		digitalWrite(PI_9, LOW);
		//SHUTDOWN OF CAN LEDS
		digitalWrite(PB_8, LOW);
		digitalWrite(PH_13, LOW);
	}

	void enableCAN() {
		can_disable = 0;
	}
	void disableCAN() {
		can_disable = 1;
	}

	UART _UART4_ = arduino::UART(PA_0, PI_9, NC, NC);
	mbed::CAN& can = _can;

	RS485Class rs485 = RS485Class(_UART4_,PA_0, PI_13,PI_10);
private:
	mbed::DigitalOut can_disable = mbed::DigitalOut(PA_13, 0);


};

extern COMMClass comm_protocols;

#define ch0_in1		ch_in[0]
#define ch0_in2		ch_in[1]
#define ch0_in3		ch_in[2]
#define ch0_in4		ch_in[3]
#define ch1_in1		ch_in[4]
#define ch1_in2		ch_in[5]
#define ch1_in3		ch_in[6]
#define ch1_in4		ch_in[7]
#define ch2_in1		ch_in[8]
#define ch2_in2		ch_in[9]
#define ch2_in3		ch_in[10]
#define ch2_in4		ch_in[11]

class AnalogInClass {
public:
	uint16_t read(int channel) {
        uint16_t value = 0;
        switch (channel) {
            case 0:
                value = in_0.read_u16();
                break;
            case 1:
                value =  in_1.read_u16();
                break;
            case 2:
                value =  in_2.read_u16();
                break;
            default:
                break;
            }
        delay(20);
        return value;
    }

	void set0_10V() {
		ch0_in1 = 1;
		ch0_in2 = 1;
		ch0_in3 = 0;
		ch0_in4 = 1;

		ch1_in1 = 1;
		ch1_in2 = 1;
		ch1_in3 = 0;
		ch1_in4 = 1;

		ch2_in1 = 1;
		ch2_in2 = 1;
		ch2_in3 = 0;
		ch2_in4 = 1;
	}

	void set4_20mA() {
		ch0_in1 = 1;
		ch0_in2 = 0;
		ch0_in3 = 1;
		ch0_in4 = 0;

		ch1_in1 = 1;
		ch1_in2 = 0;
		ch1_in3 = 1;
		ch1_in4 = 0;

		ch2_in1 = 1;
		ch2_in2 = 0;
		ch2_in3 = 1;
		ch2_in4 = 0;
	}

	void setNTC() {
		ch0_in1 = 0;
		ch0_in2 = 0;
		ch0_in3 = 1;
		ch0_in4 = 1;

		ch1_in1 = 0;
		ch1_in2 = 0;
		ch1_in3 = 1;
		ch1_in4 = 1;

		ch2_in1 = 0;
		ch2_in2 = 0;
		ch2_in3 = 1;
		ch2_in4 = 1;
	}

	mbed::AnalogIn& operator[](int index) {
		switch (index) {
			case 0:
				return in_0;
			case 1:
				return in_1;
			case 2:
				return in_2;
		}
	}

	mbed::DigitalOut ch_in[12] = { 
		mbed::DigitalOut(PD_4), mbed::DigitalOut(PD_5), mbed::DigitalOut(PE_3), mbed::DigitalOut(PG_3),
		mbed::DigitalOut(PD_7), mbed::DigitalOut(PH_6), mbed::DigitalOut(PJ_7), mbed::DigitalOut(PH_15),
		mbed::DigitalOut(PH_10), mbed::DigitalOut(PA_4), mbed::DigitalOut(PA_8), mbed::DigitalOut(PC_6) 
	};

private:
	mbed::AnalogIn in_0 = mbed::AnalogIn(PC_3C);
	mbed::AnalogIn in_1 = mbed::AnalogIn(PC_2C);
	mbed::AnalogIn in_2 = mbed::AnalogIn(PA_1C);
};

extern AnalogInClass analog_in;

class AnalogOutClass {
public:
	void write(int index, float voltage) {
		if (voltage < 0) {
			voltage = 0;
		}

		switch (index) {
			case 0:
				out_0.write(voltage / 10.5);
				break;
			case 1:
				out_1.write(voltage / 10.5);
				break;
			case 2:
				out_2.write(voltage / 10.5);
				break;
			case 3:
				out_3.write(voltage / 10.5);
				break;
		}
	}
	void period_ms(int index, uint8_t period) {
		switch (index) {
			case 0:
				out_0.period_ms(period);
				break;
			case 1:
				out_1.period_ms(period);
				break;
			case 2:
				out_2.period_ms(period);
				break;
			case 3:
				out_3.period_ms(period);
				break;
		}
	}
	mbed::PwmOut& operator[](int index) {
		switch (index) {
			case 0:
				return out_0;
			case 1:
				return out_1;
			case 2:
				return out_2;
			case 3:
				return out_3;
		}
	}
private:
	mbed::PwmOut out_0 = mbed::PwmOut(PJ_11);
	mbed::PwmOut out_1 = mbed::PwmOut(PK_1);
	mbed::PwmOut out_2 = mbed::PwmOut(PG_7);
	mbed::PwmOut out_3 = mbed::PwmOut(PC_7);
};

extern AnalogOutClass analog_out;


/* 
  TODO: writeme 
  Use QEI library for mbed since it implements index pin
*/
class EncoderClass {
public:
	EncoderClass()
		: enc_0{PJ_8, PH_12, PH_11, 0}
		, enc_1{PC_13, PI_7, PJ_10, 0} {};
	QEI& operator[](int index) {
		switch (index) {
			case 0:
				return enc_0;
			case 1:
				return enc_1;
		}
	}
private:
	QEI enc_0;
	QEI enc_1;
};

extern EncoderClass encoders;

/* 
  using gpio expander class https://www.i2cdevlib.com/devices/tca6424a#source
  Ask Giampaolo for proper porting
  Expander interrupt is PI_5
  prog_latch_retry (AKA TERM ? ) is PH_14

  TODO: check if Wire and address are correct
*/

class ProgrammableDIOClass : public ArduinoIOExpanderClass {
public:
	bool  init() {
		return begin(IO_ADD);
	}
	void setLatch() {
		prog_latch_retry = 0;
	}
	void setRetry() {
		prog_latch_retry = 1;
	}
private:
	mbed::DigitalOut prog_latch_retry = mbed::DigitalOut(PH_14);
};

extern ProgrammableDIOClass digital_programmables;


class DigitalOutputsClass {
public:
	void setAll(uint8_t val) {
		for (int i = 0; i < 8; i++) {
			out[i] = val & 0x1;
			val = val >> 1;
		}
	}
	void set(int index, bool val) {
			out[index] = val;
	}
	void setLatch() {
		dig_out_latch_retry = 0;
	}
	void setRetry() {
		dig_out_latch_retry = 1;
	}
	mbed::DigitalOut& operator[](int index) {
		return out[index];
	}
private:
	mbed::DigitalOut dig_out_latch_retry = mbed::DigitalOut(PB_2);
	mbed::DigitalOut out[8] = {
		mbed::DigitalOut(PI_6), mbed::DigitalOut(PH_9), mbed::DigitalOut(PJ_9), mbed::DigitalOut(PE_2),
		mbed::DigitalOut(PI_3), mbed::DigitalOut(PI_2), mbed::DigitalOut(PD_3), mbed::DigitalOut(PA_14)
	};
};

extern DigitalOutputsClass digital_outputs;

class ProgrammableDINClass : public ArduinoIOExpanderClass {
public:
	bool init() {
		return begin(DIN_ADD);
	}
};

extern ProgrammableDINClass digital_inputs;


class RtcControllerClass : public PCF8563TClass {
public:
	mbed::DigitalIn int_pin = mbed::DigitalIn(PB_9,PullUp);
private:

};

extern RtcControllerClass rtc_controller;



class USBClass {
public:
	void init(const tusbh_class_reg_t *class_table) {
		usb.Init(USB_CORE_ID_FS, class_table);
	}

	void powerEnable() {
		power = 0;
	}

	void powerDisable() {
		power = 1;
	}

	bool vflagRead() {
		return usbflag;
	}

	USBHost usb;

private:
	mbed::DigitalOut power = mbed::DigitalOut(PB_14);
	mbed::DigitalIn usbflag = mbed::DigitalIn(PB_15);
};


extern USBClass usb_controller;


}
#endif
