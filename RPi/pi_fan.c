#include <stdio.h> 		// No need for introductions!
#include <time.h> 		// Get time
#include <wiringPi.h> 	// PWM GPIO pins

// Function prototypes
double getTemp();
void PID_controller();

// Global variables

//clock_t t2 = clock(), diff;

// 
int main(){
	clock_t start = clock();
	printf( "%3.2f", getTemp() );
	
	while( 1 )
	{
		double msec = (double)(clock() - start) / CLOCKS_PER_SEC;
		if( msec >= 2.5 )
		{
			printf( "Time passed = %3.3f\n", msec );
			start = clock();
			printf( "Value of start = %ld\n", start );
		}
		
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
	//printf ("The temperature is %3.2f C.\n", T);
	fclose (temperatureFile);
	
	return( T );
}

// PID controller: where the magic happens
void PID_controller(){
	
	static float Kp = 15.0;
	static float Ki =  0.4;
	static float Kd =  1.2;
	
	static int max_integrator = 100;
	static int min_integrator =-100;
	
	double actualTemp = getTemp();
	double total = (Kp+Kd+Ki+max_integrator+min_integrator+actualTemp);
	printf( "%3.3f", total );
}
