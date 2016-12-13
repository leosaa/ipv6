CFLAGS=-Wall -O2 -g
CC=gcc

all: data servidor-v6 cliente-v6

data:
	dd if=/dev/urandom of=data.bin bs=1k count=10

servidor-v6:
	@echo "Compilando servidor-v6.c"
	$(CC) $(CFLAGS) -o servidor-v6 servidor-v6.c


cliente-v6:
	@echo "Compilando cliente-v6.c"
	$(CC) $(CFLAGS) -o cliente-v6 cliente-v6.c

clean:
	rm -f servidor-v6 cliente-v6 data.bin *~
