CFLAGS=-Wall -O2 -g
CC=gcc

all: data servidor-v4 cliente-v4

data:
	dd if=/dev/urandom of=data.bin bs=1k count=10

servidor-v4:
	@echo "Compilando servidor-v4.c"
	$(CC) $(CFLAGS) -o servidor-v4 servidor-v4.c


cliente-v4:
	@echo "Compilando cliente-v4.c"
	$(CC) $(CFLAGS) -o cliente-v4 cliente-v4.c

clean:
	rm -f servidor-v4 cliente-v4 data.bin *~
