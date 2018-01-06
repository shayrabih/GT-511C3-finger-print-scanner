an embedded program that implement finger print scanner usage. 
writen in c . att datasheet of finger print module.

module GT-511c3X ,up to 1000 pringr print save in device.
implement all commands and feachers of the module.
using to MINI 2440 board ,connected to RS232(UART). 
at this program i use serial connection USB0 using TTL to get 5v .. .
i m using linux os, ubuntu core 4.4.0  inclide implement server side socket for future needs...
aplication.c : main function.
driver.c : fps commands implementation
driver.h: header file.
client.c : socket code implementation.
makefile : mark in file if gcc or arm-linux-gcc compiler.

