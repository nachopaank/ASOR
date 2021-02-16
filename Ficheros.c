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
Proyecto: Comando ls extendido
Escribir un programa que cumpla las siguientes especificaciones:
El programa tiene un único argumento que es la ruta a un directorio. El programa debe comprobar la corrección del argumento.
El programa recorrerá las entradas del directorio de forma que:
Si es un fichero normal, escribirá el nombre.
Si es un directorio, escribirá el nombre seguido del carácter ‘/’.
Si es un enlace simbólico, escribirá su nombre seguido de ‘->’ y el nombre del fichero enlazado. Usar la función readlink(2) y dimensionar adecuadamente el buffer de la función.
Si el fichero es ejecutable, escribirá el nombre seguido del carácter ‘*’.
Al final de la lista el programa escribirá el tamaño total que ocupan los ficheros (no directorios) en kilobytes.
*/
int main(int argc, char **argv){
	DIR *d = opendir(argv[1]);
	if (d==NULL){
		return 0;
	}
	struct dirent* currDir;	
	currDir = readdir(d);
	off_t tam = 0;
	char* length;
	size_t l2;
	while (currDir != NULL){
		l2 = sizeof(char) * (strlen(argv[1])+strlen(currDir->d_name)+3);
		length = (char *) malloc(l2);
		sprintf(length, "%s/%s",argv[1],currDir->d_name);
		struct stat myStat;
		stat(length,&myStat);
		if(currDir->d_type == DT_LNK){
			char *storeLink = (char *) malloc(300);
			readlink(length,storeLink,sizeof(storeLink));
			printf("%s -> %s",currDir->d_name,storeLink);
			free(storeLink);
		}else if(currDir->d_type == DT_REG){
			if (myStat.st_mode & S_IXUSR >0){
				printf("%s *\n",currDir->d_name);
			}
			else{
				printf("%s\n",currDir->d_name);
			}
			tam = tam+myStat.st_size;
		}else if(currDir->d_type == DT_DIR){
			printf("%s/\n",currDir->d_name); 
		}
		free(length);
		currDir = readdir(d);
	}
	tam = tam/1024;
	printf("Tamano de ficheros total: %d kb \n",tam);
	return 0; 
}}
