#include "glskal.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QColorDialog>

glSkal::glSkal(Grafik *gr, QList <Parametr*>* parametri, QGLWidget *parent) :
    QGLWidget(parent)
{
    Act=false;
    setAcceptDrops(true);
    scrolV=new QScrollBar;
    scrolH=new QScrollBar;
    scrolH->setOrientation(Qt::Horizontal);
    G=gr;

    connect(scrolV,&QScrollBar::valueChanged,G,&Grafik::setscrV);
    connect(scrolH,&QScrollBar::valueChanged,G,&Grafik::setscrH);

    Parametri=parametri;
    setMouseTracking(true);
    Press=false;
}

glSkal::~glSkal()
{
    delete scrolV;
    delete scrolH;
    delete m_pmnu;
}

void glSkal::contextMenuEvent(QContextMenuEvent *pe)
{
    if (Act)
    {
        m_pmnu = new QMenu(this);
        m_pmnu->addAction("&Удалить");
        connect(m_pmnu,
                SIGNAL(triggered(QAction*)),
                SLOT(slotActivated(QAction*))
                );

        QWidgetAction* Toch=new QWidgetAction(NULL);
        QPushButton* butToch=new QPushButton("Точки");
        Toch->setDefaultWidget(butToch);
        m_pmnu->addAction(Toch);

        connect(butToch, &QPushButton::clicked,
        [&]()
        {
            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Act || Parametri->at(i)->Videl)
                {
                    Parametri->at(i)->boolToch=true;
                    Parametri->at(i)->boolLin=false;
                }
            }
            G->repaintAll();
        });

        QWidgetAction* Lin=new QWidgetAction(NULL);
        QPushButton* butLin=new QPushButton("Линия");
        Lin->setDefaultWidget(butLin);
        m_pmnu->addAction(Lin);

        connect(butLin, &QPushButton::clicked,
        [&]()
        {
            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Act || Parametri->at(i)->Videl)
                {
                    Parametri->at(i)->boolToch=false;
                    Parametri->at(i)->boolLin=true;
                }
            }
            G->repaintAll();
        });


        QWidgetAction* Predel=new QWidgetAction(NULL);
        QPushButton* butPredel=new QPushButton("Пределы");
        Predel->setDefaultWidget(butPredel);
        m_pmnu->addAction(Predel);

        connect(butPredel, &QPushButton::clicked,
        [&]()
        {
            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Act || Parametri->at(i)->Videl)
                {
                    Parametri->at(i)->RaschPred(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
                    Parametri->at(i)->setSkalGraf(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
                }
            }
            G->repaintAll();
        });


        QWidgetAction* SpinMax=new QWidgetAction(NULL);
        QDoubleSpinBox* spinMax=new QDoubleSpinBox();
        spinMax->setMinimum(-10000000000);
        spinMax->setMaximum(10000000000);
        spinMax->setValue(ActPar->PredVerh);
        SpinMax->setDefaultWidget(spinMax);
        m_pmnu->addAction(SpinMax);

        void (QDoubleSpinBox::*Sender)(double) = &QDoubleSpinBox::valueChanged;

        connect(spinMax, Sender,
        [&](double i)
        {
            if (i>ActPar->PredNig)
                ActPar->PredVerh=i;
            ActPar->setSkalGraf(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
            G->repaintAll();
        });

        QWidgetAction* SpinMin=new QWidgetAction(NULL);
        QDoubleSpinBox* spinMin=new QDoubleSpinBox();
        spinMin->setMinimum(-10000000000);
        spinMin->setMaximum(10000000000);
        spinMin->setValue(ActPar->PredNig);
        SpinMin->setDefaultWidget(spinMin);
        m_pmnu->addAction(SpinMin);

        connect(spinMin, Sender,
        [&](double i)
        {
            if (i<ActPar->PredVerh)
                ActPar->PredNig=i;
            ActPar->setSkalGraf(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
            G->repaintAll();
        });


        QWidgetAction* SpinTol=new QWidgetAction(NULL);
        QSpinBox* spinTol=new QSpinBox();
        spinTol->setMinimum(1);
        spinTol->setMaximum(10000000000);
        spinTol->setValue(ActPar->Tolsina);
        SpinTol->setDefaultWidget(spinTol);
        m_pmnu->addAction(SpinTol);

        void (QSpinBox::*Sender1)(int) = &QSpinBox::valueChanged;

        connect(spinTol, Sender1,
        [&](int j)
        {
            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Act || Parametri->at(i)->Videl)
                {
                    Parametri->at(i)->Tolsina=j;
                }
            }
            G->repaintAll();
        });

        QWidgetAction* SpinDlinLin=new QWidgetAction(NULL);
        QSpinBox* spinDlinLin=new QSpinBox();
        spinDlinLin->setMinimum(0);
        spinDlinLin->setMaximum(10000000000);
        spinDlinLin->setValue(ActPar->DlinOsi);
        SpinDlinLin->setDefaultWidget(spinDlinLin);
        m_pmnu->addAction(SpinDlinLin);

        connect(spinDlinLin, Sender1,
        [&](int j)
        {
            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Act || Parametri->at(i)->Videl)
                {
                    Parametri->at(i)->DlinOsi=j;
                    Parametri->at(i)->setSkalGraf(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
                }
            }
            G->repaintAll();
        });

        QWidgetAction* ColorDialog=new QWidgetAction(NULL);
        QPushButton* butColorDialog=new QPushButton("Цвет");
        ColorDialog->setDefaultWidget(butColorDialog);
        m_pmnu->addAction(ColorDialog);

        connect(butColorDialog, &QPushButton::clicked,
        [&]()
        {
            QColorDialog* colorDialog=new QColorDialog();
            colorDialog->setCurrentColor(ActPar->Color);

            connect(colorDialog, QColorDialog::currentColorChanged,
            [&](QColor j)
            {
                for(int i=0;i<Parametri->size();i++)
                {
                    if (Parametri->at(i)->Act || Parametri->at(i)->Videl)
                    {
                        Parametri->at(i)->Color=j;
                    }
                }
                G->repaintAll();
            });

            colorDialog->exec();
        });


        m_pmnu->exec(pe->globalPos());
        delete m_pmnu;
    }
}

