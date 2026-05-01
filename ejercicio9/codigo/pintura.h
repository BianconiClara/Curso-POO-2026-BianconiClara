#ifndef PINTURA_H
#define PINTURA_H

#include <QColor>
#include <QList>
#include <QWidget>

#include "admindb.h"

class QKeyEvent;
class QMouseEvent;
class QPaintEvent;
class QPainter;
class QWheelEvent;

class Pintura : public QWidget {
    Q_OBJECT

public:
    explicit Pintura(AdminDB *adminDB, const Usuario &usuario, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void dibujarTrazo(QPainter &painter, const Trazo &trazo) const;
    void actualizarTitulo();
    void limpiarLienzo();
    void deshacerUltimoTrazo();

    AdminDB *adminDB;
    Usuario usuario;
    QVector<Trazo> trazos;
    QList<int> historialDeshacer;
    Trazo trazoActual;
    QColor colorActual;
    int grosorActual;
    bool dibujando;
};

#endif // PINTURA_H
