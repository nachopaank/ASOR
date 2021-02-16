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
Escribir un programa que implemente el mecanismo de sincronización de parada y espera:
El padre leerá de la entrada estándar (terminal) y enviará el mensaje al proceso hijo, escribiéndolo en la tubería p_h. Entonces permanecerá bloqueado esperando la confirmación por parte del hijo en la otra tubería, h_p.
El hijo leerá de la tubería p_h, escribirá el mensaje por la salida estándar y esperará 1 segundo. Entonces, enviará el carácter ‘l’ al proceso padre, escribiéndolo en la tubería h_p, para indicar que está listo. Después de 10 mensajes enviará el carácter ‘q’ para indicar al padre que finalice.
*/
int main(int argc, char **argv){
	int p_h[2];
	int h_p[2];
	pipe(p_h);
	pipe(h_p);

	pid_t pid = fork();
	if(pid == 0){
		close(p_h[1]);
		close(h_p[0]);
		int max = 10;
		char buf[2048];
		while(max>0){
			read(p_h[0],&buf,sizeof(&buf));
			max--;
			printf("%s\n",&buf);

			sleep(1);
			write(h_p[1],"l",1);
		}
		write(h_p[1],"q",1);
		close(p_h[0]);
		close(h_p[1]);
	}
	else if (pid > 0){
		close(p_h[0]);
		close(h_p[1]);

		char buf2[2048];
		char recieve[1];
		while(recieve[0] != "q"){
			read(1,&buf2,sizeof(&buf2));
			write(p_h[1],buf2,sizeof(&buf2));
			read(h_p[0],recieve,1);
		}
		close(p_h[1]);
		close(h_p[0]);
		printf("Se acabo");
	}
	return 0;
}
