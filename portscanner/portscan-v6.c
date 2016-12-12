#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>



#define START 1
#define STOP 1023

void
usage (char *name)
{
	fprintf (stderr, "Error en el numero de argumentos\n");
	fprintf (stderr, "use: %s <hostname> [<start>] [<stop>]\n", name);
	exit (0);
}

int
main (int argc, char *argv[])
{
	int port, sd, start, stop, err = 0;
	char *name = argv[0];
	char *host = argv[1];
	struct addrinfo *res;
	struct addrinfo hints;
	struct sockaddr_in6 sin6;
	struct servent *serv;

	if (argc < 2 || argc == 3 || argc > 4) {
		usage (name);
	}
	if (argc == 4) {
		start = atoi (argv[2]);
		stop = atoi (argv[3]);
	} else {
		start = START;
		stop = STOP;
	}

	memset (&sin6, 0, sizeof (struct sockaddr_in6));
	memset (&hints, 0, sizeof (hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = PF_UNSPEC;

	err = getaddrinfo (host, NULL, &hints, &res);
	if (err != 0) {
		fprintf (stderr, "getaddrinfo: %s error: %d\n", host, err);
		exit (0);
	}

	if (res->ai_family == AF_INET6) {
		sin6.sin6_family = AF_INET6;
		printf ("host ipv6\n");
	}

	if (res->ai_family == AF_INET) {
		printf ("host ipv4\n");
		exit (1);
	}

	memcpy (&sin6, res->ai_addr, sizeof (struct sockaddr_in6));

	fprintf (stdout, "Comenzando el escaneo de \"puertos\"....\n");

	for (port = start; port <= stop; port++) {
		sin6.sin6_port = htons (port);

		/*sd = socket (AF_INET6, SOCK_STREAM, 0); */
		sd = socket (res->ai_family, res->ai_socktype,
			     res->ai_protocol);
		if (sd == -1) {
			perror(" socket");
			exit (errno);
		}
		if (!connect
		    (sd, (struct sockaddr *) &sin6, sizeof (sin6))) {
			/*fprintf(stdout, "port: %i\n", port); */
			serv = getservbyport (sin6.sin6_port, "tcp");
			if (serv) {
			fprintf (stdout, "servicio %s port %d\n",
				 serv->s_name, ntohs (serv->s_port));
			} else {
					fprintf(stdout, "servicio en un  port deconocido%d\n", port);
			}

		}

		if (close (sd)) {
			perror("close");
			exit (errno);
		}
	}

	freeaddrinfo (res);
	return 0;
}
