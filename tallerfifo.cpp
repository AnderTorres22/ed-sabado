//ANDER JHOVANY TORRES
//JOSSIMAR DAVID MORENO HURTADO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    int codigo;
    char nombre[50];
    int cantidad;
    int precio;
    struct Nodo* siguiente;
} Nodo;

Nodo* frente = NULL;
Nodo* fin = NULL;

void agregarProducto(int codigo, char nombre[], int cantidad, int precio) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (!nuevo) {
        printf("Error al asignar memoria.\n");
        return;
    }
    nuevo->codigo = codigo;
    strcpy(nuevo->nombre, nombre);
    nuevo->cantidad = cantidad;
    nuevo->precio = precio;
    nuevo->siguiente = NULL;
    
    if (fin == NULL) {
        frente = fin = nuevo;
    } else {
        fin->siguiente = nuevo;
        fin = nuevo;
    }
    printf("Producto agregado: Codigo: %d, Nombre: %s, Cantidad: %d, Precio: %d\n", codigo, nombre, cantidad, precio);
}

void venderProducto(int codigo, int cantidad) {
    Nodo* actual = frente;
    Nodo* anterior = NULL;
    while (actual != NULL) {
        if (actual->codigo == codigo) {
            if (actual->cantidad >= cantidad) {
                actual->cantidad -= cantidad;
                printf("Venta registrada. %d unidades restantes de %s.\n", actual->cantidad, actual->nombre);
                if (actual->cantidad == 0) {
                    printf("Producto agotado: %s. Eliminando del inventario.\n", actual->nombre);
                    if (anterior == NULL) {
                        frente = actual->siguiente;
                    } else {
                        anterior->siguiente = actual->siguiente;
                    }
                    if (actual == fin) {
                        fin = anterior;
                    }
                    free(actual);
                }
            } else {
                printf("No hay suficiente stock para la venta.\n");
            }
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Producto no encontrado.\n");
}

int calcularCostoTotal() {
    int total = 0;
    Nodo* actual = frente;
    while (actual != NULL) {
        total += actual->cantidad * actual->precio;
        actual = actual->siguiente;
    }
    return total;
}

void mostrarInventario() {
    Nodo* actual = frente;
    printf("\nInventario actual:\n");
    while (actual != NULL) {
        printf("Codigo: %d, Nombre: %s - Cantidad: %d, Precio: %d, Costo total: %d\n", actual->codigo, actual->nombre, actual->cantidad, actual->precio, actual->cantidad * actual->precio);
        actual = actual->siguiente;
    }
}

void liberarInventario() {
    Nodo* actual = frente;
    while (actual != NULL) {
        Nodo* temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
}

int main() {
    agregarProducto(101, "Manzanas", 50, 5);
    agregarProducto(102, "Naranjas", 30, 7);
    agregarProducto(103, "Platanos", 20, 3);
    
    mostrarInventario();
    
    venderProducto(101, 40); // Simular una venta
    
    mostrarInventario();
    
    printf("Costo total del inventario: %d\n", calcularCostoTotal());
    
    liberarInventario();
    return 0;
}
