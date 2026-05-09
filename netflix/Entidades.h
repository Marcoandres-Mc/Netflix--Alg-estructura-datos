#pragma once
#include <string>
#include <fstream>
#include <iomanip>
#include "Estructuras.h"
using namespace std;

class Categoria
{
public:
    Categoria(int id, string nombre, string descripcion)
        : id(id), nombre(nombre), descripcion(descripcion) {}
    ~Categoria() {}

    int    getId()          const { return id; }
    string getNombre()      const { return nombre; }
    string getDescripcion() { return descripcion; }

    void mostrar() {
        cout << "[" << id << "] " << nombre << " - " << descripcion << endl;
    }

private:
    int    id;
    string nombre;
    string descripcion;
};

class Contenido
{
public:
    Contenido(int id, string titulo, string genero, int anio,
              int duracion, string clasificacionEdad, Categoria* categoria)
        : id(id), titulo(titulo), genero(genero), anio(anio),
          duracion(duracion), clasificacionEdad(clasificacionEdad),
          categoria(categoria) {}

    ~Contenido() {}

    int    getId()               const { return id; }
    string getTitulo()           const { return titulo; }
    string getGenero()           const { return genero; }
    int    getAnio()             const { return anio; }
    int    getDuracion()         const { return duracion; }
    string getClasificacion()    const { return clasificacionEdad; }

    void mostrarDetalle() {
        cout << "  Titulo       : " << titulo           << "\n";
        cout << "  Genero       : " << genero           << "\n";
        cout << "  Anio         : " << anio             << "\n";
        cout << "  Duracion     : " << duracion << " min\n";
        cout << "  Clasificacion: " << clasificacionEdad << "\n";
        if (categoria)
            cout << "  Categoria    : " << categoria->getNombre() << "\n";
    }

    bool esAptaParaPerfil(bool esInfantil) {
        return !(esInfantil && clasificacionEdad != "ATP");
    }

protected:
    int        id;
    string     titulo;
    string     genero;
    int        anio;
    int        duracion;
    string     clasificacionEdad;
    Categoria* categoria;
};

class Episodio
{
public:
    Episodio(int numero, string titulo, int duracion)
        : numero(numero), titulo(titulo), duracion(duracion) {}
    ~Episodio() {}

    int    getNumero()   const { return numero; }
    string getTitulo()   const { return titulo; }
    int    getDuracion() { return duracion; }

private:
    int    numero;
    string titulo;
    int    duracion;
};

class Calificacion
{
public:
    Calificacion(int puntaje, string comentario, string usuario)
        : puntaje(puntaje), comentario(comentario), usuario(usuario) {}
    ~Calificacion() {}

    int    getPuntaje()    const { return puntaje; }
    string getComentario() { return comentario; }
    string getUsuario()    const { return usuario; }

private:
    int    puntaje;
    string comentario;
    string usuario;
};

class Temporada
{
public:
    Temporada(int numero, int anio) : numero(numero), anio(anio) {}
    ~Temporada() {
        Nodo<Episodio*>* actual = episodios.getCabeza();
        while (actual) { delete actual->dato; actual = actual->next; }
    }

    void agregarEpisodio(Episodio* ep) { episodios.insertarFinal(ep); }

    int               getNumero()   const { return numero; }
    int               getAnio()     const { return anio; }
    Lista<Episodio*>& getEpisodios()      { return episodios; }

    void mostrar() {
        cout << "    Temporada " << numero << " (" << anio << ") - "
             << episodios.tamanio() << " episodio(s)\n";
    }

private:
    int              numero;
    int              anio;
    Lista<Episodio*> episodios;
};

class Serie : public Contenido
{
public:
    Serie(int id, string titulo, string genero,
          string descripcion,
          string clasificacion = "ATP", Categoria* cat = nullptr)
        : Contenido(id, titulo, genero, 0, 0, clasificacion, cat),
          descripcion(descripcion) {}

    ~Serie() {
        Nodo<Temporada*>*    t = temporadas.getCabeza();
        while (t)  { delete t->dato;  t = t->next; }
        Nodo<Calificacion*>* c = calificaciones.getCabeza();
        while (c)  { delete c->dato;  c = c->next; }
    }

    void agregarTemporada(Temporada* t)       { temporadas.insertarFinal(t); }
    void agregarCalificacion(Calificacion* c) { calificaciones.insertarFinal(c); }

