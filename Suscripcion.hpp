#ifndef SUSCRIPCION_HPP
#define SUSCRIPCION_HPP
#include <string>

class Suscripcion {
private:
    int idSuscripcion;
    std::string tipo;
    float precio;
    std::string fechaInicio;
    bool estado;

public:
    Suscripcion(int id, std::string t, float p, std::string f) 
        : idSuscripcion(id), tipo(t), precio(p), fechaInicio(f), estado(true) {}

    bool estaActiva() { return estado; }
    void actualizarPlan(std::string nuevoPlan) { tipo = nuevoPlan; }
};
#endif
