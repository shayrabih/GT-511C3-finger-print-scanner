#CC       = gcc
CC       = arm-linux-gcc
CFLAGS   = -Wall
LDFLAGS  = 
OBJFILES = application.o driver.o client.o
TARGET   = fpstest
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
