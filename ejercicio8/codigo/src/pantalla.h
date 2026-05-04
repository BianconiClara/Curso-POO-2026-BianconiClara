#ifndef PANTALLA_H
#define PANTALLA_H

#include "models.h"

#include <QWidget>

class Pantalla : public QWidget
{
    Q_OBJECT

public:
    explicit Pantalla(const AppConfig &config, QWidget *parent = nullptr);
    ~Pantalla() override;

    virtual QString nombrePantalla() const = 0;
    virtual void inicializarUI() = 0;
    virtual void conectarEventos() = 0;
    virtual void cargarDatos() = 0;
    virtual bool validarEstado() = 0;
    virtual void registrarEvento(const QString &descripcion) = 0;

protected:
    AppConfig m_config;
};

#endif // PANTALLA_H
