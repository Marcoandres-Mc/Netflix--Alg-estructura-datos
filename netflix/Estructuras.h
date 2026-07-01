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

// IMPLEMENTACION DE ARBOL BINARIO
template<class K, class V>
struct NodoBB {
    K clave;
    Lista<V> valores;
    NodoBB* izquierda;
    NodoBB* derecha;
    NodoBB(K c, V v) : clave(c), izquierda(nullptr), derecha(nullptr) {
        valores.insertarFinal(v);
    }
};

template<class K, class V>
class ArbolBB {
public:
    ArbolBB() : raiz(nullptr), _tamanio(0) {}
    ~ArbolBB() { destruir(raiz); }
    void insertar(K clave, V valor) { raiz = insertarRec(raiz, clave, valor); }
 bool buscar(K clave, Lista<V>& resultado) {
        NodoBB<K, V>* n = buscarRec(raiz, clave);
        if (!n) return false;
        Nodo<V>* actual = n->valores.getCabeza();
        while (actual) { resultado.insertarFinal(actual->dato); actual = actual->next; }
        return true;
    }
bool contieneClave(K clave) { return buscarRec(raiz, clave) != nullptr; }

    NodoBB<K, V>* buscarNodo(K clave) { return buscarRec(raiz, clave); }

    bool eliminar(K clave) {
        bool eliminado = false;
        raiz = eliminarRec(raiz, clave, eliminado);
        return eliminado;
    }
 bool minimo(K& resultado) {
        if (!raiz) return false;
        NodoBB<K, V>* n = raiz;
        while (n->izquierda) n = n->izquierda;
        resultado = n->clave;
        return true;
    }

    bool maximo(K& resultado) {
        if (!raiz) return false;
        NodoBB<K, V>* n = raiz;
        while (n->derecha) n = n->derecha;
        resultado = n->clave;
        return true;
    }
 bool sucesor(K clave, K& resultado) {
        NodoBB<K, V>* objetivo = buscarRec(raiz, clave);
        if (!objetivo) return false;

        if (objetivo->derecha) {
            NodoBB<K, V>* n = objetivo->derecha;
            while (n->izquierda) n = n->izquierda;
            resultado = n->clave;
            return true;
        }
        NodoBB<K, V>* candidato = nullptr;
        NodoBB<K, V>* actual = raiz;
        while (actual && !(actual->clave == clave)) {
            if (clave < actual->clave) { candidato = actual; actual = actual->izquierda; }
            else                       { actual = actual->derecha; }
        }
        if (!candidato) return false;
        resultado = candidato->clave;
        return true;
    }

    bool predecesor(K clave, K& resultado) {
        NodoBB<K, V>* objetivo = buscarRec(raiz, clave);
        if (!objetivo) return false;

        if (objetivo->izquierda) {
            NodoBB<K, V>* n = objetivo->izquierda;
            while (n->derecha) n = n->derecha;
            resultado = n->clave;
            return true;
        }
        NodoBB<K, V>* candidato = nullptr;
        NodoBB<K, V>* actual = raiz;
        while (actual && !(actual->clave == clave)) {
            if (clave > actual->clave) { candidato = actual; actual = actual->derecha; }
            else                       { actual = actual->izquierda; }
        }
        if (!candidato) return false;
        resultado = candidato->clave;
        return true;
    }
 template<class Fn>
    void inOrden(Fn visitar) { inOrdenRec(raiz, visitar); }

    template<class Fn>
    void preOrden(Fn visitar) { preOrdenRec(raiz, visitar); }

    template<class Fn>
    void postOrden(Fn visitar) { postOrdenRec(raiz, visitar); }
void nodosPorNivel(Lista<int>& conteos) {
        if (!raiz) return;
        struct ItemNivel { NodoBB<K, V>* nodo; int nivel; };
        Cola<ItemNivel> cola;
        ItemNivel inicio; inicio.nodo = raiz; inicio.nivel = 0;
        cola.encolar(inicio);
        while (!cola.estaVacia()) {
            ItemNivel item = cola.frente();
            cola.desencolar();
            while (conteos.tamanio() <= item.nivel) conteos.insertarFinal(0);
            conteos.reemplazar(item.nivel, conteos.obtenerElemento(item.nivel) + 1);
            if (item.nodo->izquierda) {
                ItemNivel izq; izq.nodo = item.nodo->izquierda; izq.nivel = item.nivel + 1;
                cola.encolar(izq);
            }
            if (item.nodo->derecha) {
                ItemNivel der; der.nodo = item.nodo->derecha; der.nivel = item.nivel + 1;
                cola.encolar(der);
            }
        }
    }

