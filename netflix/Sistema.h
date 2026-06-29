#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Estructuras.h"
#include "Entidades.h"
#include "DataSetGenerator.h"
using namespace std;
using namespace System;

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
        indiceSeries       = new TablaHash<Serie*>(17);
        indicePeliculas    = new TablaHash<Pelicula*>(17);
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
        delete indiceSeries;
        delete indicePeliculas;
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
                case 3: DatasetGenerator::menuGenerador(); break;
                case 4:
                    linea('-');
                    setColor(ConsoleColor::Green);
                    cout << "  Hasta luego!\n";
                    resetColor();
                    linea('-');
                    break;
                default:
                    setColor(ConsoleColor::DarkRed);
                    cout << "  Opcion invalida.\n";
                    resetColor();
            }
        } while (opcion != 4);
    }

private:
    Usuario*            usuarioActual;
    Perfil*             perfilActual;
    Lista<Serie*>*      catalogoSeries;
    Lista<Pelicula*>*   catalogoPeliculas;
    Lista<Categoria*>*  categorias;
    Lista<MetodoPago*>* metodosPago;
    Lista<Factura*>*    facturas;
    int contadorFactura;
    int contadorMetodo;
    Pila<string>*         historialBusquedas;
    Cola<Contenido*>*     colaReproduccion;
    TablaHash<Serie*>*    indiceSeries;
    TablaHash<Pelicula*>* indicePeliculas;

    void setColor(ConsoleColor c) { Console::ForegroundColor = c; }
    void resetColor() { Console::ForegroundColor = ConsoleColor::White; }

    void menuOp(string key, string desc) {
        cout << "  ";
        setColor(ConsoleColor::Yellow);
        cout << key;
        resetColor();
        cout << ". " << desc << "\n";
    }
    void promptOpcion() {
        setColor(ConsoleColor::Green);
        cout << "  Opcion: ";
        resetColor();
    }
    void msgOk(string msg) {
        setColor(ConsoleColor::Green);
        cout << msg;
        resetColor();
    }
    void msgErr(string msg) {
        setColor(ConsoleColor::DarkRed);
        cout << msg;
        resetColor();
    }
    
    bool esNumero(string s) {
        if (s.empty()) return false;
        for (char c : s)
            if (c < '0' || c > '9') return false;
        return true;
    }

    void linea(char c = '=', int n = 52) {
        setColor(c == '=' ? ConsoleColor::Red : ConsoleColor::DarkGray);
        for (int i = 0; i < n; i++) cout << c;
        cout << "\n";
        resetColor();
    }

    void titulo(string t) {
        linea('=');
        setColor(ConsoleColor::Yellow);
        int pad = (52 - (int)t.size()) / 2;
        cout << string(pad, ' ') << t << "\n";
        linea('=');
        resetColor();
    }

    void pausa() {
        setColor(ConsoleColor::DarkCyan);
        cout << "\n  Presiona Enter para continuar...";
        resetColor();
        cin.ignore();
    }

    string idFactura() {
        string num = to_string(contadorFactura++);
        while (num.size() < 3) num = "0" + num;
        return "F" + num;
    }
    string fechaHoy() { return "2026-05-08"; }

    Categoria* buscarCategoriaPorGenero(string genero) {
        Nodo<Categoria*>* nodo = categorias->getCabeza();
        while (nodo) {
            if (nodo->dato->getNombre() == genero) return nodo->dato;
            nodo = nodo->next;
        }
        return nullptr;
    }

    void cargarCatalogo() {
        categorias->insertarFinal(new Categoria(1, "Accion",     "Peliculas y series de accion"));
        categorias->insertarFinal(new Categoria(2, "Drama",      "Historias dramaticas"));
        categorias->insertarFinal(new Categoria(3, "Sci-Fi",     "Ciencia ficcion"));
        categorias->insertarFinal(new Categoria(4, "Terror",     "Horror y suspenso"));
        categorias->insertarFinal(new Categoria(5, "Comedia",    "Humor y entretenimiento"));
        categorias->insertarFinal(new Categoria(6, "Documental", "Contenido educativo"));
        categorias->insertarFinal(new Categoria(7, "Thriller",   "Suspenso y tension"));

        cargarSeries();
        cargarPeliculas();
    }

    void cargarSeries() {
        ifstream f("series.txt");
        if (!f.is_open()) { cout << "  [Aviso] No se encontro series.txt\n"; return; }
        string linea;
        while (getline(f, linea)) {
            if (linea.empty()) continue;
            istringstream ss(linea);
            string sid, titulo, genero, descripcion, clasificacion;
            if (!getline(ss, sid,          '|')) continue;
            if (!getline(ss, titulo,       '|')) continue;
            if (!getline(ss, genero,       '|')) continue;
            if (!getline(ss, descripcion,  '|')) continue;
            if (!getline(ss, clasificacion,'|')) continue;
            Categoria* cat = buscarCategoriaPorGenero(genero);
            Serie* nueva = new Serie(stoi(sid), titulo, genero, descripcion, clasificacion, cat);
            catalogoSeries->insertarFinal(nueva);
            indiceSeries->insertar(titulo, nueva);
        }
        f.close();

        ifstream ft("temporadas.txt");
        if (!ft.is_open()) return;
        while (getline(ft, linea)) {
            if (linea.empty()) continue;
            istringstream ss(linea);
            string sid, snum, sanio;
            if (!getline(ss, sid,  '|')) continue;
            if (!getline(ss, snum, '|')) continue;
            if (!getline(ss, sanio,'|')) continue;
            int id = stoi(sid);
            Nodo<Serie*>* nodo = catalogoSeries->getCabeza();
            while (nodo) {
                if (nodo->dato->getId() == id) {
                    nodo->dato->agregarTemporada(new Temporada(stoi(snum), stoi(sanio)));
                    break;
                }
                nodo = nodo->next;
            }
        }
        ft.close();
    }

    void cargarPeliculas() {
        ifstream f("peliculas.txt");
        if (!f.is_open()) { cout << "  [Aviso] No se encontro peliculas.txt\n"; return; }
        string linea;
        while (getline(f, linea)) {
            if (linea.empty()) continue;
            istringstream ss(linea);
            string sid, titulo, genero, sanio, sdur, clasif, director, productora, idioma, pais;
            if (!getline(ss, sid,       '|')) continue;
            if (!getline(ss, titulo,    '|')) continue;
            if (!getline(ss, genero,    '|')) continue;
            if (!getline(ss, sanio,     '|')) continue;
            if (!getline(ss, sdur,      '|')) continue;
            if (!getline(ss, clasif,    '|')) continue;
            if (!getline(ss, director,  '|')) continue;
            if (!getline(ss, productora,'|')) continue;
            if (!getline(ss, idioma,    '|')) continue;
            if (!getline(ss, pais             )) continue;
            Categoria* cat = buscarCategoriaPorGenero(genero);
            Pelicula* nueva = new Pelicula(
                stoi(sid), titulo, genero, stoi(sanio), stoi(sdur),
                clasif, cat, director, productora, idioma, pais);
            catalogoPeliculas->insertarFinal(nueva);
            indicePeliculas->insertar(titulo, nueva);
        }
        f.close();
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

    // ── Pantalla de bienvenida con logo ASCII ─────────────────────
    void mostrarBienvenida() {
        Console::Clear();
        setColor(ConsoleColor::Red);
        cout << "\n";
        cout << "      #   # ##### ##### ##### #     ### #   #\n";
        cout << "      ##  # #       #   #     #      #   # # \n";
        cout << "      # # # ####    #   ####  #      #     #  \n";
        cout << "      #  ## #       #   #     #      #   # # \n";
        cout << "      #   # #####   #   #     ##### ###  #   #\n";
        cout << "\n";
        linea('=');
        setColor(ConsoleColor::Gray);
        cout << "           Sistema de Streaming\n";
        linea('-');
        resetColor();
        cout << "  Bienvenido a la plataforma de entretenimiento.\n";
        linea('-');
    }

    void mostrarMenuPrincipal() {
        linea('-');
        setColor(ConsoleColor::Yellow);
        cout << "  MENU PRINCIPAL\n";
        resetColor();
        linea('-');
        menuOp("1", "Registrarse");
        menuOp("2", "Iniciar sesion");
        menuOp("3", "Generador de Dataset - peliculas");
        menuOp("4", "Salir");
        linea('-');
        promptOpcion();
    }

    void flujoRegistro() {
        titulo("        REGISTRO DE USUARIO       ");
        string nombre, email, password;

        do {
            setColor(ConsoleColor::Cyan);
            cout << "  Nombre       : ";
            resetColor();
            getline(cin, nombre);
            if (nombre.empty()) msgErr("  El nombre no puede estar vacio.\n");
        } while (nombre.empty());

        do {
            setColor(ConsoleColor::Cyan);
            cout << "  Email        : ";
            resetColor();
            getline(cin, email);
            if (email.empty()) msgErr("  El email no puede estar vacio.\n");
        } while (email.empty());

        do {
            setColor(ConsoleColor::Cyan);
            cout << "  Contrasena   : ";
            resetColor();
            getline(cin, password);
            if (password.empty()) msgErr("  La contrasena no puede estar vacia.\n");
        } while (password.empty());

        Usuario tmpCheck(email, "");
        if (tmpCheck.existeUsuario(email)) {
            msgErr("\n  El email ya esta registrado. Usa otro.\n");
            pausa(); return;
        }

        cout << "\n  Selecciona plan de suscripcion:\n";
        menuOp("1", "Basico   - S/. 24.90/mes");
        menuOp("2", "Estandar - S/. 35.90/mes");
        menuOp("3", "Premium  - S/. 45.90/mes");
        linea('-');
        promptOpcion();
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
        msgOk("\n  Registro exitoso! Factura generada:\n");
        fac->mostrar();
        delete fac;
        delete nuevoU;

        msgOk("\n  Ya puedes iniciar sesion.\n");
        pausa();
    }

    void flujoInicioSesion() {
        titulo("       INICIO DE SESION          ");
        string email, password;
        setColor(ConsoleColor::Cyan);
        cout << "  Email      : "; getline(cin, email);
        cout << "  Contrasena : "; getline(cin, password);
        resetColor();

        Usuario tmpLogin(email, password);
        if (!tmpLogin.validarCredenciales(email, password)) {
            msgErr("\n  Credenciales incorrectas.\n");
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

        msgOk("\n  Bienvenido/a, ");
        setColor(ConsoleColor::Green);
        cout << email << "!\n";
        resetColor();
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
            setColor(ConsoleColor::Cyan);
            cout << "  Usuario: " << usuarioActual->getEmail() << "\n";
            cout << "  Perfil : " << perfilActual->getNombre() << "\n";
            resetColor();
            linea('-');
            menuOp("1", "Explorar Series");
            menuOp("2", "Explorar Peliculas");
            menuOp("3", "Buscar contenido");
            menuOp("4", "Mi Lista de Favoritos");
            menuOp("5", "Cola de Reproduccion");
            menuOp("6", "Mi Perfil y Cuenta");
            menuOp("7", "Cerrar sesion");
            linea('-');
            promptOpcion();
            cin >> op; cin.ignore();
            switch (op) {
                case 1: menuSeries();      break;
                case 2: menuPeliculas();   break;
                case 3: menuBuscar();      break;
                case 4: menuMiLista();     break;
                case 5: menuCola();        break;
                case 6: menuCuenta();      break;
                case 7:
                    msgOk("\n  Sesion cerrada correctamente.\n");
                    break;
                default:
                    msgErr("  Opcion invalida.\n");
            }
        } while (op != 7);
    }

    void menuSeries() {
        do {
            titulo("         SERIES               ");
            int idx = 1;
            Nodo<Serie*>* nodo = catalogoSeries->getCabeza();
            auto mostrarSerie = [&](Serie* s) {
                setColor(ConsoleColor::Cyan);
                cout << "  " << idx++ << ". ";
                resetColor();
                s->mostrarResumen();
            };
            while (nodo) { mostrarSerie(nodo->dato); nodo = nodo->next; }
            linea('-');
            menuOp("1-" + to_string(catalogoSeries->tamanio()), "Ver detalle");
            menuOp("F", "Filtrar por genero");
            menuOp("O", "Ordenar por calificacion");
            menuOp("0", "Volver");
            linea('-');
            promptOpcion();
            string entrada; getline(cin, entrada);

            if (entrada == "0") break;
            if (entrada == "F" || entrada == "f") filtrarSeriesPorGenero();
            else if (entrada == "O" || entrada == "o") ordenarSeriesPorCalif();
            else if (esNumero(entrada)) {
                int sel = stoi(entrada) - 1;
                if (sel >= 0 && sel < catalogoSeries->tamanio())
                    detalleSerie(catalogoSeries->obtenerElemento(sel));
                else
                    msgErr("  Opcion invalida.\n");
            } else {
                msgErr("  Opcion invalida.\n");
            }
        } while (true);
    }

    void detalleSerie(Serie* s) {
        titulo("       DETALLE DE SERIE         ");
        s->mostrarDetalle();
        linea('-');
        menuOp("1", "Agregar a Mi Lista");
        menuOp("2", "Calificar serie");
        menuOp("3", "Agregar a Cola de Reproduccion");
        menuOp("0", "Volver");
        linea('-');
        promptOpcion();
        int op; cin >> op; cin.ignore();
        switch (op) {
            case 1: agregarAMiLista(s);           break;
            case 2: calificarSerie(s);             break;
            case 3: agregarACola(s);               break;
        }
    }

    void calificarSerie(Serie* s) {
        setColor(ConsoleColor::Cyan); cout << "  Puntaje (1-5): "; resetColor();
        int puntaje; cin >> puntaje; cin.ignore();
        if (puntaje < 1 || puntaje > 5) { msgErr("  Puntaje invalido.\n"); return; }
        setColor(ConsoleColor::Cyan); cout << "  Comentario   : "; resetColor();
        string comentario; getline(cin, comentario);
        s->agregarCalificacion(
            new Calificacion(puntaje, comentario, usuarioActual->getEmail()));
        msgOk("  Calificacion registrada. Nuevo promedio: ");
        setColor(ConsoleColor::Green);
        cout << fixed << setprecision(1) << s->getCalificacionPromedio() << "/5\n";
        resetColor();
        pausa();
    }

    void filtrarSeriesPorGenero() {
        setColor(ConsoleColor::Cyan); cout << "  Genero a filtrar: "; resetColor();
        string genero; getline(cin, genero);
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
        if (!encontrado) msgErr("  No hay series del genero '" + genero + "'.\n");
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
        msgOk("  Series ordenadas por calificacion (algoritmo de intercambio).\n");
        pausa();
    }

    void ordenarPeliculasPorAnio() {
        int n = catalogoPeliculas->tamanio();
        for (int i = 1; i < n; i++) {
            Pelicula* clave = catalogoPeliculas->obtenerElemento(i);
            int j = i - 1;
            while (j >= 0 &&
                   catalogoPeliculas->obtenerElemento(j)->getAnio() > clave->getAnio()) {
                catalogoPeliculas->reemplazar(j + 1, catalogoPeliculas->obtenerElemento(j));
                j--;
            }
            catalogoPeliculas->reemplazar(j + 1, clave);
        }
        msgOk("  Peliculas ordenadas por anio (insercion).\n");
        pausa();
    }

    void menuPeliculas() {
        do {
            titulo("        PELICULAS              ");
            Nodo<Pelicula*>* nodo = catalogoPeliculas->getCabeza();
            int idx = 1;
            auto mostrarPelicula = [&](Pelicula* p) {
                setColor(ConsoleColor::Cyan);
                cout << "  " << idx++ << ". ";
                resetColor();
                p->mostrarResumen();
            };
            while (nodo) { mostrarPelicula(nodo->dato); nodo = nodo->next; }
            linea('-');
            menuOp("1-" + to_string(catalogoPeliculas->tamanio()), "Ver detalle");
            menuOp("F", "Filtrar por genero");
            menuOp("O", "Ordenar por anio");
            menuOp("0", "Volver");
            linea('-');
            promptOpcion();
            string entrada; getline(cin, entrada);

            if (entrada == "0") break;
            if (entrada == "F" || entrada == "f") filtrarPeliculasPorGenero();
            else if (entrada == "O" || entrada == "o") ordenarPeliculasPorAnio();
            else if (esNumero(entrada)) {
                int sel = stoi(entrada) - 1;
                if (sel >= 0 && sel < catalogoPeliculas->tamanio())
                    detallePelicula(catalogoPeliculas->obtenerElemento(sel));
                else
                    msgErr("  Opcion invalida.\n");
            } else {
                msgErr("  Opcion invalida.\n");
            }
        } while (true);
    }

    void detallePelicula(Pelicula* p) {
        titulo("      DETALLE DE PELICULA       ");
        p->mostrarDetalle();
        linea('-');
        menuOp("1", "Agregar a Mi Lista");
        menuOp("2", "Agregar a Cola de Reproduccion");
        menuOp("0", "Volver");
        linea('-');
        promptOpcion();
        int op; cin >> op; cin.ignore();
        if (op == 1) agregarAMiLista(p);
        if (op == 2) agregarACola(p);
    }

    void filtrarPeliculasPorGenero() {
        setColor(ConsoleColor::Cyan); cout << "  Genero a filtrar: "; resetColor();
        string genero; getline(cin, genero);
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
        if (!encontrado) msgErr("  No hay peliculas del genero '" + genero + "'.\n");
        pausa();
    }

    void agregarACola(Contenido* c) {
        if (colaReproduccion->contiene(c)) {
            setColor(ConsoleColor::DarkYellow);
            cout << "  '" << c->getTitulo() << "' ya esta en la cola de reproduccion.\n";
            resetColor();
        } else {
            colaReproduccion->encolar(c);
            msgOk("  '" + c->getTitulo() + "' agregado a la cola de reproduccion.\n");
        }
        pausa();
    }

    void mostrarHistorial() {
        int op;
        do {
            titulo("  HISTORIAL DE BUSQUEDAS       ");
            if (historialBusquedas->estaVacia()) {
                msgErr("  No hay busquedas recientes.\n");
                pausa(); return;
            }
            int i = 1;
            Nodo<string>* nodo = historialBusquedas->getTope();
            auto mostrarEntrada = [&](string s) {
                setColor(ConsoleColor::Cyan); cout << "  " << i++ << ". "; resetColor();
                cout << s << "\n";
            };
            while (nodo) { mostrarEntrada(nodo->dato); nodo = nodo->next; }
            if (historialBusquedas->contieneDuplicados()) {
                setColor(ConsoleColor::DarkYellow);
                cout << "  (hay busquedas repetidas en el historial)\n";
                resetColor();
            }
            linea('-');
            menuOp("1", "Limpiar historial");
            menuOp("0", "Volver");
            linea('-');
            promptOpcion();
            cin >> op; cin.ignore();
            if (op == 1) {
                historialBusquedas->vaciar();
                msgOk("  Historial limpiado.\n");
                pausa(); return;
            }
        } while (op != 0);
    }

    void menuCola() {
        int op;
        do {
            titulo("    COLA DE REPRODUCCION       ");
            if (colaReproduccion->estaVacia()) {
                msgErr("  La cola esta vacia.\n");
            } else {
                setColor(ConsoleColor::Cyan); cout << "  Siguiente: "; resetColor();
                cout << colaReproduccion->frente()->getTitulo() << "\n";
                linea('-');
                int i = 1;
                Nodo<Contenido*>* nodo = colaReproduccion->getFrente();
                auto mostrarEnCola = [&](Contenido* c) {
                    setColor(ConsoleColor::Yellow); cout << "  " << i++ << ". "; resetColor();
                    cout << c->getTitulo() << "\n";
                };
                while (nodo) { mostrarEnCola(nodo->dato); nodo = nodo->next; }
            }
            linea('-');
            menuOp("1", "Reproducir siguiente");
            menuOp("2", "Quitar de la cola (por posicion)");
            menuOp("3", "Vaciar cola");
            menuOp("0", "Volver");
            linea('-');
            promptOpcion();
            cin >> op; cin.ignore();
            if (op == 1) {
                if (!colaReproduccion->estaVacia()) {
                    setColor(ConsoleColor::Magenta);
                    cout << "\n  Reproduciendo: " << colaReproduccion->frente()->getTitulo() << "\n";
                    resetColor();
                    colaReproduccion->frente()->mostrarDetalle();
                    colaReproduccion->desencolar();
                    pausa();
                } else {
                    msgErr("  No hay contenido en la cola.\n");
                    pausa();
                }
            } else if (op == 2) {
                if (!colaReproduccion->estaVacia()) {
                    setColor(ConsoleColor::Cyan); cout << "  Numero a quitar (1-" << colaReproduccion->tamanio() << "): "; resetColor();
                    int pos; cin >> pos; cin.ignore();
                    if (pos >= 1 && pos <= colaReproduccion->tamanio()) {
                        colaReproduccion->eliminarEn(pos - 1);
                        msgOk("  Elemento eliminado de la cola.\n");
                    } else {
                        msgErr("  Posicion invalida.\n");
                    }
                    pausa();
                } else {
                    msgErr("  La cola esta vacia.\n"); pausa();
                }
            } else if (op == 3) {
                colaReproduccion->vaciar();
                msgOk("  Cola vaciada.\n");
                pausa();
            }
        } while (op != 0);
    }

    void agregarAMiLista(Contenido* c) {
        if (perfilActual->buscarEnLista(0, c->getTitulo())) {
            setColor(ConsoleColor::DarkYellow);
            cout << "  '" << c->getTitulo() << "' ya esta en tu lista.\n";
            resetColor();
        } else {
            perfilActual->getLista()->agregarContenido(c);
            msgOk("  '" + c->getTitulo() + "' agregado a tu lista.\n");
        }
        pausa();
    }

    Serie* buscarSerie(Nodo<Serie*>* nodo, string titulo) {
        if (nodo == nullptr) return nullptr;
        if (nodo->dato->getTitulo() == titulo) return nodo->dato;
        return buscarSerie(nodo->next, titulo);
    }

    Pelicula* buscarPelicula(Nodo<Pelicula*>* nodo, string titulo) {
        if (nodo == nullptr) return nullptr;
        if (nodo->dato->getTitulo() == titulo) return nodo->dato;
        return buscarPelicula(nodo->next, titulo);
    }

    void menuBuscar() {
        titulo("     BUSCAR CONTENIDO           ");
        menuOp("H", "Ver historial de busquedas");
        linea('-');
        setColor(ConsoleColor::Cyan); cout << "  Titulo a buscar: "; resetColor();
        string t; getline(cin, t);

        if (t == "H" || t == "h") { mostrarHistorial(); return; }

        if (historialBusquedas->buscar(t)) {
            setColor(ConsoleColor::DarkYellow);
            cout << "  (ya buscaste '" << t << "' antes)\n";
            resetColor();
        }
        historialBusquedas->push(t);

        Serie* s = indiceSeries->buscar(t);
        if (s) {
            msgOk("\n  Serie encontrada:\n");
            s->mostrarDetalle();
            linea('-');
            menuOp("1", "Agregar a Mi Lista");
            menuOp("0", "Volver");
            promptOpcion();
            int op; cin >> op; cin.ignore();
            if (op == 1) agregarAMiLista(s);
            return;
        }

        Pelicula* p = indicePeliculas->buscar(t);
        if (p) {
            msgOk("\n  Pelicula encontrada:\n");
            p->mostrarDetalle();
            linea('-');
            menuOp("1", "Agregar a Mi Lista");
            menuOp("0", "Volver");
            promptOpcion();
            int op; cin >> op; cin.ignore();
            if (op == 1) agregarAMiLista(p);
            return;
        }

        msgErr("\n  No se encontro contenido con ese titulo.\n");
        pausa();
    }

    void menuMiLista() {
        MiLista* lista = perfilActual->getLista();
        int op;
        do {
            titulo("      MI LISTA DE FAVORITOS    ");
            lista->mostrar();
            linea('-');
            menuOp("1", "Ordenar alfabeticamente (Shell sort)");
            menuOp("2", "Filtrar por genero (lambda)");
            menuOp("3", "Buscar titulo en mi lista (recursivo)");
            menuOp("4", "Guardar mi lista");
            menuOp("0", "Volver");
            linea('-');
            promptOpcion();
            cin >> op; cin.ignore();
            switch (op) {
                case 1:
                    lista->ordenarPorTitulo();
                    msgOk("  Lista ordenada alfabeticamente.\n");
                    pausa(); break;
                case 2: {
                    setColor(ConsoleColor::Cyan); cout << "  Genero a filtrar: "; resetColor();
                    string g; getline(cin, g);
                    lista->filtrar([&g](Contenido* c) { return c->getGenero() == g; });
                    pausa(); break;
                }
                case 3: {
                    setColor(ConsoleColor::Cyan); cout << "  Titulo a buscar: "; resetColor();
                    string t; getline(cin, t);
                    bool enc = perfilActual->buscarEnLista(0, t);
                    if (enc) msgOk("  Encontrado en tu lista.\n");
                    else     msgErr("  No esta en tu lista.\n");
                    pausa(); break;
                }
                case 4:
                    perfilActual->guardarMiLista(usuarioActual->getEmail());
                    msgOk("  Lista guardada.\n"); pausa(); break;
            }
        } while (op != 0);
    }

    void menuCuenta() {
        int op;
        do {
            titulo("       MI PERFIL Y CUENTA      ");
            setColor(ConsoleColor::Cyan);
            cout << "  Email  : " << usuarioActual->getEmail() << "\n";
            cout << "  Perfil : " << perfilActual->getNombre() << "\n";
            resetColor();
            if (usuarioActual->getSuscripcion())
                usuarioActual->getSuscripcion()->mostrar();
            linea('-');
            menuOp("1", "Ver metodos de pago");
            menuOp("2", "Agregar metodo de pago");
            menuOp("3", "Ver historial de facturas");
            menuOp("4", "Cambiar plan");
            menuOp("0", "Volver");
            linea('-');
            promptOpcion();
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
            msgErr("  No tienes metodos de pago registrados.\n");
        } else {
            setColor(ConsoleColor::Cyan);
            cout << "  ID  | Tipo     | Numero               | Titular\n";
            resetColor();
            linea('-');
            Nodo<MetodoPago*>* n = metodosPago->getCabeza();
            auto mostrar = [](MetodoPago* m) { m->mostrar(); };
            while (n) { mostrar(n->dato); n = n->next; }
        }
        pausa();
    }

    void agregarMetodoPago() {
        titulo("   AGREGAR METODO DE PAGO      ");
        setColor(ConsoleColor::Cyan);
        cout << "  Tipo (Credito/Debito/PayPal): "; string tipo;    getline(cin, tipo);
        cout << "  Numero                      : "; string numero;  getline(cin, numero);
        cout << "  Titular                     : "; string titular; getline(cin, titular);
        resetColor();

        MetodoPago* mp = new MetodoPago(contadorMetodo++, tipo, numero, titular);
        metodosPago->insertarFinal(mp);
        mp->guardarEnArchivo(usuarioActual->getEmail());
        msgOk("  Metodo de pago agregado correctamente.\n");
        pausa();
    }

    void verFacturas() {
        titulo("     HISTORIAL DE FACTURAS     ");
        if (facturas->estaVacia()) {
            ifstream f("facturas.txt");
            if (!f.is_open()) {
                msgErr("  No hay facturas registradas.\n"); pausa(); return;
            }
            setColor(ConsoleColor::Cyan);
            cout << "  ID    | Fecha      | Monto     | Metodo\n";
            resetColor();
            linea('-');
            string fila;
            while (getline(f, fila)) cout << "  " << fila << "\n";
            f.close();
        } else {
            setColor(ConsoleColor::Cyan);
            cout << "  ID    | Fecha      | Monto     | Metodo\n";
            resetColor();
            linea('-');
            Nodo<Factura*>* n = facturas->getCabeza();
            auto mostrar = [](Factura* fac) { fac->mostrar(); };
            while (n) { mostrar(n->dato); n = n->next; }
        }
        pausa();
    }

    void cambiarPlan() {
        titulo("        CAMBIAR PLAN           ");
        menuOp("1", "Basico   - S/. 24.90/mes");
        menuOp("2", "Estandar - S/. 35.90/mes");
        menuOp("3", "Premium  - S/. 45.90/mes");
        linea('-');
        promptOpcion();
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
        msgOk("  Plan actualizado a ");
        setColor(ConsoleColor::Green);
        cout << tipo << ". ";
        resetColor();
        cout << "Factura generada:\n";
        fac->mostrar();
        pausa();
    }
};
