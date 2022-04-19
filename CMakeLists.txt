cmake_minimum_required(VERSION 3.21)
project(FinalUARTClock C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(FinalUARTClock
        DS1302.h
        Functions.c
        KeyScan.h
        LCD1602.c
        LCD1602.h
        main.c
        main.h
        ShowTIme.h
        Status.h
        Timer.h
        Uart.h)
