#include <iostream>
using namespace std;

int productTriple(int n) {
	if (n == 1) {
		return 3;
	}
	return (3 * n) * productTriple(n - 1);

}

int hola() {
	int n;
	cout << "Ingrese n: ";
	cin >> n;


	if (n <= 0) {
		cout << "n debe ser mayor a 0" << endl;
		return 0;

	}

	cout << "Resultado: " << productTriple(n) << endl;
	return 0;

}
