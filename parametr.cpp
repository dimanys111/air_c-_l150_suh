#include "parametr.h"

Parametr::Parametr(int w, float sdvig, float skal, QString s, QList<float> *V, QList<float> *Tim, int n, QColor c,Grafik* g)
{
    Tolsina=1;
    boolToch=false;
    boolLin=true;
    G=g;
    Nom=n;
    Y=V;
    X=Tim;
    Act=true;
    Videl=false;
    if (X->size())
    {
        MaxZnX=X->at(0);
        MinZnX=X->at(0);
    }
    for (int i=0;i<X->size();i++)
    {
        if (MaxZnX<X->at(i))
            MaxZnX=X->at(i);
        if (MinZnX>X->at(i))
            MinZnX=X->at(i);
    }
    if (Y->size())
    {
        MaxZnY=Y->at(0);
        MinZnY=Y->at(0);
    }
    for (int i=0;i<X->size();i++)
    {
        if (MaxZnY<Y->at(i))
            MaxZnY=Y->at(i);
        if (MinZnY>Y->at(i))
            MinZnY=Y->at(i);
    }

    Name=s;
    Color=c;

    Sm.x=10;
    Sm.y=10;
    DlinOsi=1;

    PredNig=MinZnY;
    PredVerh=MaxZnY;
    if (PredVerh==PredNig)
    {
        PredVerh=PredNig+1;
    }
    DlinIntPics=30;

    F.setFamily("Times");
    F.setPointSize(12);

    QFontMetricsF fm(F);
    r=fm.boundingRect(Name);
    r.setRect(0,0-r.height()/2,r.width(),r.height());

    setSkalGraf(w,sdvig,skal);
}

Parametr::~Parametr()
{
    Y->clear();
    delete Y;
}

void Parametr::drawSkal(QPainter* painter)
{
    painter->save();
        painter->setPen(Color);
        painter->translate(Sm.x, Sm.y);


        painter->drawText(r,Qt::AlignVCenter | Qt::AlignLeft,Name,&r);

        if (Act)
        {
            for (int i=0;i<=DlinOsi;i++)
            {
                QPointF P1=QPointF(r.width()+5,-DlinIntPics*DlinOsi/2+DlinIntPics*i);
                QPointF P2=QPointF(r.width()+10,-DlinIntPics*DlinOsi/2+DlinIntPics*i);
                painter->drawLine(P1,P2);
            }
            QPointF P1=QPointF(r.width()+10,-DlinIntPics*DlinOsi/2);
            QPointF P2=QPointF(r.width()+10,DlinIntPics*DlinOsi/2);
            painter->drawLine(P1,P2);

            if (DlinOsi!=0)
            {
                QFontMetricsF fm(F);
                QString S=QString().number(PredVerh,'f',2);
                QRectF r1=fm.boundingRect(S);
                r1.setRect(P1.x()-r1.width()-12,P1.y()-r1.height()/2,r1.width(),r1.height());
                painter->drawText(r1,S);

                S=QString().number(PredNig,'f',2);
                r1=fm.boundingRect(S);
                r1.setRect(P2.x()-r1.width()-12,P2.y()-r1.height()/2,r1.width(),r1.height());
                painter->drawText(r1,S);
            }
        }

        if (Videl)
        {
            painter->drawRect(r);
        }

    painter->restore();
}

