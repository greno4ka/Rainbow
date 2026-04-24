#include "formulaFactory.h"

void mathToLabel(const QWidget *parent, QLabel* label, const QString& formula,
                 int width, int height, int fontSize)
{
    JKQTMathText math;
    math.useXITS();
    math.setFontSize(fontSize);
    math.setFontColor(parent->palette().color(QPalette::WindowText));
    math.parse(formula);

    double dpr = parent->devicePixelRatioF();

    QImage image(width * dpr, height * dpr, QImage::Format_ARGB32);
    image.setDevicePixelRatio(dpr);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    math.draw(painter, Qt::AlignCenter, QRectF(0, 0, width, height), false);

    label->setPixmap(QPixmap::fromImage(image));
}