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
Escribir un servidor UDP de hora de forma que:
La dirección y el puerto son el primer y segundo argumento del programa. Las direcciones pueden expresarse en cualquier formato (nombre de host, notación de punto…). Además, el servidor debe funcionar con direcciones IPv4 e IPv6 .
El servidor recibirá un comando (codificado en un carácter), de forma que ‘t’ devuelva la hora, ‘d’ devuelve la fecha y ‘q’ termina el proceso servidor.
En cada mensaje el servidor debe imprimir el nombre y puerto del cliente, usar getnameinfo(3).

*/
int main(int argc, char ** argv){
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    int info = getaddrinfo(argv[1],argv[2],&hints,&res);
    int mySocket = socket(res->ai_family,res->ai_socktype,res->ai_protocol); 
    int a =bind(mySocket,res->ai_addr,res->ai_addrlen);
    freeaddrinfo(res);
   
        char buf[1024];
        struct sockaddr_in servaddr, clientaddr;
        socklen_t c_len = sizeof(clientaddr);
        
    int exit = 1;
    while(exit == 1){
        int test = recvfrom(mySocket,buf,1024,0,(struct sockaddr *)&clientaddr,&c_len);
        buf[test] = '\0';

        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];
        getnameinfo((struct sockaddr *)&clientaddr,&c_len,host,NI_MAXHOST,serv,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);
        printf("Mensaje de: %s:%s\n",host,serv);
        time_t tim = time(NULL);
        struct tm *myTime;
        myTime = localtime(&tim);
        char buf2[256];
        if (buf[0] == 't'){
            strftime(buf2,256,"%T",myTime);
            sendto(mySocket,buf2,256,0,(struct sockaddr *)&clientaddr,c_len);
        }
        if (buf[0] == 'd'){
            strftime(buf2,256,"%F",myTime);
            sendto(mySocket,buf2,256,0,(struct sockaddr *)&clientaddr,c_len);
        }
        if (buf[0] == 'q'){
            exit = 0;
        }}
    return 0;
}
