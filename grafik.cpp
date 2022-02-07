#include "grafik.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QDebug>

int ind;

static const Qt::GlobalColor colors[] =
{
    Qt::blue,
    Qt::cyan,
    Qt::magenta,
    Qt::darkBlue,
    Qt::darkCyan,
    Qt::darkMagenta,
    Qt::darkYellow,
    Qt::black,
    Qt::red,
    Qt::darkRed,
    Qt::green,
    Qt::darkGreen,
    Qt::darkGray
};

Grafik::Grafik()
{
    ind=0;
    scrV=0;
    scrH=0;
    this->setOrientation(Qt::Horizontal);

    WidgTime=new QWidget();
    WidgTime->setMaximumHeight(20);

    skal=new glSkal(this,&parametri);
    graf=new glGraf(this,&parametri);

    QWidget *widSkal=new QWidget(this);
    QWidget *widGraf=new QWidget(this);

    setStretchFactor(0,1);
    setStretchFactor(1,3);

    QVBoxLayout* l= new QVBoxLayout(widGraf);
    l->addWidget(WidgTime);
    l->addWidget(graf);
    l->addWidget(graf->scrolH);
    l->setContentsMargins(0,0,0,0);
    l->setSpacing(0);
    widGraf->setLayout(l);

    QWidget *w1=new QWidget();
    w1->setMaximumHeight(20);
    QVBoxLayout* l1= new QVBoxLayout(widSkal);
    l1->addWidget(w1);
    QWidget *w2=new QWidget();
    QHBoxLayout *l3=new QHBoxLayout(w2);
    l3->addWidget(skal);
    l3->addWidget(skal->scrolV);
    w2->setLayout(l3);
    l1->addWidget(w2);
    l1->addWidget(skal->scrolH);
    widSkal->setLayout(l1);
    l3->setContentsMargins(0,0,0,0);
    l3->setSpacing(0);
    l1->setContentsMargins(0,0,0,0);
    l1->setSpacing(0);
}

void Grafik::repaintAll()
{
    skal->repaint();
    graf->repaint();
}

void Grafik::clear()
{
    for(int i=0;i<parametri.size();i++)
    {
        delete parametri[i];
    }
    parametri.clear();
}

void Grafik::addParametr(QString s, QList<float> *V, QList<float> *Tim, int n, QColor c)
{
    if (V->size() && Tim->size())
    {
        if (n==-1)
            n=parametri.size();
        if (c==Qt::white)
        {
            c = QColor( colors[ind] );

            ind++;
            if (ind>(sizeof(colors) / sizeof(*colors)-1))
                ind=0;
        }
        Parametr *p=new Parametr(graf->width(),graf->Sdvig,graf->Skal,s,V,Tim,n,c,this);

        if (p->MaxZnX>graf->Max)
            graf->Max=p->MaxZnX;
        else
            p->MaxZnX=graf->Max;

        if (p->MinZnX<graf->Min)
            graf->Min=p->MinZnX;
        else
            p->MinZnX=graf->Min;

        for(int i=0;i<parametri.size();i++)
        {
            parametri[i]->MaxZnX=graf->Max;
            parametri[i]->MinZnX=graf->Min;
        }

        if (parametri.size()>0)
            p->Sm.y=parametri.last()->Sm.y+(parametri.last()->DlinOsi*parametri.last()->DlinIntPics)/2;

        parametri<<p;

        graf->getLabelTime();

        skal->repaint();
        graf->repaint();
    }
}

Grafik::~Grafik()
{
    delete skal;
    delete graf;
    for(int i=0;i<parametri.size();i++)
    {
        delete parametri[i];
    }
}

void Grafik::setscrV(int i)
{
    scrV=-i;
    skal->repaint();
    graf->repaint();
}

void Grafik::setscrH(int i)
{
    scrH=-i;
    skal->repaint();
    graf->repaint();
}

void Grafik::setgrH(int i)
{
    graf->Sdvig=i;

    for(int i=0;i<parametri.size();i++)
    {
        parametri[i]->setSkalGraf(graf->width(),graf->Sdvig,graf->Skal);
    }

    graf->getLabelTime();

    graf->repaint();
}

void Grafik::delit_Videl_par()
{
    for(int i=parametri.size()-1;i>=0;i--)
    {
        if (parametri.at(i)->Videl || parametri.at(i)->Act)
        {
            delete parametri.takeAt(i);
        }
    }
    repaintAll();
}
