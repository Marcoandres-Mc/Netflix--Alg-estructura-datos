#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Estructuras.h"
#include "Entidades.h"
using namespace std;

class Sistema
{
public:
    Sistema()
        : usuarioActual(nullptr), perfilActual(nullptr),
          contadorFactura(1), contadorMetodo(1)
    {
        catalogoSeries     = new Lista<Serie*>();
        catalogoPeliculas  = new Lista<Pelicula*>();
        categorias         = new Lista<Categoria*>();
        metodosPago        = new Lista<MetodoPago*>();
        facturas           = new Lista<Factura*>();
        historialBusquedas = new Pila<string>();
        colaReproduccion   = new Cola<Contenido*>();
    }

    ~Sistema() {
        Nodo<Serie*>*    s = catalogoSeries->getCabeza();
        while (s) { delete s->dato; s = s->next; }
        Nodo<Pelicula*>* p = catalogoPeliculas->getCabeza();
        while (p) { delete p->dato; p = p->next; }
        Nodo<Categoria*>* c = categorias->getCabeza();
        while (c) { delete c->dato; c = c->next; }
        Nodo<MetodoPago*>* m = metodosPago->getCabeza();
        while (m) { delete m->dato; m = m->next; }
        Nodo<Factura*>* f = facturas->getCabeza();
        while (f) { delete f->dato; f = f->next; }

        delete catalogoSeries;
        delete catalogoPeliculas;
        delete categorias;
        delete metodosPago;
        delete facturas;
        delete historialBusquedas;
        delete colaReproduccion;
        if (usuarioActual) delete usuarioActual;
    }

    void iniciar() {
        cargarCatalogo();
        mostrarBienvenida();
        int opcion;
        do {
            mostrarMenuPrincipal();
            cin >> opcion; cin.ignore();
            switch (opcion) {
                case 1: flujoRegistro();     break;
                case 2: flujoInicioSesion(); break;
                case 3: linea('-'); cout << "  Hasta luego!\n"; linea('-'); break;
                default: cout << "  Opcion invalida.\n";
            }
        } while (opcion != 3);
    }

private:
    Usuario*           usuarioActual;
    Perfil*            perfilActual;
    Lista<Serie*>*     catalogoSeries;
    Lista<Pelicula*>*  catalogoPeliculas;
    Lista<Categoria*>* categorias;
    Lista<MetodoPago*>* metodosPago;
    Lista<Factura*>*    facturas;
    int contadorFactura;
    int contadorMetodo;
    Pila<string>*       historialBusquedas;
    Cola<Contenido*>*   colaReproduccion;

    void linea(char c = '=', int n = 52) const {
        for (int i = 0; i < n; i++) cout << c;
        cout << "\n";
    }
    void titulo(const string& t) const {
        linea('=');
        int pad = (52 - (int)t.size()) / 2;
        cout << string(pad, ' ') << t << "\n";
        linea('=');
    }
    void pausa() const {
        cout << "\n  Presiona Enter para continuar...";
        cin.ignore();
    }

    string idFactura() {
        string num = to_string(contadorFactura++);
        while (num.size() < 3) num = "0" + num;
        return "F" + num;
    }
    string fechaHoy() const { return "2026-05-08"; }

