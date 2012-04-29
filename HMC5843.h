#ifndef HMC5843_H
#define HMC5843_H

#include "declarations.h"

//Constants
namespace MGT_Registers {
	//I2C Address
	const uint8_t ADDRESS = 0x1E;
	
	// HMC58X3 register map
	//Configuration registers
	const uint8_t REG_CONFA = 0;
	const uint8_t REG_CONFB = 1;
	const uint8_t REG_MODE = 2;
	
	//Output registers
	const uint8_t REG_XH = 3;
	const uint8_t REG_XL = 4;
	const uint8_t REG_YH = 5;
	const uint8_t REG_YL = 6;
	const uint8_t REG_ZH = 7;
	const uint8_t REG_ZL = 8;
	
	//Status registers
	const uint8_t REG_STATUS = 9;
	const uint8_t REG_IDA = 10;
	const uint8_t REG_IDB = 11;
	const uint8_t REG_IDC = 12;
}

class Magnetometer{
	public:
		//Configures the device
		Magnetometer();

		//Sets the raw and scaled data in the struct passed
		void getData(s_sensor_data* sen_data);
		//True if the new measurement are ready to be read
		bool dataReady();
	
	private:
		//Calibrate the device
		void calibrate();

		//Scales from calibrate
		float xScale, yScale, zScale;
};

#endif