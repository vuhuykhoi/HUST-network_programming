CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = linklist.o main.o
TARGET = run
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~