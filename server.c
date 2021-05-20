#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void setHora(char *Time){

        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        /*char output[128];*/
        strftime(Time,10, /*" %d/%m/%y */ "%H:%M:%S",tlocal);
        /*printf("%s\n",output);
        strcpy(Time, output);*/
}

void setFecha(char *fecha){

        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        strftime(fecha,10, "%d/%m/%y",tlocal);
}

int main(){

    char Hora[10];
    char Fecha[10];
    printf("Corriendo Server.\n");
    setHora(Hora);
    setFecha(Fecha);
    printf("Es el dia: %s.\n", Fecha);
    printf("La hora es: %s.\n", Hora);

    return 0;
}