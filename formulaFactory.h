#ifndef FORMULAFACTORY_H
#define FORMULAFACTORY_H

#include <QPainter>

#include <jkqtmathtext.h>

void mathToLabel(const QWidget *parent,
                 QLabel* label,
                 const QString& formula,
                 bool darkThemeEnabled,
                 int width = 650,
                 int height = 100,
                 int fontSize = 20);

#endif // FORMULAFACTORY_H
