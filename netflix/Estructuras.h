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

    bool buscar(T dato) const {
        Nodo<T>* actual = cabeza;
        while (actual) {
            if (actual->dato == dato) return true;
            actual = actual->next;
        }
        return false;
    }

    bool estaVacia() const { return cabeza == nullptr; }
    int  tamanio()   const { return _tamanio; }

    Nodo<T>* getCabeza() const { return cabeza; }

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

    T& tope() const {
        if (estaVacia()) cout << "Error: Pila vacia\n";
        return tope_->dato;
    }

    bool estaVacia() const { return tope_ == nullptr; }
    int  tamanio()   const { return _tamanio; }

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

    T& frente() const {
        if (estaVacia()) cout << "Error: Cola vacia\n";
        return frente_->dato;
    }

    bool estaVacia() const { return frente_ == nullptr; }
    int  tamanio()   const { return _tamanio; }

private:
    Nodo<T>* frente_;
    Nodo<T>* final_;
    int _tamanio;
};
