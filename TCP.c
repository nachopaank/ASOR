#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/utsname.h>
/*
Crear un servidor TCP de eco que escuche por conexiones entrantes en una dirección (IPv4 o IPv6) y puerto dados. 
Cuando reciba una conexión entrante, debe mostrar la dirección y número de puerto del cliente. 
A partir de ese momento, enviará al cliente todo lo que reciba desde el mismo (eco). 
Comprobar su funcionamiento empleando la herramienta Netcat como cliente. 
Comprobar qué sucede si varios clientes intentan conectar al mismo tiempo.
*/
int main(int argc, char ** argv){
	
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	int info = getaddrinfo(argv[1],argv[2],&hints,&res);
	int mySocket = socket(res->ai_family,res->ai_socktype,res->ai_protocol); 
	int a =bind(mySocket,res->ai_addr,res->ai_addrlen);
	freeaddrinfo(res);
	char buf[1024];
	struct sockaddr_in servaddr, clientaddr;
	socklen_t c_len = sizeof(clientaddr);
	int as = listen(mySocket,5);
	int exit = 1;
	int test = accept(mySocket,(struct sockaddr *)&clientaddr,&c_len);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	getnameinfo((struct sockaddr *)&clientaddr,&c_len,host,NI_MAXHOST,serv,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);
	printf("Mensaje de: %s:%s\n",host,serv);
	char buf2[256];
	printf("ay");
	while(exit == 1){
		int e = recv(test,buf2,sizeof(buf2),0);
		buf2[e] = '\0';
		if (buf2[0] == 'q'){
			exit = 0;
		}
		printf("%s",buf2);
		send(test,buf2,sizeof(buf2),0);
	}
	return 0;
}
