#ifndef ITG3200_H
#define ITG3200_H
#include <stdint.h>
#include "declarations.h"
#include "vector.h"

namespace GYR_Registers{
	const uint8_t ADDRESS =		0x68;
	const uint8_t SMPLRT_DIV =	0x15;
	const uint8_t DLPF_FS =		0x16;
	const uint8_t INT_CFG =		0x17;
	const uint8_t INT_STATUS =	0x1A;
	const uint8_t DATA =		0x1B;
	const uint8_t PWR_MGM =		0x3E;

	const int8_t TEMP_OFFSET =	13;		//LSB
	const float TEMP_SCALE = 	280.0f;		//LSb/ºC
	//823.627 LSb/(rad/s) = 14.375 LSb/(º/s) * 180º/PI rad
	const float GYRO_SCALE = 	823.627f;	//LSb/(rad/s)
};

class Gyroscope{
	public:
		Gyroscope();

		bool refreshData(Vector<float> &vector, float &temperature);

		uint8_t deltaT;
		uint8_t deltaT_old;

	private:
		bool dataReady();
};

#endif
