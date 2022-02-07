#ifndef GRAFIK_H
#define GRAFIK_H

#include <QSplitter>

#include <QTimer>
#include <glskal.h>
#include <glgraf.h>
#include <parametr.h>

class glSkal;
class glGraf;



class Grafik : public QSplitter
{
    Q_OBJECT
public:
    int scrV;
    int scrH;

    QWidget *WidgTime;

    glSkal * skal;
    glGraf * graf;

    QList <Parametr*> parametri;
    Grafik();
    void repaintAll();
    void clear();
    void addParametr(QString s, QList<float> *V, QList<float> *Tim,int n=-1,QColor c=Qt::white);
    ~Grafik();

public slots:
    void setscrV(int i);
    void setscrH(int i);
    void setgrH(int i);
    void delit_Videl_par();
private slots:

};

#endif // GRAFIK_H
