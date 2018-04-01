#!/usr/bin/env python3
# Author: Andreas Spiess

from    time            import  sleep
import  RPi.GPIO        as      GPIO
import  os, time, signal, sys

fanPin = 18                 # PWM pin
desiredTemp = 40            # Desired temperature

dt_ON = time.time()         # ON time
dt_OFF= time.time()         # OFF time
turbo_ON = 5*60             # Switch turbo ON  after 5 minutes
turbo_OFF = turbo_ON + 2*60 # Switch turbo OFF after 2 minutes

fanSpeed = 100              # Max fan speed
sum = 0                     # Error sum (?)
pTemp = 15                  # Kp
iTemp = 0.4                 # Ki
    
def getCPUtemperature():
    res = os.popen( 'vcgencmd measure_temp' ).readline()
    temp =( res.replace("temp=", "").replace("'C\n", "") )
    #print("temp is {0}".format(temp)) #Uncomment here for testing
    return( temp )

def fanOFF():
    myPWM.ChangeDutyCycle( 0 )                  # switch fan off
    return( 1 )                                 # Return successfuly

def handleFan():
    global fanSpeed, sum, dt_ON                 # Change global variables
    
    actualTemp = float( getCPUtemperature() )   # Get current temperature

    diff  = actualTemp - desiredTemp            # 2 plus 2 is 4
    sum   = sum  + diff                         # minus 1 dats
    pDiff = diff * pTemp                        # 3, quick mafs
    iDiff = sum  * iTemp                        # ...

    fanSpeed = pDiff + iDiff                    # Set fan speed

    # Check if we need to enable turbo boost
    if( time.time() - dt_ON > turbo_ON and diff > 1.0 ):
        fanSpeed = 100                          # Set speed @ MAX
        
        if( time.time() - dt_ON > turbo_OFF ):  # If past allowed time
            dt_ON = time.time()                 # Reset timer

    # Else if turbo is not needed
    else:
        # Check as to not overshoot fan speed and burn GPIO pin
        if  ( fanSpeed > 100 )  : fanSpeed = 100
        elif( fanSpeed < 15  )  : fanSpeed =   0
        else                    : pass

    # Set limits to sum
    if  ( sum >  100 )      : sum =  100
    elif( sum < -100 )      : sum = -100
    else                    : pass

    print( "actualTemp {:4.2f} ".format(actualTemp) ) ,
    print( "TempDiff {:4.2f} "  .format( diff )     ) ,
    print( "pDiff {:4.2f} "     .format( pDiff )    ) ,
    print( "iDiff {:4.2f} "     .format( iDiff )    ) ,
    print( "fanSpeed {:4.2f}"   .format( fanSpeed ) )
    
    myPWM.ChangeDutyCycle( fanSpeed )

    return()	

# A little redundant function but useful if you want to add logging
def setPin( mode ):
    GPIO.output( fanPin, mode )
    return()

# **************
# MAKE IT HAPPEN
# **************
try:
    GPIO.setwarnings( False )
    GPIO.setmode( GPIO.BCM )
    GPIO.setup( fanPin, GPIO.OUT )
    myPWM = GPIO.PWM( fanPin, 50 )
    myPWM.start( 50 )
    GPIO.setwarnings( False )
    fanOFF()
    while True:
        handleFan()         # Do maffs here
        sleep( 1.0 )        # Read temperature every 1 sec 

except KeyboardInterrupt:   # Raise keyboard interrupt ( CTRL+C )
    fanOFF()                # Shutdown fan

finally:
    GPIO.cleanup()          # Reset GPIO ports used
