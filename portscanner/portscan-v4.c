#include <stdio.h> /* fprintf() perror */
#include <sys/socket.h> /* socket(), connect() */
#include <sys/types.h> /* AF_INET, SOCK_STREAM */
#include <stdlib.h> /* exit () */
#include <unistd.h> /* close () */
#include <netdb.h> /* gethostbyname(), hostent, hostent, getservbyport() */
#include <errno.h> /* errno*/

#define START 1
#define STOP 1023

void
usage (char *name)
{
	fprintf (stderr, "Error en el numero argumentos \n");
	fprintf (stderr, "use: %s <hostname> [<inicio>] [<final>]\n",
		 name);
	exit (0);
}

int
main (int argc, char *argv[])
{
	int port, sd, start, stop;
	char *name = argv[0];
	struct hostent *host;
	struct sockaddr_in sin4;
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

	host = gethostbyname (argv[1]);
	if (!host) {
		perror("gethostbyname");
		exit (errno);
	}

	sin4.sin_family = AF_INET;
	sin4.sin_addr.s_addr = *(long *) (host->h_addr);

	fprintf (stdout, "Comenzado el \"escaneo de puertos\"....\n");

	for (port = start; port <= stop; port++) {
		sin4.sin_port = htons (port);
		sd = socket (AF_INET, SOCK_STREAM, 0);
		if (sd < 0) {
			perror(" socket");
			exit (errno);
		}
		if (!connect
		    (sd, (struct sockaddr *) &sin4, sizeof (sin4))) {
			/* fprintf(stdout, "port: %i\n", port); */
			serv = getservbyport (htons (port), "tcp");
			if (serv) {
			fprintf (stdout, " %s port %d \n", serv->s_name,
				 (int) ntohs (serv->s_port));
			} else {
					fprintf(stdout, "servicio en un  port %d desconocido\n", port);

			}
		}
		if (close (sd)) {
			perror(" close");
			exit (errno);
		}
	}
	return 0;
}
