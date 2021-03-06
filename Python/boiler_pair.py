#!/usr/bin/python
import time
# Written tng@chegwin.org 3-Jul-2014
# Python runs slower than C, so 18.9 factor below becomes 16.9 (microseconds)
sample_length=0.8
# Setup Pin 18 as output (Pin 12 in Pi numbering)
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.OUT)

#Calculate our on/offs up front
fourseventy=sample_length*400/1000000.0
fivesixty=sample_length*500/1000000.0
tenseventy=sample_length*1150/1000000.0
nineseventy=sample_length*1000/1000000.0

#thirty=30*sample_length #27
#thirtyonepointfive=31.5*sample_length
#six=6*sample_length
#four=4*sample_length


def two():
    #On  = 1 = 31.5 samples high, 6 low
    GPIO.output(18,True)
    time.sleep(tenseventy)
    GPIO.output(18,False)
    time.sleep(nineseventy)

def one():
    #Off = 0 = 6 samples high, 30 low
    GPIO.output(18,True)
    time.sleep(fivesixty)
    GPIO.output(18,False)
    time.sleep(fourseventy)

def sequence(incoming_string):
    # Splits an incoming set of ones and zeros, runs the appropriate function
    # Loops 8 times through
    count =0
    # Make sure we are all off to start with
    GPIO.output(18,False)
#    time.sleep(0.5)
    while count < 1:
        # Split the 1's and 0 into component parts and then run associated 
        #function
        for b in list(incoming_string):
            b = int(b)
            if (b == 2):
                two()
            elif (b == 1):
                one()
            else:
                print ("Something gone wrong")
        count += 1
        GPIO.output(18,False)
    # Sleep 2 seconds between sequences
    time.sleep(0.9) 

def main(): 
     # Boiler Pairing
     # 1,2,3,4 on

     sequence('111112121111111211211112111111')
     sequence('111112121111111211211112111111')
     sequence('111112121111111211211112111111')
     sequence('111112121111111211111111211211')
     sequence('111112121111111211111111211211')
     sequence('111112121111111211111111211211')



#     sequence('1011111100010000000011110')
#     sequence('1011111100010000000001110')
#     sequence('1011111100010000000010110')
#     sequence('1011111100010000000000110')
     # 1,2,3,4 off
#     sequence('1011111100010000000011100')
#     sequence('1011111100010000000001100')
#     sequence('1011111100010000000010100')
#     sequence('1011111100010000000000100')
     # Master
#     sequence('1011111100010000000011010')
#     sequence('1011111100010000000011000')
     GPIO.cleanup()

if __name__ == "__main__":
    try: 
        main()
    except (KeyboardInterrupt, SystemExit):
        print ("Thanks. Goodbye")
        GPIO.cleanup()
        exit();
