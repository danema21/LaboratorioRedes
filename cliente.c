#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAX_BUFFER 800

int openSocket();
void connectToServer(int, int, const char*);
void chat(int);

int main(int argc, const char *argv[]){

    if(argc > 2){   // control ingreso de parametros
        int PORT = atoi(argv[2]);
        const char *IP = argv[1];

        //Crea el socket
        int socket = openSocket();

        //Conecta el socket del cliente al socket del server
        connectToServer(socket, PORT, IP);

        //Empieza la comunicacion del cliente con el servidor
        chat(socket);
        
        //Cierra la conexion
        close(socket);
}
else{
    printf("Error: Debe ingresar ip y puerto por parametro\n");
}
    return 0;
}

int openSocket(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("Error al abrir el socket\n");
        exit(0);
    }
    return sock;
}

void connectToServer(int socket, int port, const char *ip){
    //Convierte la direccion ip que pasamos como parametro
    struct hostent *he;
    if((he=gethostbyname(ip)) == NULL){
        printf("Error de gethostbyname()\n");
        exit(0);
    }

    
    //Asigna ip y puerto
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr = *((struct in_addr *)he->h_addr);
    server.sin_port = (in_port_t)htons(port);

    //hace la conexion
    if (connect(socket , (struct sockaddr *)&server , sizeof(server)) < 0){
		perror("No se pudo conectar al server\n");
		exit(0);
	}
     system("hostname");
}
 
void chat(int socket){
    int i;
    char buffer[MAX_BUFFER];
    recv(socket, buffer, sizeof(buffer), 0);
    printf("Server dijo: %s\n", buffer);

    while(1){
        bzero(buffer, sizeof(buffer));
        i = 0;
        while((buffer[i++] = getchar()) != '\n');
        send(socket, buffer, sizeof(buffer), 0);
        bzero(buffer, sizeof(buffer));
        recv(socket, buffer, sizeof(buffer), 0);
        printf("Server dijo: %s\n", buffer);
        if(strcmp(buffer, "cerrando la conexion...") == 0){
            printf("Finalizo la conexion\n");
            break;
        }
    }
}