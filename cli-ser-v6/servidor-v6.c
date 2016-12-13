#include <stdio.h>		/* printf, fprintf, perror */
#include <stdlib.h>		/* exit */
#include <fcntl.h>		/* open */
#include <unistd.h>		/* close */
#include <errno.h>		/* errno */
#include <string.h>		/* memset */
#include <sys/socket.h>		/* socket */
#include <netdb.h>		/* socket, getaddrinfo */
#include <netinet/in.h>		/* sockaddr_in6 */
#include <sys/sendfile.h>	/* sendfile (SOLO LINUX) */

#define BUFF_SIZE (10*1024)	/* tamano del buffer */
#define PORT 15000

/*
 * Por el momento funciona con un archivo y un puerto
 * pre-establecidos. Aunque se podrian ingresar por parametros
 * directos a main
 */


void
usage (char *name)
{
	fprintf (stderr, "Error en el numero de argumentos\n");
	fprintf (stderr, "use: %s <hostname> [<port>]\n", name);
	exit (0);
}


int
main (int argc, char *argv[])
{
	int sd, port, err, client, cnt;
	off_t off = 0;
	int fd = 0;
	char *name = argv[0];
	char *host = argv[1];
	struct addrinfo *res;
	struct addrinfo hints;
	struct sockaddr_in6 sin6;

	if ((argc < 2) || (argc > 3)) {
			usage(name);
	}
	if (argc == 2) {
			port = PORT;
	} else {
			port = atoi(argv[2]);
	}
			
			


	fd = open ("data.bin", O_RDONLY);
	if (fd < 0) {
		fprintf (stderr, "error no existe el archivo\n");
		exit (1);
	}

	memset (&sin6, 0, sizeof (struct sockaddr_in6));
	memset (&hints, 0, sizeof (hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = PF_UNSPEC;

	err = getaddrinfo (host, NULL, &hints, &res);
	if (err != 0) {
		fprintf (stderr, "host %s error numero  %d\n", host, err);
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

	fprintf (stdout, "Servidor escuchando en [%s]:%d\n", argv[1], port);

	if (bind (sd, (struct sockaddr *) &sin6, sizeof (sin6)) < 0) {
		perror ("bind");
		exit (errno);
	}

	if (listen (sd, 5) < 0) {
		perror ("listen");
		exit (errno);
	}
	if ((client = accept (sd, NULL, NULL)) < 0) {
		perror ("accept");
		exit (errno);
	}

	if ((cnt = sendfile (client, fd, &off, BUFF_SIZE)) < 0) {
		perror ("sendfile");
		exit (errno);
	}

	printf ("El servidor envio %d bytes.\n", cnt);
	close (client);
	close (sd);
	close (fd);

	return 0;

}
