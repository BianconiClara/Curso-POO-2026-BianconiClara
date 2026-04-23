#ifndef PANTALLA_H
#define PANTALLA_H

#include <QWidget>

class Pantalla : public QWidget
{
    Q_OBJECT

public:
    explicit Pantalla(QWidget *parent = nullptr);
    ~Pantalla() override;

    virtual QString nombrePantalla() const = 0;

public slots:
    virtual void inicializarPantalla() = 0;
    virtual void actualizarContenido() = 0;
};

#endif // PANTALLA_H