void glSkal::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QRect r(0, 0, 3000, 3000);
    QFont Font("Times", 12);
    painter.setFont(Font);
    painter.setViewport(r);
    painter.setWindow(r);

    painter.setPen(QPen(Qt::white, 3, Qt::NoPen));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(r);

    painter.setBrush(QBrush(Qt::blue, Qt::NoBrush));
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));

    painter.setRenderHint(QPainter::Antialiasing); // Врубаем антиальясинг :-) Вот так вот просто :-)
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    if (videlProcess)
    {
        painter.save();
        painter.drawRect(r1);
        painter.restore();
    }

    painter.translate(G->scrH, G->scrV);



    for(int i=0;i<Parametri->size();i++)
    {
        Parametri->at(i)->drawSkal(&painter);
    }


}

void glSkal::mousePressEvent(QMouseEvent *pe)
{
    setFocus();

    int n=0;
    for(int i=0;i<Parametri->size();i++)
    {
        if (    pe->x()>=Parametri->at(i)->Sm.x+G->scrH &&
                pe->x()<=Parametri->at(i)->Sm.x+G->scrH+Parametri->at(i)->r.width() &&
                pe->y()>=Parametri->at(i)->Sm.y+G->scrV-Parametri->at(i)->r.height()/2 &&
                pe->y()<=Parametri->at(i)->Sm.y+G->scrV+Parametri->at(i)->r.height()/2)
        {
            if (Parametri->at(i)->Videl)
            {
                n=0;
                break;
            }
            else
                n++;

        }
        else
        {
            n++;
        }
    }
    if (n==Parametri->size())
    {
        for(int i=0;i<Parametri->size();i++)
        {
            Parametri->at(i)->Videl=false;
        }
    }


    Press=true;
    m_ptPosition = pe->pos();
    m_ptPosition1 = pe->pos();

    if (!Act)
        videlProcess=true;
    else
        repaint();
}

