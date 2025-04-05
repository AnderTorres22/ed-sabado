#include <iostream>

using namespace std;


void verificarEdad() {
    int edad;
    cout << "Ingrese su edad: ";
    cin >> edad;
    
    if (edad >= 18) {
        cout << "Acceso permitido. Bienvenido!" << endl;
    } else {
        cout << "Acceso denegado. Debes ser mayor de 18 años." << endl;
    }
}

int main() {
    verificarEdad();
    return 0;
}




