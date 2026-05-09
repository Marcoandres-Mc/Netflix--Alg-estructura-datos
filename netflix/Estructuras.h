#pragma once
#include <iostream>
using namespace std;

template<class T>
struct Nodo {
    T dato;
    Nodo* next;
    Nodo(T dato) : dato(dato), next(nullptr) {}
};


template<class T>
class Lista {
public:
    Lista() : cabeza(nullptr), _tamanio(0) {}

    ~Lista() {
        Nodo<T>* actual = cabeza;
        while (actual) {
            Nodo<T>* siguiente = actual->next;
            delete actual;
            actual = siguiente;
        }
    }

    void insertarInicio(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        nuevo->next = cabeza;
        cabeza = nuevo;
        _tamanio++;
    }

    void insertarFinal(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (!cabeza) { cabeza = nuevo; }
        else {
            Nodo<T>* actual = cabeza;
            while (actual->next) actual = actual->next;
            actual->next = nuevo;
        }
        _tamanio++;
    }

    T obtenerElemento(int indice){
    if (indice<0 || indice >= _tamanio) return T();
    Nodo<T>* actual=cabeza;
    for( int i=0; i<indice; i++){
        actual=actual->next;
    }
    return actual->dato;
}

    void reemplazar (int indice, T nuevoDato){
    if(indice<0 || indice>= _tamanio) return;
    Nodo<T>* actual=cabeza;
    for(int i=0; i<indice; i++){
        actual=actual->next;
    }
    actual->dato = nuevoDato;
}

    bool buscarRecursivo(Nodo<T>* nodo, T dato){
    if (nodo==nullptr) return false;
    if(nodo->dato==dato) return true;
    return buscarRecursivo(nodo->next, dato);
}

void vaciar(){
    while(cabeza != nullptr){
        Nodo<T>* temp = cabeza;
        cabeza=cabeza->next;
        delete temp;
    }
    _tamanio=0;
}

    void eliminar(T dato) {
        if (!cabeza) return;
        if (cabeza->dato == dato) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->next;
            delete temp;
            _tamanio--;
            return;
        }
        Nodo<T>* actual = cabeza;
        while (actual->next && actual->next->dato != dato)
            actual = actual->next;
        if (actual->next) {
            Nodo<T>* temp = actual->next;
            actual->next = temp->next;
            delete temp;
            _tamanio--;
        }
    }

    bool buscar(T dato) {
        Nodo<T>* actual = cabeza;
        while (actual) {
            if (actual->dato == dato) return true;
            actual = actual->next;
        }
        return false;
    }

    bool estaVacia() { return cabeza == nullptr; }
    int  tamanio()   { return _tamanio; }

    Nodo<T>* getCabeza() { return cabeza; }

private:
    Nodo<T>* cabeza;
    int _tamanio;
};

template<class T>
class Pila {
public:
    Pila() : tope_(nullptr), _tamanio(0) {}

    ~Pila() {
        while (!estaVacia()) pop();
    }

    void push(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        nuevo->next = tope_;
        tope_ = nuevo;
        _tamanio++;
    }

    void pop() {
        if (estaVacia()) { cout << "Error: Pila vacia\n"; return; }
        Nodo<T>* temp = tope_;
        tope_ = tope_->next;
        delete temp;
        _tamanio--;
    }

    T& tope() {
        if (estaVacia()) cout << "Error: Pila vacia\n";
        return tope_->dato;
    }

    bool buscar(T dato) {
        Nodo<T>* actual = tope_;
        while (actual) {
            if (actual->dato == dato) return true;
            actual = actual->next;
        }
        return false;
    }

    void vaciar() {
        while (!estaVacia()) pop();
    }

    bool contieneDuplicados() {
        Nodo<T>* actual = tope_;
        while (actual) {
            Nodo<T>* siguiente = actual->next;
            while (siguiente) {
                if (actual->dato == siguiente->dato) return true;
                siguiente = siguiente->next;
            }
            actual = actual->next;
        }
        return false;
    }

    bool     estaVacia() { return tope_ == nullptr; }
    int      tamanio()   { return _tamanio; }
    Nodo<T>* getTope()   { return tope_; }

private:
    Nodo<T>* tope_;
    int _tamanio;
};


template<class T>
class Cola {
public:
    Cola() : frente_(nullptr), final_(nullptr), _tamanio(0) {}

    ~Cola() {
        while (!estaVacia()) desencolar();
    }

    void encolar(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (!final_) { frente_ = final_ = nuevo; }
        else {
            final_->next = nuevo;
            final_ = nuevo;
        }
        _tamanio++;
    }

    void desencolar() {
        if (estaVacia()) { cout << "Error: Cola vacia\n"; return; }
        Nodo<T>* temp = frente_;
        frente_ = frente_->next;
        if (!frente_) final_ = nullptr;
        delete temp;
        _tamanio--;
    }

    T& frente() {
        if (estaVacia()) cout << "Error: Cola vacia\n";
        return frente_->dato;
    }

    bool contiene(T dato) {
        Nodo<T>* actual = frente_;
        while (actual) {
            if (actual->dato == dato) return true;
            actual = actual->next;
        }
        return false;
    }

    void eliminarEn(int pos) {
        if (pos < 0 || pos >= _tamanio) return;
        if (pos == 0) { desencolar(); return; }
        Nodo<T>* actual = frente_;
        for (int i = 0; i < pos - 1; i++) actual = actual->next;
        Nodo<T>* temp = actual->next;
        actual->next = temp->next;
        if (temp == final_) final_ = actual;
        delete temp;
        _tamanio--;
    }

    void vaciar() {
        while (!estaVacia()) desencolar();
    }

    bool     estaVacia()  { return frente_ == nullptr; }
    int      tamanio()    { return _tamanio; }
    Nodo<T>* getFrente()  { return frente_; }

private:
    Nodo<T>* frente_;
    Nodo<T>* final_;
    int _tamanio;
};
