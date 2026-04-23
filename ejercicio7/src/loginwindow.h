#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "climawidget.h"
#include "pantalla.h"

#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginForm;
}
QT_END_NAMESPACE

class LoginWindow : public Pantalla
{
    Q_OBJECT

public:
    explicit LoginWindow(int lockSeconds, QWidget *parent = nullptr);
    ~LoginWindow() override;

    QString nombrePantalla() const override;
    void setWeatherWidget(ClimaWidget *weatherWidget);

public slots:
    void inicializarPantalla() override;
    void actualizarContenido() override;
    void showUserMessage(const QString &message, bool error = false);
    void setBackgroundPending(bool pending);

signals:
    void loginValidated();

private slots:
    void handleLoginClicked();
    void handleLockTick();

private:
    void startTemporaryLock();
    void updateButtonState();

    Ui::LoginForm *ui = nullptr;
    int m_failedAttempts = 0;
    int m_lockSeconds = 15;
    int m_remainingLockSeconds = 0;
    bool m_waitingForBackground = false;
    QTimer m_lockTimer;
};

#endif // LOGINWINDOW_H
