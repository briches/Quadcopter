/******************************************************
Library designed to manage, update, and Control the
state of quadcopter

Author  : Brandon Riches
Date     :	June 2013

Updated for compatability with main polling loop and GPS interrupts


******************************************************/
#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED


#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SENSORLIB.h>
#include <OseppGyro.h>
#include <I2C.h>
#include <math.h>
#include <Servo.h>


#define Pi  (3.14159F)									// Its pi.
#define poll2_interrupt (50000)				// The latency period between readings of the USRF, in microseconds
#define poll3_interrupt (100000)				// The latency period between readings of the USRF, in microseconds
#define USRF_pin (0x0)								// Connect the USRF AN pin to this pin

#define MOTOR_MIN   50
#define MOTOR_MAX  105

/*=========================================================================
    I2C Addresses
    -----------------------------------------------------------------------*/
#define Accel_Address   (0x19)
#define Gyro_Address    (0x69)

/*=========================================================================
    STATUS LEDs
    -----------------------------------------------------------------------*/
#define GREEN_LED 	27
#define RED_LED 			25
#define YELLOW_LED 	23


/*=========================================================================
    mov avg (Soon to be Chebyshev 4th order LPF)
    -----------------------------------------------------------------------*/
#define DATA_POINTS  30


	/*===============================================
    Device settings (Options for sensors)
    - Explaining these is kinda hard, go read the data sheets.
    -----------------------------------------------------------------------*/
	const int d_ScaleRange = FULL_SCALE_RANGE_250; // x250,x500,x1000,x2000
	const int DLPF = 6;                 // 0,1,2,3,4,5,6,7 // See data sheet
	const bool HighDef = true;          // Is accel output 2byte or 1byte

// These are the drift rates in d/(s*mus)
	#define GYRO_DRIFT_RATE_X   -9E-10
	#define GYRO_DRIFT_RATE_Y     3E-10
	#define GYRO_DRIFT_RATE_Z   -3E-11


/***************************************************************************
 *! @QCopterData
 ***************************************************************************/
/*==========================================================================
	Main struct, holds settings, vars, and some data
    ------------------------------------------------------------------------------------------------------------*/
class Quadcopter
{
	public:

		double 	movave[10][DATA_POINTS];
		int     overwrite;

		/*===============================================
		Current motor speeds
		// Given in percent.
		-----------------------------------------------------------------------*/
		int motor1s;
		int motor2s;
		int motor3s;
		int motor4s;

		/*===============================================
		Sensor data, current and buffer.
		-----------------------------------------------------------------------*/
		double ax;                   // Basic sensor data
		double ay;
		double az;
		double wx;
		double wy;
		double wz;
		double mx;
		double my;
		double mz;
		double elev;

		double 	prev_ax[10],
						prev_ay[10],
						prev_az[10],
						prev_wx[10],
						prev_wy[10],
						prev_wz[10],
						prev_mx[10],
						prev_my[10],
						prev_mz[10],
						prev_elev[10];

		double alpha_gyro;
		double beta_gyro;
		double heading_gyro;

		/*===============================================
		Time keeping for polling and interrupts
		-----------------------------------------------------------------------*/
		double tpoll1;
		double tpoll2;
		double tpoll3;
		int freq;				    		// Frequency of calls to update();


		/*===============================================
		Sensor initial offset data
		-----------------------------------------------------------------------*/
		double io_ax;                 // Offsets from initial position sensor data
		double io_ay;
		double io_az;
		double io_wx;
		double io_wy;
		double io_wz;

		/*=========================================================================
		State variables
		- The program didnt like having these in the class.
		-----------------------------------------------------------------------*/
		double alpha;
		double beta;
		double heading;

		/***************************************************************************
		 *! @FUNCTIONS
		 ***************************************************************************/
		bool initSensor();                      							// Initializes the two sensors
		bool initMotors(int speed);                     							// Initializes the 4 motors
		void update(double aPID_out, double bPID_out);
		bool updateMotors(double aPID_out, double bPID_out);

		void ERROR_LED(int LED_SEL);


		/*===============================================
			Class initializations
			-----------------------------------------------------------------------*/
	  SENSORLIB     accelmag;       		// Accel class
	  OseppGyro             gyro;			// Gyro class
	  Servo                  motor1;			// Motor 1
	  Servo                  motor2;			// Motor 2
	  Servo                  motor3;			// Motor 3
	  Servo                  motor4;			// Motor 4

		private:
		void SI_convert();                 							// Convert to SI
		void mov_avg();												// Runs a moving average
		void get_Initial_Offsets();                           	// Gets the initial Offsets
};




#endif // CONTROL_H_INCLUDED
