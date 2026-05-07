#pragma once
#include <iostream>
#include <string>
#include "Estructuras.h"
#include "Entidades.h"
using namespace std;

class Sistema
{
public:
    Sistema() {}
    ~Sistema() {}

    void iniciar() {
        mostrarBienvenida();
        int opcion;
        do {
            mostrarMenuPrincipal();
            cin >> opcion;
            cin.ignore();
            switch (opcion) {
            case 1: flujoRegistro();     break;
            case 2: flujoInicioSesion(); break;
            case 3: cout << "\nHasta luego!\n"; break;
            default: cout << "\nOpcion invalida.\n";
            }
        } while (opcion != 3);
    }

private:

    void mostrarBienvenida() {
        cout << "========================================\n";
        cout << "        Bienvenido a NetflixApp         \n";
        cout << "========================================\n";
    }

    void mostrarMenuPrincipal() {
        cout << "\n--- Menu Principal ---\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar sesion\n";
        cout << "3. Salir\n";
        cout << "Opcion: ";
    }
    void flujoRegistro() {
        cout << "\n--- Registro de usuario ---\n";
        string nombre, email, password;
        cout << "Nombre de usuario : "; getline(cin, nombre);
        cout << "Email             : "; getline(cin, email);
        cout << "Contrasena        : "; getline(cin, password);

        cout << "\nSelecciona un metodo de pago:\n";
        cout << "1. Tarjeta de credito\n";
        cout << "2. Tarjeta de debito\n";
        cout << "3. PayPal\n";
        cout << "Opcion: ";
        int tipoPago; cin >> tipoPago; cin.ignore();

        string numTarjeta, titular;
        cout << "Numero de tarjeta/cuenta : "; getline(cin, numTarjeta);
        cout << "Titular                  : "; getline(cin, titular);
        cout << "\nUsuario registrado exitosamente. Ya puedes iniciar sesion.\n";

        string tipoPlan = (tipoPago == 3) ? "Premiun" : "Basico";
        float precio = (tipoPago == 3) ? 45.90 : 24.90;

        Suscripcion* nuevaSub = new Suscripcion(101, tipoPlan, precio, "2026-05-07");
        cout<<"\nSuscripcion " << nuevaSub->getTipo() << "activada correctamente." <<endl;

        Factura nuevaFactura("F001", "2026-05-07", precio, titular);
        nuevaFactura.guardarEnArchivo();
    }
    void flujoInicioSesion() {
        cout << "\n--- Inicio de sesion ---\n";

        string email, password;
        cout << "Email     : "; getline(cin, email);
        cout << "Contrasena: "; getline(cin, password);
        bool autenticado = true; 

        if (autenticado) {
            cout << "\nBienvenido, " << email << "!\n";
            menuUsuario(email);
        } else {
            cout << "\nCredenciales incorrectas.\n";
        }
    }

    void menuUsuario(const string& usuario) {
        int opcion;
        do {
            cout << "\n--- Menu de usuario ---\n";
            cout << "1. Explorar series\n";
            cout << "2. Buscar serie\n";
            cout << "3. Mi perfil\n";
            cout << "4. Cerrar sesion\n";
            cout << "Opcion: ";
            cin >> opcion; cin.ignore();

            switch (opcion) {
            case 1: explorarSeries();      break;
            case 2: buscarSerie();         break;
            case 3: menuPerfil(usuario);   break;
            case 4: cout << "\nSesion cerrada.\n"; break;
            case 5: gestionarMiLista(usuario); break;
            default: cout << "\nOpcion invalida.\n";
            }
        } while (opcion != 4);
    }

    void gestionarMiLista(const string& usuario){
    cout<<"\n----- Gestionar Mi Lista de Favoritos ----------"<<endl;
    cout<< " 1. Ver mi Lista ordenada (Alfabeticamente)\n";
    cout<< " 2. Filtrar por genero (Terror)\n";
    cout<< "Opcion: ";
    int op; cin>>op; cin.ignore();

    if (op==1){
        miPerfilActual->getLista()->ordenarPorTitulo();
    }
    else if (op==2){
        miPerfilActual->getLista()->filtrar([](Contenido* c){
            return c->getGenero() == "Terror";
        });
    }
    }

