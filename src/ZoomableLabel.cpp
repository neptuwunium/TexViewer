#include "ZoomableLabel.h"

ZoomableLabel::ZoomableLabel(QWidget *parent) : QLabel(parent), scaleFactor(1.0) {}

void ZoomableLabel::reset()
{
    scaleFactor = 1.0;
    this->move(QPoint(0, 0));
    updateImage();
}

void ZoomableLabel::setOriginalPixmap(const QPixmap &pixmap)
{
    originalPixmap = pixmap;
    reset();
}

void ZoomableLabel::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        scaleFactor *= 1.1;
    else
        scaleFactor /= 1.1;
    updateImage();
}
void ZoomableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragPosition = event->globalPosition().toPoint() - this->pos();
}

void ZoomableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        this->move(event->globalPosition().toPoint() - dragPosition);
}

void ZoomableLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        reset();
}

void ZoomableLabel::updateImage()
{
    if (originalPixmap.isNull())
        return;

    QPixmap scaled = originalPixmap.scaled(
        originalPixmap.size() * scaleFactor,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    setPixmap(scaled);
    resize(scaled.size());
}