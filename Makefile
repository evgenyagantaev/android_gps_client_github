TARGET=/root/gps
LIBS=-lncurses -lpthread -lm

all: $(TARGET)

$(TARGET): main.c 
	gcc -g main.c -o $(TARGET) $(LIBS)

clean:
	rm -rf *.o $(TARGET)