    float calcularPromedio(Nodo<Calificacion*>* nodo,
                                    float suma, int count) {
        if (nodo == nullptr)
            return (count == 0) ? 0.0f : suma / count;
        return calcularPromedio(nodo->next,
                                         suma + nodo->dato->getPuntaje(),
                                         count + 1);
    }

    float getCalificacionPromedio() {
        return calcularPromedio(calificaciones.getCabeza(), 0.0f, 0);
    }

    string                getDescripcion() { return descripcion; }
    Lista<Temporada*>&    getTemporadas()        { return temporadas; }
    Lista<Calificacion*>& getCalificaciones()    { return calificaciones; }

    void mostrarResumen() {
        cout << "  [" << id << "] " << titulo
             << " | " << genero
             << " | Temp: " << temporadas.tamanio()
             << " | Calif: " << fixed << setprecision(1) << getCalificacionPromedio() << "/5\n";
    }

    void mostrarDetalle() {
        cout << "  Titulo     : " << titulo      << "\n";
        cout << "  Genero     : " << genero      << "\n";
        cout << "  Descripcion: " << descripcion << "\n";
        cout << "  Temporadas : " << temporadas.tamanio() << "\n";
        cout << "  Calificacion promedio: " << fixed << setprecision(1) << getCalificacionPromedio() << "/5\n";
        Nodo<Temporada*>* t = temporadas.getCabeza();
        while (t) { t->dato->mostrar(); t = t->next; }
    }

    void guardarEnArchivo() {
        ofstream f("series.txt", ios::app);
        if (f.is_open()) {
            f << id << "|" << titulo << "|" << genero << "|"
              << descripcion << "|" << temporadas.tamanio() << "\n";
            f.close();
        }
    }

private:
    string               descripcion;
    Lista<Temporada*>    temporadas;
    Lista<Calificacion*> calificaciones;
};

class MiLista
{
public:
    MiLista() : cantidadElementos(0) {
        contenidos = new Lista<Contenido*>();
    }
    ~MiLista() { delete contenidos; }

    void agregarContenido(Contenido* c) {
        contenidos->insertarFinal(c);
        cantidadElementos++;
    }

    int        getCantidad()           const { return cantidadElementos; }
    Contenido* getContenidoEn(int idx) { return contenidos->obtenerElemento(idx); }

    void ordenarPorTitulo() {
        if (cantidadElementos < 2) return;
        for (int gap = cantidadElementos / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < cantidadElementos; i++) {
                Contenido* temp = contenidos->obtenerElemento(i);
                int j = i;
                while (j >= gap &&
                       contenidos->obtenerElemento(j - gap)->getTitulo() > temp->getTitulo()) {
                    contenidos->reemplazar(j, contenidos->obtenerElemento(j - gap));
                    j -= gap;
                }
                contenidos->reemplazar(j, temp);
            }
        }
        cout << "  Lista ordenada alfabeticamente (Shell sort).\n";
    }

    template<class T>
    void filtrar(T criterio) {
        bool encontrado = false;
        for (int i = 0; i < cantidadElementos; i++) {
            Contenido* item = contenidos->obtenerElemento(i);
            if (criterio(item)) {
                item->mostrarDetalle();
                cout << "  ----------------------------------------\n";
                encontrado = true;
            }
        }
        if (!encontrado)
            cout << "  No hay elementos que coincidan con el criterio.\n";
    }

    void mostrar() {
        if (cantidadElementos == 0) {
            cout << "  Tu lista esta vacia.\n"; return;
        }
        for (int i = 0; i < cantidadElementos; i++) {
            cout << "  [" << (i + 1) << "] "
                 << contenidos->obtenerElemento(i)->getTitulo() << "\n";
        }
    }

    bool estaVacia() { return cantidadElementos == 0; }

private:
    Lista<Contenido*>* contenidos;
    int cantidadElementos;
};

class Perfil
{
public:
    Perfil(int _id, string _nom, bool _infantil)
        : id(_id), nombre(_nom), esInfantil(_infantil) {
        miListaPersonal = new MiLista();
    }
    ~Perfil() { delete miListaPersonal; }

    int      getId()        const { return id; }
    string   getNombre()    const { return nombre; }
    bool     getEsInfantil() { return esInfantil; }
    MiLista* getLista()           { return miListaPersonal; }

