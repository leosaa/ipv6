#include <stdio.h>		/* printf, fprintf, perror */
#include <stdlib.h>		/* exit */
#include <fcntl.h>		/* open */
#include <unistd.h>		/* close */
#include <errno.h>		/* errno */
#include <string.h>		/* memset */
#include <sys/socket.h>		/* socket */
#include <netdb.h>		/* socket */
#include <netinet/in.h>		/* sockaddr_in6 */
#include <sys/sendfile.h>	/* sendfile (SOLO LINUX) */
#define BUFF_SIZE (10*1024)	/* tamano del buffer */
#define PORT 15000

void
usage (char *name)
{
	fprintf (stderr, "Error en el numero de argumentos\n");
	fprintf (stderr, "use: %s <hostname> [<port>] \n", name);
	exit (0);
}


int
main (int argc, char *argv[])
{
	int sd, port, err, cnt;
	int total = 0;
	char *name = argv[0];
	char *host = argv[1];
	struct addrinfo *res;
	struct addrinfo hints;
	struct sockaddr_in6 sin6;
	char buffer[BUFF_SIZE];

	 if ((argc < 2) || (argc > 3)) {
			 usage(name);
	 }
	 if (argc == 2) {
			 port = PORT;
	 } else {
			 port = atoi(argv[2]);
	 }

	memset (&sin6, 0, sizeof (struct sockaddr_in6));
	memset (&hints, 0, sizeof (hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = PF_UNSPEC;

	err = getaddrinfo (host, NULL, &hints, &res);
	if (err != 0) {
		fprintf (stderr, "host %s error numero %d\n", host, err);
		exit (1);
	}

	if (res->ai_family == AF_INET6) {
		sin6.sin6_family = AF_INET6;
		printf ("host ipv6\n");
	}

	memcpy (&sin6, res->ai_addr, sizeof (struct sockaddr_in6));

	sin6.sin6_port = htons (port);
	sin6.sin6_family = res->ai_family;

	sd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sd == -1) {
		perror ("socket");
		exit (errno);
	}


	printf ("Cliente conectandose a [%s]:%d\n", argv[1], port);
	if (connect (sd, (struct sockaddr *) &sin6, sizeof (sin6)) < 0) {
		perror ("errno");
		exit (errno);
	}


	while ((cnt = read (sd, &buffer[total], (BUFF_SIZE - total))))
		total += cnt;
	printf ("El cliente recibio %d bytes.\n", total);

	close (sd);

	return 0;

}
