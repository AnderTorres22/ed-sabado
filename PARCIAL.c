#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estructura para representar un jugador
typedef struct Jugador {
    int id;
    char nombre[30];
    int edad;
    char genero; // 'M' o 'F'
    char deporte[50];
    struct Jugador *siguiente;
} Jugador;

// Estructuras para la cola (FIFO) y pila (LIFO)
typedef struct {
    Jugador *frente;
    Jugador *final;
    int cantidad;
} Cola;

typedef struct {
    Jugador *tope;
    int cantidad;
} Pila;

// Prototipos de funciones
void inicializarCola(Cola *cola);
void inicializarPila(Pila *pila);
bool estaColaVacia(Cola *cola);
bool estaPilaVacia(Pila *pila);
void agregarJugadorACola(Cola *cola, Pila *historial);
void mostrarJugadoresEnEspera(Cola *cola);
int contarJugadoresEnEspera(Cola *cola);
void permitirParticipacion(Cola *cola, Pila *historial);
void mostrarHistorialParticipacion(Pila *historial);
void deshacerUltimaParticipacion(Cola *cola, Pila *historial);
int diferenciaGeneros(Cola *cola);
void liberarMemoria(Cola *cola, Pila *historial);

// Función principal
int main() {
    Cola colaEspera;
    Pila historial;
    inicializarCola(&colaEspera);
    inicializarPila(&historial);
    
    int opcion;
    do {
        printf("\n--- MENU DE GESTION DE TORNEO ---\n");
        printf("1. Agregar jugador a la cola\n");
        printf("2. Mostrar jugadores en espera\n");
        printf("3. Contar jugadores en espera\n");
        printf("4. Permitir participacion del siguiente jugador\n");
        printf("5. Mostrar historial de participacion\n");
        printf("6. Deshacer ultima participacion\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1:
                agregarJugadorACola(&colaEspera, &historial);
                break;
            case 2:
                mostrarJugadoresEnEspera(&colaEspera);
                break;
            case 3:
                printf("Jugadores en espera: %d\n", contarJugadoresEnEspera(&colaEspera));
                break;
            case 4:
                permitirParticipacion(&colaEspera, &historial);
                break;
            case 5:
                mostrarHistorialParticipacion(&historial);
                break;
            case 6:
                deshacerUltimaParticipacion(&colaEspera, &historial);
                break;
            case 7:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while(opcion != 7);
    
    liberarMemoria(&colaEspera, &historial);
    return 0;
}

// Implementación de funciones

void inicializarCola(Cola *cola) {
    cola->frente = NULL;
    cola->final = NULL;
    cola->cantidad = 0;
}

void inicializarPila(Pila *pila) {
    pila->tope = NULL;
    pila->cantidad = 0;
}

bool estaColaVacia(Cola *cola) {
    return cola->frente == NULL;
}

bool estaPilaVacia(Pila *pila) {
    return pila->tope == NULL;
}

void agregarJugadorACola(Cola *cola, Pila *historial) {
    // Verificar diferencia de géneros
    int diferencia = diferenciaGeneros(cola);
    if (abs(diferencia) > 4) {
        printf("No se pueden agregar mas jugadores. Diferencia de generos: %d (max 4 permitido).\n", abs(diferencia));
        return;
    }
    
    Jugador *nuevo = (Jugador*)malloc(sizeof(Jugador));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }
    
    printf("\nIngrese los datos del jugador:\n");
    printf("ID: ");
    scanf("%d", &nuevo->id);
    printf("Nombre: ");
    scanf(" %[^\n]", nuevo->nombre);
    printf("Edad: ");
    scanf("%d", &nuevo->edad);
    printf("Genero (M/F): ");
    scanf(" %c", &nuevo->genero);
    printf("Deporte: ");
    scanf(" %[^\n]", nuevo->deporte);
    
    nuevo->siguiente = NULL;
    
    if (estaColaVacia(cola)) {
        cola->frente = nuevo;
    } else {
        cola->final->siguiente = nuevo;
    }
    cola->final = nuevo;
    cola->cantidad++;
    
    printf("Jugador agregado a la cola de espera.\n");
}

void mostrarJugadoresEnEspera(Cola *cola) {
    if (estaColaVacia(cola)) {
        printf("No hay jugadores en espera.\n");
        return;
    }
    
    printf("\n--- JUGADORES EN ESPERA ---\n");
    Jugador *actual = cola->frente;
    while (actual != NULL) {
        printf("ID: %d, Nombre: %s, Edad: %d, Genero: %c, Deporte: %s\n", 
               actual->id, actual->nombre, actual->edad, actual->genero, actual->deporte);
        actual = actual->siguiente;
    }
}

int contarJugadoresEnEspera(Cola *cola) {
    return cola->cantidad;
}

void permitirParticipacion(Cola *cola, Pila *historial) {
    if (estaColaVacia(cola)) {
        printf("No hay jugadores en espera.\n");
        return;
    }
    
    Jugador *participante = cola->frente;
    
    // Validar edad
    if (participante->edad < 15) {
        printf("El jugador %s no puede participar por ser menor de 15 años.\n", participante->nombre);
        return;
    }
    
    // Quitar de la cola
    cola->frente = cola->frente->siguiente;
    if (cola->frente == NULL) {
        cola->final = NULL;
    }
    cola->cantidad--;
    
    // Agregar al historial (pila)
    participante->siguiente = historial->tope;
    historial->tope = participante;
    historial->cantidad++;
    
    printf("Jugador %s ha participado en el torneo.\n", participante->nombre);
}

void mostrarHistorialParticipacion(Pila *historial) {
    if (estaPilaVacia(historial)) {
        printf("No hay historial de participacion.\n");
        return;
    }
    
    printf("\n--- HISTORIAL DE PARTICIPACION (ultimo al primero) ---\n");
    Jugador *actual = historial->tope;
    while (actual != NULL) {
        printf("ID: %d, Nombre: %s, Edad: %d, Genero: %c, Deporte: %s\n", 
               actual->id, actual->nombre, actual->edad, actual->genero, actual->deporte);
        actual = actual->siguiente;
    }
}

void deshacerUltimaParticipacion(Cola *cola, Pila *historial) {
    if (estaPilaVacia(historial)) {
        printf("No hay participaciones para deshacer.\n");
        return;
    }
    
    Jugador *jugador = historial->tope;
    historial->tope = historial->tope->siguiente;
    historial->cantidad--;
    
    // Agregar al final de la cola
    jugador->siguiente = NULL;
    if (estaColaVacia(cola)) {
        cola->frente = jugador;
    } else {
        cola->final->siguiente = jugador;
    }
    cola->final = jugador;
    cola->cantidad++;
    
    printf("Se ha deshecho la ultima participacion. %s vuelve a la cola de espera.\n", jugador->nombre);
}

int diferenciaGeneros(Cola *cola) {
    int hombres = 0, mujeres = 0;
    Jugador *actual = cola->frente;
    
    while (actual != NULL) {
        if (actual->genero == 'M' || actual->genero == 'm') {
            hombres++;
        } else if (actual->genero == 'F' || actual->genero == 'f') {
            mujeres++;
        }
        actual = actual->siguiente;
    }
    
    return hombres - mujeres;
}

void liberarMemoria(Cola *cola, Pila *historial) {
    // Liberar memoria de la cola
    while (!estaColaVacia(cola)) {
        Jugador *temp = cola->frente;
        cola->frente = cola->frente->siguiente;
        free(temp);
    }
    
    // Liberar memoria de la pila
    while (!estaPilaVacia(historial)) {
        Jugador *temp = historial->tope;
        historial->tope = historial->tope->siguiente;
        free(temp);
    }
}