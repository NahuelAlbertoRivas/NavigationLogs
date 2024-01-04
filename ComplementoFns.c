#include "ComplementoFns.h"

float aRadianes(float grados) {
    return grados/180 * pi;
}

float calcularDistanciaEnMetros(float lat1, float long1, float lat2, float long2) {
    //NOTA: distancia aproximada en metros para puntos poco distantes y alejado de los polos.
    //es una aproximacion, no usar en proyectos reales donde se requiera precision.
    double dist;
    dist = sin(aRadianes(lat1)) * sin(aRadianes(lat2)) + cos(aRadianes(lat1)) * cos(aRadianes(lat2)) * cos(aRadianes(long1 - long2));
    dist = acos(dist);
    dist = 6371*1000 * dist;
    return dist;
}

void realizarInformeDistanciaMinima(const char *nombArchivo){
    FILE *pEmbarcaciones;
    tEmbarcacion aux, *posicionEncontrada, *auxExpandir; // ' aux ' servirá para ir recuperando y procesando los datos de las embarcaciones; ' posicionEncontrada ' guardará la dirección del registro correspondiente; ' auxExpandir ' servirá para determinar si pudimos obtener más memoria o no
    GenEmbarcacion listaEmbarcaciones;
    char linea[MAXLINE], *p; // ' noBarraN ' ayudará a la correcta finalización de lectura por línea
    int mult, i; // ' mult ' hace referencia a la proporción en que se expandirá, x 1, x 2, x3, ..., x n veces su tamaño original
    float distCalculada;
    pEmbarcaciones = fopen(nombArchivo, "rt");
    if(!pEmbarcaciones){ printf("Error al abrir el archivo."); exit(1); }
    listaEmbarcaciones.vec = crearVectorDinamico();
    if(!listaEmbarcaciones.vec){ printf("Error interno."); exit(1); }
    listaEmbarcaciones.ce = 0;
    listaEmbarcaciones.tam = MINALLOC;
    inicializarBloqueEmbarcaciones(listaEmbarcaciones.vec, listaEmbarcaciones.ce);
    while(fgets(linea, MAXLINE, pEmbarcaciones)){
        p = strrchr(linea, '\n');
        *p = '\0';
        p = strrchr(linea, ',');
        *p = '\0';
        strcpy(aux.matricula, p + 1);
        p = strrchr(linea, ',');
        *p = '\0';
        aux.ultLon = atof(p + 1);
        aux.ultLat = atof(linea);
        posicionEncontrada = buscarEmbarcacion(listaEmbarcaciones, aux);
        if(posicionEncontrada){ // existe el registro
            distCalculada = calcularDistanciaEnMetros(posicionEncontrada->ultLat, posicionEncontrada->ultLon, aux.ultLat, aux.ultLon);
            if(posicionEncontrada->distRecorrida == 0){ posicionEncontrada->distRecorrida = distCalculada; }
            else if((posicionEncontrada->distRecorrida - distCalculada) > 0){ posicionEncontrada->distRecorrida = distCalculada; }
            posicionEncontrada->ultLat = aux.ultLat;
            posicionEncontrada->ultLon = aux.ultLon;
        }
        else{ // no existe el resgistro
            if((listaEmbarcaciones.ce < MAXEMBARCACIONES)){ // si no sobrepasamos el tamaño total
                if(listaEmbarcaciones.ce < listaEmbarcaciones.tam){  // y hay espacio disponible
                    strcpy((listaEmbarcaciones.vec + listaEmbarcaciones.ce)->matricula, aux.matricula);
                    (listaEmbarcaciones.vec + listaEmbarcaciones.ce)->ultLat = aux.ultLat;
                    (listaEmbarcaciones.vec + listaEmbarcaciones.ce)->ultLon = aux.ultLon;
                    inicializarBloqueEmbarcaciones(listaEmbarcaciones.vec, listaEmbarcaciones.ce);
                    listaEmbarcaciones.ce++;
                }
                else{ // no hay espacio libre
                    mult = sizeof(listaEmbarcaciones.tam) / MINALLOC; // calculamos la proporción a expandir
                    auxExpandir = expandirVectorDinamico(listaEmbarcaciones.vec, mult); // tratamos de expandir
                    if(!auxExpandir){ // al encontrarnos con una falla donde no se pueda adquirir más memoria, simplemente finalizaremos este bucle (provocando un EOF, consecuentemente un NULL en el fgets) y listaremos hasta donde llegamos
                        fseek(pEmbarcaciones, 0, SEEK_END);
                    }
                    else{ // se creó el vec, actualizamos datos
                        listaEmbarcaciones.tam += MINALLOC;
                        strcpy((listaEmbarcaciones.vec + listaEmbarcaciones.ce)->matricula, aux.matricula);
                        (listaEmbarcaciones.vec + listaEmbarcaciones.ce)->ultLat = aux.ultLat;
                        (listaEmbarcaciones.vec + listaEmbarcaciones.ce)->ultLon = aux.ultLon;
                        listaEmbarcaciones.ce++;
                    }
                }
            }
        }
    }
    for(i = 0; i < listaEmbarcaciones.ce; i++){
        printf("Minima distancia recorrida por embarcacion matricula %s: %f\n", (listaEmbarcaciones.vec + i)->matricula, (listaEmbarcaciones.vec + i)->distRecorrida);
    }
    free(listaEmbarcaciones.vec);
    fclose(pEmbarcaciones);
}

tEmbarcacion* crearVectorDinamico(){
    tEmbarcacion *vec;
    vec = malloc(sizeof(tEmbarcacion) * MINALLOC);
    if(!vec){ return NULL; }
    return vec; // retorna dir si se realizó exitosamente, caso contrario NULL
}

tEmbarcacion* expandirVectorDinamico(tEmbarcacion *vec, size_t mult){ // ' mult ' hace referencia a la proporción en que se expandirá, x 1, x 2, x3, ..., x n veces su tamaño original
    void *aux;
    aux = realloc(vec, sizeof(tEmbarcacion) * mult);
    if(!aux){ return NULL; }
    return aux;  // retorna dir si se realizó exitosamente, caso contrario NULL
}

tEmbarcacion* buscarEmbarcacion(GenEmbarcacion lista, tEmbarcacion aux){
    int i;
    for(i = 0; i < lista.ce; i++){
        if(strcmp((lista.vec + i)->matricula, aux.matricula) == 0){ return lista.vec + i; }
    }
    return NULL;
}

void inicializarBloqueEmbarcaciones(tEmbarcacion *vec, size_t inicio){ //
    int i;
    for(i = inicio; i < MINALLOC; i++){
        (vec + i)->distRecorrida = 0;
        (vec + i)->ultLat = 0;
        (vec + i)->ultLon = 0;
    }
}