    int altura() { return alturaRec(raiz); }
    int tamanio() const { return _tamanio; }
    bool estaVacio() const { return raiz == nullptr; }
    NodoBB<K, V>* getRaiz() { return raiz; }

private:
    NodoBB<K, V>* raiz;
    int _tamanio;

    NodoBB<K, V>* insertarRec(NodoBB<K, V>* nodo, K clave, V valor) {
        if (!nodo) { _tamanio++; return new NodoBB<K, V>(clave, valor); }
        if (clave < nodo->clave)      nodo->izquierda = insertarRec(nodo->izquierda, clave, valor);
        else if (nodo->clave < clave) nodo->derecha   = insertarRec(nodo->derecha, clave, valor);
        else                          nodo->valores.insertarFinal(valor); // clave repetida
        return nodo;
    }

    NodoBB<K, V>* buscarRec(NodoBB<K, V>* nodo, K clave) {
        if (!nodo) return nullptr;
        if (clave == nodo->clave) return nodo;
        if (clave < nodo->clave)  return buscarRec(nodo->izquierda, clave);
        return buscarRec(nodo->derecha, clave);
    }

    NodoBB<K, V>* eliminarRec(NodoBB<K, V>* nodo, K clave, bool& eliminado) {
        if (!nodo) return nullptr;
        if (clave < nodo->clave) {
            nodo->izquierda = eliminarRec(nodo->izquierda, clave, eliminado);
        } else if (nodo->clave < clave) {
            nodo->derecha = eliminarRec(nodo->derecha, clave, eliminado);
        } else {
            eliminado = true;
            if (!nodo->izquierda && !nodo->derecha) {
                // Caso 1: hoja -> se convierte en arbol vacio
                delete nodo; _tamanio--; return nullptr;
            } else if (!nodo->izquierda) {
                // Caso 2: izquierda vacia, derecha no -> sube la derecha
                NodoBB<K, V>* temp = nodo->derecha;
                delete nodo; _tamanio--; return temp;
            } else if (!nodo->derecha) {
                // Caso 3: derecha vacia, izquierda no -> sube la izquierda
                NodoBB<K, V>* temp = nodo->izquierda;
                delete nodo; _tamanio--; return temp;
                //Caso 4: dos hijos -> se reemplaza por el predecesor
                //(el nodo mas a la derecha del subarbol izquierdo).
            } else { NodoBB<K, V>* izqOriginal = nodo->izquierda;
                NodoBB<K, V>* derOriginal = nodo->derecha;

                NodoBB<K, V>* padrePred = nodo;
                NodoBB<K, V>* pred = izqOriginal;
                bool esHijoDirecto = true;
                while (pred->derecha) {
                    padrePred = pred;
                    pred = pred->derecha;
                    esHijoDirecto = false;
                }

                if (esHijoDirecto) {                   
                pred->derecha = derOriginal;
} else {
                    padrePred->derecha = pred->izquierda; 
                    pred->izquierda = izqOriginal;
                    pred->derecha   = derOriginal;
                }

                delete nodo;
                _tamanio--;
                return pred;
            }
        }
        return nodo;
    }

    template<class Fn>
    void inOrdenRec(NodoBB<K, V>* nodo, Fn visitar) {
        if (!nodo) return;
        inOrdenRec(nodo->izquierda, visitar);
        visitar(nodo);
        inOrdenRec(nodo->derecha, visitar);
    }

    template<class Fn>
    void preOrdenRec(NodoBB<K, V>* nodo, Fn visitar) {
        if (!nodo) return;
        visitar(nodo);
        preOrdenRec(nodo->izquierda, visitar);
        preOrdenRec(nodo->derecha, visitar);
    }

    template<class Fn>
    void postOrdenRec(NodoBB<K, V>* nodo, Fn visitar) {
        if (!nodo) return;
        postOrdenRec(nodo->izquierda, visitar);
        postOrdenRec(nodo->derecha, visitar);
        visitar(nodo);
    }

    int alturaRec(NodoBB<K, V>* nodo) {
        if (!nodo) return -1;
        int hi = alturaRec(nodo->izquierda);
        int hd = alturaRec(nodo->derecha);
        return 1 + (hi > hd ? hi : hd);
    }

    void destruir(NodoBB<K, V>* nodo) {
        if (!nodo) return;
        destruir(nodo->izquierda);
        destruir(nodo->derecha);
        delete nodo;
    }
};