    void cargarCatalogo() {
        categorias->insertarFinal(new Categoria(1, "Accion",       "Peliculas y series de accion"));
        categorias->insertarFinal(new Categoria(2, "Drama",        "Historias dramaticas"));
        categorias->insertarFinal(new Categoria(3, "Sci-Fi",       "Ciencia ficcion"));
        categorias->insertarFinal(new Categoria(4, "Terror",       "Horror y suspenso"));
        categorias->insertarFinal(new Categoria(5, "Comedia",      "Humor y entretenimiento"));
        categorias->insertarFinal(new Categoria(6, "Documental",   "Contenido educativo"));
        categorias->insertarFinal(new Categoria(7, "Thriller",     "Suspenso y tension"));

        Categoria* sci  = categorias->obtenerElemento(2);
        Categoria* dra  = categorias->obtenerElemento(1);
        Categoria* thr  = categorias->obtenerElemento(6);
        Categoria* acc  = categorias->obtenerElemento(0);
        Categoria* com  = categorias->obtenerElemento(4);

        Serie* s1 = new Serie(1, "Stranger Things", "Sci-Fi",
            "Un grupo de ninos enfrenta fuerzas sobrenaturales en Hawkins.", "+13", sci);
        s1->agregarTemporada(new Temporada(1, 2016));
        s1->agregarTemporada(new Temporada(2, 2017));
        s1->agregarTemporada(new Temporada(3, 2019));
        s1->agregarCalificacion(new Calificacion(5, "Obra maestra", "user1"));
        s1->agregarCalificacion(new Calificacion(4, "Muy buena",    "user2"));
        catalogoSeries->insertarFinal(s1);

        Serie* s2 = new Serie(2, "Breaking Bad", "Drama",
            "Un profesor de quimica se convierte en fabricante de drogas.", "+16", dra);
        s2->agregarTemporada(new Temporada(1, 2008));
        s2->agregarTemporada(new Temporada(2, 2009));
        s2->agregarTemporada(new Temporada(3, 2010));
        s2->agregarTemporada(new Temporada(4, 2011));
        s2->agregarTemporada(new Temporada(5, 2012));
        s2->agregarCalificacion(new Calificacion(5, "La mejor serie", "user3"));
        catalogoSeries->insertarFinal(s2);

        Serie* s3 = new Serie(3, "Dark", "Sci-Fi",
            "Viajes en el tiempo y conspiraciones en un pueblo aleman.", "+14", sci);
        s3->agregarTemporada(new Temporada(1, 2017));
        s3->agregarTemporada(new Temporada(2, 2019));
        s3->agregarTemporada(new Temporada(3, 2020));
        s3->agregarCalificacion(new Calificacion(5, "Increible",    "user4"));
        s3->agregarCalificacion(new Calificacion(5, "Unica",        "user5"));
        catalogoSeries->insertarFinal(s3);

        Serie* s4 = new Serie(4, "Squid Game", "Thriller",
            "Personas endeudadas compiten en juegos mortales por dinero.", "+16", thr);
        s4->agregarTemporada(new Temporada(1, 2021));
        s4->agregarTemporada(new Temporada(2, 2024));
        s4->agregarCalificacion(new Calificacion(4, "Muy intensa", "user6"));
        catalogoSeries->insertarFinal(s4);

        Serie* s5 = new Serie(5, "The Crown", "Drama",
            "La historia de la familia real britanica.", "+7", dra);
        s5->agregarTemporada(new Temporada(1, 2016));
        s5->agregarTemporada(new Temporada(2, 2017));
        s5->agregarCalificacion(new Calificacion(3, "Interesante", "user7"));
        catalogoSeries->insertarFinal(s5);

        Serie* s6 = new Serie(6, "Money Heist", "Thriller",
            "Un genial ladron planea el robo perfecto a la Casa de la Moneda.", "+14", thr);
        s6->agregarTemporada(new Temporada(1, 2017));
        s6->agregarTemporada(new Temporada(2, 2018));
        s6->agregarTemporada(new Temporada(3, 2019));
        s6->agregarCalificacion(new Calificacion(5, "Adictiva", "user8"));
        s6->agregarCalificacion(new Calificacion(4, "Buenisima", "user9"));
        catalogoSeries->insertarFinal(s6);

        catalogoPeliculas->insertarFinal(new Pelicula(
            101, "Inception", "Sci-Fi", 2010, 148, "+13", sci,
            "Christopher Nolan", "Warner Bros", "Ingles", "EE.UU."));
        catalogoPeliculas->insertarFinal(new Pelicula(
            102, "Interstellar", "Sci-Fi", 2014, 169, "+7", sci,
            "Christopher Nolan", "Paramount", "Ingles", "EE.UU."));
        catalogoPeliculas->insertarFinal(new Pelicula(
            103, "The Godfather", "Drama", 1972, 175, "+16", dra,
            "Francis Ford Coppola", "Paramount", "Ingles", "EE.UU."));
        catalogoPeliculas->insertarFinal(new Pelicula(
            104, "Parasite", "Thriller", 2019, 132, "+14", thr,
            "Bong Joon-ho", "CJ Entertainment", "Coreano", "Corea del Sur"));
        catalogoPeliculas->insertarFinal(new Pelicula(
            105, "Mad Max: Fury Road", "Accion", 2015, 120, "+14", acc,
            "George Miller", "Warner Bros", "Ingles", "Australia"));
        catalogoPeliculas->insertarFinal(new Pelicula(
            106, "The Grand Budapest Hotel", "Comedia", 2014, 99, "ATP", com,
            "Wes Anderson", "Fox Searchlight", "Ingles", "EE.UU."));
    }

