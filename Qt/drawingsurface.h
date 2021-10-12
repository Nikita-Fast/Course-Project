#ifndef DRAWINGSURFACE_H
#define DRAWINGSURFACE_H

#include <QWidget>

class DrawingSurface : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingSurface(QWidget *parent = nullptr);
    std::vector<QPoint> &getMyPoints();

signals:

public slots:
    void clearButtonClicked();

protected:
    void paintEvent(QPaintEvent *event);

private:
    std::vector<QPoint> myPoints;

};

#endif // DRAWINGSURFACE_H
