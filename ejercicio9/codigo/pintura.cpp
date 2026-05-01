#include "pintura.h"

#include <QKeyEvent>
#include <QKeySequence>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QWheelEvent>
#include <QtGlobal>

namespace {
QPoint obtenerPosicionMouse(const QMouseEvent *event) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return event->position().toPoint();
#else
    return event->pos();
#endif
}
}

Pintura::Pintura(AdminDB *adminDB, const Usuario &usuario, QWidget *parent)
    : QWidget(parent),
      adminDB(adminDB),
      usuario(usuario),
      colorActual(Qt::black),
      grosorActual(3),
      dibujando(false) {
    setAttribute(Qt::WA_DeleteOnClose);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(900, 600);
    setMouseTracking(true);

    trazos = adminDB->cargarTrazos(usuario.id);
    actualizarTitulo();
}

void Pintura::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QColor("#ffffff"));

    painter.setPen(QPen(QColor("#d9d9d9"), 1));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    for (const Trazo &trazo : trazos) {
        dibujarTrazo(painter, trazo);
    }

    if (!trazoActual.puntos.isEmpty()) {
        dibujarTrazo(painter, trazoActual);
    }

    painter.setPen(QColor("#444444"));
    painter.drawText(
        QRect(16, 16, width() - 32, 48),
        Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,
        "R/G/B cambia color | Rueda ajusta grosor | Esc borra el lienzo | Ctrl+Z deshace hasta 10 acciones"
    );

    if (trazos.isEmpty() && trazoActual.puntos.isEmpty()) {
        painter.setPen(QColor("#999999"));
        painter.drawText(rect(), Qt::AlignCenter, "Comienza a dibujar con el mouse.");
    }
}

void Pintura::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        return QWidget::mousePressEvent(event);
    }

    setFocus();
    dibujando = true;
    trazoActual = Trazo();
    trazoActual.color = colorActual;
    trazoActual.grosor = grosorActual;
    trazoActual.puntos.append(obtenerPosicionMouse(event));

    update();
}

void Pintura::mouseMoveEvent(QMouseEvent *event) {
    if (!dibujando || !(event->buttons() & Qt::LeftButton)) {
        return QWidget::mouseMoveEvent(event);
    }

    const QPoint punto = obtenerPosicionMouse(event);
    if (trazoActual.puntos.isEmpty() || trazoActual.puntos.constLast() != punto) {
        trazoActual.puntos.append(punto);
        update();
    }
}

void Pintura::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton || !dibujando) {
        return QWidget::mouseReleaseEvent(event);
    }

    dibujando = false;

    const QPoint puntoFinal = obtenerPosicionMouse(event);
    if (trazoActual.puntos.isEmpty() || trazoActual.puntos.constLast() != puntoFinal) {
        trazoActual.puntos.append(puntoFinal);
    }

    const int trazoId = adminDB->guardarTrazo(
        usuario.id,
        trazoActual.color,
        trazoActual.grosor,
        trazoActual.puntos
    );

    if (trazoId > 0) {
        trazoActual.id = trazoId;
        trazos.append(trazoActual);
        historialDeshacer.append(trazoId);
        if (historialDeshacer.size() > 10) {
            historialDeshacer.removeFirst();
        }
    }

    trazoActual = Trazo();
    actualizarTitulo();
    update();
}

void Pintura::wheelEvent(QWheelEvent *event) {
    const int delta = event->angleDelta().y() > 0 ? 1 : -1;
    grosorActual = qBound(1, grosorActual + delta, 30);
    actualizarTitulo();
    update();
    event->accept();
}

void Pintura::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Undo)) {
        deshacerUltimoTrazo();
        return;
    }

    switch (event->key()) {
    case Qt::Key_R:
        colorActual = Qt::red;
        break;
    case Qt::Key_G:
        colorActual = Qt::green;
        break;
    case Qt::Key_B:
        colorActual = Qt::blue;
        break;
    case Qt::Key_Escape:
        limpiarLienzo();
        return;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    actualizarTitulo();
    update();
}

void Pintura::dibujarTrazo(QPainter &painter, const Trazo &trazo) const {
    QPen pen(trazo.color, trazo.grosor, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    if (trazo.puntos.size() == 1) {
        painter.drawPoint(trazo.puntos.constFirst());
        return;
    }

    for (int i = 1; i < trazo.puntos.size(); ++i) {
        painter.drawLine(trazo.puntos.at(i - 1), trazo.puntos.at(i));
    }
}

void Pintura::actualizarTitulo() {
    QString colorNombre = "Negro";
    if (colorActual == Qt::red) {
        colorNombre = "Rojo";
    } else if (colorActual == Qt::green) {
        colorNombre = "Verde";
    } else if (colorActual == Qt::blue) {
        colorNombre = "Azul";
    }

    setWindowTitle(
        QString("Lienzo de %1 %2 | Color: %3 | Grosor: %4 px | Trazos guardados: %5")
            .arg(usuario.nombre)
            .arg(usuario.apellido)
            .arg(colorNombre)
            .arg(grosorActual)
            .arg(trazos.size())
    );
}

void Pintura::limpiarLienzo() {
    if (adminDB->borrarTrazosUsuario(usuario.id)) {
        trazos.clear();
        historialDeshacer.clear();
        trazoActual = Trazo();
        dibujando = false;
        actualizarTitulo();
        update();
    }
}

void Pintura::deshacerUltimoTrazo() {
    if (dibujando && !trazoActual.puntos.isEmpty()) {
        trazoActual = Trazo();
        dibujando = false;
        update();
        return;
    }

    if (historialDeshacer.isEmpty()) {
        return;
    }

    const int trazoId = historialDeshacer.takeLast();
    if (!adminDB->eliminarTrazo(trazoId)) {
        return;
    }

    for (int i = trazos.size() - 1; i >= 0; --i) {
        if (trazos.at(i).id == trazoId) {
            trazos.removeAt(i);
            break;
        }
    }

    actualizarTitulo();
    update();
}
