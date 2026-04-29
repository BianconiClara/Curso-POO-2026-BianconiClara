#include "pantalla.h"

Pantalla::Pantalla(const AppConfig &config, QWidget *parent)
    : QWidget(parent)
    , m_config(config)
{
}

Pantalla::~Pantalla() = default;