    bool buscarEnLista(int index, string tituloBuscado) {
        if (index >= miListaPersonal->getCantidad()) return false;
        if (miListaPersonal->getContenidoEn(index)->getTitulo() == tituloBuscado)
            return true;
        return buscarEnLista(index + 1, tituloBuscado);
    }

    void guardarEnArchivo(string emailUsuario) {
        ofstream f("perfiles.txt", ios::app);
        if (f.is_open()) {
            f << id << "|" << nombre << "|"
              << (esInfantil ? "1" : "0") << "|" << emailUsuario << "\n";
            f.close();
        }
    }

    void guardarMiLista(string emailUsuario) {
        ofstream f("milista.txt", ios::app);
        if (f.is_open()) {
            for (int i = 0; i < miListaPersonal->getCantidad(); i++) {
                f << emailUsuario << "|"
                  << miListaPersonal->getContenidoEn(i)->getTitulo() << "\n";
            }
            f.close();
        }
    }

private:
    int      id;
    string   nombre;
    bool     esInfantil;
    MiLista* miListaPersonal;
};

class Suscripcion
{
public:
    Suscripcion(int id, string t, float p, string inicio)
        : idSuscripcion(id), tipo(t), precio(p), fechaInicio(inicio), estado(true) {}
    ~Suscripcion() {}

    bool   estaActiva()      const { return estado; }
    string getTipo()         const { return tipo; }
    float  getPrecio()       const { return precio; }
    string getFechaInicio()  const { return fechaInicio; }
    int    getIdSuscripcion() { return idSuscripcion; }

    void actualizarPlan(string nuevoPlan, float nuevoPrecio) {
        tipo   = nuevoPlan;
        precio = nuevoPrecio;
    }
    void setEstado(bool s) { estado = s; }

    void mostrar() {
        cout << "  Plan    : " << tipo            << "\n";
        cout << "  Precio  : S/. " << fixed << setprecision(2) << precio << "\n";
        cout << "  Inicio  : " << fechaInicio     << "\n";
        cout << "  Estado  : " << (estado ? "Activa" : "Inactiva") << "\n";
    }

    void guardarEnArchivo(string emailUsuario) {
        ofstream f("suscripciones.txt", ios::app);
        if (f.is_open()) {
            f << idSuscripcion << "|" << tipo << "|"
              << fixed << setprecision(2) << precio << "|"
              << fechaInicio << "|" << emailUsuario << "\n";
            f.close();
        }
    }

private:
    int    idSuscripcion;
    string tipo;
    float  precio;
    string fechaInicio;
    bool   estado;
};

class Usuario
{
public:
    Usuario(string em, string pass)
        : email(em), password(pass), suscripcionActiva(nullptr) {
        perfiles = new Lista<Perfil*>();
    }
    ~Usuario() { delete perfiles; }

    void         setSuscripcion(Suscripcion* s) { suscripcionActiva = s; }
    void         anadirPerfil(Perfil* p)        { perfiles->insertarFinal(p); }
    string       getEmail()                const { return email; }
    Suscripcion* getSuscripcion()          const { return suscripcionActiva; }
    Lista<Perfil*>* getPerfiles()                { return perfiles; }

    bool validarAcceso(string em, string pass) {
        return (email == em && password == pass);
    }

    void guardarEnArchivo() {
        ofstream f("usuarios.txt", ios::app);
        if (f.is_open()) {
            f << email << "|" << password << "\n";
            f.close();
        }
    }

    bool existeUsuario(string email) {
        ifstream f("usuarios.txt");
        string linea;
        while (getline(f, linea)) {
            size_t pos = linea.find('|');
            if (pos != string::npos && linea.substr(0, pos) == email) return true;
        }
        return false;
    }

    bool validarCredenciales(string email, string pass) {
        ifstream f("usuarios.txt");
        string linea;
        while (getline(f, linea)) {
            size_t p1 = linea.find('|');
            if (p1 == string::npos) continue;
            string em = linea.substr(0, p1);
            string pw = linea.substr(p1 + 1);
            if (em == email && pw == pass) return true;
        }
        return false;
    }

private:
    string          email;
    string          password;
    Suscripcion*    suscripcionActiva;
    Lista<Perfil*>* perfiles;
};

