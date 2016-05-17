CC = gcc
CFLAGS = -Wall -O0 -g
LFLAGS = -lm -lpthread
OUT = ep2 
IN = ep2.c
OBJS = ep2.o StringOps.o 

all: $(OUT) ep2

clean:
	rm -f $(OUT) $(OBJS) ep2.o ep2
	

ep2: ep2.o StringOps.o
	$(CC) $(OBJS) $(LFLAGS) -o ep2

$(OUT): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $(OUT)

ep2.o: $(IN)
	$(CC) $(IN) -c -o ep2.o

StringOps.o: StringOps.c
	$(CC) StringOps.c -c -o StringOps.o

