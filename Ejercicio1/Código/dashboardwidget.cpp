#include "dashboardwidget.h"
#include "filemanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QFile>
#include <QTextStream>

// ================= CONSTRUCTOR =================

DashboardWidget::DashboardWidget(QString usuario, QWidget *parent)
    : QWidget(parent), usuario(usuario)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 🔹 Layout filtros
    QHBoxLayout *filtrosLayout = new QHBoxLayout();
    filtrosLayout->setSpacing(10);

    QLabel *lblEstado = new QLabel("Estado:");
    QLabel *lblPrioridad = new QLabel("Prioridad:");

    // 🔹 Filtro estado
    filtroEstado = new QComboBox();
    filtroEstado->addItems({"Todos", "Pendiente", "En progreso", "Completado"});
    filtroEstado->setMinimumWidth(150);

    // 🔹 Filtro prioridad
    filtroPrioridad = new QComboBox();
    filtroPrioridad->addItems({"Todas", "Baja", "Media", "Alta"});
    filtroPrioridad->setMinimumWidth(150);

    filtrosLayout->addWidget(lblEstado);
    filtrosLayout->addWidget(filtroEstado);
    filtrosLayout->addSpacing(20);
    filtrosLayout->addWidget(lblPrioridad);
    filtrosLayout->addWidget(filtroPrioridad);
    filtrosLayout->addStretch();

    mainLayout->addLayout(filtrosLayout);

    // 🔹 Botón agregar
    btnAgregar = new QPushButton("Agregar TP");
    btnAgregar->setObjectName("btnAgregar");
    btnAgregar->setMinimumHeight(40);
    mainLayout->addWidget(btnAgregar);

    // 🔹 Grilla
    grid = new QGridLayout();
    grid->setHorizontalSpacing(20);
    grid->setVerticalSpacing(10);
    mainLayout->addLayout(grid);

    // 🔹 Historial
    historialView = new QTextEdit();
    historialView->setReadOnly(true);
    historialView->setMinimumHeight(120);
    historialView->setPlaceholderText("Historial de acciones...");

    mainLayout->addWidget(new QLabel("Historial:"));
    mainLayout->addWidget(historialView);

    // 🔹 cargar datos
    cargarTasks();
    dibujarTasks();
    cargarHistorial();

    // 🔹 eventos
    connect(btnAgregar, &QPushButton::clicked, [=]() {
        QString nombre = "Nuevo TP";

        tasks.append(Task(nombre, "Pendiente", "Media", ""));
        FileManager::guardarTasks(tasks);
        FileManager::guardarHistorial("Se agregó", nombre);

        dibujarTasks();
        cargarHistorial();
    });

    connect(filtroEstado, &QComboBox::currentTextChanged, [=]() {
        dibujarTasks();
    });

    connect(filtroPrioridad, &QComboBox::currentTextChanged, [=]() {
        dibujarTasks();
    });

    // 🔹 ESTILOS
    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e2f;
            color: #e0e0e0;
            font-family: Segoe UI;
            font-size: 14px;
        }

        QLabel {
            font-weight: 500;
        }

        QComboBox, QLineEdit, QTextEdit {
            background-color: #2a2a40;
            border: 1px solid #3a3a5a;
            border-radius: 6px;
            padding: 5px;
        }

        QPushButton {
            background-color: #3a3a5a;
            border-radius: 8px;
            padding: 6px 12px;
        }

        QPushButton:hover {
            background-color: #5a5aff;
        }

        QPushButton#btnAgregar {
            background-color: #28a745;
            font-weight: bold;
        }

        QPushButton#btnAgregar:hover {
            background-color: #34d058;
        }

        QTextEdit {
            background-color: #111122;
            border: 1px solid #333;
        }
    )");
}

// ================= DATOS =================

void DashboardWidget::cargarTasks()
{
    tasks = FileManager::leerTasks();
}

// ================= UI =================

