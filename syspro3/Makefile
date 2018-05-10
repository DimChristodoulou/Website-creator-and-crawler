# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -c -std=c++11

# the build target executable:
myhttpd: TARGET=myhttpd
testClient: TARGET=testClient
mycrawler: TARGET=mycrawler

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp

clean:
	rm -fv myhttpd
	rm -fv testClient
	rm -fv mycrawler
