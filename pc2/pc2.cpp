
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <functional>

using namespace std;
//1. Estructura de pacientes

struct Paciente {
    string nombre;
    int edad;

    Paciente(string n = "", int e = 0) : nombre(n), edad(e) {}
};

// Función para mostrar un paciente
static void mostrarPaciente(const Paciente& p) {
    cout << p.nombre << " (" << p.edad << " anos)";
}

//Merge sort 

static void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

static void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


// 3. Ordenamiento con template

template<typename T>
void ordenamientoAvanzado(vector<T>& arr, function<bool(const T&, const T&)> comparar) {
    
    for (size_t i = 0; i < arr.size() - 1; i++) {
        for (size_t j = 0; j < arr.size() - i - 1; j++) {
            if (comparar(arr[j + 1], arr[j])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Función de comparación para pacientes: por nombre, luego por edad
static bool compararPaciente(const Paciente& a, const Paciente& b) {
    if (a.nombre != b.nombre)
        return a.nombre < b.nombre;
    return a.edad < b.edad;
}



// Busqueda binaria


static int busquedaBinaria(const vector<int>& arr, int objetivo) {
    int left = 0, right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == objetivo)
            return mid;
        else if (arr[mid] < objetivo)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}


// Hastable con colisiones


class HashTable {
private:
    struct Entrada {
        string clave;
        int valor;
        bool ocupado;
        Entrada() : clave(""), valor(0), ocupado(false) {}
    };

    vector<Entrada> tabla;
    int tamano;
    int numElementos;

    // Función hash que reconoce letras 
    int funcionHash(const string& clave) const {
        unsigned long hash = 0;
        for (char c : clave) {
            hash = hash * 31 + tolower(c);  
        }
        return hash % tamano;
    }

public:
    HashTable(int size = 101) : tamano(size), numElementos(0) {
        tabla.resize(tamano);
    }

    void insertar(const string& clave, int valor) {
        if (numElementos >= tamano) {
            cout << "Tabla llena" << endl;
            return;
        }

        int indice = funcionHash(clave);
        int intentos = 0;

        // Prueba lineal para resolver colisiones
        while (tabla[indice].ocupado && tabla[indice].clave != clave) {
            indice = (indice + 1) % tamano;
            intentos++;
            if (intentos >= tamano) {
                cout << "No se pudo insertar " << clave << endl;
                return;
            }
        }

        if (!tabla[indice].ocupado) {
            tabla[indice].ocupado = true;
            numElementos++;
        }
        tabla[indice].clave = clave;
        tabla[indice].valor = valor;
    }

    int buscar(const string& clave) {
        int indice = funcionHash(clave);
        int intentos = 0;

        while (tabla[indice].ocupado) {
            if (tabla[indice].clave == clave) {
                return tabla[indice].valor;
            }
            indice = (indice + 1) % tamano;
            intentos++;
            if (intentos >= tamano) break;
        }
        return -1;  // No encontrado
    }

    void mostrar() {
        for (int i = 0; i < tamano; i++) {
            if (tabla[i].ocupado) {
                cout << i << ": " << tabla[i].clave << " -> " << tabla[i].valor << endl;
            }
        }
    }
};

// Arboles binarios de busqueda con puntero a funcion


template<typename T>
class NodoABB {
public:
    T dato;
    NodoABB* izquierda;
    NodoABB* derecha;

    NodoABB(T d) : dato(d), izquierda(nullptr), derecha(nullptr) {}
};

template<typename T>
class ArbolBinarioBusqueda {
private:
    NodoABB<T>* raiz;

    bool (*criterioComparacion)(const T&, const T&);

    NodoABB<T>* insertarRec(NodoABB<T>* nodo, T dato) {
        if (nodo == nullptr) {
            return new NodoABB<T>(dato);
        }

        if (criterioComparacion(dato, nodo->dato)) {
            nodo->izquierda = insertarRec(nodo->izquierda, dato);
        }
        else if (criterioComparacion(nodo->dato, dato)) {
            nodo->derecha = insertarRec(nodo->derecha, dato);
        }

        return nodo;
    }

    // Buscar recursivo
    NodoABB<T>* buscarRec(NodoABB<T>* nodo, T dato) {
        if (nodo == nullptr) return nullptr;
        if (nodo->dato == dato) return nodo;

        if (criterioComparacion(dato, nodo->dato)) {
            return buscarRec(nodo->izquierda, dato);
        }
        else {
            return buscarRec(nodo->derecha, dato);
        }
    }

    // Encontrar mínimo (para eliminar con dos hijos)
    NodoABB<T>* encontrarMin(NodoABB<T>* nodo) {
        while (nodo && nodo->izquierda != nullptr) {
            nodo = nodo->izquierda;
        }
        return nodo;
    }

    // Eliminar recursivo
    NodoABB<T>* eliminarRec(NodoABB<T>* nodo, T dato) {
        if (nodo == nullptr) return nullptr;

        if (criterioComparacion(dato, nodo->dato)) {
            nodo->izquierda = eliminarRec(nodo->izquierda, dato);
        }
        else if (criterioComparacion(nodo->dato, dato)) {
            nodo->derecha = eliminarRec(nodo->derecha, dato);
        }
        else {
            // Nodo encontrado

            // Caso 1: Hoja
            if (nodo->izquierda == nullptr && nodo->derecha == nullptr) {
                delete nodo;
                return nullptr;
            }
            // Caso 2: Solo hijo izquierdo
            else if (nodo->derecha == nullptr) {
                NodoABB<T>* temp = nodo->izquierda;
                delete nodo;
                return temp;
            }
            // Caso 3: Solo hijo derecho
            else if (nodo->izquierda == nullptr) {
                NodoABB<T>* temp = nodo->derecha;
                delete nodo;
                return temp;
            }
            // Caso 4: Dos hijos - usar el mínimo del subárbol derecho
            else {
                NodoABB<T>* minNodo = encontrarMin(nodo->derecha);
                nodo->dato = minNodo->dato;
                nodo->derecha = eliminarRec(nodo->derecha, minNodo->dato);
            }
        }
        return nodo;
    }

    void inOrdenRec(NodoABB<T>* nodo, int profundidad, int maxProfundidad) {
        if (nodo == nullptr || profundidad > maxProfundidad) return;

        inOrdenRec(nodo->izquierda, profundidad + 1, maxProfundidad);
        cout << nodo->dato << " ";
        inOrdenRec(nodo->derecha, profundidad + 1, maxProfundidad);
    }

    // Recorrido PreOrden con límite de profundidad
    void preOrdenRec(NodoABB<T>* nodo, int profundidad, int maxProfundidad) {
        if (nodo == nullptr || profundidad > maxProfundidad) return;

        cout << nodo->dato << " ";
        preOrdenRec(nodo->izquierda, profundidad + 1, maxProfundidad);
        preOrdenRec(nodo->derecha, profundidad + 1, maxProfundidad);
    }

    // Recorrido PostOrden con límite de profundidad
    void postOrdenRec(NodoABB<T>* nodo, int profundidad, int maxProfundidad) {
        if (nodo == nullptr || profundidad > maxProfundidad) return;

        postOrdenRec(nodo->izquierda, profundidad + 1, maxProfundidad);
        postOrdenRec(nodo->derecha, profundidad + 1, maxProfundidad);
        cout << nodo->dato << " ";
    }

    // Encontrar máximo (para verificación)
    T maximoRec(NodoABB<T>* nodo) {
        if (nodo == nullptr) return T(); // Retorna valor por defecto

        while (nodo->derecha != nullptr) {
            nodo = nodo->derecha;
        }
        return nodo->dato;
    }

public:
    ArbolBinarioBusqueda(bool (*comp)(const T&, const T&))
        : raiz(nullptr), criterioComparacion(comp) {
    }

    void insertar(T dato) {
        raiz = insertarRec(raiz, dato);
    }

    NodoABB<T>* buscar(T dato) {
        return buscarRec(raiz, dato);
    }

    bool eliminar(T dato) {
        if (buscar(dato) == nullptr) return false;
        raiz = eliminarRec(raiz, dato);
        return true;
    }

    T maximo() {
        if (raiz == nullptr) return T();
        return maximoRec(raiz);
    }

    // Recorridos con profundidad
    void inOrden(int maxProfundidad) {
        inOrdenRec(raiz, 1, maxProfundidad);
        cout << endl;
    }

    void preOrden(int maxProfundidad) {
        preOrdenRec(raiz, 1, maxProfundidad);
        cout << endl;
    }

    void postOrden(int maxProfundidad) {
        postOrdenRec(raiz, 1, maxProfundidad);
        cout << endl;
    }

    bool vacio() {
        return raiz == nullptr;
    }

    // Función para contar mayores de edad usando lambda
    int contarMayoresDeEdad() {
        int contador = 0;
        function<void(NodoABB<T>*)> contar = [&](NodoABB<T>* nodo) {
            if (nodo == nullptr) return;
            if (nodo->dato >= 18) contador++;
            contar(nodo->izquierda);
            contar(nodo->derecha);
            };
        contar(raiz);
        return contador;
    }

    // Función para verificar duplicados
    bool tieneDuplicados() {
        vector<T> valores;
        function<void(NodoABB<T>*)> recolectar = [&](NodoABB<T>* nodo) {
            if (nodo == nullptr) return;
            valores.push_back(nodo->dato);
            recolectar(nodo->izquierda);
            recolectar(nodo->derecha);
            };
        recolectar(raiz);

        sort(valores.begin(), valores.end());
        for (size_t i = 1; i < valores.size(); i++) {
            if (valores[i] == valores[i - 1]) return true;
        }
        return false;
    }
};


// Lamdas operacion del arbol

auto esMayorDeEdad = [](int edad) -> bool {
    return edad >= 18;
    };

auto esMenorDeEdad = [](int edad) -> bool {
    return edad < 18;
    };


auto esTerceraEdad = [](int edad) -> bool {
    return edad >= 60;
    };

bool compararInt(const int& a, const int& b) {
    return a < b;
};


//funcion principal
int main() {
    srand(time(nullptr));

    cout << "=== SISTEMA DE REGISTRO DE PACIENTES ===\n\n";

    // REQUERIMIENTO 1: Generar 300 edades aleatorias y ordenar con Merge Sort
    cout << "1. Generando 300 edades aleatorias...\n";
    vector<int> edades;
    for (int i = 0; i < 300; i++) {
        edades.push_back(rand() % 100 + 1); 
    }

    cout << "   Ordenando con Merge Sort...\n";
    mergeSort(edades, 0, edades.size() - 1);

    cout << "   Reduciendo cada edad en 1 ano (simulando error de registro)...\n";
    for (int& edad : edades) {
        edad--; 
    }

    cout << "   Primeras 10 edades despues de ordenar y reducir: ";
    for (int i = 0; i < 10 && i < edades.size(); i++) {
        cout << edades[i] << " ";
    }
    cout << "...\n\n";



    // REQUERIMIENTO 2: Ordenamiento avanzado con templates
    cout << "2. Ordenamiento avanzado con templates y funcion de comparacion:\n";

    vector<Paciente> pacientes = {
        {"Maria Lopez", 34},
        {"Juan Perez", 17},
        {"Ana Torres", 65},
        {"Luis Ramirez", 70},
        {"Carla Mendoza", 15}
    };

    cout << "   Pacientes originales:\n";
    for (const auto& p : pacientes) {
        cout << "     "; mostrarPaciente(p); cout << endl;
    }

    ordenamientoAvanzado<Paciente>(pacientes, compararPaciente);

    cout << "   Pacientes ordenados (por nombre, luego edad):\n";
    for (const auto& p : pacientes) {
        cout << "     "; mostrarPaciente(p); cout << endl;
    }
    cout << endl;


    // REQUERIMIENTO 3: Búsqueda binaria
    cout << "3. Busqueda binaria:\n";
    int edadBuscada;
    cout << "   Ingrese una edad para buscar: ";
    cin >> edadBuscada;

    int posicion = busquedaBinaria(edades, edadBuscada);
    if (posicion != -1) {
        cout << "   Edad " << edadBuscada << " encontrada en la posicion " << posicion << endl;
    }
    else {
        cout << "   Edad " << edadBuscada << " no encontrada\n";
    }
    cout << endl;


    // REQUERIMIENTO 4: HashTable con colisiones
    cout << "4. Tabla Hash con colisiones:\n";
    HashTable ht(101);

    for (const auto& p : pacientes) {
        ht.insertar(p.nombre, p.edad);
        cout << "   Insertado: " << p.nombre << " -> " << p.edad << endl;
    }

    cout << "\n   Buscando en la HashTable:\n";
    for (const auto& p : pacientes) {
        int edad = ht.buscar(p.nombre);
        cout << "     " << p.nombre << " -> " << edad << endl;
    }
    cout << endl;


    // REQUERIMIENTO 5: Árbol binario de búsqueda con puntero a función
    cout << "5. Arbol binario de busqueda:\n";


   

    ArbolBinarioBusqueda<int> arbol(compararInt);


    int numInsertar = min(20, (int)edades.size());
    cout << "   Insertando " << numInsertar << " edades en el arbol...\n";
    for (int i = 0; i < numInsertar; i++) {
        arbol.insertar(edades[i]);
    }

    cout << endl;


    // REQUERIMIENTO 6: Recorridos con límite de profundidad

    cout << "6. Recorridos con limite de profundidad:\n";
    int profundidadMaxima;
    cout << "   Ingrese la profundidad maxima para los recorridos: ";
    cin >> profundidadMaxima;

    cout << "   InOrden (profundidad <= " << profundidadMaxima << "): ";
    arbol.inOrden(profundidadMaxima);

    cout << "   PreOrden (profundidad <= " << profundidadMaxima << "): ";
    arbol.preOrden(profundidadMaxima);

    cout << "   PostOrden (profundidad <= " << profundidadMaxima << "): ";
    arbol.postOrden(profundidadMaxima);
    cout << endl;


    // REQUERIMIENTO 7: Uso de lambdas libres-
    cout << "7. Uso de lambdas libres:\n";


    int mayoresDeEdad = arbol.contarMayoresDeEdad();
    cout << "   Mayores de edad (>= 18 anos): " << mayoresDeEdad << endl;


    bool duplicados = arbol.tieneDuplicados();
    cout << "   Hay edades repetidas en el arbol?: " << (duplicados ? "Si" : "No") << endl;

    auto contarTerceraEdad = [&arbol]() {
        int contador = 0;
        function<void(NodoABB<int>*)> recorrer = [&](NodoABB<int>* nodo) {
            if (nodo == nullptr) return;
            if (esTerceraEdad(nodo->dato)) contador++;
            recorrer(nodo->izquierda);
            recorrer(nodo->derecha);
            };
        recorrer(arbol.buscar(0) == nullptr ? nullptr : arbol.buscar(0)); 
      
        return contador;
        };

    cout << endl;


    // REQUERIMIENTO 8: Verificar valor máximo del árbol

    cout << "8. Verificando valor maximo del arbol:\n";
    int maxEdad = arbol.maximo();
    cout << "   Edad maxima en el arbol: " << maxEdad << endl;

    if (maxEdad > 65) {
        cout << "   Hay pacientes mayores de 65 anos\n";
    }
    else {
        cout << "   No hay pacientes mayores de 65 anos\n";
    }
    cout << endl;

    // REQUERIMIENTO 9: Eliminar un nodo del árbol

    cout << "9. Eliminando un nodo del arbol:\n";
    if (!arbol.vacio()) {
        int edadEliminar = edades[rand() % min(20, (int)edades.size())];
        cout << "   Intentando eliminar la edad: " << edadEliminar << endl;

        if (arbol.eliminar(edadEliminar)) {
            cout << "   Edad " << edadEliminar << " eliminada correctamente\n";
            if (esMenorDeEdad(edadEliminar)) {
                cout << "   Se elimino un paciente menor de edad! (<18 anos)\n";
            }
        }
        else {
            cout << "   No se encontro la edad " << edadEliminar << " en el arbol\n";
        }
    }
    cout << endl;

    cout << "=== FIN DEL PROGRAMA ===\n";

    return 0;
}

