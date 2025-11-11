CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lwayland-client -lwayland-egl -lEGL -lGLESv2 -lwayland-cursor -lm
INCLUDES = -I../common

SOURCES = main.c ../common/common.c ../common/window.c ../common/transform.c ../common/xdg-shell-protocol.c
TARGET = combined

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean