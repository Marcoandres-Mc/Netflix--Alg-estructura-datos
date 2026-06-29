#include <iostream>
#include <string>
using namespace std;

struct Examen {
	int numeroExamen = 0;
	int cantidadPreguntas = 0;
	string fecha = "";
	double calificacion = 0.0;
};

struct Practica {
	string codigo = "";
	string titulo = "";
	int dificultad = 0;
	string fecha = "";
	double nota = 0.0;
};

struct Alumno {
	string nombreCompleto = "";
	string grupo = "";
	int maticula = 0;
};

struct NodoExamen {
	Examen examen;
	NodoExamen* siguiente;
	NodoExamen(Examen examen) {
		this->examen = examen;
		this->siguiente = nullptr;
	}
};

struct NodoPractica {
	Practica practica;
	NodoPractica* siguiente;

	NodoPractica(Practica practica) {
		this->practica = practica;
		this->siguiente = nullptr;
	}
};

struct NodoAlumno {
	Alumno alumno;
	NodoExamen* examenes;
	NodoPractica* practicas;
	NodoAlumno* siguiente;
	NodoAlumno(Alumno alumno) {
		this->alumno = alumno;
		this->examenes = nullptr;
		this->practicas = nullptr;
		this->siguiente = nullptr;
	}
};;

class SistemaAcademico {
private:
	NodoAlumno* cabeza;

	bool existeMatricula(int matricula) {
		NodoAlumno* actual = cabeza;
		while (actual != nullptr) {
			if (actual->alumno.maticula == matricula) {
				return true;
			}
			actual = actual->siguiente;
		}
		return false;
	};
	void agregarExamenAAlumno(NodoAlumno* nodoAlumno, Examen examen) {
		NodoExamen* nuevoNodoExamen = new NodoExamen(examen);
		if (nodoAlumno->examenes == nullptr) {
			nodoAlumno->examenes = nuevoNodoExamen;
		}
		else {
			NodoExamen* actual = nodoAlumno->examenes;
			while (actual->siguiente != nullptr) {
				actual = actual->siguiente;
			}
			actual->siguiente = nuevoNodoExamen;
		}
	};

	void agregarPracticaAAlumno(NodoAlumno* nodoAlumno, Practica practica) {
		NodoPractica* nuevoNodoPractica = new NodoPractica(practica);
		if (nodoAlumno->practicas == nullptr) {
			nodoAlumno->practicas = nuevoNodoPractica;
		}
		else {
			NodoPractica* actual = nodoAlumno->practicas;
			while (actual->siguiente != nullptr) {
				actual = actual->siguiente;
			}
			actual->siguiente = nuevoNodoPractica;
		}
	};
public:
	SistemaAcademico() {
		cabeza = nullptr;
	};
	void agregarAlumno() {
		Alumno alumno;
		do {
			cout << "Ingrese numero de matricula: ";
			cin >> alumno.maticula;

			if (alumno.maticula <= 0) {
				cout << "La matricula debe ser mayor que 0" << endl;
			}
			else if (existeMatricula(alumno.maticula)) {
				cout << "La matricula ya existe. Ingrese una matricula diferente." << endl;
			}
		} while (alumno.maticula <= 0 || existeMatricula(alumno.maticula));
		
		cin.ignore();
		do {
			cout << "Ingrese nombre completo: ";
			getline(cin, alumno.nombreCompleto);

			if (alumno.nombreCompleto == "") {
				cout << "El nombre completo no puede estar vacio" << endl;
			}
		} while (alumno.nombreCompleto == "");

		do {
			cout << "Ingrese grupo (A, B o otro):  ";
			getline(cin, alumno.grupo);
			if (alumno.grupo == "") {
				cout << "El grupo no puede estar vacio" << endl;
			}
		} while (alumno.grupo == "");

		NodoAlumno* nuevoNodoAlumno = new NodoAlumno(alumno);
		int cantidadExamenes;
		do {
			cout << "Ingrese cantidad de examenes del alumno: ";
			cin >> cantidadExamenes;
			if (cantidadExamenes < 0) {
				cout << "La cantidad de examenes no puede ser negativa" << endl;
			}
		} while (cantidadExamenes < 0);
		
		for (int i = 0; i < cantidadExamenes; i++) {
			Examen examen;

			cout << "\nExamen " << i + 1 << endl;

			do {
				cout << "Ingrese numero de examen: ";
				cin >> examen.numeroExamen;
				
			} while (examen.numeroExamen <= 0);

			do {
				cout << "Ingrese cantidad de preguntas: ";
				cin >> examen.cantidadPreguntas;
			} while (examen.cantidadPreguntas <= 0);
			cin.ignore();

			cout << "Fecha de realizacion del examen: ";
			getline(cin, examen.fecha);

			do {
				cout << "Ingrese calificacion del examen: ";
				cin >> examen.calificacion;
				
			} while (examen.calificacion < 0 || examen.calificacion > 20);

			agregarExamenAAlumno(nuevoNodoAlumno, examen);

		}

		int cantidadPracticas;

		do {
			cout << "Ingrese cantidad de practicas del alumno: ";
			cin >> cantidadPracticas;
			if (cantidadPracticas < 0) {
				cout << "La cantidad de practicas no puede ser negativa" << endl;
			}
		} while (cantidadPracticas < 0);

		for (int i = 0; i < cantidadPracticas; i++) {
			Practica practica;
			cout << "\nPractica " << i + 1 << endl;
			cin.ignore();
			do {
				cout << "Ingrese codigo de la practica: ";
				getline(cin, practica.codigo);
				if (practica.codigo == "") {
					cout << "El codigo de la practica no puede estar vacio" << endl;
				}
			} while (practica.codigo == "");
			
			do {
				cout << "Ingrese titulo de la practica: ";
				getline(cin, practica.titulo);
				if (practica.titulo == "") {
					cout << "El titulo de la practica no puede estar vacio" << endl;
				}
			} while (practica.titulo == "");
			
			do {
				cout << "Ingrese dificultad de la practica (1-5): ";
				cin >> practica.dificultad;
				if (practica.dificultad < 1 || practica.dificultad > 5) {
					cout << "La dificultad debe ser entre 1 y 5" << endl;
				}
			} while (practica.dificultad < 1 || practica.dificultad > 5);
			cin.ignore();
			cout << "Fecha de realizacion de la practica: ";
			getline(cin, practica.fecha);

			do {
				cout << "Ingrese nota de la practica: ";
				cin >> practica.nota;

			} while (practica.nota < 0 || practica.nota > 20);
			agregarPracticaAAlumno(nuevoNodoAlumno, practica);
		}

		if (cabeza == nullptr) {
			cabeza = nuevoNodoAlumno;
		}
		else {
			NodoAlumno* actual = cabeza;
			while (actual->siguiente != nullptr) {
				actual = actual->siguiente;
			}
			actual->siguiente = nuevoNodoAlumno;
		}

		cout << "Alumno agregado exitosamente" << endl;
	};