    void cargarMetodosPago() {
        ifstream f("metodos_pago.txt");
        if (!f.is_open()) return;
        string linea;
        while (getline(f, linea)) {
            istringstream ss(linea);
            string sid, tipo, numero, titular, email;
            if (!getline(ss, sid,     '|')) continue;
            if (!getline(ss, tipo,    '|')) continue;
            if (!getline(ss, numero,  '|')) continue;
            if (!getline(ss, titular, '|')) continue;
            if (!getline(ss, email,   '|')) continue;
            if (email == usuarioActual->getEmail()) {
                metodosPago->insertarFinal(
                    new MetodoPago(stoi(sid), tipo, numero, titular));
                if (stoi(sid) >= contadorMetodo) contadorMetodo = stoi(sid) + 1;
            }
        }
        f.close();
    }

    void cargarFacturas() {
        ifstream f("facturas.txt");
        if (!f.is_open()) return;
        string linea;
        while (getline(f, linea)) {
            istringstream ss(linea);
            string sid, fecha, monto, metodo;
            if (!getline(ss, sid,   '|')) continue;
            if (!getline(ss, fecha, '|')) continue;
            if (!getline(ss, monto, '|')) continue;
            if (!getline(ss, metodo,'|')) continue;
            facturas->insertarFinal(new Factura(sid, fecha, stof(monto), metodo));
            if (sid.size() > 1) {
                int n = stoi(sid.substr(1));
                if (n >= contadorFactura) contadorFactura = n + 1;
            }
        }
        f.close();
    }

    void limpiarMetodosPago() {
        Nodo<MetodoPago*>* m = metodosPago->getCabeza();
        while (m) { delete m->dato; m = m->next; }
        metodosPago->vaciar();
    }
    void limpiarFacturas() {
        Nodo<Factura*>* f = facturas->getCabeza();
        while (f) { delete f->dato; f = f->next; }
        facturas->vaciar();
    }

    void mostrarBienvenida() const {
        titulo("  NETFLIX - Sistema de Streaming  ");
        cout << "  Bienvenido a la plataforma de entretenimiento.\n";
        linea('-');
    }

    void mostrarMenuPrincipal() const {
        linea('-');
        cout << "  MENU PRINCIPAL\n";
        linea('-');
        cout << "  1. Registrarse\n";
        cout << "  2. Iniciar sesion\n";
        cout << "  3. Salir\n";
        linea('-');
        cout << "  Opcion: ";
    }

