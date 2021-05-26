#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>
#define MAX_BUFFER 800

int openSocket();
void bindPort(int, int);
void chat(int, struct sockaddr_in);
void setHora(char *);
void setFecha(char *);

int main(int argc, const char *argv[]){
    if(argc > 1){
        int PORT = atoi(argv[1]);

        //Crea el socket
        int socket = openSocket();

        //Bind el socket creado con el puerto y la ip asignada
        bindPort(socket, PORT);
        printf("Quedamos conectados al puerto.\n");

        //El server se pone a escuchar
        int result = listen(socket, 10);
        if(result == -1){
            perror("Error en listen.\n");
            exit(0);
        }
        printf("Esperando conexion de un cliente\n");

        //Acepta el paquete del cliente
        struct sockaddr_in cliente;
        unsigned int addres_size = sizeof(cliente);
        int fd2 = accept(socket, (struct sockaddr*) &cliente, &addres_size);
        if(fd2 == -1){
            perror("No se pudo conectar al socket.\n");
            exit(0);
        }
        printf("Atendiendo al cliente...\n");

        //Empieza la comunicacion del sevidor con el cliente
        chat(fd2,cliente);
        
        //Cierra la conexion
        close(socket);
    }
    else{
        printf("Error: No se ingreso parametro\n");
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

void bindPort(int socket, int port){
    //Asigna ip y puerto
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = (in_port_t)htons(port);

    //Hace el bind
    int b = bind(socket, (struct sockaddr*)&server, sizeof(server));
    if(b == -1){
        perror("No se pudo enlazar el puerto con bind\n");
        exit(0);
    }
}

void chat(int fd2, struct sockaddr_in cliente){
    int i;
    char buffer[MAX_BUFFER] = "Buenos dias en que puedo ayudarte!!!, enviar help para ver los comandos aceptados.";
    send(fd2, buffer, sizeof(buffer), 0);

    while(1){
        bzero(buffer, sizeof(buffer));
        recv(fd2, buffer, sizeof(buffer), 0);
        printf("Cliente dijo: %s", buffer);
        if(strcmp(buffer, "help\n") == 0){
            strcpy(buffer, "\n*hora: retorna la hora del sistema\n*ip: retorna la IP del servidor\n*nombre: retorna el nombre del servidor\n*fecha: retorna la fecha\n*exit: cierra la conexion");
            send(fd2, buffer, sizeof(buffer), 0);
        }else if(strcmp(buffer, "hora\n") == 0){
            setHora(buffer);
            send(fd2, buffer, sizeof(buffer), 0);
        }else if(strcmp(buffer, "ip\n") == 0){
            struct in_addr clientIP = cliente.sin_addr;
            //char buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientIP, buffer, INET_ADDRSTRLEN);
            send(fd2, buffer, sizeof(buffer), 0);            
        }else if(strcmp(buffer, "nombre\n") == 0){
            strcpy(buffer,"Servidor Quesos");
            send(fd2, buffer, sizeof(buffer), 0);
        }else if(strcmp(buffer, "fecha\n") == 0){
            setFecha(buffer);
            send(fd2, buffer, sizeof(buffer), 0);
        }else if(strcmp(buffer, "exit\n") == 0){
            strcpy(buffer, "cerrando la conexion...");
            send(fd2, buffer, sizeof(buffer), 0);
            break;
        }else{
            strcpy(buffer, "Ese comando no existe.");
            send(fd2, buffer, sizeof(buffer), 0);
        }
    }
}

void setHora(char *Hora){
        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        strftime(Hora,10, /*" %d/%m/%y */ "%H:%M:%S", tlocal);
        
}

void setFecha(char *fecha){
        time_t date = time(0);
        struct tm *tlocal = localtime(&date);
        strftime(fecha,10, "%d/%m/%y", tlocal);
}