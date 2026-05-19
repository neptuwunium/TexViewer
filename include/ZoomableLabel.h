#pragma once

#include <QLabel>
#include <QPixmap>

class ZoomableLabel : public QLabel
{
public:
    explicit ZoomableLabel(QWidget *parent = nullptr);
    void setOriginalPixmap(const QPixmap &pixmap);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void updateImage();
    void reset();

    QPixmap originalPixmap;
    double scaleFactor;
    QPoint dragPosition;
};