    void flujoRegistro() {
        titulo("        REGISTRO DE USUARIO       ");
        string nombre, email, password;
        cout << "  Nombre       : "; getline(cin, nombre);
        cout << "  Email        : "; getline(cin, email);
        cout << "  Contrasena   : "; getline(cin, password);

        if (Usuario::existeUsuario(email)) {
            cout << "\n  El email ya esta registrado. Usa otro.\n";
            pausa(); return;
        }

        cout << "\n  Selecciona plan de suscripcion:\n";
        cout << "  1. Basico   - S/. 24.90/mes\n";
        cout << "  2. Estandar - S/. 35.90/mes\n";
        cout << "  3. Premium  - S/. 45.90/mes\n";
        linea('-');
        cout << "  Opcion: ";
        int plan; cin >> plan; cin.ignore();

        string tipoPlan; float precio;
        switch (plan) {
            case 1:  tipoPlan = "Basico";    precio = 24.90f; break;
            case 2:  tipoPlan = "Estandar";  precio = 35.90f; break;
            default: tipoPlan = "Premium";   precio = 45.90f;
        }

        Usuario* nuevoU = new Usuario(email, password);
        nuevoU->guardarEnArchivo();

        Suscripcion* sub = new Suscripcion(1, tipoPlan, precio, fechaHoy());
        sub->guardarEnArchivo(email);
        nuevoU->setSuscripcion(sub);

        Perfil* p = new Perfil(1, nombre, false);
        p->guardarEnArchivo(email);
        nuevoU->anadirPerfil(p);

        Factura* fac = new Factura(idFactura(), fechaHoy(), precio, "Pendiente");
        fac->guardarEnArchivo();
        cout << "\n  Registro exitoso! Factura generada:\n";
        fac->mostrar();
        delete fac;
        delete nuevoU;

        cout << "\n  Ya puedes iniciar sesion.\n";
        pausa();
    }

    void flujoInicioSesion() {
        titulo("       INICIO DE SESION          ");
        string email, password;
        cout << "  Email      : "; getline(cin, email);
        cout << "  Contrasena : "; getline(cin, password);

        if (!Usuario::validarCredenciales(email, password)) {
            cout << "\n  Credenciales incorrectas.\n";
            pausa(); return;
        }

        if (usuarioActual) { delete usuarioActual; usuarioActual = nullptr; }
        usuarioActual = new Usuario(email, password);

        {
            ifstream f("suscripciones.txt");
            string linea;
            while (getline(f, linea)) {
                istringstream ss(linea);
                string sid, tipo, precioS, fecha, em;
                if (!getline(ss, sid,    '|')) continue;
                if (!getline(ss, tipo,   '|')) continue;
                if (!getline(ss, precioS,'|')) continue;
                if (!getline(ss, fecha,  '|')) continue;
                if (!getline(ss, em,     '|')) continue;
                if (em == email) {
                    usuarioActual->setSuscripcion(
                        new Suscripcion(stoi(sid), tipo, stof(precioS), fecha));
                    break;
                }
            }
        }

        perfilActual = nullptr;
        {
            ifstream f("perfiles.txt");
            string linea;
            while (getline(f, linea)) {
                istringstream ss(linea);
                string sid, nom, infantilS, em;
                if (!getline(ss, sid,      '|')) continue;
                if (!getline(ss, nom,      '|')) continue;
                if (!getline(ss, infantilS,'|')) continue;
                if (!getline(ss, em,       '|')) continue;
                if (em == email) {
                    Perfil* p = new Perfil(stoi(sid), nom, infantilS == "1");
                    usuarioActual->anadirPerfil(p);
                    if (!perfilActual) perfilActual = p;
                    break;
                }
            }
        }

        if (!perfilActual) {
            perfilActual = new Perfil(1, email, false);
            usuarioActual->anadirPerfil(perfilActual);
        }

        limpiarMetodosPago(); limpiarFacturas();
        cargarMetodosPago();
        cargarFacturas();

        cout << "\n  Bienvenido/a, " << email << "!\n";
        pausa();
        menuUsuario();

        delete usuarioActual; usuarioActual = nullptr;
        perfilActual = nullptr;
        limpiarMetodosPago(); limpiarFacturas();
        delete historialBusquedas; historialBusquedas = new Pila<string>();
        delete colaReproduccion;   colaReproduccion   = new Cola<Contenido*>();
    }

    void menuUsuario() {
        int op;
        do {
            titulo("        MENU PRINCIPAL          ");
            cout << "  Usuario: " << usuarioActual->getEmail() << "\n";
            cout << "  Perfil : " << perfilActual->getNombre() << "\n";
            linea('-');
            cout << "  1. Explorar Series\n";
            cout << "  2. Explorar Peliculas\n";
            cout << "  3. Buscar contenido\n";
            cout << "  4. Mi Lista de Favoritos\n";
            cout << "  5. Cola de Reproduccion\n";
            cout << "  6. Mi Perfil y Cuenta\n";
            cout << "  7. Cerrar sesion\n";
            linea('-');
            cout << "  Opcion: ";
            cin >> op; cin.ignore();
            switch (op) {
                case 1: menuSeries();      break;
                case 2: menuPeliculas();   break;
                case 3: menuBuscar();      break;
                case 4: menuMiLista();     break;
                case 5: menuCola();        break;
                case 6: menuCuenta();      break;
                case 7: cout << "\n  Sesion cerrada correctamente.\n"; break;
                default: cout << "  Opcion invalida.\n";
            }
        } while (op != 7);
    }