void Parametr::drawGraf(QPainter* painter)
{
    painter->save();

        if (Act)
            painter->setPen(QPen(Color, Tolsina+2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        else
            painter->setPen(QPen(Color, Tolsina, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->translate(0, Sm.y);
        if (boolToch)
            painter->drawPoints(Vec);
        if (boolLin)
            painter->drawPolyline(Vec);
        painter->setPen(Color);
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->drawRect(rZnach);
        painter->drawText(rZnach,Qt::AlignTop | Qt::AlignLeft,S,&rZnach);
        painter->setPen(Qt::DotLine);
        painter->drawLine(p,p1);
        painter->drawRect(rZnachTek);
        painter->drawText(rZnachTek,Qt::AlignTop | Qt::AlignLeft,S1,&rZnachTek);
        painter->drawLine(pp,pp1);
    painter->restore();
}

void Parametr::setpoz(int x,int y)
{
    Sm.x=x;
    Sm.y=y;
}

void Parametr::setZnach(float x,int w,float sdvig,float skal)
{
    float rast;
    float rastPol;

    if (DlinOsi==0)
    {
        rast=5/(PredVerh-PredNig);
        rastPol=5/2;
    }
    else
    {
        rast=DlinIntPics*DlinOsi/(PredVerh-PredNig);
        rastPol=DlinIntPics*DlinOsi/2;
    }


    int j=poisk(x,X);
    p.setY((-Y->at(j)+PredNig)*rast+rastPol);
    p.setX(((X->at(j)-MinZnX)/(MaxZnX-MinZnX))*w*skal-sdvig);

    p1.setX(p.x()+10);
    p1.setY(p.y()-10);

    QFontMetricsF fm(F);
    S=QString().number(Y->at(j),'f',2);
    rZnach=fm.boundingRect(S);

    rZnach.setRect(p1.x(),p1.y()-rZnach.height(),rZnach.width(),rZnach.height());
}

void Parametr::setZnachTek(float x,int w,float sdvig,float skal)
{
        int j=0;
        for (int i=j;i<X->size();i++)
        {
            if (x<=X->at(i))
            {
                j=i;
                break;
            }
        }
        pp.setY(-Y->at(j)/(PredVerh-PredNig)*DlinIntPics*DlinOsi+PredNig/(PredVerh-PredNig)*DlinIntPics*DlinOsi+DlinIntPics*DlinOsi/2);
        pp.setX(((x-MinZnX)/(MaxZnX-MinZnX))*w*skal-sdvig);

        pp1.setX(pp.x()+10);
        pp1.setY(pp.y()-10);

        QFontMetricsF fm(F);
        S1=QString().number(Y->at(j),'f');
        rZnachTek=fm.boundingRect(S1);

        rZnachTek.setRect(pp1.x(),pp1.y()-rZnachTek.height(),rZnachTek.width(),rZnachTek.height());
}

void Parametr::setSkalGraf(int w,float sdvig,float skal)
{
    Vec.resize(0);

    float rast;
    float rastPol;

    if (DlinOsi==0)
    {
        rast=5/(PredVerh-PredNig);
        rastPol=5/2;
    }
    else
    {
        rast=DlinIntPics*DlinOsi/(PredVerh-PredNig);
        rastPol=DlinIntPics*DlinOsi/2;
    }


//    float x1=(sdvig)*(MaxZnX-MinZnX)/(float)w/skal+MinZnX;
//    float x2=(w+sdvig)*(MaxZnX-MinZnX)/(float)w/skal+MinZnX;

    int min=0;
    int max=X->size()-1;

//    int min=poisk(x1,X);
//    int max=poiskm(x2,X);

    int n=((X->at(min)-MinZnX)/(MaxZnX-MinZnX))*w*skal-sdvig;
    QPoint p;
    p.setX(n);
    p.setY((-Y->at(min)+PredNig)*rast+rastPol);
    Vec<<p;

    float n1=0;
    float n2=((X->at(min)-MinZnX)/(MaxZnX-MinZnX))*w*skal-sdvig;
    for (int i=min+1;i<=max;i++)
    {

        n1=n2;
        n2=((X->at(i)-MinZnX)/(MaxZnX-MinZnX))*w*skal-sdvig;

//        if ((n2-n1)<1)
//        {
//            float max_=Y->at(i);
//            float min_=Y->at(i);

//            while ((n2-n1)<1 && i<max)
//            {
//                i++;
//                n2=X->at(i)*w*skal/MaxZnX-sdvig;

//                if (max_<Y->at(i))
//                    max_=Y->at(i);
//                if (min_>Y->at(i))
//                    min_=Y->at(i);
//            }

//            QPoint p;
//            p.setX(n2);
//            if (fabs(Y->at(i)-min_)>fabs(Y->at(i)-max_))
//            {
//                p.setY(-min_/(PredVerh-PredNig)*DlinIntPics*DlinOsi+PredNig/(PredVerh-PredNig)*DlinIntPics*DlinOsi+DlinIntPics*DlinOsi/2);
//                Vec<<p;
//                p.setY(-max_/(PredVerh-PredNig)*DlinIntPics*DlinOsi+PredNig/(PredVerh-PredNig)*DlinIntPics*DlinOsi+DlinIntPics*DlinOsi/2);
//                Vec<<p;
//            }
//            else
//            {
//                p.setY(-max_/(PredVerh-PredNig)*DlinIntPics*DlinOsi+PredNig/(PredVerh-PredNig)*DlinIntPics*DlinOsi+DlinIntPics*DlinOsi/2);
//                Vec<<p;
//                p.setY(-min_/(PredVerh-PredNig)*DlinIntPics*DlinOsi+PredNig/(PredVerh-PredNig)*DlinIntPics*DlinOsi+DlinIntPics*DlinOsi/2);
//                Vec<<p;
//            }
//        }
//        else
        {
            QPoint p;
            if(boolLin)
            {
                p.setX(n2);
                p.setY((-Y->at(i-1)+PredNig)*rast+rastPol);
                Vec<<p;
            }
            p.setX(n2);
            p.setY((-Y->at(i)+PredNig)*rast+rastPol);
            Vec<<p;
        }
    }
}

int Parametr::poisk(float x,QList<float>* X)
{
    float min=X->at(0);
    int imin=0;
    float max=X->at(X->size()-1);
    int imax=X->size()-1;
    float meg=X->at((X->size()-1)/2);
    int imeg=(X->size()-1)/2;
    while (imeg!=imin)
    {
        if (x>meg)
        {
            imin=imeg;
            min=X->at(imin);
            imeg=(imin+imax)/2;
            meg=X->at(imeg);
        }
        else
        {
            imax=imeg;
            max=X->at(imax);
            imeg=(imin+imax)/2;
            meg=X->at(imeg);
        }
    }
    return imeg;
}

int Parametr::poiskm(float x,QList<float>* X)
{
    float min=X->at(0);
    int imin=0;
    float max=X->at(X->size()-1);
    int imax=X->size()-1;
    float meg=X->at((X->size()-1)/2);
    int imeg=(X->size()-1)/2;
    while (imeg!=imin)
    {
        if (x>meg)
        {
            imin=imeg;
            min=X->at(imin);
            imeg=(imin+imax)/2;
            meg=X->at(imeg);
        }
        else
        {
            imax=imeg;
            max=X->at(imax);
            imeg=(imin+imax)/2;
            meg=X->at(imeg);
        }
    }
    return imax;
}

void Parametr::RaschPred(int w,float sdvig,float skal)
{
    float x1=(0+sdvig)*(MaxZnX-MinZnX)/w/skal+MinZnX;
    float x2=(w+sdvig)*(MaxZnX-MinZnX)/w/skal+MinZnX;

    PredVerh=-100000000;
    PredNig=100000000;
    for (int i=0;i<X->size();i++)
    {
        if (X->at(i)>=x1 && X->at(i)<=x2)
        {
            if (PredVerh<Y->at(i))
                PredVerh=Y->at(i);
            if (PredNig>Y->at(i))
                PredNig=Y->at(i);
        }
    }

    if (PredVerh==PredNig)
    {
        PredVerh=PredNig+1;
    }
}
