#ifndef MODELDRAWER_H
#define MODELDRAWER_H

#include <QObject>
#include <QGLViewer/qglviewer.h>

using namespace qglviewer;

class ModelDrawer : public QObject
{
    Q_OBJECT
public:
    explicit ModelDrawer(QObject *parent = 0);

    static void cylinder(float radius,float height,int sides);
    static void line(Vec v1, Vec v2);

signals:

public slots:

};

#endif // MODELDRAWER_H
