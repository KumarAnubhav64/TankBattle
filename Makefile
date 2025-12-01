CC = g++
CFLAGS = -O2
LIBS = -lXbgi -lX11 -lm

SRCS = main.cpp Game.cpp Tank.cpp Bullet.cpp Network.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = tank_battle

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