void glSkal::mouseMoveEvent(QMouseEvent *pe)
{
    if (!Press)
    {
        int n=0;
        Act=false;
        for(int i=0;i<Parametri->size();i++)
        {
            if (    pe->x()>=Parametri->at(i)->Sm.x+G->scrH &&
                    pe->x()<=Parametri->at(i)->Sm.x+G->scrH+Parametri->at(i)->r.width() &&
                    pe->y()>=Parametri->at(i)->Sm.y+G->scrV-Parametri->at(i)->r.height()/2 &&
                    pe->y()<=Parametri->at(i)->Sm.y+G->scrV+Parametri->at(i)->r.height()/2)
            {
                Parametri->at(i)->Act=true;

                ActPar=Parametri->at(i);

                for(int j=i+1;j<Parametri->size();j++)
                {
                    Parametri->at(j)->Act=false;
                }

                repaint();
                G->graf->repaint();

                Act=true;
                break;
            }
            else
            {
                Parametri->at(i)->Act=false;
                n++;
            }
        }

        if (n==Parametri->size())
        {
            ActPar=0;
            repaint();
            G->graf->repaint();
        }
    }
    else
        if (!videlProcess)
        {
            int dx=(pe->x() - m_ptPosition.x());
            int dy=(pe->y() - m_ptPosition.y());

            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Videl)
                {
                    bool b=true;
                    for(int j=i;j<Parametri->size();j++)
                    {
                        if (Parametri->at(j)->Videl)
                        {
                            if ((Parametri->at(j)->Sm.x<10 && dx<0) || (Parametri->at(j)->Sm.y<10 && dy<0))
                            {
                                b=false;
                                break;
                            }
                        }
                    }

                    if (b)
                    {
                        for(int j=i;j<Parametri->size();j++)
                        {
                            if (Parametri->at(j)->Videl)
                            {
                                Parametri->at(j)->Sm.x=Parametri->at(j)->Sm.x+dx;
                                Parametri->at(j)->Sm.y=Parametri->at(j)->Sm.y+dy;
                            }
                        }
                        repaint();
                        G->graf->repaint();
                    }

                    break;
                }
                if (Parametri->at(i)->Act)
                {
                    if ((Parametri->at(i)->Sm.x<10 && dx<0) || (Parametri->at(i)->Sm.y<10 && dy<0))
                        break;

                    Parametri->at(i)->Sm.x=Parametri->at(i)->Sm.x+dx;
                    Parametri->at(i)->Sm.y=Parametri->at(i)->Sm.y+dy;

                    repaint();
                    G->graf->repaint();
                    break;
                }
            }
        }

    if (videlProcess)
    {
        m_ptPosition2=pe->pos();
        int x1,x2,y1,y2;
        if (m_ptPosition2.x()>m_ptPosition1.x())
        {
            x1=m_ptPosition1.x();
            x2=m_ptPosition2.x();
        }
        else
        {
            x1=m_ptPosition2.x();
            x2=m_ptPosition1.x();
        }
        if (m_ptPosition2.y()>m_ptPosition1.y())
        {
            y1=m_ptPosition1.y();
            y2=m_ptPosition2.y();
        }
        else
        {
            y1=m_ptPosition2.y();
            y2=m_ptPosition1.y();
        }
        r1.setCoords(x1,y1,x2,y2);

        for(int i=0;i<Parametri->size();i++)
        {
            if (    (r1.left()<=Parametri->at(i)->Sm.x+G->scrH &&
                    r1.right()>=Parametri->at(i)->Sm.x+G->scrH &&
                    r1.bottom()>=Parametri->at(i)->Sm.y+G->scrV-Parametri->at(i)->r.height()/2 &&
                    r1.top()<=Parametri->at(i)->Sm.y+G->scrV-Parametri->at(i)->r.height()/2) ||
                    (r1.left()<=Parametri->at(i)->Sm.x+G->scrH+Parametri->at(i)->r.width() &&
                    r1.right()>=Parametri->at(i)->Sm.x+G->scrH+Parametri->at(i)->r.width() &&
                    r1.bottom()>=Parametri->at(i)->Sm.y+G->scrV-Parametri->at(i)->r.height()/2 &&
                    r1.top()<=Parametri->at(i)->Sm.y+G->scrV-Parametri->at(i)->r.height()/2)||
                    (r1.left()<=Parametri->at(i)->Sm.x+G->scrH &&
                    r1.right()>=Parametri->at(i)->Sm.x+G->scrH &&
                    r1.bottom()>=Parametri->at(i)->Sm.y+G->scrV+Parametri->at(i)->r.height()/2 &&
                    r1.top()<=Parametri->at(i)->Sm.y+G->scrV+Parametri->at(i)->r.height()/2) ||
                    (r1.left()<=Parametri->at(i)->Sm.x+G->scrH+Parametri->at(i)->r.width() &&
                    r1.right()>=Parametri->at(i)->Sm.x+G->scrH+Parametri->at(i)->r.width() &&
                    r1.bottom()>=Parametri->at(i)->Sm.y+G->scrV+Parametri->at(i)->r.height()/2 &&
                    r1.top()<=Parametri->at(i)->Sm.y+G->scrV+Parametri->at(i)->r.height()/2)
                    )
            {

                Parametri->at(i)->Videl=true;
            }
            else
            {

                Parametri->at(i)->Videl=false;
            }
        }
        repaint();
    }

    int maxV=0;

    for(int i=0;i<Parametri->size();i++)
    {
        if (Parametri->at(i)->Sm.y > maxV)
        {
            maxV=Parametri->at(i)->Sm.y;
        }
    }

    if (maxV<this->height())
        maxV=0;
    else
        maxV=maxV-this->height()+50;

    int maxH=0;

    for(int i=0;i<Parametri->size();i++)
    {
        if (    Parametri->at(i)->Sm.x > maxH)
        {
            maxH=Parametri->at(i)->Sm.x;
        }
    }

    if (maxH<this->width())
        maxH=0;
    else
        maxH=maxH-this->width()+100;

    scrolV->setMaximum(maxV);
    scrolV->setPageStep(this->height());

    scrolH->setMaximum(maxH);
    scrolH->setPageStep(this->width());

    m_ptPosition = pe->pos();
}

