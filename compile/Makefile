
CC=g++
LDLIBS = -lwiringPi -lRTIMULib -lpthread -lncurses
CFLAGS = -c -g -Wall -std=c++11
LDFLAGS=
SRC = ../src/ping/ping.cpp ../src/servo/servo.cpp ../src/scan/scanner.cpp ../src/pos/position.cpp ../src/sentry/sentry.cpp ../src/driver.cpp
NAMES = $(SRC:.cpp=)
OBJ = $(SRC:.cpp=.o)
EXE = Sentry.exe

all: $(NAMES) $(EXE)

$(EXE): $(OBJ)
			$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ) -o $@

$(NAMES):
			$(CC) $(CFLAGS) $@.cpp -o $@.o

clean:
	rm -f $(OBJ)
