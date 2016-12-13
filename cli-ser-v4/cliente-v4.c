#include <stdio.h>		/* printf(), perror() */
#include <stdlib.h>		/* exit(), atoi() */
#include <fcntl.h>		/* open */
#include <unistd.h>		/* close */
#include <errno.h>		/* errno */
#include <string.h>		/* memset */
#include <sys/socket.h>		/* socket */
#include <sys/types.h>	/* AF_INET, SOCK_STREAM */
#include <sys/sendfile.h>	/* sendfile (SOLO LINUX)*/
#include <arpa/inet.h>		/* inet_addr */

#define BUFF_SIZE (10*1024)
#define PORT 15000

void
usage (char *name)
{
	fprintf (stderr, "Error en el numero de argumentos\n");
	fprintf (stderr, "usar: %s <direccion> [<port>]\n", name);
	exit (0);
}

int
main (int argc, char **argv)
{
	int sd = 0;
	int cnt, port;
	char *name = argv[0];
	struct sockaddr_in sin4;
	char buffer[BUFF_SIZE];
	int total = 0;

	 if ((argc < 2) || (argc > 3)) {
			 usage(name);
	 }
	 if (argc == 2) {
			 port = PORT;
	 } else {
			 port = atoi(argv[2]);
	 }
		 
	sd = socket (PF_INET, SOCK_STREAM, 0);

	if (sd < 0) {
		perror ("socket");
		exit (errno);
	}

	memset (&sin4, 0, sizeof (struct sockaddr_in));

	sin4.sin_family = PF_INET;
	sin4.sin_port = htons (port);
	sin4.sin_addr.s_addr = inet_addr (argv[1]);

	printf ("Cliente conectandose a [%s]:%d\n", argv[1], port);

	if (connect (sd, (struct sockaddr *) &sin4, sizeof (sin4)) < 0) {
		perror ("errno");
		exit (errno);
	}
	while ((cnt = read (sd, &buffer[total], (BUFF_SIZE - total))))
		total += cnt;
	printf ("cliente recibio %d bytes.\n", total);

	close (sd);

	return 0;
}
