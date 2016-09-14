# listen.py

import sys
import glob
import serial
import subprocess

audio = 'snow.mp3'

def startListening():
    port = '/dev/tty.usbmodem1422'

    s = serial.Serial()
    s.port = port
    s.baudrate = 115200
    s.bytesize = serial.EIGHTBITS
    s.open()

    while True:
        read = s.readline()
        print read
        if 'The current map is' in read:
            audio = read[27:len(read)-7]+'.mp3'
            print(audio)
        if 'Game:Start' in read:
        	break

    print 'music'
    return_code = subprocess.call(['afplay', audio])

if __name__ == '__main__':
    startListening()