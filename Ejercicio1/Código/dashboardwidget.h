#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QList>
#include "task.h"
#include <QTextEdit>



class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(QString usuario, QWidget *parent = nullptr);


private:
    QString usuario;

    QList<Task> tasks;

    // UI
    QGridLayout *grid;
    QComboBox *filtroEstado;
    QComboBox *filtroPrioridad;
    QPushButton *btnAgregar;

    // lógica
    void cargarTasks();
    void dibujarTasks();
    void abrirEditor(int index);

    //historial
    QTextEdit *historialView;
    void cargarHistorial();
    QVBoxLayout *mainLayout;
};

#endif