    void menuSeries() {
        do {
            titulo("         SERIES               ");
            int idx = 1;
            Nodo<Serie*>* nodo = catalogoSeries->getCabeza();
            while (nodo) {
                cout << "  " << idx++ << ". ";
                nodo->dato->mostrarResumen();
                nodo = nodo->next;
            }
            linea('-');
            cout << "  1-" << catalogoSeries->tamanio() << ". Ver detalle\n";
            cout << "  F. Filtrar por genero\n";
            cout << "  O. Ordenar por calificacion\n";
            cout << "  0. Volver\n";
            linea('-');
            cout << "  Opcion: ";
            string entrada; getline(cin, entrada);

            if (entrada == "0") break;
            if (entrada == "F" || entrada == "f") filtrarSeriesPorGenero();
            else if (entrada == "O" || entrada == "o") ordenarSeriesPorCalif();
            else {
                try {
                    int sel = stoi(entrada) - 1;
                    if (sel >= 0 && sel < catalogoSeries->tamanio())
                        detalleSerie(catalogoSeries->obtenerElemento(sel));
                    else
                        cout << "  Opcion invalida.\n";
                } catch (...) { cout << "  Opcion invalida.\n"; }
            }
        } while (true);
    }

    void detalleSerie(Serie* s) {
        titulo("       DETALLE DE SERIE         ");
        s->mostrarDetalle();
        linea('-');
        cout << "  1. Agregar a Mi Lista\n";
        cout << "  2. Calificar serie\n";
        cout << "  3. Agregar a Cola de Reproduccion\n";
        cout << "  0. Volver\n";
        linea('-');
        cout << "  Opcion: ";
        int op; cin >> op; cin.ignore();
        switch (op) {
            case 1: agregarAMiLista(s);           break;
            case 2: calificarSerie(s);             break;
            case 3: agregarACola(s);               break;
        }
    }

    void calificarSerie(Serie* s) {
        cout << "  Puntaje (1-5): ";
        int puntaje; cin >> puntaje; cin.ignore();
        if (puntaje < 1 || puntaje > 5) { cout << "  Puntaje invalido.\n"; return; }
        cout << "  Comentario   : "; string comentario; getline(cin, comentario);
        s->agregarCalificacion(
            new Calificacion(puntaje, comentario, usuarioActual->getEmail()));
        cout << "  Calificacion registrada. Nuevo promedio: "
             << fixed << setprecision(1) << s->getCalificacionPromedio() << "/5\n";
        pausa();
    }

    void filtrarSeriesPorGenero() {
        cout << "  Genero a filtrar: "; string genero; getline(cin, genero);
        titulo("     SERIES FILTRADAS           ");
        bool encontrado = false;
        Nodo<Serie*>* nodo = catalogoSeries->getCabeza();
        auto mostrarSiGenero = [&](Serie* s) {
            if (s->getGenero() == genero) {
                s->mostrarResumen();
                encontrado = true;
            }
        };
        while (nodo) { mostrarSiGenero(nodo->dato); nodo = nodo->next; }
        if (!encontrado) cout << "  No hay series del genero '" << genero << "'.\n";
        pausa();
    }

