#ifndef MODOBLOQUEADO_H
#define MODOBLOQUEADO_H

#include "pantalla.h"

#include <QLabel>
#include <QProgressBar>
#include <QTimer>

class ModoBloqueado : public Pantalla
{
    Q_OBJECT

public:
    explicit ModoBloqueado(const AppConfig &config, QWidget *parent = nullptr);

    QString nombrePantalla() const override;
    void inicializarUI() override;
    void conectarEventos() override;
    void cargarDatos() override;
    bool validarEstado() override;
    void registrarEvento(const QString &descripcion) override;

public slots:
    void iniciarBloqueo(int seconds);

signals:
    void bloqueoFinalizado();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void actualizarCuentaRegresiva();

private:
    void actualizarVista();

    QLabel *m_badgeLabel = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_subtitleLabel = nullptr;
    QLabel *m_countdownLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QTimer m_timer;
    int m_totalSeconds = 0;
    int m_remainingSeconds = 0;
};

#endif // MODOBLOQUEADO_H
