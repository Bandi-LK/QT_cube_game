#ifndef CUBE_H
#define CUBE_H

#include <QGLWidget>
#include <QRadialGradient>
#include <QDialog>

class Cube : public QGLWidget
{
public:
    Cube(QWidget *parent = 0);
    ~Cube();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void createGradient();
    void createGLObject();
    void drawBackground(QPainter *painter);
    void drawCube();
    void drawLegend(QPainter *painter);

    GLuint glObject;
    QRadialGradient gradient;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat scaling;
    QPoint lastPos;
};

#endif // CUBE_H