void DashboardWidget::dibujarTasks()
{
    // limpiar grilla
    QLayoutItem *item;
    while ((item = grid->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    // 🔹 encabezados en negrita
    QLabel *h1 = new QLabel("Título");
    QLabel *h2 = new QLabel("Estado");
    QLabel *h3 = new QLabel("Prioridad");
    QLabel *h4 = new QLabel("Acciones");

    h1->setStyleSheet("font-weight: bold;");
    h2->setStyleSheet("font-weight: bold;");
    h3->setStyleSheet("font-weight: bold;");
    h4->setStyleSheet("font-weight: bold;");

    grid->addWidget(h1, 0, 0);
    grid->addWidget(h2, 0, 1);
    grid->addWidget(h3, 0, 2);
    grid->addWidget(h4, 0, 3);

    int row = 1;

    for (int i = 0; i < tasks.size(); i++)
    {
        Task &t = tasks[i];

        // 🔹 filtros
        if (filtroEstado->currentText() != "Todos" &&
            t.estado != filtroEstado->currentText())
            continue;

        if (filtroPrioridad->currentText() != "Todas" &&
            t.prioridad != filtroPrioridad->currentText())
            continue;

        QLabel *lblTitulo = new QLabel(t.titulo);
        QLabel *lblEstado = new QLabel(t.estado);
        QLabel *lblPrioridad = new QLabel(t.prioridad);

        // 🔹 colores estado
        if (t.estado == "Pendiente")
            lblEstado->setStyleSheet("color: orange;");
        else if (t.estado == "En progreso")
            lblEstado->setStyleSheet("color: #00bfff;");
        else if (t.estado == "Completado")
            lblEstado->setStyleSheet("color: #00ff99;");

        // 🔹 colores prioridad
        if (t.prioridad == "Alta")
            lblPrioridad->setStyleSheet("color: red;");
        else if (t.prioridad == "Media")
            lblPrioridad->setStyleSheet("color: yellow;");
        else
            lblPrioridad->setStyleSheet("color: lightgreen;");

        QPushButton *btnEditar = new QPushButton("Editar");
        QPushButton *btnEliminar = new QPushButton("Eliminar");

        btnEditar->setStyleSheet("background-color: #007bff;");
        btnEliminar->setStyleSheet("background-color: #dc3545;");

        grid->addWidget(lblTitulo, row, 0);
        grid->addWidget(lblEstado, row, 1);
        grid->addWidget(lblPrioridad, row, 2);

        QHBoxLayout *acciones = new QHBoxLayout();
        acciones->addWidget(btnEditar);
        acciones->addWidget(btnEliminar);

        QWidget *accionesWidget = new QWidget();
        accionesWidget->setLayout(acciones);

        grid->addWidget(accionesWidget, row, 3);

        // 🔹 eliminar
        connect(btnEliminar, &QPushButton::clicked, [=]() {
            tasks.removeAt(i);
            FileManager::guardarTasks(tasks);
            FileManager::guardarHistorial("Se eliminó", t.titulo);

            dibujarTasks();
            cargarHistorial();
        });

        // 🔹 editar
        connect(btnEditar, &QPushButton::clicked, [=]() {
            abrirEditor(i);
        });

        row++;
    }
}

// ================= EDITOR =================

void DashboardWidget::abrirEditor(int index)
{
    Task &t = tasks[index];

    QDialog dialog(this);
    dialog.setWindowTitle("Editar TP");

    QFormLayout *form = new QFormLayout();

    QLineEdit *tituloEdit = new QLineEdit(t.titulo);

    QComboBox *estadoEdit = new QComboBox();
    estadoEdit->addItems({"Pendiente", "En progreso", "Completado"});
    estadoEdit->setCurrentText(t.estado);

    QComboBox *prioridadEdit = new QComboBox();
    prioridadEdit->addItems({"Baja", "Media", "Alta"});
    prioridadEdit->setCurrentText(t.prioridad);

    QTextEdit *notaEdit = new QTextEdit(t.nota);

    form->addRow("Título:", tituloEdit);
    form->addRow("Estado:", estadoEdit);
    form->addRow("Prioridad:", prioridadEdit);
    form->addRow("Notas:", notaEdit);

    QDialogButtonBox *botones =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    form->addWidget(botones);
    dialog.setLayout(form);

    connect(botones, &QDialogButtonBox::accepted, [&]() {
        t.titulo = tituloEdit->text();
        t.estado = estadoEdit->currentText();
        t.prioridad = prioridadEdit->currentText();
        t.nota = notaEdit->toPlainText();

        FileManager::guardarTasks(tasks);
        FileManager::guardarHistorial("Se editó", t.titulo);

        dialog.accept();
        dibujarTasks();
        cargarHistorial();
    });

    connect(botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    dialog.exec();
}

// ================= HISTORIAL =================

void DashboardWidget::cargarHistorial()
{
    QFile file("historial.txt");

    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&file);
    historialView->setText(in.readAll());
}