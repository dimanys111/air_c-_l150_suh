#ifndef OBRABOTKA_H
#define OBRABOTKA_H

#include <QStringList>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTextStream>
#include <QPushButton>
#include <QCheckBox>
#include <grafik.h>
#include <QComboBox>


const double Rz=6371032, Pi=3.14159265358979323846; // глобальная переменная

double DegToRad( double D );
double RadToDeg( double Radians);

//Расстояние на Земле///////////////////////////////////////////////////////////
double calculateTheDistance (double l1,double f1,double l2,double f2);
//Начальный курс ///////////////////////////////////////////////
double nachazim(double l1,double f1,double l2,double f2);

class Filtr : public QHBoxLayout
{
    Q_OBJECT
public:
    //QLineEdit Name;
    QLineEdit Min;
    QLineEdit Max;
    float min,max;
    QString name;
    QList<Filtr *>* Fil;
    QPushButton Udal;

    QComboBox Name;

    void setFiltr(QString nam,float mi,float ma);
    explicit Filtr(QList<Filtr *>* filtr, QWidget *parent = 0);
    virtual ~Filtr() {}
};

class FiltrTime : public QHBoxLayout
{
    Q_OBJECT
public:
    //QLineEdit Name;
    QLineEdit Min;
    QLineEdit Max;
    float min,max;
    QString name;
    QList<FiltrTime *>* Fil;
    QPushButton Udal;

    QLineEdit Name;

    void setFiltr(QString nam,float mi,float ma);
    explicit FiltrTime(QList<FiltrTime *> *filtr, QWidget *parent = 0);
    virtual ~FiltrTime() {}
};

class obrabotka : public QObject
{
    Q_OBJECT
public:
    QComboBox* boxGrafik;

    Grafik* grafik;
    bool Polusfer;

    QList<Filtr*> filtri;
    QList<FiltrTime*> sko;
    QList<FiltrTime*> timVichet;
    QList<FiltrTime*> timTolko;

    QList<QStringList> parametri;
    
    float DolgCeli;
    float ShirCeli;

    float MaxOtkl;
    float MaxDaln;
    
    QVBoxLayout * LayoutFiltri;
    QVBoxLayout * LayoutSKO;
    QVBoxLayout * LayoutTime;
    QVBoxLayout * LayoutTime1;

    QLineEdit*maxotklLaut;
    QLineEdit*maxDalnLaut;

    QLineEdit*KolCelLaut;
    QLineEdit*KolParCelLaut;
    QLineEdit*KolParObLaut;

    QLineEdit*lC;
    QLineEdit*fC;

    QList<QStringList> obparametri;
    QList<QStringList> filtrparametri;
    QList<QStringList> dvoenie;
    QList<QStringList> zzz;
    QList<QStringList> otchparametri;

    QCheckBox *CheckBox;

    int kolcel;
    int kolosnpar;
    int NomPerOsnPar;

    explicit obrabotka(QLineEdit*kolCelLaut,QLineEdit*kolParCelLaut,QLineEdit*kolParObLaut,
                       QLineEdit *LC, QLineEdit *FC, QLineEdit*maxotkl, QLineEdit*maxdal,  QWidget * parent,
                       QWidget * parent1, QWidget *parent2, QWidget *parent3, QCheckBox *checkBox);
    bool ZagrDan();
    void DobFiltr();
    void DobSKO();
    bool Filracia();
    void SohrFiltra();
    void setDolgCeli(QString s);
    void setShirCeli(QString s);
    void setMaxOtkl(QString s);
    void setDaln(QString s);
    void ZagrFiltra();
    void addparam(QList<QStringList> &o, QString s);
    void ekspOtch(float MatOg, float skoZnach, QString t="");
    void ekspFiltr();
    void ekspDvoen();
    void razb_odin(QStringList &list, QTextStream &in);
    void setKolCel(QString s);
    void setKolParCel(QString s);
    void setKolOsnPar(QString s);
    void DobTimeFilVichet();
    void DobTimeFilTolko();
    void SetPolusfer(bool b);

    void Graf(QList<QStringList> &list);
};

#endif // OBRABOTKA_H
