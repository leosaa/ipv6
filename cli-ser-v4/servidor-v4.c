#include <stdio.h>		/* printf, perror */
#include <stdlib.h>		/* exit */
#include <fcntl.h>		/* open */
#include <unistd.h>		/* close */
#include <errno.h>		/* errno */
#include <string.h>		/* memset */
#include <sys/socket.h>		/* socket */
#include <netinet/in.h>		/* sockaddr_in */
#include <sys/sendfile.h>	/* sendfile */
#include <arpa/inet.h>		/* inet_addr */

#define BUFF_SIZE (10*1024)	/* tamano del buffer tmp */
#define PORT 15000

void
usage (char *name)
{
	fprintf (stderr, "error en el numero de argumentos\n");
	fprintf (stderr, "usar : %s <direccion> [<port>]\n", name);
	exit (0);
}

int
main (int argc, char *argv[])
{
	int sd = 0, fd = 0;
	int cnt, client, port;
	char *name = argv[0];
	off_t off = 0;
	struct sockaddr_in sin4;

	 if ((argc < 2) || (argc > 3)) {
			 usage(name);
	 }
	 if (argc == 2) {
			 port = PORT;
	 } else {
			 port = atoi(argv[2]);
	 }

	sd = socket (PF_INET, SOCK_STREAM, 0);
	fd = open ("data.bin", O_RDONLY);

	if (sd < 0 || fd < 0) {
		perror ("socket/open");
		exit (errno);
	}

	memset (&sin4, 0, sizeof (struct sockaddr_in));

	sin4.sin_family = PF_INET;
	sin4.sin_port = htons (port);
	sin4.sin_addr.s_addr = inet_addr (argv[1]);

	printf ("server escuchando en  [%s]:%d\n", argv[1], port);

	if (bind (sd, (struct sockaddr *) &sin4, sizeof (sin4)) < 0) {
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
	printf ("el servidor envio %d bytes.\n", cnt);

	close (client);
	close (sd);
	close (fd);

	return 0;
}