    void ordenarSeriesPorCalif() {
        int n = catalogoSeries->tamanio();
        bool intercambiado;
        for (int i = 0; i < n - 1; i++) {
            intercambiado = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (catalogoSeries->obtenerElemento(j)->getCalificacionPromedio() <
                    catalogoSeries->obtenerElemento(j + 1)->getCalificacionPromedio()) {
                    Serie* tmp = catalogoSeries->obtenerElemento(j);
                    catalogoSeries->reemplazar(j,     catalogoSeries->obtenerElemento(j + 1));
                    catalogoSeries->reemplazar(j + 1, tmp);
                    intercambiado = true;
                }
            }
            if (!intercambiado) break;
        }
        cout << "  Series ordenadas por calificacion (algoritmo de intercambio).\n";
        pausa();
    }

    void menuPeliculas() {
        do {
            titulo("        PELICULAS              ");
            Nodo<Pelicula*>* nodo = catalogoPeliculas->getCabeza();
            while (nodo) {
                nodo->dato->mostrarResumen();
                nodo = nodo->next;
            }
            linea('-');
            cout << "  1-" << catalogoPeliculas->tamanio() << ". Ver detalle\n";
            cout << "  F. Filtrar por genero\n";
            cout << "  0. Volver\n";
            linea('-');
            cout << "  Opcion: ";
            string entrada; getline(cin, entrada);

            if (entrada == "0") break;
            if (entrada == "F" || entrada == "f") filtrarPeliculasPorGenero();
            else {
                try {
                    int sel = stoi(entrada) - 1;
                    if (sel >= 0 && sel < catalogoPeliculas->tamanio())
                        detallePelicula(catalogoPeliculas->obtenerElemento(sel));
                    else cout << "  Opcion invalida.\n";
                } catch (...) { cout << "  Opcion invalida.\n"; }
            }
        } while (true);
    }

    void detallePelicula(Pelicula* p) {
        titulo("      DETALLE DE PELICULA       ");
        p->mostrarDetalle();
        linea('-');
        cout << "  1. Agregar a Mi Lista\n";
        cout << "  2. Agregar a Cola de Reproduccion\n";
        cout << "  0. Volver\n";
        linea('-');
        cout << "  Opcion: ";
        int op; cin >> op; cin.ignore();
        if (op == 1) agregarAMiLista(p);
        if (op == 2) agregarACola(p);
    }

    void filtrarPeliculasPorGenero() {
        cout << "  Genero a filtrar: "; string genero; getline(cin, genero);
        titulo("    PELICULAS FILTRADAS          ");
        bool encontrado = false;
        Nodo<Pelicula*>* nodo = catalogoPeliculas->getCabeza();
        auto mostrarSiGenero = [&](Pelicula* p) {
            if (p->getGenero() == genero) {
                p->mostrarDetalle();
                linea('-');
                encontrado = true;
            }
        };
        while (nodo) { mostrarSiGenero(nodo->dato); nodo = nodo->next; }
        if (!encontrado) cout << "  No hay peliculas del genero '" << genero << "'.\n";
        pausa();
    }

    void agregarACola(Contenido* c) {
        colaReproduccion->encolar(c);
        cout << "  '" << c->getTitulo() << "' agregado a la cola de reproduccion.\n";
        pausa();
    }

    void mostrarHistorial() {
        titulo("  HISTORIAL DE BUSQUEDAS       ");
        if (historialBusquedas->estaVacia()) {
            cout << "  No hay busquedas recientes.\n";
            pausa(); return;
        }
        int i = 1;
        Nodo<string>* nodo = historialBusquedas->getTope();
        while (nodo) {
            cout << "  " << i++ << ". " << nodo->dato << "\n";
            nodo = nodo->next;
        }
        pausa();
    }

    void menuCola() {
        int op;
        do {
            titulo("    COLA DE REPRODUCCION       ");
            if (colaReproduccion->estaVacia()) {
                cout << "  La cola esta vacia.\n";
            } else {
                cout << "  Siguiente: " << colaReproduccion->frente()->getTitulo() << "\n";
                linea('-');
                int i = 1;
                Nodo<Contenido*>* nodo = colaReproduccion->getFrente();
                while (nodo) {
                    cout << "  " << i++ << ". " << nodo->dato->getTitulo() << "\n";
                    nodo = nodo->next;
                }
            }
            linea('-');
            cout << "  1. Reproducir siguiente\n";
            cout << "  0. Volver\n";
            linea('-');
            cout << "  Opcion: ";
            cin >> op; cin.ignore();
            if (op == 1) {
                if (!colaReproduccion->estaVacia()) {
                    cout << "\n  Reproduciendo: " << colaReproduccion->frente()->getTitulo() << "\n";
                    colaReproduccion->frente()->mostrarDetalle();
                    colaReproduccion->desencolar();
                    pausa();
                } else {
                    cout << "  No hay contenido en la cola.\n";
                    pausa();
                }
            }
        } while (op != 0);
    }

    void agregarAMiLista(Contenido* c) {
        if (perfilActual->buscarEnLista(0, c->getTitulo())) {
            cout << "  '" << c->getTitulo() << "' ya esta en tu lista.\n";
        } else {
            perfilActual->getLista()->agregarContenido(c);
            cout << "  '" << c->getTitulo() << "' agregado a tu lista.\n";
        }
        pausa();
    }

    Serie* buscarSerie(Nodo<Serie*>* nodo, const string& titulo) {
        if (nodo == nullptr) return nullptr;
        if (nodo->dato->getTitulo() == titulo) return nodo->dato;
        return buscarSerie(nodo->next, titulo);
    }

    Pelicula* buscarPelicula(Nodo<Pelicula*>* nodo, const string& titulo) {
        if (nodo == nullptr) return nullptr;
        if (nodo->dato->getTitulo() == titulo) return nodo->dato;
        return buscarPelicula(nodo->next, titulo);
    }

    void menuBuscar() {
        titulo("     BUSCAR CONTENIDO           ");
        cout << "  H. Ver historial de busquedas\n";
        linea('-');
        cout << "  Titulo a buscar: "; string t; getline(cin, t);

        if (t == "H" || t == "h") { mostrarHistorial(); return; }

        historialBusquedas->push(t);

        Serie* s = buscarSerie(catalogoSeries->getCabeza(), t);
        if (s) {
            cout << "\n  Serie encontrada:\n";
            s->mostrarDetalle();
            linea('-');
            cout << "  1. Agregar a Mi Lista  0. Volver\n  Opcion: ";
            int op; cin >> op; cin.ignore();
            if (op == 1) agregarAMiLista(s);
            return;
        }

        Pelicula* p = buscarPelicula(catalogoPeliculas->getCabeza(), t);
        if (p) {
            cout << "\n  Pelicula encontrada:\n";
            p->mostrarDetalle();
            linea('-');
            cout << "  1. Agregar a Mi Lista  0. Volver\n  Opcion: ";
            int op; cin >> op; cin.ignore();
            if (op == 1) agregarAMiLista(p);
            return;
        }

        cout << "\n  No se encontro contenido con ese titulo.\n";
        pausa();
    }

    void menuMiLista() {
        MiLista* lista = perfilActual->getLista();
        int op;
        do {
            titulo("      MI LISTA DE FAVORITOS    ");
            lista->mostrar();
            linea('-');
            cout << "  1. Ordenar alfabeticamente (intercambio)\n";
            cout << "  2. Filtrar por genero (lambda)\n";
            cout << "  3. Buscar titulo en mi lista (recursivo)\n";
            cout << "  4. Guardar mi lista\n";
            cout << "  0. Volver\n";
            linea('-');
            cout << "  Opcion: ";
            cin >> op; cin.ignore();
            switch (op) {
                case 1:
                    lista->ordenarPorTitulo();
                    pausa(); break;
                case 2: {
                    cout << "  Genero a filtrar: "; string g; getline(cin, g);
                    lista->filtrar([&g](Contenido* c) { return c->getGenero() == g; });
                    pausa(); break;
                }
                case 3: {
                    cout << "  Titulo a buscar: "; string t; getline(cin, t);
                    bool enc = perfilActual->buscarEnLista(0, t);
                    cout << "  " << (enc ? "Encontrado en tu lista." : "No esta en tu lista.") << "\n";
                    pausa(); break;
                }
                case 4:
                    perfilActual->guardarMiLista(usuarioActual->getEmail());
                    cout << "  Lista guardada.\n"; pausa(); break;
            }
        } while (op != 0);
    }

    void menuCuenta() {
        int op;
        do {
            titulo("       MI PERFIL Y CUENTA      ");
            cout << "  Email  : " << usuarioActual->getEmail() << "\n";
            cout << "  Perfil : " << perfilActual->getNombre() << "\n";
            if (usuarioActual->getSuscripcion())
                usuarioActual->getSuscripcion()->mostrar();
            linea('-');
            cout << "  1. Ver metodos de pago\n";
            cout << "  2. Agregar metodo de pago\n";
            cout << "  3. Ver historial de facturas\n";
            cout << "  4. Cambiar plan\n";
            cout << "  0. Volver\n";
            linea('-');
            cout << "  Opcion: ";
            cin >> op; cin.ignore();
            switch (op) {
                case 1: verMetodosPago();    break;
                case 2: agregarMetodoPago(); break;
                case 3: verFacturas();       break;
                case 4: cambiarPlan();       break;
            }
        } while (op != 0);
    }

    void verMetodosPago() {
        titulo("      METODOS DE PAGO          ");
        if (metodosPago->estaVacia()) {
            cout << "  No tienes metodos de pago registrados.\n";
        } else {
            cout << "  ID  | Tipo     | Numero               | Titular\n";
            linea('-');
            Nodo<MetodoPago*>* n = metodosPago->getCabeza();
            auto mostrar = [](MetodoPago* m) { m->mostrar(); };
            while (n) { mostrar(n->dato); n = n->next; }
        }
        pausa();
    }

    void agregarMetodoPago() {
        titulo("   AGREGAR METODO DE PAGO      ");
        cout << "  Tipo (Credito/Debito/PayPal): "; string tipo;   getline(cin, tipo);
        cout << "  Numero                      : "; string numero; getline(cin, numero);
        cout << "  Titular                     : "; string titular; getline(cin, titular);

        MetodoPago* mp = new MetodoPago(contadorMetodo++, tipo, numero, titular);
        metodosPago->insertarFinal(mp);
        mp->guardarEnArchivo(usuarioActual->getEmail());
        cout << "  Metodo de pago agregado correctamente.\n";
        pausa();
    }

    void verFacturas() {
        titulo("     HISTORIAL DE FACTURAS     ");
        if (facturas->estaVacia()) {
            ifstream f("facturas.txt");
            if (!f.is_open()) { cout << "  No hay facturas registradas.\n"; pausa(); return; }
            string fila;
            cout << "  ID    | Fecha      | Monto     | Metodo\n";
            linea('-');
            while (getline(f, fila)) cout << "  " << fila << "\n";
            f.close();
        } else {
            cout << "  ID    | Fecha      | Monto     | Metodo\n";
            linea('-');
            Nodo<Factura*>* n = facturas->getCabeza();
            auto mostrar = [](Factura* fac) { fac->mostrar(); };
            while (n) { mostrar(n->dato); n = n->next; }
        }
        pausa();
    }

    void cambiarPlan() {
        titulo("        CAMBIAR PLAN           ");
        cout << "  1. Basico   - S/. 24.90/mes\n";
        cout << "  2. Estandar - S/. 35.90/mes\n";
        cout << "  3. Premium  - S/. 45.90/mes\n";
        linea('-');
        cout << "  Opcion: ";
        int op; cin >> op; cin.ignore();

        string tipo; float precio;
        switch (op) {
            case 1:  tipo = "Basico";    precio = 24.90f; break;
            case 2:  tipo = "Estandar";  precio = 35.90f; break;
            default: tipo = "Premium";   precio = 45.90f;
        }

        if (usuarioActual->getSuscripcion())
            usuarioActual->getSuscripcion()->actualizarPlan(tipo, precio);

        Factura* fac = new Factura(idFactura(), fechaHoy(), precio, "Cambio de plan");
        fac->guardarEnArchivo();
        facturas->insertarFinal(fac);
        cout << "  Plan actualizado a " << tipo << ". Factura generada:\n";
        fac->mostrar();
        pausa();
    }
};
