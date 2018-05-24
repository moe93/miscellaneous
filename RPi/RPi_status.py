'''
*
* MQTT Subscriber
*
* VERSION: 0.2
*   - ADDED   : Initial, basic-ass release
*   - ADDED   : Read PID info from RPi
*
* KNOWN ISSUES:
*   - Needs better commenting/documentation
*
* AUTHOR                    :   Mohammad Odeh
* DATE                      :   May. 22nd, 2018 Year of Our Lord
* LAST CONTRIBUTION DATE    :   May. 24th, 2018 Year of Our Lord
*
'''

# Import modules
import paho.mqtt.client as mqtt

# This is the Subscriber

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("RPi/#")

def on_message(client, userdata, msg):
    if( msg.topic == "RPi/armFreq" ):
        print( "============== START ==============" )
        inData =    float( msg.payload.decode("utf-8") )
        print( " cpuFreq : {:6.2f} ,".format( inData ) ) ,
        
    elif( msg.topic == "RPi/temperature" ):
        inData =    float( msg.payload.decode("utf-8") )
        print( "Temp    : {:5.2f}   ".format( inData ) )
        
    elif( msg.topic == "RPi/fanSpeed" ):
        inData =    float( msg.payload.decode("utf-8") )
        print( " fanSpeed: {:6.2f} ,".format( inData ) ) ,
        
    elif( msg.topic == "RPi/turboMode" ):
        inData =    msg.payload.decode( "utf-8" )
        print( "Turbo   : {} ".format( inData ) )

    elif( msg.topic == "RPi/PID" ):
        inData = msg.payload.decode("utf-8")
        inData = inData.split(",")
        print( "-----------------------------------"   ) 
        print( " Error   : {:6.2f} ,".format( float(inData[0]) ) ) ,
        print( "P-value : {:5.2f}   ".format( float(inData[1]) ) )
        print( " I-value : {:6.2f} ,".format( float(inData[2]) ) ) ,
        print( "D-value : {:5.2f}   ".format( float(inData[3]) ) )
        print( "=============== END ===============\n" )        
        
    else: pass

addr = "localhost"
client = mqtt.Client()
client.connect( addr, 1883, 60 )

client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
