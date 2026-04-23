#include "mainwindow.h"

#include <QApplication>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
QFrame *createInfoCard(const QString &title, const QString &body, QWidget *parent)
{
    QFrame *card = new QFrame(parent);
    card->setStyleSheet(
        "QFrame {"
        "  background-color: rgba(255, 255, 255, 22);"
        "  border: 1px solid rgba(255, 255, 255, 26);"
        "  border-radius: 18px;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(18, 16, 18, 16);
    layout->setSpacing(8);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet("color: #f8fafc; font-size: 14px; font-weight: 700;");

    QLabel *bodyLabel = new QLabel(body, card);
    bodyLabel->setWordWrap(true);
    bodyLabel->setStyleSheet("color: #d5dfeb; font-size: 13px; line-height: 1.3;");

    layout->addWidget(titleLabel);
    layout->addWidget(bodyLabel);

    return card;
}
}

MainWindow::MainWindow(const QString &backgroundPath, QWidget *parent)
    : Pantalla(parent)
    , m_backgroundPath(backgroundPath)
{
    if (!m_backgroundPixmap.load(m_backgroundPath)) {
        m_backgroundPixmap.load(":/images/offline-background.jpg");
    }

   MainWindow::inicializarPantalla();
}

QString MainWindow::nombrePantalla() const
{
    return "Ventana";
}

