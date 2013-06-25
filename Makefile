CFLAGS=-Wall
SOURCES=z80.cc main.cc
EXECUTABLE=z80

ifeq ($(DEBUG),0)
	CFLAGS += -O2
else
	CFLAGS += -DDEBUG
endif

all: z80_opcodes.h $(EXECUTABLE)

z80_opcodes.h:
	python gen_opcodes.py > z80_opcodes.h

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) *.o z80_opcodes.h
