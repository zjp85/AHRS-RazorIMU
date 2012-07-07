#include <util/delay.h>
#include <avr/interrupt.h>
#include "declarations.h"
#include <Wire.h>
#include <math.h>

#include "timer8b.h"
#include "ADXL345.h"
#include "HMC5843.h"
#include "ITG3200.h"
#include "AHRS.h"
#include "USART.h"

extern "C" void __cxa_pure_virtual(void) {
	while(1);
}

s_sensor_offsets sen_offset = {{0,0,0},{0,0,0},{0,0,0},0,0,0,0};
s_sensor_data sen_data = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
euler_angles eAngles;

//These counters allow us to sample some of the sensors at lower rates
unsigned int  Print_counter=0;

//DCM variables
float G_Dt=0.03;    // Integration time. We will run the integration loop at 50Hz if possible

int main()
{
	sei();

	Wire.begin();    //Init the I2C
	_delay_ms(20);
	USART usart(9600);
	Gyroscope gyr;
	Accelerometer acc;
	Magnetometer mgt;
	Timer8b timer;

	register uint8_t timeStamp = 0;
	register uint8_t gyrOldTime = 0;
	register uint8_t gyrDTime = 0;
	int i;

	while(1){
		timer.start(Timer8b_Const::CLK_8);

		//if((DIYmillis()-timer)>=5)  // Main loop runs at 50Hz
		//{
			//timer_old = timer;
			//timer=DIYmillis();
			//G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
			//if(G_Dt > 1)
			//	G_Dt = 0;  //keeps dt from blowing up, goes to zero to keep gyros from departing

			Print_counter++;

			//=================================================================================//
			//=======================  Data adquisition of all sensors ========================//
			if(gyr.dataReady()){
				gyr.getData(&sen_data);
				usart.send(TCNT0);
				timer.reset();
				++i;
				//timeStamp = TCNT0;
				//gyrDTime = timeStamp - gyrOldTime;
				//gyrOldTime = timeStamp;
			}

			if(acc.dataReady())
				acc.getData(&sen_data);

			if(mgt.dataReady())
				mgt.getData(&sen_data);

			//=================================================================================//
			AHRSupdate(&sen_data, G_Dt/2.0);

			//=================================================================================//
			//============================= Data Display/User Code ============================//
			// Make sure you don't take too long here!

			if (Print_counter > 4)
			{
				Print_counter=0;

				//eAngles.roll = atan(2*(q0*q1 + q2*q3)/(1-2*(q1*q1 + q2*q2)));
				eAngles.roll = q0 + q1 + q2 + q3;
				eAngles.pitch = asin(2*(q0*q2 - q1*q3));
				eAngles.yaw = atan(2*(q0*q3 + q1*q2)/(1-2*(q2*q2 + q3*q3)));

				//printdata(&sen_data, &eAngles); 
				usart.send(gyrDTime);
			}
			//StatusLEDToggle();
			//digitalWrite(debugPin,LOW);
		//}
	}
}
