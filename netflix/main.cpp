#include <iostream>
#include "Sistema.h"
using namespace std;
using namespace System;
int main() {
    DataSetGenerator :: menuGenerador();
    Sistema sistema;
    sistema.iniciar();

    cin.get();
    return 0;
}