void glSkal::mouseReleaseEvent(QMouseEvent *pe)
{
    Press=false;
    m_ptPosition = pe->pos();
    videlProcess=false;
    repaint();
}

void glSkal::wheelEvent(QWheelEvent *pe)
{
    if ((pe->delta())>0)
    {
        for(int i=0;i<Parametri->size();i++)
        {
            if (Parametri->at(i)->Act)
            {
                Parametri->at(i)->DlinOsi=Parametri->at(i)->DlinOsi+1;
                break;
            }
        }
    }
    else
        if ((pe->delta())<0)
        {
            for(int i=0;i<Parametri->size();i++)
            {
                if (Parametri->at(i)->Act)
                {
                    Parametri->at(i)->DlinOsi=Parametri->at(i)->DlinOsi-1;
                    if (Parametri->at(i)->DlinOsi<0)
                    {
                        Parametri->at(i)->DlinOsi=0;
                    }

                }
            }
        }

    for(int i=0;i<Parametri->size();i++)
    {
        if (Parametri->at(i)->Act)
        {
            Parametri->at(i)->setSkalGraf(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
            G->repaintAll();
            break;
        }
    }

    if (videlProcess)

    m_ptPosition = pe->pos();
}

void glSkal::mouseDoubleClickEvent(QMouseEvent *pe)
{
    for(int i=0;i<Parametri->size();i++)
    {
        if (Parametri->at(i)->Act)
        {
            Parametri->at(i)->RaschPred(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
            Parametri->at(i)->setSkalGraf(G->graf->width(),G->graf->Sdvig,G->graf->Skal);
            repaint();
            G->graf->repaint();
            break;
        }
    }

    m_ptPosition = pe->pos();
}

void glSkal::dropEvent(QDropEvent *event)
{
//                    QByteArray encoded = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
//                    QDataStream stream(&encoded, QIODevice::ReadOnly);

//                    const int x = event->pos().x();
//                    const int y = event->pos().y();
//                    QTableWidgetItem * item = itemAt(x,y) ;
//                    if (NULL == item) {
//                        event->ignore();
//                        return;
//                    }

//                    while (!stream.atEnd())
//                    {
//                        int row, col;
//                        QMap<int, QVariant> itemData;
//                        QMap<int, QVariant> roleDataMap;
//                        stream >> row >> col >> roleDataMap;

//                         QMapIterator<int, QVariant> iterRoleData(roleDataMap);
//                         while (iterRoleData.hasNext()) {
//                             iterRoleData.next();
//                             const int key = iterRoleData.key();
//                             QVariant data = item->data(key);
//                             if (false == data.isNull()) {
//                                itemData.insert(key, data);
//                             }
//                         }
//                        if (this == event->source()) {
//                            QTableWidgetItem * dragItem = QTableWidget::item(row, col);
//                            if (NULL == dragItem) {
//                                event->ignore();
//                                return;
//                            }
//                            QMapIterator<int, QVariant> iter(itemData);
//                            while (iter.hasNext()) {
//                                iter.next();
//                                dragItem->setData(iter.key(), iter.value());
//                            }
//                        } else {
//                            emit dropped(row, col, itemData);
//                        }

//                        QMapIterator<int, QVariant> iter(roleDataMap);
//                        while (iter.hasNext()) {
//                            iter.next();
//                            item->setData(iter.key(), iter.value());
//                        }
//                    }

//                    event->acceptProposedAction();
//                    event->accept();
}

void glSkal::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Delete)
    {
        G->delit_Videl_par();
    }
}

void glSkal::slotActivated(QAction *pAction)
{
    if (pAction->text()=="&Удалить")
        G->delit_Videl_par();
}
