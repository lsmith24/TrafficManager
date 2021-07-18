#Usage:
#make          # compile project binary
#make clean    # remove all binaries and objects

PROG = project3c
CC = g++
CFLAGS = -pthread -Wall
LDFLAGS = -lpthread -lpaho-mqtt3as -lpaho-mqttpp3

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))  # find all .cpp files in the directory
HEADERS = $(wildcard *.h)                        # include all .h files in the directory

all: $(PROG)

$(PROG): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rvf *.o $(PROG)