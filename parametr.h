#ifndef PARAMETR_H
#define PARAMETR_H

#include <QVector>
#include <QPainter>
#include <grafik.h>

struct smesch
{
    int x;
    int y;
};

class Grafik;

class Parametr
{
public:
    int Tolsina;
    int Nom;
    QFont F;
    float PredVerh;
    float PredNig;
    int DlinIntPics;
    QRectF r,rZnach,rZnachTek;
    QPoint p,p1;
    QPoint pp,pp1;
    int DlinOsi;
    QPolygon Vec;
    bool Act;
    bool Videl;

    bool boolToch;
    bool boolLin;

    QString Name;
    smesch Sm;
    QList<float>* X;
    QList<float>* Y;
    QColor Color;

    QString S,S1;

    float MaxZnX;
    float MinZnX;

    float MaxZnY;
    float MinZnY;

    Parametr(int w, float sdvig, float skal, QString s, QList<float>* V, QList<float>* Tim, int n, QColor c, Grafik* g);
    ~Parametr();
    void drawSkal(QPainter *painter);
    void drawGraf(QPainter *painter);
    void setSkalGraf(int w,float sdvig,float skal);
    void RaschPred(int w, float sdvig, float skal);
    void setZnach(float x, int w, float sdvig, float skal);
    void setZnachTek(float x, int w, float sdvig, float skal);
    void setpoz(int x, int y);
private:
    Grafik* G;
    int poisk(float x, QList<float> *X);
    int poiskm(float x, QList<float> *X);
};

#endif // PARAMETR_H
