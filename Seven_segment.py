from machine import Pin
import time

PinList = [15,21,16,17,5,18,19];  #  Seven segment machine.Pin is connected to Digital machine.Pin 15,2,4,5,18,19,21 to a,b,c,d,e,f,g
TruthTable=[ # truth table for numbers 0 to 9
    [0,0,0,0,0,0,1], #0
    [1,0,0,1,1,1,1], #1
    [0,0,1,0,0,1,0], #2
    [0,0,0,0,1,1,0], #3
    [1,0,0,1,1,0,0], #4
    [0,1,0,0,1,0,0], #5
    [0,1,0,0,0,0,0], #6
    [0,0,0,1,1,1,1], #7
    [0,0,0,0,0,0,0], #8
    [0,0,0,0,1,0,0]  #9
    ];

CurrNum=0;

def Display(TruthTable):
    i= 0
    for bits in TruthTable: # from 0 to 6 a,b,c,d,e,f,g
        Pin(PinList[i],Pin.OUT).value(bits)
        i+=1

def blueLight():
    Pin(2,Pin.OUT).value(1)
    time.sleep(0.005)
    Pin(2,Pin.OUT).value(0)

def inc(CurrNum):
    CurrNum=CurrNum+1;
    if (CurrNum>9):
            CurrNum=0;
    blueLight()
    Display(TruthTable[CurrNum])
    
def dec(CurrNum):
    CurrNum=CurrNum-1;
    if (CurrNum<0):
        CurrNum=0;
    blueLight()
    Display(TruthTable[CurrNum])

def rst(CurrNum):
    CurrNum=0;
    blueLight()
    Display(TruthTable[CurrNum])
    

Display(TruthTable[CurrNum])

IncBtn = Pin(12, Pin.IN, Pin.PULL_UP)
RstBtn = Pin(14, Pin.IN, Pin.PULL_UP)
DecBtn = Pin(27, Pin.IN, Pin.PULL_UP)

while True:
    firstread_incbtn = IncBtn.value() # not pressed 
    firstread_rstbtn  = RstBtn.value() # not pressed
    firstread_decbtn  = DecBtn.value() # not pressed
    time.sleep(0.01) 
    secondread_incbtn  = IncBtn.value() #pressed
    secondread_rstbtn = RstBtn.value() #pressed
    secondread_decbtn = DecBtn.value() #pressed

    if firstread_incbtn and not secondread_incbtn: #لو القيمتين مختلفين عن بعض ينفز الفانكشن و حيعمل كده عشان eldelay
        inc()
    elif firstread_rstbtn and not secondread_rstbtn:
        rst()
    elif firstread_decbtn and not secondread_decbtn:
        dec()  

    