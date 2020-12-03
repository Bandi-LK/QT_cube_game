#include <QtGui>
#include <QtOpenGL>
#include <cmath>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <iostream>
#include <cstdlib>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

#include "cube.h"

Cube::Cube(QWidget *parent)
    : QGLWidget(parent)
{
    //调用setFormat（）使OpenGL的显示描述表支持反走样。
    setFormat(QGLFormat(QGL::SampleBuffers));

    //初始化私有变量
    rotationX = -18.0;
    rotationY = -58.0;
    rotationZ = 0.0;
    scaling = 1.0;
    //设置填充背景的QRadialGradient
    createGradient();
    //创建OpenGL立方体对象
    createGLObject();

    setAutoBufferSwap( false );
    setAutoFillBackground( false );
    setMaximumSize(420, 300);
}

Cube::~Cube()
{
    makeCurrent();
    //删除构造函数创建的OpenGL立方体对象
    glDeleteLists(glObject, 1);
}

//绘制,在paintEvent()中创建一个QPainter,在进行纯OpenGL操作时
//保存和恢复其状态。
//QPainter的构造函数(或者QPainter::begin())自动调用glClear
//除非预先调用窗口部件的setAutoFillBackground(false)
//QPainter的析构函数（或者QPainter::end()）自动调用QGLWidget::swapBuffers()
//切换可见缓存和离屏缓存，除非预先调用setAutoBufferSwap(false).
//当QPainter被激活，我们可以交叉使用纯OpenGL命令，只要在执行纯OpenGL命令之前保存OpenGL状态，之后恢复OpenGL状态
void Cube::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter( this );
    //绘制背景
    drawBackground( &painter );
    //背景绘制结束
    painter.end( );
    //绘制立方体
    drawCube();
    //绘制开始
    painter.begin(this);
    //使用HTML文字设置QTextDocument对象
    drawLegend(&painter);
    painter.end( );
    swapBuffers( );
}


//使用蓝色渐变色设置QRadialGradient
void Cube::createGradient()
{
    //确保指定的中心和焦点坐标根据窗口部件大小进行调整
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    //位置用0和1之间的浮点数表示，0对应焦点坐标，1对应圆的边缘
    gradient.setCenter(0.45, 0.50);
    gradient.setFocalPoint(0.40, 0.45);
    /*gradient.setColorAt(0.0, QColor(105, 146, 182));
    gradient.setColorAt(0.4, QColor(81, 113, 150));
    gradient.setColorAt(0.8, QColor(16, 56, 121));*/
    gradient.setColorAt(0.0, QColor(207, 238, 214));
    gradient.setColorAt(0.5, QColor(247, 208, 214));
    gradient.setColorAt(0.8, QColor(247, 238, 164));
}

//创建OpenGL列表，该列表保存绘制的立方体的边。
void Cube::createGLObject()
{
    makeCurrent();

    glShadeModel(GL_FLAT);

    glObject = glGenLists(1);
    glNewList(glObject, GL_COMPILE);
    qglColor(QColor(0, 0, 0));
    glLineWidth(1.0);

    glBegin(GL_LINES);
    glVertex3f(+1.0, +1.0, -1.0);
    glVertex3f(-1.0, +1.0, -1.0);
    glVertex3f(+1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(+1.0, -1.0, +1.0);
    glVertex3f(-1.0, -1.0, +1.0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(+1.0, +1.0, +1.0);
    glVertex3f(+1.0, +1.0, -1.0);
    glVertex3f(+1.0, -1.0, -1.0);
    glVertex3f(+1.0, -1.0, +1.0);
    glVertex3f(+1.0, +1.0, +1.0);
    glVertex3f(-1.0, +1.0, +1.0);
    glVertex3f(-1.0, +1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, +1.0);
    glVertex3f(-1.0, +1.0, +1.0);
    glEnd();

    glEndList();
}

void Cube::drawBackground(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawRect(rect());
}

//绘制立方体
void Cube::drawCube()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    GLfloat x = 3.0 * GLfloat(width()) / height();
    glOrtho(-x, +x, -3.0, +3.0, 4.0, 15.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glScalef(scaling, scaling, scaling);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    //设置反走样
    glEnable(GL_MULTISAMPLE);

    //绘制立方体
    glCallList(glObject);

    //设置文字颜色和字体
    setFont(QFont("Microsoft YaHei", 16));
    qglColor(QColor(10, 10, 10));

    //绘制字符，renderText使文字不变形
    //，上下，左右
    renderText(+1.1, +1.1, +1.1, QChar('a'));
    renderText(-1.1, +1.1, +1.1, QChar('b'));
    renderText(+1.1, +1.1, -1.1, QChar('c'));
    renderText(-1.1, +1.1, -1.0, QChar('d'));
    renderText(+1.1, -1.3, +1.1, QChar('e'));
    renderText(-1.1, -1.1, +1.2, QChar('f'));
    renderText(+1.1, -1.1, -1.1, QChar('g'));
    renderText(-1.1, -1.3, -1.1, QChar('h'));

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}


//使用HTML文字设置QTextDocument对象，在半透明的蓝色矩形上绘制它们
void Cube::drawLegend(QPainter *painter)
{
    const int Margin = 11;
    const int Padding = 6;

    /*QTextDocument textDocument;
    textDocument.setDefaultStyleSheet("* { color: #000000 }");
    textDocument.setHtml("<h4 align=\"center\">OUTPUT NUMBERS</h4>"
                         "<p align=\"center\"><table width=\"100%\">"
                         "<tr><td>a<td>:<td>10"
                         "<tr><td>b<td>:<td>10"
                         "<tr><td>c<td>:<td>10"
                         "<tr><td>d<td>:<td>10"
                         "<tr><td>e<td>:<td>10"
                         "<tr><td>f<td>:<td>10"
                         "<tr><td>g<td>:<td>10"
                         "<tr><td>h<td>:<td>10"
                         "</table>");
    textDocument.setTextWidth(textDocument.size().width());

    QRect rect(QPoint(0, 0), textDocument.size().toSize()
                             + QSize(2 * Padding, 2 * Padding));
    painter->translate(width() - rect.width() - Margin,
                       height() - rect.height() - Margin);
    painter->setPen(QColor(255, 239, 239));
    painter->setBrush(QColor(255, 0, 0, 31));
    painter->drawRect(rect);

    painter->translate(Padding, Padding);
    textDocument.drawContents(painter);*/
}
