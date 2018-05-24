#!/usr/bin/python
'''
*
* PID Controller for 5V fan adapted
* from Andreas Spiess
*
* VERSION: 1.0.1
*   - ADDED   : Proper PID controllers
*   - MODIFIED: Better documentation/streamline code
*   - ADDED   : Send PID controller info over MQTT
*
* KNOWN ISSUES:
*   - Non atm
*
* AUTHOR                    :   Mohammad Odeh
* WRITTEN                   :   Mar. 31st, 2018 Year of Our Lord
* LAST CONTRIBUTION DATE    :   May. 23rd, 2018 Year of Our Lord
*
'''

# Import modules
from    time                    import  sleep           # Delays
import  RPi.GPIO                as      GPIO            # PWM
import  paho.mqtt.client        as      mqtt            # Communication
import  os, time, signal, sys                           # OS commands


# ************************************************************************
# =====================> DEFINE NECESSARY FUNCTIONS <=====================
# ************************************************************************

def getCPUtemperature():
    '''
    Read CPU temperature
    '''
    
    res  = os.popen( 'vcgencmd measure_temp' ).readline()
    temp = ( res.replace("temp=", "").replace("'C\n", "") )
    return( temp )

# ------------------------------------------------------------------------

def fanOFF():
    '''
    Turn fan OFF
    '''
    
    myPWM.ChangeDutyCycle( 0 )                          # Switch fan off
    return( 1 )                                         # Return successfuly

# ------------------------------------------------------------------------

def handleFan():
    '''
    PID controller for the fan. Adjusts fan speed.
    '''
    
    global fanSpeed, dt_ON, turboStatus                 # Change global variables
    global integrator, derivator                        # ...
    
    actualTemp = float( getCPUtemperature() )           # Get current temperature

    error = actualTemp - desiredTemp                    # 2 plus 2 is 4
    integrator  = integrator + error                    # ...
    P_val = Kp * error                                  # minus 1 dats
    I_val = Ki * integrator                             # ...
    D_val = Kd * ( error - derivator )                  # 3, quick mafs

    derivator   = error                                 # Update derivator
    fanSpeed    = P_val + I_val + D_val                 # Set fan speed                             

    # Check if we need to enable turbo boost
    if( time.time() - dt_ON > turbo_ON ):
        turboStatus = "ON"                              # Turbo Status
        fanSpeed = 100                                  # Set speed @ MAX
        
        if( time.time() - dt_ON > turbo_OFF ):          # If past allowed time
            turboStatus = "OFF"                         # Turbo Status
            dt_ON = time.time()                         # Reset timer

    # Else if turbo is not needed
    else:
        # Check as to not overshoot fan speed and burn GPIO pin
        if  ( fanSpeed > 100 )  : fanSpeed = 100        # Limit max speed to 100
        elif( fanSpeed <  35  ) : fanSpeed =  35        # Limit min speed to  35
        else                    : pass                  # ...

    # Set limits on integrator
    if  ( integrator > max_int ): integrator = max_int  # Windup guard for integrator
    elif( integrator < min_int ): integrator = min_int  # ...
    else                        : pass                  # ...

##    print( "actualTemp {:4.2f} ".format(actualTemp) ) ,
##    print( "TempDiff {:5.2f} "  .format( error )    ) ,
##    print( "pDiff {:6.2f} "     .format( P_val )    ) ,
##    print( "iDiff {:6.2f} "     .format( I_val )    ) ,
##    print( "dDiff {:6.2f} "     .format( D_val )    ) ,
##    print( "fanSpeed {:6.2f}"   .format( fanSpeed ) )
    
    myPWM.ChangeDutyCycle( fanSpeed )                   # Set fan speed

    # Publish on MQTT
    PID = "{}, {}, {}, {}".format(error, P_val, I_val, D_val)
    arm_freq = os.popen( "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq" ).readline()
    client.publish( "RPi/armFreq"       , int(arm_freq)/1000000.    )
    client.publish( "RPi/temperature"   , actualTemp                )
    client.publish( "RPi/fanSpeed"      , fanSpeed                  )
    client.publish( "RPi/turboMode"     , turboStatus               )
    client.publish( "RPi/PID"           , PID                       )
    
    return()	

# ------------------------------------------------------------------------

def setPin( mode ):
    '''
    A little redundant function but useful if you
    want to add logging
    '''
    
    GPIO.output( fanPin, mode )
    return()

# ************************************************************************
# ===========================> SETUP PROGRAM <===========================
# ************************************************************************

# General info/variables
fanPin = 18                                             # PWM pin
desiredTemp = 35                                        # Desired temperature

dt_ON = time.time()                                     # ON time
dt_OFF= time.time()                                     # OFF time
turbo_ON = 30*60                                        # Turbo ON  after 30 minutes
turbo_OFF = turbo_ON + 2*60                             # Turbo OFF after  2 minutes
turboStatus = "OFF"                                     # Turbo OFF initially

# Setup PID
fanSpeed = 100                                          # Max fan speed
Kp = 15                                                 # Kp
Ki = 0.4                                                # Ki
Kd = 1.2                                                # Kd

integrator  = 0                                         # Integrator
derivator   = 0                                         # Derivator
max_int     = 100                                       # Max value attained by integrator
min_int     =-100                                       # Min value attained by integrator

# Start MQTT publisher
addr = "localhost"                                      # Address to publish to
client = mqtt.Client()                                  # Initialize MQTT client 
client.connect( addr, 1883, 60 )                        # Connect client to host

# ************************************************************************
# =========================> MAKE IT ALL HAPPEN <=========================
# ************************************************************************
try:
    GPIO.setwarnings( False )                           # Disable system warnings
    GPIO.setmode( GPIO.BCM )                            # Use BCM numbering scheme
    GPIO.setup( fanPin, GPIO.OUT )                      # Set pin as OUTPUT
    myPWM = GPIO.PWM( fanPin, 50 )                      # Define PWM pin at 50Hz
    myPWM.start( 50 )                                   # Start with 50%
    fanOFF()                                            # Turn fan OFF before running
    
    while( True ):                                      # Loop 43va!
        handleFan()                                     # Do maffs here
        sleep( 1.0 )                                    # Read temperature every 1 sec 

except KeyboardInterrupt:                               # Raise keyboard interrupt ( CTRL+C )
    fanOFF()                                            # Shutdown fan

finally:
    GPIO.cleanup()                                      # Reset GPIO ports used
