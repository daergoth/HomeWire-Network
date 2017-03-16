ARCH=armv6zk
ifeq "$(shell uname -m)" "armv7l"
ARCH=armv7-a
endif

# Detect the Raspberry Pi from cpuinfo
#Count the matches for BCM2708 or BCM2709 in cpuinfo
RPI=$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM2708)
ifneq "${RPI}" "1"
RPI=$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM2709)
endif

CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=$(ARCH) -mtune=arm1176jzf-s -std=gnu++11


# define all programs
PROGRAMS = network_server mesh_handler socket_handler message_converter
SOURCES = ${PROGRAMS:=.cpp}

all:
	g++ -std=gnu++11 -Wall -I../ -pthread -lboost_system -lboost_thread -lrf24-bcm -lrf24network -lrf24mesh ${SOURCES} -o network_server

debug:
	g++ -std=gnu++11 -g -Wall -I../ -pthread -lboost_system -lboost_thread -lrf24-bcm -lrf24network -lrf24mesh ${SOURCES} -o network_server
	

clean:
	rm -rf network_server
