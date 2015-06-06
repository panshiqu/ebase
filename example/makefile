CC = gcc
XX = g++
CFLAGS = -g -std=c++0x -lpthread

TARGET = example
SOURCES += $(wildcard *.c *.cc *.cpp *.cxx)
OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SOURCES))))

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

%.o:%.cc
	$(XX) $(CFLAGS) $(INC) -c $< -o $@

%.o:%.cpp
	$(XX) $(CFLAGS) $(INC) -c $< -o $@

%.o:%.cxx
	$(XX) $(CFLAGS) $(INC) -c $< -o $@

$(TARGET) : $(OBJECTS)
	@echo $(SOURCES)
	@echo $(OBJECTS)
	$(XX) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -rf $(OBJECTS) $(TARGET)