    void explorarSeries() {
        cout << "\n--- Series disponibles ---\n";
        cout << "[1] Stranger Things   | Ciencia Ficcion | Calif: 4.5\n";
        cout << "[2] Breaking Bad      | Drama           | Calif: 5.0\n";
        cout << "[3] The Crown         | Historia        | Calif: 3.0\n";
        cout << "[4] Dark              | Ciencia Ficcion | Calif: 5.0\n";
        cout << "[5] Squid Game        | Thriller        | Calif: 4.0\n";
        cout << "\nIngresa el numero de una serie para ver detalle (0 para volver): ";
        int sel; cin >> sel; cin.ignore();
        if (sel >= 1 && sel <= 5) detalleSerie(sel);
    }

    void detalleSerie(int id) {
        cout << "\n--- Detalle de serie ---\n";
        cout << "Titulo      : Stranger Things (ejemplo)\n";
        cout << "Genero      : Ciencia Ficcion\n";
        cout << "Descripcion : Un grupo de ninos enfrenta fuerzas sobrenaturales.\n";
        cout << "Temporadas  : 3\n";
        cout << "Calificacion: 4.5 / 5\n";

        cout << "\n1. Ver temporadas\n2. Calificar serie\n0. Volver\nOpcion: ";
        int op; cin >> op; cin.ignore();
        if (op == 1) verTemporadas(id);
        if (op == 2) calificarSerie(id);
    }

    void verTemporadas(int idSerie) {   
        cout << "\n--- Temporadas ---\n";
        cout << "T1 (2016) - 8 episodios\n";
        cout << "T2 (2017) - 9 episodios\n";
        cout << "T3 (2019) - 8 episodios\n";
    }

    void calificarSerie(int idSerie) {
        cout << "\nPuntaje (1-5): ";
        int puntaje; cin >> puntaje; cin.ignore();
        cout << "Comentario  : "; string comentario; getline(cin, comentario);
        cout << "Calificacion registrada.\n";
    }

    void buscarSerie() {
        cout << "\n--- Buscar serie ---\n";
        cout << "Titulo: "; string titulo; getline(cin, titulo);
        cout << "Resultados para \"" << titulo << "\":\n";
        cout << "(sin resultados - pendiente de implementar busqueda)\n";
    }
    void menuPerfil(const string& usuario) {
        int opcion;
        do {
            cout << "\n--- Mi perfil (" << usuario << ") ---\n";
            cout << "1. Ver metodos de pago\n";
            cout << "2. Agregar metodo de pago\n";
            cout << "3. Ver historial de facturas\n";
            cout << "4. Volver\n";
            cout << "Opcion: ";
            cin >> opcion; cin.ignore();

            switch (opcion) {
            case 1: verMetodosPago();      break;
            case 2: agregarMetodoPago();   break;
            case 3: verFacturas();         break;
            case 4: break;
            default: cout << "\nOpcion invalida.\n";
            }
        } while (opcion != 4);
    }

    void verMetodosPago() {
        cout << "\n--- Metodos de pago ---\n";
        cout << "[1] Credito  | **** **** **** 1111 | Carlos Ordaz\n";
        cout << "[2] Debito   | **** **** **** 2222 | Carlos Ordaz\n";
    }

    void agregarMetodoPago() {
        cout << "\n--- Agregar metodo de pago ---\n";
        cout << "Tipo (Credito/Debito/PayPal): "; string tipo; getline(cin, tipo);
        cout << "Numero                      : "; string numero; getline(cin, numero);
        cout << "Titular                     : "; string titular; getline(cin, titular);
        cout << "Metodo de pago agregado.\n";
    }

    void verFacturas() {
        cout << "\n--- Historial de facturas ---\n";
        issftream archivo("datos/facturas.txt");
        string linea;
        if(archivo.is_open()){
            cout << "ID | Monto   |  Fecha      | Metodo\n";
            while (getline(archivo, linea)){
                cout<< linea <<endl;
            }
            archivo.close();
        }else{
            cout<<"No hay facturas registradas en el sistema."<<endl;
    }
};
