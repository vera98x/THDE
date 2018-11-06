#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)

SOURCES := hardware_usart.cpp WifiControl.cpp button.cpp buzzer.cpp ir_decoder.cpp ir_detector.cpp ir_send.cpp OLEDcontroller.cpp runGameController.cpp 

# header files in this project
HEADERS := ir_decoder.hpp ir_detector.hpp ir_send.hpp buzzer.hpp runGameController.hpp OLEDcontroller.hpp button.hpp msg.hpp commandListener.hpp WifiControl.hpp hardware_usart.hpp

# other places to look for files for this project
SEARCH  := 

# set RELATIVE to the next higher directory 
# and defer to the Makefile.* there
RELATIVE := ..
include $(RELATIVE)/Makefile.due