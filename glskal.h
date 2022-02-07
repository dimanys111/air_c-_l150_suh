#ifndef GLSKAL_H
#define GLSKAL_H

#include <QGLWidget>
#include <QPainter>
#include <QScrollBar>
#include <QDropEvent>
#include <QMenu>
#include <parametr.h>
#include "grafik.h"

#include <QWidgetAction>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>

class SpinBoxAction : public QWidgetAction {
public:
    SpinBoxAction (const QString& title) :
      QWidgetAction (NULL) {
        QWidget* pWidget = new QWidget (NULL);
        QHBoxLayout* pLayout = new QHBoxLayout();
        QLabel* pLabel = new QLabel (title);
        pLayout->addWidget (pLabel);
        pSpinBox = new QSpinBox(NULL);
        pLayout->addWidget (pSpinBox);
        pWidget->setLayout (pLayout);

        setDefaultWidget(pWidget);
    }

    QSpinBox * spinBox () {
        return pSpinBox;
    }

private:
    QSpinBox * pSpinBox;
};


class Parametr;
class Grafik;

class glSkal : public QGLWidget
{
    Q_OBJECT
public:
    QScrollBar * scrolV;
    QScrollBar * scrolH;
    QPoint  m_ptPosition,m_ptPosition1,m_ptPosition2;
    bool Press;
    explicit glSkal(Grafik*gr,QList <Parametr*>* parametri,QGLWidget *parent = 0);
    ~glSkal();
private:
    //Meny * meny;
    QMenu* m_pmnu;
    QRect r1;
    Grafik*G;
    QList <Parametr*>* Parametri;
    Parametr* ActPar;
    bool videlProcess;
    bool Act;

protected:
    virtual void contextMenuEvent(QContextMenuEvent* pe);
    void paintEvent(QPaintEvent *event);
    virtual void   mousePressEvent(QMouseEvent* pe        );
    virtual void   mouseMoveEvent (QMouseEvent* pe        );
    virtual void   mouseReleaseEvent(QMouseEvent *pe      );
    virtual void   wheelEvent     (QWheelEvent* pe        );
    virtual void   mouseDoubleClickEvent(QMouseEvent *pe  );
    virtual void   dropEvent(QDropEvent *event);
    virtual void   keyPressEvent(QKeyEvent *event);
signals:
    void dropped(int row, int col, QMap<int,  QVariant> data);
public slots:
    void slotActivated(QAction* pAction);


};

#endif // GLSKAL_H
