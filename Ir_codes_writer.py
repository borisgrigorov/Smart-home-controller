#!/usr/bin/env python3
from pathlib import Path
import io
text = ""In *Smart_Home_Controller.ino* file in callback function set this function on the message you want to.
ffile = open("done.txt", "a")
with open('ir.txt', 'r', encoding="utf-8") as file:
    for raw in file:
        ffile.write("delayMicroseconds(" + raw.split(' ')[0] + ");\n")
        print(raw.split(' '))
        ffile.write("pulseIR(" + raw.split(' ')[2] + ");\n")