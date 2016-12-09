/*
 * 
 * Este pragrama verifica si el sistema donde se ejecuta
 * es capaz de crear un socket TCP bajo IPv6 (AF_INET6)
 * Si crea el socket el sistema soporta IPv6 en la IPv6.
 * En caso contrario no es capaz de soportar IPv6 en la API.
 *
 * 
 * Jul 19, 2003
 * leo@ubiobio.cl
 */ 


#include<stdio.h> /* fprintf() */
#include<stdlib.h> /* exit() */
#include<sys/types.h> /* SOCK_STREAM */
#include<sys/socket.h> /* socket(), AF_INET6 */
#include<unistd.h> /* close() */

int
main (int argc, char *argv[])
{
	int sd = 0;
	if ((sd = socket (AF_INET6, SOCK_STREAM, 0)) == -1) {
			fprintf (stdout, "solo socket ipv4 estan soportados\n");
			exit (1);
	} else {
			close(sd);
			fprintf(stdout, "socket ipv6 estan soportados\n");
			exit (0);
	}
	return 0;
}

