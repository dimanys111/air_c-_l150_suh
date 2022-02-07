#include "glgraf.h"

#include <QMouseEvent>
#include <QDebug>
#include <QBitmap>
#include <QApplication>
#include <cmath>

using namespace std;

glGraf::glGraf(Grafik *gr, QList<Parametr *> *parametri, QGLWidget *parent) :
    QGLWidget(parent)
{
    Min=100000000;
    Max=-100000000;
    Gr=gr;
    scrolH=new QScrollBar;
    scrolH->setOrientation(Qt::Horizontal);

    connect(scrolH,&QScrollBar::valueChanged,Gr,&Grafik::setgrH);

    setCursor(QCursor(QPixmap(1, 1)));
    b=false;

    WidthOld=width();
    Skal=1;
    Sdvig=0;
    Parametri=parametri;
    setMouseTracking(true);

    prov=false;
    Press=false;

    Font.setFamily("Times");
    Font.setPointSize(12);
    rectWind.setRect(0, 0, 3000, 3000);


    for (int i=0;i<20;i++)
    {
        l<<new QLabel("00:00:00",Gr->WidgTime);
        l.back()->setFont(Font);
        QFontMetrics fm(Font);
        QString S="00:00:00";
        QRect r1=fm.boundingRect(S);
        l.back()->setGeometry(r1);
    }

    LTime=new QLineEdit("00:00:00.000",Gr->WidgTime);
    LTime->setFont(Font);
    QFontMetrics fm(Font);
    QString S="00:00:00.000";
    QRect r1=fm.boundingRect(S);
    r1.setWidth(r1.width()+5);
    LTime->setGeometry(r1);
    LTime->setModified(false);
    LTime->setAlignment ( Qt :: AlignCenter );
    LTime->show();
}

glGraf::~glGraf()
{
    delete scrolH;
    delete LTime;

    for (int i=0;i<20;i++)
    {
        delete l[i];
    }
}

void glGraf::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.setViewport(rectWind);
    painter.setWindow(rectWind);
    painter.setFont(Font);

    painter.setPen(QPen(Qt::white, 3, Qt::NoPen));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(rectWind);

    painter.setBrush(QBrush(Qt::NoBrush));

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));

    painter.drawLine(0,m_ptPosition.y(),width(),m_ptPosition.y());
    painter.drawLine(m_ptPosition.x(),0,m_ptPosition.x(),height());

    if (Press)
    {
        QRectF rect(m_ptPosition1.x(),0,m_ptPosition.x()-m_ptPosition1.x(),height());
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(rect, Qt::transparent);
    }

    painter.setPen(QPen(QColor(200,200,200), 1, Qt::DotLine));
    int i=0;
    int j=0;
    while (width()>i) {
        i=i+100;

        painter.drawLine(i,0,i,height());
    }

    while (height()>j) {
        j=j+100;
        painter.drawLine(0,j,width(),j);
    }

    painter.translate(0, Gr->scrV);

    for(int i=0;i<Parametri->size();i++)
    {
        Parametri->at(i)->drawGraf(&painter);
    }
}

void glGraf::mousePressEvent(QMouseEvent *pe)
{
    m_ptPosition1 = pe->pos();
    Press=true;
}

float glGraf::setLabelTimeTek()
{
    float x=(m_ptPosition.x()+Sdvig)*(Max-Min)/width()/Skal+Min;
    int chas=x/3600;
    int min=(x-chas*3600)/60;
    int sec=x-chas*3600-min*60;
    float msec=x-chas*3600-min*60-sec;
    QString sc=QString().number(chas);
    if (sc.length()==1)
    sc="0"+sc;
    QString sm=QString().number(min);
    if (sm.length()==1)
    sm="0"+sm;
    QString ss=QString().number(sec);
    if (ss.length()==1)
    ss="0"+ss;
    QString sms=QString().number((int)(msec*1000));
    if (b)
    {
        S=sc+":"+sm+":"+ss+"."+sms;
    }
    else
    {
        S=sc+":"+sm+":"+ss;
    }

    LTime->setText(S);
    LTime->setGeometry(m_ptPosition.x()-LTime->width()/2,0,LTime->width(),LTime->height());

    return x;
}

void glGraf::mouseMoveEvent(QMouseEvent *pe)
{
    m_ptPosition = pe->pos();

    float x = setLabelTimeTek();


    for(int i=0;i<Parametri->size();i++)
    {
        Parametri->at(i)->setZnach(x,width(),Sdvig,Skal);
    }

    repaint();
}

void glGraf::setPolTek(float x)
{
    m_ptPosition3.setX((x-Min)*width()*Skal/(Max-Min)-Sdvig);

    int min=(int)x/60;
    int chas=min/60;
    min=min-chas*60;
    int sec=x-chas*60*60-min*60;
    S=QString().number(chas)+":"+QString().number(min)+":"+QString().number(sec);
    LTime->setText(S);
    LTime->setGeometry(m_ptPosition3.x()-LTime->width()/2,0,LTime->width(),LTime->height());

    for(int i=0;i<Parametri->size();i++)
    {
        Parametri->at(i)->setZnachTek(x,width(),Sdvig,Skal);
    }

    repaint();
}

