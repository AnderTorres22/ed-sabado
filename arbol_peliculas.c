#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Pelicula {
    char *nombre;
    int anio;
    char *genero;
    double recaudacion;
    struct Pelicula *izquierda;
    struct Pelicula *derecha;
} Pelicula;

Pelicula* crearNodo(char *nombre, int anio, char *genero, double recaudacion) {
    Pelicula* nuevoNodo = (Pelicula*) malloc(sizeof(Pelicula));
    if (nuevoNodo == NULL) {
        perror("Error al asignar memoria para el nuevo nodo");
        exit(EXIT_FAILURE);
    }
    nuevoNodo->nombre = strdup(nombre);
    nuevoNodo->anio = anio;
    nuevoNodo->genero = strdup(genero);
    nuevoNodo->recaudacion = recaudacion;
    nuevoNodo->izquierda = NULL;
    nuevoNodo->derecha = NULL;
    return nuevoNodo;
}

Pelicula* insertarPelicula(Pelicula* raiz, char *nombre, int anio, char *genero, double recaudacion) {
    if (raiz == NULL) {
        return crearNodo(nombre, anio, genero, recaudacion);
    }
    if (anio < raiz->anio || (anio == raiz->anio)) {
        raiz->izquierda = insertarPelicula(raiz->izquierda, nombre, anio, genero, recaudacion);
    } else {
        raiz->derecha = insertarPelicula(raiz->derecha, nombre, anio, genero, recaudacion);
    }
    return raiz;
}

void inorden(Pelicula* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izquierda);
        printf("Nombre: %s, Año: %d, Género: %s, Recaudación: %.6f millones\n",
               raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        inorden(raiz->derecha);
    }
}

