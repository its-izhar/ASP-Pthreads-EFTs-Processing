# @Author: Izhar Shaikh
# @Date:   2017-02-13T15:55:29-05:00
# @Email:  izharits@gmail.com
# @Filename: Makefile
# @Last modified by:   Izhar Shaikh
# @Last modified time: 2017-02-15T15:04:28-05:00


# Add the new TARGETS here
TARGETS = transfProg
CC = g++
HEADERS = -I.
CFLAGS = -Wall -Werror -std=c++11 -pthread -O2
#DEBUG_FLAGS = -g -DDEBUG
SOURCES = transfProg.cpp classMethods.cpp processThreads.cpp

all: clean $(TARGETS)

transfProg:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SOURCES) $(HEADERS) -o $@

clean:
	rm -rf $(TARGETS) *.o
