#ifndef GLGRAF_H
#define GLGRAF_H

#include <QGLWidget>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QThread>
#include <parametr.h>
#include "grafik.h"

class Parametr;
class Grafik;
class ToThredGraf;

class glGraf : public QGLWidget
{
    Q_OBJECT
    void getStringTime(int pos, bool b, QLabel * l);
public:
    bool b;
    QScrollBar * scrolH;
    QLineEdit *LTime;
    QList<QLabel*> l;
    bool Press;
    QFont Font;
    bool prov;
    Grafik*Gr;
    QString S;
    float Max;
    float Min;
    int WidthOld;
    float Skal,SkalOld,Sdvig;
    QPoint  m_ptPosition, m_ptPositionSkal, m_ptPosition1,m_ptPosition2, m_ptPosition3;
    QRect rectWind;
    QList <Parametr*>* Parametri;
    explicit glGraf(Grafik*gr,QList <Parametr*>* parametri, QGLWidget *parent = 0);
    ~glGraf();
    void getLabelTime();
    void setPolTek(float x);
    float setLabelTimeTek();
protected:
    void paintEvent(QPaintEvent *event);
    virtual void   mousePressEvent(QMouseEvent* pe        );
    virtual void   mouseMoveEvent (QMouseEvent* pe        );
    virtual void   mouseReleaseEvent(QMouseEvent *pe      );
    virtual void   wheelEvent     (QWheelEvent* pe        );
    virtual void   mouseDoubleClickEvent(QMouseEvent *pe  );
    void resizeEvent(QResizeEvent *e);
};

#endif // GLGRAF_H
