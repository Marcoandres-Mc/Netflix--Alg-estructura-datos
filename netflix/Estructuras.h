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


// ─────────────────────────────────────────────────────
//  Nodo par clave-valor para colisiones en tabla hash
// ─────────────────────────────────────────────────────
template<class K, class V>
struct NodoPar {
    K clave;
    V valor;
    NodoPar<K, V>* siguiente;
    NodoPar(const K& c, const V& v) : clave(c), valor(v), siguiente(nullptr) {}
};

// Lista enlazada de pares (encadenamiento separado)
template<class K, class V>
struct ListaHash {
    NodoPar<K, V>* cabeza;

    ListaHash() : cabeza(nullptr) {}

    // Inserta o actualiza si la clave ya existe
    void insertar(const K& clave, const V& valor) {
        NodoPar<K, V>* actual = cabeza;
        while (actual) {
            if (actual->clave == clave) { actual->valor = valor; return; }
            actual = actual->siguiente;
        }
        NodoPar<K, V>* nuevo = new NodoPar<K, V>(clave, valor);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    // Devuelve V() (nullptr para punteros) si no encuentra la clave
    V buscar(const K& clave) const {
        NodoPar<K, V>* actual = cabeza;
        while (actual) {
            if (actual->clave == clave) return actual->valor;
            actual = actual->siguiente;
        }
        return V();
    }

    bool eliminar(const K& clave) {
        if (!cabeza) return false;
        if (cabeza->clave == clave) {
            NodoPar<K, V>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return true;
        }
        NodoPar<K, V>* ant = cabeza;
        while (ant->siguiente) {
            if (ant->siguiente->clave == clave) {
                NodoPar<K, V>* temp = ant->siguiente;
                ant->siguiente = temp->siguiente;
                delete temp;
                return true;
            }
            ant = ant->siguiente;
        }
        return false;
    }

    void limpiar() {
        while (cabeza) {
            NodoPar<K, V>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

// ─────────────────────────────────────────────────────
//  Tabla Hash con encadenamiento separado
//  Clave: string  |  Valor: V (cualquier tipo)
//  Hash polinomial rolling con base 31
// ─────────────────────────────────────────────────────
template<class V>
class TablaHash {
    int tam;
    ListaHash<string, V>* tabla;

    int calcularHash(const string& clave) const {
        unsigned long h = 0;
        for (char c : clave) h = h * 31 + c;
        return (int)(h % (unsigned long)tam);
    }

public:
    explicit TablaHash(int tam = 17) : tam(tam) {
        tabla = new ListaHash<string, V>[tam];
    }

    ~TablaHash() {
        for (int i = 0; i < tam; i++) tabla[i].limpiar();
        delete[] tabla;
    }

    void insertar(const string& clave, const V& valor) {
        tabla[calcularHash(clave)].insertar(clave, valor);
    }

    // Devuelve nullptr si la clave no existe (para tipos puntero)
    V buscar(const string& clave) const {
        return tabla[calcularHash(clave)].buscar(clave);
    }

    bool eliminar(const string& clave) {
        return tabla[calcularHash(clave)].eliminar(clave);
    }

    int getTam() const { return tam; }
};
