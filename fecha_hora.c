#include <stdio.h>
#include <string.h>
#include <time.h>

char* archivo_log(char *nombre_log) 
{
    static char nombre_completo[256];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Formatear la fecha y hora
    char fecha_hora[20];
    snprintf(fecha_hora, sizeof(fecha_hora), "_%02d%02d%02d_%02d%02d",
             tm.tm_year % 100, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min);

    // Crear el nombre completo
    snprintf(nombre_completo, sizeof(nombre_completo), "%s%s", nombre_log, fecha_hora);

    return nombre_completo;
}

int main()
{
    char nombre_log[256];
    strcpy(nombre_log, "prueba");

    strcpy(nombre_log, archivo_log(nombre_log));
    printf("Nombre del archivo: %s\n", nombre_log);

    return 0;
}

