CXXFLAGS += -std=c++11
SOURCES=$(shell find . -name "*.cpp") ../Hardware.cpp ../../../Connection.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=thingTwo.o
FIDOLIB=../../../../Fido/build/fido.a
LDFLAGS=-pthread -lwiringPi

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(FIDOLIB) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
