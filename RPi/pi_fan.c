/*
 *
 * PID Controller
 *
 * VERSION: 1.0
 *   	- MODIFIED: Initial release.
 *
 * KNOWN ISSUES:
 *   	- Controller doesn't interface fan yet.
 * 		- Code needs to be simplified and better
 * 		  written with C in mind
 *
 * AUTHOR                    :   Mohammad Odeh
 * WRITTEN                   :   Jun.  2nd, 2018 Year of Our Lord
 * LAST CONTRIBUTION DATE    :   ---
 *
 */


#include <stdio.h> 											// No need for introductions!
#include <time.h> 											// Get time
#include <wiringPi.h> 										// PWM GPIO pins

// Function prototypes
double getTemp();
double sleep_millis( int );
void PID_controller( float );

// Global variables
float integrator= 0;										// Integrator
float derivator = 0;										// Derivator
float error_old = 0; 										// Old error for derivator
float fanSpeed 	= 100; 										// Read variable name

// Main function
int main(){
	
	// Loop 43va!
	for(;;)
	{
		PID_controller( sleep_millis( 250 ) ); 				// Run PID controller every 0.25 seconds
	}
}

// Get CPU temperature
double getTemp(){
	FILE *temperatureFile;
	double T;
	temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
	if (temperatureFile == NULL)
	  ; //print some message
	fscanf (temperatureFile, "%lf", &T);
	T /= 1000;
	fclose (temperatureFile);
	
	return( T );
}

// Timer function: sleep (do nothing really) for x amount of time
double sleep_millis( int millisec ){
	clock_t start = clock();
	
	// Evaluate time difference in milliseconds
	double dt = (double)(clock() - start) * 1000 / CLOCKS_PER_SEC;
	
	// Do nothing for millisec amount of time
	while( millisec >= dt )
	{
		dt = (double)(clock() - start) * 1000 / CLOCKS_PER_SEC;
	}
	
	return (dt/1000); 										// Return time in seconds

}

// PID controller: where the magic happens
void PID_controller( float dt ){
	
	static int desiredTemp = 40; 							// Desired temperature
	
	static float Kp = 15.0;									// Define PID values
	static float Ki =  0.4; 								// ...
	static float Kd =  1.2; 								// ...
	
	static int max_int = 100; 								// Min/Max value
	static int min_int =-100; 								// attained by integrator
	
	double actualTemp = getTemp(); 							// Get current temperature
	
	float error = actualTemp - desiredTemp;              	// Get proportional error
    integrator  = integrator + error*dt;                 	// Get integral error
    derivator   = ( error - error_old ) / dt;            	// Get derivative error
    error_old   = error;                                 	// Update error_old

    float P_val = Kp * error;                            	// Compute PID parts
    float I_val = Ki * integrator;                       	// ...
    float D_val = Kd * derivator;                        	// ...

    fanSpeed    = P_val + I_val + D_val;                 	// Set fan speed                             

    // Set limits on integrator
    if  	( integrator > max_int ) integrator = max_int;  // Windup guard for integrator
    else if ( integrator < min_int ) integrator = min_int;  // ...

	// Check as to not overshoot fan speed and burn GPIO pin
	if  	( fanSpeed > 100 ) fanSpeed = 100;        		// Limit max speed to 100
	else if ( fanSpeed <  15 ) fanSpeed =  15;        		// Limit min speed to  15
	
	printf( "actualTemp %4.2f "	, 	actualTemp 	);
    printf( "TempDiff %5.2f "	, 	error     	);
    printf( "pDiff %6.2f "		, 	P_val    	);
    printf( "iDiff %6.2f "		, 	I_val    	);
    printf( "dDiff %6.2f "		, 	D_val    	);
    printf( "fanSpeed %6.2f\n"	, 	fanSpeed  	);
}
