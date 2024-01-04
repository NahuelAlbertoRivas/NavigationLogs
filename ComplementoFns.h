#ifndef COMPLEMENTOFNS_H_INCLUDED
#define COMPLEMENTOFNS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define pi 3.141592
#define MAXLINE 1024
#define MINALLOC 5
#define MAXEMBARCACIONES 20


typedef struct{
    char matricula[9];
    float ultLat;
    float ultLon;
    float distRecorrida; // mínima
}tEmbarcacion;

typedef struct{
    tEmbarcacion *vec;
    int ce;
    int tam; // ' tam ' refiere a la capacidad máxima actual que abarca el array (inicia valiendo MINALLOC)
}GenEmbarcacion;

float aRadianes(float);

float calcularDistanciaEnMetros(float, float, float, float);

void realizarInformeDistanciaMinima(const char *);

tEmbarcacion* crearVectorDinamico();

tEmbarcacion* expandirVectorDinamico(tEmbarcacion *, size_t);

void inicializarBloqueEmbarcaciones(tEmbarcacion*, size_t);

tEmbarcacion* buscarEmbarcacion(GenEmbarcacion, tEmbarcacion);

#endif // COMPLEMENTOFNS_H_INCLUDED