void glGraf::mouseReleaseEvent(QMouseEvent *pe)
{
    Press=false;
    m_ptPosition2 = pe->pos();

    int x1=m_ptPosition1.x();
    int x2=pe->pos().x();

    if (x1>x2)
    {
        x2=m_ptPosition1.x();
        x1=pe->pos().x();
    }

    if (abs(x2-x1)>5)
    {
        float k=fabs(x2-x1)/width();
        if (Skal/k<1048576)
        {
            Sdvig=Sdvig/Skal+x1/Skal;
            Skal=Skal/k;
            Sdvig=Sdvig*Skal;

            scrolH->blockSignals(true);
            scrolH->setMaximum(width()*Skal-width());
            scrolH->setPageStep(width());
            scrolH->setValue(Sdvig);
            scrolH->blockSignals(false);

            for(int i=0;i<Parametri->size();i++)
            {
                Parametri->at(i)->setSkalGraf(width(),Sdvig,Skal);
            }

            getLabelTime();
            repaint();
        }
    }
}


void glGraf::getStringTime(int pos,bool b, QLabel * l)
{
    float x=(pos+Sdvig)*(Max-Min)/width()/Skal+Min;
    int chas=x/3600;
    int min=(x-chas*3600)/60;
    int sec=x-chas*3600-min*60;
    float msec=x-chas*3600-min*60-sec;
    QString sc=QString().number(chas);
    if (sc.length()==1)
    sc="0"+sc;
    QString sm=QString().number(min);
    if (sm.length()==1)
    sm="0"+sm;
    QString ss=QString().number(sec);
    if (ss.length()==1)
    ss="0"+ss;
    QString sms=QString().number((int)(msec*1000));
    if (b)
    {
        S=sc+":"+sm+":"+ss+"."+sms;
        l->setFont(Font);
        QFontMetrics fm(Font);
        QRect r1=fm.boundingRect(S);
        l->setGeometry(r1);
        if (pos)
        {
            if (pos==width())
                l->setGeometry(pos-l->width(),0,l->width(),l->height());
            else
                l->setGeometry(pos-l->width()/2,0,l->width(),l->height());
        }
        else
            l->setGeometry(pos,0,l->width(),l->height());

    }
    else
    {
        S=sc+":"+sm+":"+ss;
        l->setFont(Font);
        QFontMetrics fm(Font);
        QRect r1=fm.boundingRect(S);
        l->setGeometry(r1);
        if (pos)
        {
            if (pos==width())
                l->setGeometry(pos-l->width(),0,l->width(),l->height());
            else
                l->setGeometry(pos-l->width()/2,0,l->width(),l->height());
        }
        else
            l->setGeometry(pos,0,l->width(),l->height());
    }

    l->setText(S);
    l->show();

}

void glGraf::getLabelTime()
{
    for (int i=0;i<20;i++)
    {
        l[i]->hide();
    }

    if ((100+Sdvig)*(Max-Min)/width()/Skal-(Sdvig)*(Max-Min)/width()/Skal<1)
        b=true;
    else
        b=false;

    getStringTime(0,b,l[0]);
    int j=l.size()-1;
    getStringTime(width(),b,l[j]);

    int i=0;
    int n=1;
    int h=0;
    int ll=0;
    if (b)
    {
        h=200;
        ll=300;
    }
    else
    {
        h=100;
        ll=200;
    }

    while (width()-ll>i) {
        i=i+h;
        getStringTime(i,b,l[n]);
        n++;
    }
}

void glGraf::wheelEvent(QWheelEvent *pe)
{
    m_ptPositionSkal = pe->pos();
    SkalOld=Skal;
    if ((pe->delta())>0)
    {
        Skal=Skal/2;
    }
    else
        if ((pe->delta())<0)
        {
            Skal=Skal*2;
        }
    if (Skal<1)
    {
        Skal=1;
        Sdvig=0;
    }

    if (Skal<=1048576)
    {
        Sdvig=Sdvig+(m_ptPositionSkal.x()+Sdvig)/SkalOld*Skal-(m_ptPositionSkal.x()+Sdvig);

        for(int i=0;i<Parametri->size();i++)
        {
            Parametri->at(i)->setSkalGraf(width(),Sdvig,Skal);
        }

        scrolH->blockSignals(true);
        scrolH->setMaximum(width()*Skal-width());
        scrolH->setPageStep(width());
        scrolH->setValue(Sdvig);
        scrolH->blockSignals(false);

        getLabelTime();
        repaint();
    }
    else
    {
        Skal=Skal/2;
    }
}

void glGraf::mouseDoubleClickEvent(QMouseEvent *pe)
{
    m_ptPosition = pe->pos();
}

void glGraf::resizeEvent(QResizeEvent *e)
{
    float koef;
    if (WidthOld>1)
        koef=(float)width()/WidthOld;
    else
        koef=1;

    Sdvig=Sdvig*koef;

    for(int i=0;i<Parametri->size();i++)
    {
        Parametri->at(i)->setSkalGraf(width(),Sdvig*koef,Skal);
    }

    getLabelTime();
    repaint();

    WidthOld=width();
}
