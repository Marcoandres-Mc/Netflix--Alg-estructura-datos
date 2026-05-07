#pragma once
#include <string>
#include "Estructuras.h"
using namespace std;

class Perfil
{
public:
    Perfil(int_id, string_nom, bool_infantil)
    : id(id), nombre(_nom), esInfantil(_infantil){
    miListaPersonal=new MiLista();
    }
    ~Perfil(){delete miListaPersonal; }

    string getNombre() const{return nombre; }
    MiLista*getLista() {return miListaPersonal; }

    bool buscarEnListaRecursivo(int index, string tituloBuscado);

private:
    int id;
    string nombre;
    bool esInfantil;
    MiLista* miListaPersonal;
    };

class Usuario{
public:
    Usuario(string em, string pass)
        : email(em), password(pass), suscripcionActiva(nullptr){
    perfiles=new ListaGenerica<Perfil*();
            }
     ~Usuario(){delete perfiles; }

    void setSuscripcion(Suscripcion* s){ suscripcionActiva=s; }
    void anadirPerfil(Perfil*p){
        perfiles->insertarFinal(p);
    }
    bool validarAcceso(string em, string pass){
        return(email==em && password==pass);
    }
private:
    string email;
    string password;
    Suscripcion* suscripcionActiva;
    ListaGenerica<Perfil*>*perfiles;
    };

class Suscripcion
{
public:
    Suscripcion(int id, string t, float p, string inicio)
        : idSuscripcion(id), tipo(t), precio(p), fechaInicio(inicio), estado (true){}
    ~Suscripcion(){}

    bool estaActiva() const { return estado; }
    string getTipo() const { return tipo; }

    void actualizarPlan(string nuevoPlan, float nuevoPrecio){
    tipo = nuevoPlan;
    precio = nuevoPrecio;
    }

    private:
    int idSuscripcion;
    string tipo;
    float precio;
    string fechaInicio;
    bool estado; 
    };

class MiLista
{
public:
    MiLista() : cantidadElementos(0){
    contenidos = new ListaGenerica<Contenido*>();
    }
    ~MiLista(){
        delete contenidos; }

    void agregarContenido(Contenido * c){
    contenidos -> insertarFinal (c);
    cantidadElementos++;
    }

    void ordenarPorTitulo(){
    if (cantidadElementos < 2) return;
    for (int i=1; i<cantidadElementos; i++){
    Contenido* llave = contenidos ->obtenerElemento (i);
    int j=i-1;

    while(j>=0 && contenidos -> contenidos->obtenerElemento(j)->getTitulo() > llave->getTitulo()){
    contenidos->reemplazar(j+1, contenidos->obtenerElemento(j));
    j--;
    }
    contenidos->reemplazar(j+1, llave);
    }
    cout<< "Lista ordenada por titulo con exito." <<endl;
}

    void filtrar(function<bool(Contenido*)> criterio){
        bool encontrado=false;
        cout<<"--- Resultados del filtrado ---"<<endl;

        for(int i=0; i<cantidadElementos; i++){
            Contenido*item=contenidos->obtenerElemento(i);
            if(criterio(item)){
                item->mostrarDetalle();
                encontrado=true;
    }
}
    if (!encontrado){
        cout<<"No se encontraron elementos que coincidan con el criterio."<<endl;
    }
}

private:    
    ListaGenerica<Contenido*>* contenidos;
    int cantidadElementos;
    };

class Episodio
{
public:
    Episodio(int numero, const string& titulo, int duracion)
        : numero(numero), titulo(titulo), duracion(duracion) {}
    ~Episodio() {}

    int         getNumero()   const { return numero; }
    string getTitulo()   const { return titulo; }
    int         getDuracion() const { return duracion; }

private:
    int         numero;
    string titulo;
    int         duracion; 
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

    int              getNumero()    const { return numero; }
    int              getAnio()      const { return anio; }
    Lista<Episodio*>& getEpisodios()      { return episodios; }

private:
    int              numero;
    int              anio;
    Lista<Episodio*> episodios;
};

class MotodoPago
{
public:
    MotodoPago(int id, const string& tipo,
               const string& numero, const string& titular)
        : id(id), tipo(tipo), numero(numero), titular(titular) {}
    ~MotodoPago() {}

    int         getId()      const { return id; }
    string getTipo()    const { return tipo; }
    string getNumero()  const { return numero; }
    string getTitular() const { return titular; }

private:
    int         id;
    string tipo;     
    string numero;   
    string titular;
};

class Calificacion
{
public:
    Calificacion(int puntaje, const string& comentario,
                 const string& usuario)
        : puntaje(puntaje), comentario(comentario), usuario(usuario) {}
    ~Calificacion() {}

    int         getPuntaje()    const { return puntaje; }
    string getComentario() const { return comentario; }
    string getUsuario()    const { return usuario; }

private:
    int         puntaje;   
    string comentario;
    string usuario;
};

class Factura
{
public:
    Factura(string id, string fecha, float monto, string metodo)
        : idFactura(id), fechaEmision(fecha), montoTotal(monto), metodoPago(metodoPago), estadoPago(true) {}
    ~Factura() {}

    string getIdFactura() const { return idFactura; }
    float getMontoTotal() const { return montoTotal; }
    
    void generarRecibo() const{
        cout<<"Factura: "<<idFactura<<" | Total: S/."<<montoTotal<<endl;
    }
   
private:
    string idFactura;
    string fechaEmision;
    float montoTotal;
    string metodoPago;
    bool estadoPago;
};

class Serie
{
public:
    Serie(int id, const string& titulo,
          const string& genero, const string& descripcion)
        : id(id), titulo(titulo), genero(genero), descripcion(descripcion) {}
    ~Serie() {
        Nodo<Temporada*>* t = temporadas.getCabeza();
        while (t)   { delete t->dato;   t = t->next; }
        Nodo<Calificacion*>* c = calificaciones.getCabeza();
        while (c)   { delete c->dato;   c = c->next; }
    }

    void agregarTemporada(Temporada* t)     { temporadas.insertarFinal(t); }
    void agregarCalificacion(Calificacion* c) { calificaciones.insertarFinal(c); }

    float getCalificacionPromedio() const {
        if (calificaciones.estaVacia()) return 0.0f;
        float suma = 0;
        Nodo<Calificacion*>* actual = calificaciones.getCabeza();
        while (actual) { suma += actual->dato->getPuntaje(); actual = actual->next; }
        return suma / calificaciones.tamanio();
    }

    int                  getId()            const { return id; }
    string          getTitulo()        const { return titulo; }
    string          getGenero()        const { return genero; }
    string          getDescripcion()   const { return descripcion; }
    Lista<Temporada*>&   getTemporadas()          { return temporadas; }
    Lista<Calificacion*>& getCalificaciones()     { return calificaciones; }

private:
    int                  id;
    string          titulo;
    string          genero;
    string          descripcion;
    Lista<Temporada*>    temporadas;
    Lista<Calificacion*> calificaciones;
};
