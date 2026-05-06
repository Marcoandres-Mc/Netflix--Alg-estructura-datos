#pragma once
#include <string>
#include "Estructuras.h"
using namespace std;

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
    Factura(int id, float monto, const string& fecha, MotodoPago* metodoPago)
        : id(id), monto(monto), fecha(fecha), metodoPago(metodoPago) {}
    ~Factura() {}

    int          getId()          const { return id; }
    float        getMonto()       const { return monto; }
    string  getFecha()       const { return fecha; }
    MotodoPago*  getMetodoPago()  const { return metodoPago; }

private:
    int         id;
    float       monto;
    string fecha;
    MotodoPago* metodoPago; 
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