	void examenMenorCantidadPreguntasDivisiblePor3() {
		if(cabeza == nullptr) {
			cout << "No hay alumnos registrados" << endl;
			return;
		}
		bool encontrado = false;
		Examen menorExamen;
		Alumno alumnoEncontrado;

		NodoAlumno* actualAlumno = cabeza;

		while (actualAlumno != nullptr) {
			NodoExamen* examenActual = actualAlumno->examenes;
			while (examenActual != nullptr) {
				int preguntas = examenActual->examen.cantidadPreguntas;

				if (preguntas % 3 == 0) {
					if (!encontrado || preguntas < menorExamen.cantidadPreguntas) {
						menorExamen = examenActual->examen;
						alumnoEncontrado = actualAlumno->alumno;
						encontrado = true;
					}
				}
				
				examenActual = examenActual->siguiente;
			}
			actualAlumno = actualAlumno->siguiente;
		}

		if (encontrado) {
			cout << "Alumno con examen de menor cantidad de preguntas divisible por 3:" << endl;
			cout << "Alumno: " << alumnoEncontrado.nombreCompleto << endl;
			cout << "Matricula: " << alumnoEncontrado.maticula << endl;
			cout << "Grupo: " << alumnoEncontrado.grupo << endl;
			cout << "Numero de examen: " << menorExamen.numeroExamen << endl;
			cout << "Cantidad de preguntas: " << menorExamen.cantidadPreguntas << endl;
			cout << "Fecha: " << menorExamen.fecha << endl;
			cout << "Calificacion: " << menorExamen.calificacion << endl;
		}
		else {
			cout << "No se encontraron examenes con cantidad de preguntas divisible por 3" << endl;
		}

	}

	void practicaMayorDificultadMultiploDe5() {
		if (cabeza == nullptr) {
			cout << "No hay alumnos registrados" << endl;
			return;
		}

		bool encontrado = false;
		Practica practicaMayorDificultad;
		Alumno alumnoEncontrado;

		NodoAlumno* alumnoActual = cabeza;


		while (alumnoActual != nullptr) {
			NodoPractica* practicaActual = alumnoActual->practicas;

			while (practicaActual != nullptr) {

				int dificultad = practicaActual->practica.dificultad;

				if (dificultad % 5 == 0) {
					if (!encontrado || dificultad > practicaMayorDificultad.dificultad) {
						practicaMayorDificultad = practicaActual->practica;
						alumnoEncontrado = alumnoActual->alumno;
						encontrado = true;
					}
				}
				practicaActual = practicaActual->siguiente;
			}

			alumnoActual = alumnoActual->siguiente;


		}

		if (encontrado) {
			cout << "Alumno con practica de mayor dificultad multiplo de 5:" << endl;
			cout << "Alumno: " << alumnoEncontrado.nombreCompleto << endl;
			cout << "Matricula: " << alumnoEncontrado.maticula << endl;
			cout << "Grupo: " << alumnoEncontrado.grupo << endl;
			cout << "Codigo de practica: " << practicaMayorDificultad.codigo << endl;
			cout << "Titulo de practica: " << practicaMayorDificultad.titulo << endl;
			cout << "Dificultad: " << practicaMayorDificultad.dificultad << endl;
			cout << "Fecha: " << practicaMayorDificultad.fecha << endl;
			cout << "Nota: " << practicaMayorDificultad.nota << endl;
		}
		else {
			cout << "No se encontraron practicas con dificultad multiplo de 5" << endl;
		}
	};

	void menu() {
		int opcion;
		do {
			cout << "\nMenu:" << endl;
			cout << "1. Agregar alumno" << endl;
			cout << "2. Examen con menor cantidad de preguntas divisible por 3" << endl;
			cout << "3. Practica con mayor dificultad multiplo de 5" << endl;
			cout << "4. Salir" << endl;
			cout << "Ingrese una opcion: ";
			cin >> opcion;
			switch (opcion) {
			case 1:
				agregarAlumno();
				break;
			case 2:
				examenMenorCantidadPreguntasDivisiblePor3();
				break;
			case 3:
				practicaMayorDificultadMultiploDe5();
				break;
			case 4:
				cout << "Saliendo del programa..." << endl;
				break;
			default:
				cout << "Opcion no valida. Intente nuevamente." << endl;
			}
		} while (opcion != 4);
	};
};

int main() {
	SistemaAcademico sistema;
	sistema.menu();
	return 0;
}