void preorden(Pelicula* raiz) {
    if (raiz != NULL) {
        printf("Nombre: %s, Año: %d, Género: %s, Recaudación: %.6f millones\n",
               raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        preorden(raiz->izquierda);
        preorden(raiz->derecha);
    }
}

void posorden(Pelicula* raiz) {
    if (raiz != NULL) {
        posorden(raiz->izquierda);
        posorden(raiz->derecha);
        printf("Nombre: %s, Año: %d, Género: %s, Recaudación: %.6f millones\n",
               raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
    }
}

void buscarPelicula(Pelicula* raiz, char *nombreBuscar) {
    if (raiz == NULL) {
        printf("La película '%s' no se encontró en el árbol.\n", nombreBuscar);
        return;
    }
    int comparacion = strcmp(nombreBuscar, raiz->nombre);
    if (comparacion == 0) {
        printf("Película encontrada:\n");
        printf("Nombre: %s, Año: %d, Género: %s, Recaudación: %.6f millones\n",
               raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
    } else if (comparacion < 0) {
        buscarPelicula(raiz->izquierda, nombreBuscar);
    } else {
        buscarPelicula(raiz->derecha, nombreBuscar);
    }
}

void mostrarPeliculasPorGenero(Pelicula* raiz, char *generoBuscar) {
    if (raiz != NULL) {
        mostrarPeliculasPorGenero(raiz->izquierda, generoBuscar);
        if (strcmp(raiz->genero, generoBuscar) == 0) {
            printf("Nombre: %s, Año: %d, Género: %s, Recaudación: %.6f millones\n",
                   raiz->nombre, raiz->anio, raiz->genero, raiz->recaudacion);
        }
        mostrarPeliculasPorGenero(raiz->derecha, generoBuscar);
    }
}

typedef struct {
    Pelicula* pelicula;
} PeliculaWrapper;

int compararRecaudacion(const void *a, const void *b) {
    double recaudacionA = (*(PeliculaWrapper*)a).pelicula->recaudacion;
    double recaudacionB = (*(PeliculaWrapper*)b).pelicula->recaudacion;
    if (recaudacionA < recaudacionB) return -1;
    if (recaudacionA > recaudacionB) return 1;
    return 0;
}

void recolectarPeliculas(Pelicula* raiz, PeliculaWrapper *array, int *indice) {
    if (raiz != NULL) {
        recolectarPeliculas(raiz->izquierda, array, indice);
        array[*indice].pelicula = raiz;
        (*indice)++;
        recolectarPeliculas(raiz->derecha, array, indice);
    }
}

void mostrarFracasosTaquilleros(Pelicula* raiz) {
    if (raiz == NULL) {
        printf("El árbol está vacío.\n");
        return;
    }
    int numPeliculas = 0;
    void contar(Pelicula* nodo) {
        if (nodo != NULL) {
            contar(nodo->izquierda);
            numPeliculas++;
            contar(nodo->derecha);
        }
    }
    contar(raiz);
    if (numPeliculas < 3) {
        printf("No hay suficientes películas para mostrar los 3 fracasos taquilleros.\n");
        return;
    }
    PeliculaWrapper *peliculasArray = (PeliculaWrapper*) malloc(numPeliculas * sizeof(PeliculaWrapper));
    if (peliculasArray == NULL) {
        perror("Error al asignar memoria para el array de películas");
        return;
    }
    int indice = 0;
    recolectarPeliculas(raiz, peliculasArray, &indice);
    qsort(peliculasArray, numPeliculas, sizeof(PeliculaWrapper), compararRecaudacion);
    printf("\nLos 3 fracasos taquilleros son:\n");
    for (int i = 0; i < fmin(3, numPeliculas); i++) {
        printf("%d. Nombre: %s, Año: %d, Género: %s, Recaudación: %.6f millones\n",
               i + 1, peliculasArray[i].pelicula->nombre, peliculasArray[i].pelicula->anio,
               peliculasArray[i].pelicula->genero, peliculasArray[i].pelicula->recaudacion);
    }
    free(peliculasArray);
}

void liberarArbol(Pelicula* raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izquierda);
        liberarArbol(raiz->derecha);
        free(raiz->nombre);
        free(raiz->genero);
        free(raiz);
    }
}

int main() {
    Pelicula* raiz = NULL;
    raiz = insertarPelicula(raiz, "El Padrino", 1972, "Drama", 246.131);
    raiz = insertarPelicula(raiz, "El Señor de los Anillos: La Comunidad del Anillo", 2001, "Fantasía", 897.690);
    raiz = insertarPelicula(raiz, "Pulp Fiction", 1994, "Crimen", 213.928);
    raiz = insertarPelicula(raiz, "Forrest Gump", 1994, "Drama", 677.945);
    raiz = insertarPelicula(raiz, "Titanic", 1997, "Romance", 2201.647);
    raiz = insertarPelicula(raiz, "Avatar", 2009, "Ciencia Ficción", 2923.706);
    raiz = insertarPelicula(raiz, "Interestelar", 2014, "Ciencia Ficción", 701.729);
    raiz = insertarPelicula(raiz, "La La Land", 2016, "Musical", 446.092);
    raiz = insertarPelicula(raiz, "Parasite", 2019, "Drama", 258.917);
    raiz = insertarPelicula(raiz, "Dune", 2021, "Ciencia Ficción", 400.671);
    raiz = insertarPelicula(raiz, "Oppenheimer", 2023, "Biográfico", 952.365);
    raiz = insertarPelicula(raiz, "Barbie", 2023, "Comedia", 1441.823);
    raiz = insertarPelicula(raiz, "El Silencio de los Inocentes", 1991, "Thriller", 272.742);
    raiz = insertarPelicula(raiz, "Matrix", 1999, "Ciencia Ficción", 467.650);
    raiz = insertarPelicula(raiz, "Gladiador", 2000, "Acción", 460.583);
    raiz = insertarPelicula(raiz, "Memento", 2000, "Misterio", 39.723);

    printf("Recorrido Inorden:\n");
    inorden(raiz);
    printf("\n");

    printf("Recorrido Preorden:\n");
    preorden(raiz);
    printf("\n");

    printf("Recorrido Posorden:\n");
    posorden(raiz);
    printf("\n");

    char nombreBuscar[100];
    printf("Ingrese el nombre de la película a buscar: ");
    fgets(nombreBuscar, sizeof(nombreBuscar), stdin);
    nombreBuscar[strcspn(nombreBuscar, "\n")] = 0;
    buscarPelicula(raiz, nombreBuscar);
    printf("\n");

    char generoBuscar[50];
    printf("Ingrese el género de las películas a mostrar: ");
    fgets(generoBuscar, sizeof(generoBuscar), stdin);
    generoBuscar[strcspn(generoBuscar, "\n")] = 0;
    printf("Películas del género '%s':\n", generoBuscar);
    mostrarPeliculasPorGenero(raiz, generoBuscar);
    printf("\n");

    mostrarFracasosTaquilleros(raiz);

    liberarArbol(raiz);

    return 0;
}