class MetodoPago
{
public:
    MetodoPago(int id, string tipo,
               string numero, string titular)
        : id(id), tipo(tipo), numero(numero), titular(titular) {}
    ~MetodoPago() {}

    int    getId()      const { return id; }
    string getTipo()    const { return tipo; }
    string getNumero()  const { return numero; }
    string getTitular() { return titular; }

    void mostrar() {
        cout << "  [" << id << "] " << tipo << "  |  "
             << numero << "  |  " << titular << "\n";
    }

    void guardarEnArchivo(string emailUsuario) {
        ofstream f("metodos_pago.txt", ios::app);
        if (f.is_open()) {
            f << id << "|" << tipo << "|" << numero << "|"
              << titular << "|" << emailUsuario << "\n";
            f.close();
        }
    }

private:
    int    id;
    string tipo;
    string numero;
    string titular;
};

class Factura
{
public:
    Factura(string id, string fecha, float monto, string metodo)
        : idFactura(id), fechaEmision(fecha), montoTotal(monto),
          metodoPago(metodo), estadoPago(true) {}
    ~Factura() {}

    string getIdFactura()  const { return idFactura; }
    float  getMontoTotal() { return montoTotal; }

    void mostrar() {
        cout << "  " << idFactura << "  |  "
             << fechaEmision << "  |  S/. "
             << fixed << setprecision(2) << montoTotal
             << "  |  " << metodoPago << "\n";
    }

    void guardarEnArchivo() {
        ofstream f("facturas.txt", ios::app);
        if (f.is_open()) {
            f << idFactura << "|" << fechaEmision << "|"
              << fixed << setprecision(2) << montoTotal
              << "|" << metodoPago << "\n";
            f.close();
        }
    }

private:
    string idFactura;
    string fechaEmision;
    float  montoTotal;
    string metodoPago;
    bool   estadoPago;
};

class Pelicula : public Contenido
{
public:
    Pelicula(int id, string titulo, string genero, int anio, int duracion,
             string clasificacionEdad, Categoria* categoria,
             string director, string productora, string idioma, string paisOrigen)
        : Contenido(id, titulo, genero, anio, duracion, clasificacionEdad, categoria),
          director(director), productora(productora),
          idioma(idioma), paisOrigen(paisOrigen) {}
    ~Pelicula() {}

    string getDirector()   const { return director; }
    string getProductora() { return productora; }

    void mostrarDetalle() {
        Contenido::mostrarDetalle();
        cout << "  Director     : " << director   << "\n";
        cout << "  Productora   : " << productora << "\n";
        cout << "  Idioma       : " << idioma     << "\n";
        cout << "  Pais origen  : " << paisOrigen << "\n";
    }

    void mostrarResumen() {
        cout << "  [" << id << "] " << titulo
             << " | " << genero
             << " | " << anio
             << " | Dir: " << director << "\n";
    }

    void guardarEnArchivo() {
        ofstream f("peliculas.txt", ios::app);
        if (f.is_open()) {
            f << id << "|" << titulo << "|" << genero << "|"
              << anio << "|" << duracion << "|" << clasificacionEdad << "|"
              << director << "|" << productora << "|"
              << idioma  << "|" << paisOrigen << "\n";
            f.close();
        }
    }

private:
    string director;
    string productora;
    string idioma;
    string paisOrigen;
};

class Recomendacion
{
public:
    Recomendacion(int id, string motivo, float puntajeCoincidencia,
                  Contenido* contenido, Perfil* perfil)
        : id(id), motivo(motivo), puntajeCoincidencia(puntajeCoincidencia),
          contenido(contenido), perfil(perfil) {}
    ~Recomendacion() {}

    string generarMotivo()        const { return motivo; }
    float  calcularCoincidencia() { return puntajeCoincidencia; }
    bool   esRelevante()          const { return puntajeCoincidencia >= 70; }

    void mostrarRecomendacion() {
        cout << "  Recomendado : " << contenido->getTitulo()   << "\n";
        cout << "  Motivo      : " << motivo                   << "\n";
        cout << "  Coincidencia: " << puntajeCoincidencia      << "%\n";
    }

private:
    int        id;
    string     motivo;
    float      puntajeCoincidencia;
    Contenido* contenido;
    Perfil*    perfil;
};
