#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pantalla.h"

#include <QPixmap>

class QPaintEvent;

class MainWindow : public Pantalla
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &backgroundPath, QWidget *parent = nullptr);
    QString nombrePantalla() const override;

public slots:
    void inicializarPantalla() override;
    void actualizarContenido() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_backgroundPath;
    QPixmap m_backgroundPixmap;
};

#endif // MAINWINDOW_H
