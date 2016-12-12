CC=gcc
CFLAGS=-g -Wall -O2

all: portscan-v4 portscan-v6

portscan-v4: portscan-v4.c
	@echo "Compilling portscan-v4.c"
	$(CC) $(CFLAGS) -o portscan-v4 portscan-v4.c

portscan-v6: portscan-v6.c
	@echo "Compilling portscan-v6.c"
	$(CC) $(CFLAGS) -o portscan-v6 portscan-v6.c

clean:
	rm -f portscan-v4 portscan-v6 *~

