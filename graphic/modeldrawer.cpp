#include "modeldrawer.h"

#include <GL/gl.h>
#include <QtMath>
#include <QDebug>

ModelDrawer::ModelDrawer(QObject *parent) :
    QObject(parent)
{
}

void ModelDrawer::cylinder(float radius,float height,int sides)
{
    //上面
    glNormal3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for(double i = 0; i < sides; i++) {
        double t = M_PI*2/sides * (double)i;
        glVertex3d(radius * cos(t), radius * sin(t), height);
    }
    glEnd();
    //側面
    glBegin(GL_QUAD_STRIP);
    for(double i=0;i<=sides;i=i+1){
        double t = i*2*M_PI/sides;
        glNormal3f((GLfloat)cos(t),0.0,(GLfloat)sin(t));
        glVertex3f((GLfloat)(radius*cos(t)),(GLfloat)(radius*sin(t)), 0);
        glVertex3f((GLfloat)(radius*cos(t)),(GLfloat)(radius*sin(t)), height);
    }
    glEnd();
    //下面
    glNormal3d(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
    for(double i = sides; i >= 0; --i) {
        double t = M_PI*2/sides * (double)i;
        glVertex3d(radius * cos(t), radius * sin(t), 0);
    }
    glEnd();
}

void ModelDrawer::line(Vec v1, Vec v2)
{
    glBegin(GL_LINES);
    glVertex3d(v1.x, v1.y, v1.z);
    glVertex3d(v2.x, v2.y, v2.z);
    glEnd();
}
