#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdint.h>
#include "vector.h"
#include "quaternion.h"

#include "accelerometer.h"
#include "magnetometer.h"

//Nota: Cargamos y guardamos la clase Vector entera, que es igual que
//cargar/guardar sus tres atributos (x,y,z) de manera independiente.

//Dirección del vector M y su norma en la EEPROM
namespace Algoritm_Const{
	const uint8_t * M_dir = (uint8_t*)0;
	const uint8_t * Mnorm_dir = M_dir + sizeof(Vector<float>);

	//Número de muestras para calcular la media en la calibración
	const uint8_t numSamples = 10;
}

//Notación para cuaterniones: {Sist. referencia}{Sist. rotado}q_{Sub índice}
class Algorithm{
	public:
		Algorithm();

		void calibration(Accelerometer &acc, const Magnetometer &mag);
		void gyroscope(const Vector<float> &gyr, uint8_t deltaT);
		void magnetometer(const Vector<int> &mag);
		void oarOrientationCorrection();

		Vector<float> M;
		Quaternion ESq;
		Quaternion SEq_G;
		Quaternion ESq_M;
		Quaternion SScq_C;
};

#endif