CFLAGS  = -std=gnu11 -Werror -Wall
LIBS = -pthread -lm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all clean

all: train_station

clean:
	$(RM) train_station

train_station: trains.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^