void MainWindow::inicializarPantalla()
{
    setWindowTitle("Ejercicio 07 - Ventana principal");
    setAttribute(Qt::WA_StyledBackground, true);

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(56, 40, 56, 48);
    pageLayout->setSpacing(18);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setSpacing(16);

    QLabel *topLabel = new QLabel("Curriculum", this);
    topLabel->setStyleSheet(
        "color: #f8fafc;"
        "font-size: 15px;"
        "font-weight: 600;"
        "background-color: rgba(8, 16, 28, 120);"
        "padding: 10px 16px;"
        "border-radius: 16px;"
    );
    headerLayout->addWidget(topLabel, 0, Qt::AlignLeft);
    headerLayout->addStretch();

    QPushButton *closeButton = new QPushButton("Cerrar", this);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setMinimumHeight(42);
    closeButton->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(208, 103, 79, 230);"
        "  color: white;"
        "  border: none;"
        "  border-radius: 16px;"
        "  padding: 10px 20px;"
        "  font-size: 14px;"
        "  font-weight: 700;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(221, 117, 92, 240);"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(178, 78, 58, 240);"
        "}"
    );
    connect(closeButton, &QPushButton::clicked, qApp, &QCoreApplication::quit);
    headerLayout->addWidget(closeButton, 0, Qt::AlignRight);

    pageLayout->addLayout(headerLayout);
    pageLayout->addStretch();

    QWidget *heroCard = new QWidget(this);
    heroCard->setObjectName("heroCard");
    heroCard->setMaximumWidth(1120);
    heroCard->setStyleSheet(
        "QWidget#heroCard {"
        "  background-color: rgba(8, 20, 34, 205);"
        "  border: 1px solid rgba(255, 255, 255, 38);"
        "  border-radius: 28px;"
        "}"
    );

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(heroCard);
    shadow->setBlurRadius(40);
    shadow->setOffset(0, 18);
    shadow->setColor(QColor(0, 0, 0, 120));
    heroCard->setGraphicsEffect(shadow);

    QHBoxLayout *heroLayout = new QHBoxLayout(heroCard);
    heroLayout->setContentsMargins(34, 34, 34, 34);
    heroLayout->setSpacing(28);

    QLabel *photoLabel = new QLabel(heroCard);
    photoLabel->setFixedSize(190, 190);
    photoLabel->setPixmap(QPixmap(":/profile/professional-profile.jpeg").scaled(
        photoLabel->size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation));
    photoLabel->setStyleSheet(
        "background-color: rgba(255, 255, 255, 20);"
        "border-radius: 95px;"
        "border: 4px solid rgba(255, 255, 255, 45);"
    );
    photoLabel->setAlignment(Qt::AlignCenter);
    heroLayout->addWidget(photoLabel, 0, Qt::AlignTop);

    QWidget *contentWidget = new QWidget(heroCard);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(14);

    QLabel *eyebrow = new QLabel("Curriculum | LinkedIn style", contentWidget);
    eyebrow->setStyleSheet("color: #91c6ff; font-size: 13px; font-weight: 700;");

    QLabel *title = new QLabel("Clara Bianconi", contentWidget);
    title->setWordWrap(true);
    title->setStyleSheet("color: #f8fafc; font-size: 28px; font-weight: 800;");

    QLabel *headline = new QLabel("Computer Engineering Student | Python | C++ | Data Structures | Machine Learning Fundamentals",
                                  contentWidget);
    headline->setWordWrap(true);
    headline->setStyleSheet("color: #d5dfeb; font-size: 15px; font-weight: 600;");

    QLabel *summary = new QLabel(
        "Estudiante de Ingenieria en Computacion en la Universidad Blas Pascal. "
        "Cuento con conocimientos de Python, estructuras de datos y fundamentos de Machine Learning, "
        "ademas de experiencia en tareas de desarrollo y trabajo en equipo adquirida durante una pasantia. "
        "Mis principales intereses estan en software engineering y automation, con rapida adaptacion a nuevas tecnologias.",
        contentWidget);
    summary->setWordWrap(true);
    summary->setStyleSheet("color: #dce6f1; font-size: 14px; line-height: 1.45;");

    QGridLayout *cardsLayout = new QGridLayout;
    cardsLayout->setHorizontalSpacing(14);
    cardsLayout->setVerticalSpacing(14);
    cardsLayout->addWidget(createInfoCard(
        "Experiencia profesional",
        "Globant | Software Development Internship | Mayo 2023. "
        "Experiencia inicial en tareas de desarrollo y trabajo colaborativo dentro de un entorno profesional.",
        contentWidget), 0, 0);
    cardsLayout->addWidget(createInfoCard(
        "Formacion academica",
        "Ingenieria en Computacion (2024 - Actualidad) en Universidad Blas Pascal, Cordoba. "
        "Bachiller en Humanidades y Ciencias Sociales, Colegio del Carmen, graduada en 2023.",
        contentWidget), 0, 1);
    cardsLayout->addWidget(createInfoCard(
        "Skills y cursos",
        "Lenguajes: Python, C++, HTML, CSS y Java. "
        "Cursos: Machine Learning with Python en UTN Buenos Aires - SCEU (2025) y Python en Digital Skills Academy de UBP.",
        contentWidget), 1, 0);
    cardsLayout->addWidget(createInfoCard(
        "Idiomas y disponibilidad",
        "Ingles B2 (Cambridge FCE). Disponibilidad por la manana, en modalidad remota o presencial. "
        "Ubicacion: Cordoba Capital.",
        contentWidget), 1, 1);

    QLabel *footer = new QLabel(
        "Contacto: clarabianconi02@gmail.com | 351 506 9143. ",
        contentWidget);
    footer->setWordWrap(true);
    footer->setStyleSheet("color: #b9c8d9; font-size: 13px;");

    contentLayout->addWidget(eyebrow);
    contentLayout->addWidget(title);
    contentLayout->addWidget(headline);
    contentLayout->addWidget(summary);
    contentLayout->addLayout(cardsLayout);
    contentLayout->addWidget(footer);
    contentLayout->addStretch();

    heroLayout->addWidget(contentWidget, 1);
    pageLayout->addWidget(heroCard, 0, Qt::AlignHCenter | Qt::AlignBottom);
}

void MainWindow::actualizarContenido()
{
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    if (!m_backgroundPixmap.isNull()) {
        const QPixmap scaled = m_backgroundPixmap.scaled(size(),
                                                         Qt::KeepAspectRatioByExpanding,
                                                         Qt::SmoothTransformation);
        const int x = (width() - scaled.width()) / 2;
        const int y = (height() - scaled.height()) / 2;
        painter.drawPixmap(x, y, scaled);
    } else {
        painter.fillRect(rect(), QColor(22, 40, 63));
    }

    painter.fillRect(rect(), QColor(4, 10, 20, 120));
}
