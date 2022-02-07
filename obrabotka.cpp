#include "obrabotka.h"

#include <qfileinfo.h>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QFile>
#include <QLabel>
#include <QApplication>

#include <QDebug>

#include <cmath>

using namespace std;

QStringList SpisPar;

double DegToRad( double D )
{
   return D/180.0*Pi;
}

double RadToDeg( double Radians )
{
  return Radians * (180.0 / Pi);
}

//Расстояние на Земле///////////////////////////////////////////////////////////
double calculateTheDistance (double l1,double f1,double l2,double f2)
{
    double lat1,lat2,long1,long2,cl1,cl2,sl1,sl2,delta,cdelta,sdelta,x,y,ad,dist;

    // перевести координаты в радианы
    lat1 = f1 * Pi / 180;
    lat2 = f2 * Pi / 180;
    long1 = l1 * Pi / 180;
    long2 = l2 * Pi / 180;

    // косинусы и синусы широт и разницы долгот
    cl1 = cos(lat1);
    cl2 = cos(lat2);
    sl1 = sin(lat1);
    sl2 = sin(lat2);
    delta = long2 - long1;
    cdelta = cos(delta);
    sdelta = sin(delta);

    // вычисления длины большого круга
    y = sqrt(pow(cl2 * sdelta,2) + pow(cl1 * sl2 - sl1 * cl2 * cdelta,2));
    x = sl1 * sl2 + cl1 * cl2 * cdelta;

    ad = atan2(y, x);
    dist = ad * Rz;

    return dist;
}

//Начальный курс ///////////////////////////////////////////////
double nachazim(double l1,double f1,double l2,double f2)
{
    l1=DegToRad(l1);
    f1=DegToRad(f1);
    l2=DegToRad(l2);
    f2=DegToRad(f2);

    double f=atan(sin(l2-l1)/(cos(f1)*tan(Pi/2))+sin(f1)*sin(l2-l1)/(cos(f1)*tan(l2-l1)));
    double a=atan(1/(cos(f1)*tan(f2)/sin(l2-l1)-sin(f1)/tan(l2-l1)))/Pi*180.0;

    if (l2>l1)
    {
        if (f2>f)
            {
                return a;
            }
        else
            {
                return 180.0+a;
            }
    }
    else
    {
        if (f2>f)
            {
                return 360.0+a;
            }
        else
            {
                return 180.0+a;
            }
    }
}

obrabotka::obrabotka(QLineEdit *kolCelLaut, QLineEdit *kolParCelLaut, QLineEdit *kolParObLaut, QLineEdit*LC, QLineEdit*FC, QLineEdit*maxotkl, QLineEdit*maxdal,
                     QWidget *parent, QWidget *parent1, QWidget *parent2, QWidget *parent3, QCheckBox *checkBox)
{
    SpisPar<<"Выберете"<<"AZ_RIC"
       <<"F_RIC"
         <<"T_RIC"
           <<"D_RIC"
             <<"NUM_LIB_RIC"
               <<"PRIZ_MEM_RIC"
                 <<"PRIZ_SOURCE_RIC"
                   <<"Prior"
                     <<"Time"
                       <<"H"
                         <<"B"
                           <<"L"
                             <<"Курс"
                               <<"Крен"
                                 <<"Тангаж"
                                   <<"Носки"
                                     <<"Шасси";


    KolCelLaut=kolCelLaut;
    KolParCelLaut=kolParCelLaut;
    KolParObLaut=kolParObLaut;

    CheckBox=checkBox;
    MaxDaln=200000;
    MaxOtkl=30;
    kolcel=12;
    kolosnpar=6;
    NomPerOsnPar=9;
    Polusfer=true;

    maxotklLaut=maxotkl;
    maxDalnLaut=maxdal;

    lC=LC;
    fC=FC;

    LayoutFiltri=new QVBoxLayout;
    parent->setLayout(LayoutFiltri);
    QHBoxLayout*l=new QHBoxLayout;
    l->addWidget(new QLabel("Имя"));
    l->addWidget(new QLabel("Мин"));
    l->addWidget(new QLabel("Мах"));
    LayoutFiltri->addLayout(l);

    LayoutSKO=new QVBoxLayout;
    parent1->setLayout(LayoutSKO);
    l=new QHBoxLayout;
    l->addWidget(new QLabel("Имя"));
    l->addWidget(new QLabel("Мин"));
    l->addWidget(new QLabel("Мах"));
    LayoutSKO->addLayout(l);

    LayoutTime=new QVBoxLayout;
    parent2->setLayout(LayoutTime);
    l=new QHBoxLayout;
    l->addWidget(new QLabel("Имя"));
    l->addWidget(new QLabel("Мин"));
    l->addWidget(new QLabel("Мах"));
    LayoutTime->addLayout(l);

    LayoutTime1=new QVBoxLayout;
    parent3->setLayout(LayoutTime1);
    l=new QHBoxLayout;
    l->addWidget(new QLabel("Имя"));
    l->addWidget(new QLabel("Мин"));
    l->addWidget(new QLabel("Мах"));
    LayoutTime1->addLayout(l);
}

void obrabotka::razb_odin(QStringList& list, QTextStream& in)
{
    QString s=in.readLine().replace("     "," ").replace("    "," ").replace("   "," ").replace("  "," ").replace(",",".").replace("\"","").replace("=","");
    if (s.size()>2)
    {
        if (s.at(0)==' ')
            s.remove(0,1);
        if (s.at(s.length()-1)==' ')
            s.remove(s.length()-1,1);
    }
    if (s.indexOf(";")>-1)
    {
        list = s.split(";");
        list.removeFirst();
    }
    else
        list = s.split(" ");
}

void obrabotka::SetPolusfer(bool b)
{
    Polusfer=b;
}

bool obrabotka::ZagrDan()
{
    obparametri.clear();
    zzz.clear();

    QString files = QFileDialog::getOpenFileName(0, "Загрузка Таблицы", qApp->applicationDirPath()+"/tbl", "TBL (*.*)");
    QFile file(files);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QStringList list,buf;

//        if (s.at(0)==QString("[") || s.at(0)==QString("-"))
//        {
//            QTextCodec* codec=QTextCodec::codecForName("CP866");
//            in.setCodec(codec);
//            s=in.readLine();
//        }

        razb_odin(list, in);
        for (int i=0;i<list.size();i++)
        {
            buf<<"";
        }
        for (int i=0;i<list.size();i++)
        {
            QStringList l;
            parametri<<l;
            parametri[i].append(list[i]);
        }

        razb_odin(list, in);

        while (list.size()!=buf.size()) {
            razb_odin(list, in);
        }

        for (int i=0;i<list.size();i++)
        {
            buf[i]=list[i];
        }

        while (!in.atEnd())
        {
            razb_odin(list, in);
            while (list.size()!=buf.size()) {
                razb_odin(list, in);
            }

            for (int i=0;i<list.size();i++)
            {
                buf[i]=list[i];
                parametri[i].append(buf[i]);
            }
        }

        for (int i=0;i<kolosnpar;i++)
        {
            switch (i) {
            case 0:
                addparam(obparametri,SpisPar[1]);
                break;
            case 1:
                addparam(obparametri,SpisPar[2]);
                break;
            case 2:
                addparam(obparametri,SpisPar[3]);
                break;
            case 3:
                addparam(obparametri,SpisPar[4]);
                break;
            case 4:
                addparam(obparametri,SpisPar[5]);
                break;
            case 5:
                addparam(obparametri,SpisPar[6]);
                break;
            case 6:
                addparam(obparametri,SpisPar[7]);
                break;
            default:
                break;
            }
        }

        addparam(obparametri,SpisPar[8]);

        addparam(obparametri,SpisPar[9]);
        addparam(obparametri,SpisPar[10]);
        addparam(obparametri,SpisPar[11]);
        addparam(obparametri,SpisPar[12]);
        addparam(obparametri,SpisPar[13]);
        addparam(obparametri,SpisPar[14]);
        addparam(obparametri,SpisPar[15]);
        addparam(obparametri,SpisPar[16]);
        addparam(obparametri,SpisPar[17]);

        zzz=obparametri;

        for (int i=1;i<parametri[0].size();i++)
        {
            for (int j=0;j<kolcel;j++)
            {
                for (int n=0;n<kolosnpar;n++)
                {
                    obparametri[n]<<parametri[NomPerOsnPar+(j*kolosnpar)+n].at(i);
                }
                obparametri[kolosnpar]<<QString().number(j);
                for (int n=kolosnpar+1;n<obparametri.size();n++)
                {
                    obparametri[n]<<parametri[n-kolosnpar-1].at(i);
                }
            }
        }
        parametri.clear();
        file.close();
        return true;
    }
    else
        return false;
}

void obrabotka::addparam(QList<QStringList> & o,QString s)
{
    QStringList l;
    o<<l;
    o.last()<<s;
}

void obrabotka::DobFiltr()
{
    filtri<<new Filtr(&filtri);
    LayoutFiltri->addLayout(filtri.last());
}

void obrabotka::DobSKO()
{
    sko<<new FiltrTime(&sko);
    LayoutSKO->addLayout(sko.last());
}

void obrabotka::DobTimeFilVichet()
{
    timVichet<<new FiltrTime(&timVichet);
    LayoutTime->addLayout(timVichet.last());
}

void obrabotka::DobTimeFilTolko()
{
    timTolko<<new FiltrTime(&timTolko);
    LayoutTime1->addLayout(timTolko.last());
}

void obrabotka::ekspOtch(float MatOg,float skoZnach,QString t)
{
    QString files;
    if (t=="")
    {
        files = QFileDialog::getSaveFileName(0, "Save Dialog", qApp->applicationDirPath(), "CSV (*.csv)");

        if (files!="")
        {
            if ( files.section( ".", -1, -1 ) != "csv" )
                     files+= ".csv";
        }
    }
    else
        files=t;

    if (files!="")
    {
        QFile file(files);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            for (int l=0;l<otchparametri[0].size();l++)
            {
                QString s;
                for (int i=0;i<otchparametri.size();i++)
                {
                    s=s+otchparametri[i].at(l)+";";
                }
                if (l!=0)
                {
                    s=s+"\r\n";
                }
                stream << s;

                if (l==0)
                {
                    QString s;
                    s="MatOg="+QString().number(MatOg)+";"+"SKO="+QString().number(skoZnach)+"\r\n";
                    stream << s;
                }
            }
        }
    }
}

void obrabotka::ekspFiltr()
{
    QString files = QFileDialog::getSaveFileName(0, "Save Dialog", qApp->applicationDirPath(), "CSV (*.csv)");

    if (files!="")
    {
        if ( files.section( ".", -1, -1 ) != "csv" )
                 files+= ".csv";

        QFile file(files);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            for (int l=0;l<filtrparametri[0].size();l++)
            {
                QString s;
                for (int i=0;i<filtrparametri.size();i++)
                {
                    s=s+filtrparametri[i].at(l)+";";
                }
                s=s+"\r\n";
                stream << s;
            }
        }
    }
}

void obrabotka::ekspDvoen()
{
    QString files = QFileDialog::getSaveFileName(0, "Save Dialog", qApp->applicationDirPath(), "CSV (*.csv)");

    if (files!="")
    {
        if ( files.section( ".", -1, -1 ) != "csv" )
                 files+= ".csv";

        QFile file(files);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            for (int l=0;l<dvoenie[0].size();l++)
            {
                QString s;
                for (int i=0;i<dvoenie.size();i++)
                {
                    s=s+dvoenie[i].at(l)+";";
                }
                s=s+"\r\n";
                stream << s;
            }
        }
    }
}

bool obrabotka::Filracia()
{   
    int sch=0;
    int dob=0;

    int nomDolgSam=0;
    int nomShirSam=0;
    int nomKursSam=0;
    int nomNoski=0;
    int nomTang=0;
    int nomH=0;

    filtrparametri.clear();
    dvoenie.clear();
    otchparametri.clear();

    filtrparametri=zzz;

    addparam(filtrparametri,"АзРасч");
    addparam(filtrparametri,"DРасч");
    addparam(filtrparametri,"dAz");
    addparam(filtrparametri,"MatOg");
    addparam(filtrparametri,"SKO");

    otchparametri.clear();
    otchparametri=filtrparametri;
    dvoenie=filtrparametri;

    for (int j=0;j<obparametri.size();j++)
    {
        if (obparametri[j].at(0).indexOf("L")>-1)
        {
            nomDolgSam=j;
        }
        if (obparametri[j].at(0).indexOf("B")>-1)
        {
            nomShirSam=j;
        }
        if (obparametri[j].at(0).indexOf("Курс")>-1)
        {
            nomKursSam=j;
        }
        if (obparametri[j].at(0).indexOf("Тангаж")>-1)
        {
            nomTang=j;
        }
        if (obparametri[j].at(0).indexOf("Носки")>-1)
        {
            nomNoski=j;
        }
        if (obparametri[j].at(0).indexOf("H")>-1)
        {
            nomH=j;
        }
    }

    for (int l=1;l<obparametri[0].size();l++)
    {
        int n=0;
        if(timTolko.size()>0)
        {
            for (int k=0;k<timTolko.size();k++)
            {
                float zn=obparametri[kolosnpar+1].at(l).toFloat();
                if ((zn>timTolko[k]->min
                        && zn<timTolko[k]->max))
                {
                    n=1;
                }
            }
        }
        else
            n=1;
        if (n==1)
        {
            n=0;
            if(timVichet.size()>0)
            {
                for (int k=0;k<timVichet.size();k++)
                {
                    float zn=obparametri[kolosnpar+1].at(l).toFloat();
                    if (!(zn>timVichet[k]->min
                          && zn<timVichet[k]->max))
                    {
                        n=1;
                    }
                }
            }
            else
                n=1;
            if (n==1)
            {
                n=0;
                for (int i=0;i<filtri.size();i++)
                {
                    for (int j=0;j<obparametri.size();j++)
                    {
                        if (obparametri[j].at(0).indexOf(filtri[i]->name)>-1)
                        {
                            float f=obparametri[j].at(l).toFloat();
                            if (f>=filtri[i]->min && f<=filtri[i]->max)
                            {
                                n++;
                            }
                        }
                    }
                }
                if (n==filtri.size())
                {
                    dob++;
                    for (int k=0;k<obparametri.size();k++)
                    {
                        filtrparametri[k]<<obparametri[k].at(l);
                    }
                    float dolgsam=filtrparametri[nomDolgSam].last().toFloat();
                    float shirsam=filtrparametri[nomShirSam].last().toFloat();
                    float kurs=filtrparametri[nomKursSam].last().toFloat();
                    float AzRas=nachazim(dolgsam,shirsam,DolgCeli,ShirCeli)-kurs;

                    if (AzRas>=360)
                        AzRas=AzRas-360.0;

                    if (AzRas<0)
                        AzRas=AzRas+360.0;

                    if (AzRas>180)
                        AzRas=AzRas-360.0;

                    float DalRas=calculateTheDistance(dolgsam,shirsam,DolgCeli,ShirCeli);
                    float AzIzm=filtrparametri[0].last().toFloat();

                    float delta;
                    if (fabs(AzIzm-AzRas)<=180)
                    {
                        delta=fabs(AzIzm-AzRas);
                    }
                    else
                    {
                        delta=360-fabs(AzIzm-AzRas);
                    }

                    filtrparametri[obparametri.size()]<<QString().number(AzRas);
                    filtrparametri[obparametri.size()+1]<<QString().number(DalRas);
                    filtrparametri[obparametri.size()+2]<<QString().number(delta);
                    filtrparametri[obparametri.size()+3]<<"";
                    filtrparametri[obparametri.size()+4]<<"";
                }
            }
        }

        sch++;

        if (sch==kolcel)
        {
            if (dob>1)
            {
                for(int kl=0;kl<dob;kl++)
                {
                    for(int nm=0;nm<filtrparametri.size();nm++)
                        dvoenie[nm]<<filtrparametri[nm][filtrparametri[nm].size()-(dob-kl)];
                }
                for(int kl=0;kl<dob;kl++)
                {
                    for(int nm=0;nm<filtrparametri.size();nm++)
                    {
                        filtrparametri[nm].removeLast();
                    }
                }
            }
            dob=0;
            sch=0;
        }
    }

    float s=0;
    float MatOg=0;
    float skoZnach=0;

    addparam(otchparametri,"M_D");

    sch=0;

    for (int i=1;i<filtrparametri[0].size();i++)
    {
        float AzR=filtrparametri[obparametri.size()].at(i).toFloat();
        float DalR=filtrparametri[obparametri.size()+1].at(i).toFloat();
        float delta=filtrparametri[obparametri.size()+2].at(i).toFloat();
        float Tang=filtrparametri[nomTang].at(i).toFloat();
        float Noski=filtrparametri[nomNoski].at(i).toFloat();
        float H=filtrparametri[nomH].at(i).toFloat();

        bool ProvPolusfer=false;

        if (Polusfer)
        {
            if (AzR>=-40 && AzR<=40)
                ProvPolusfer=true;
        }
        else
        {
            if (AzR<0)
                AzR=360.0+AzR;
            if (AzR>=40 && AzR<=320)
                ProvPolusfer=true;
        }

        float Dmin=H/cos(DegToRad(90-(30-Tang+Noski)));

        if (ProvPolusfer && DalR>Dmin && DalR<MaxDaln && delta<MaxOtkl)
        {
            for (int k=0;k<filtrparametri.size();k++)
            {
                otchparametri[k]<<filtrparametri[k].at(i);
            }

            s=s+otchparametri[obparametri.size()+2].last().toFloat();
            sch++;

            MatOg=s/sch;
            otchparametri[obparametri.size()+3].last()=QString().number(MatOg);

            float m_d=MatOg-otchparametri[obparametri.size()+2].last().toFloat();
            skoZnach=skoZnach+m_d*m_d;

            float z=skoZnach/(otchparametri[0].size()-1);
            z=sqrt(z);

            otchparametri[obparametri.size()+4].last()=QString().number(z);

            otchparametri.last()<<QString().number(m_d);
        }
    }

    MatOg=s/sch;

    skoZnach=skoZnach/(otchparametri[0].size()-1);

    skoZnach=sqrt(skoZnach);


    int pred=dvoenie[0].size();

    for (int i=1;i<pred;i++)
    {
        float AzR=dvoenie[obparametri.size()].at(i).toFloat();
        float DalR=dvoenie[obparametri.size()+1].at(i).toFloat();
        float delta=dvoenie[obparametri.size()+2].at(i).toFloat();
        float Tang=dvoenie[nomTang].at(i).toFloat();
        float Noski=dvoenie[nomNoski].at(i).toFloat();
        float H=dvoenie[nomH].at(i).toFloat();

        bool ProvPolusfer=false;

        if (Polusfer)
        {
            if (AzR>=-40 && AzR<=40)
                ProvPolusfer=true;
        }
        else
        {
            if (AzR<0)
                AzR=360.0+AzR;
            if (AzR>=40 && AzR<=320)
                ProvPolusfer=true;
        }

        float Dmin=H/cos(DegToRad(90-(30-Tang+Noski)));

        if (!(ProvPolusfer && DalR>Dmin && DalR<MaxDaln && delta<MaxOtkl))
        {
            for (int k=0;k<dvoenie.size();k++)
            {
                dvoenie[k].removeAt(i);
            }
            pred--;
            i--;
        }
    }

    //ekspFiltr();

    ekspOtch(MatOg,skoZnach);

    ekspDvoen();

    ekspOtch(MatOg,skoZnach,qApp->applicationDirPath()+"/1.pvt");

    return true;
}

void obrabotka::ZagrFiltra()
{
    for (int i=0;i<filtri.size();i++)
        delete filtri[i];
    filtri.clear();

    for (int i=0;i<timVichet.size();i++)
        delete timVichet[i];
    timVichet.clear();

    for (int i=0;i<timTolko.size();i++)
        delete timTolko[i];
    timTolko.clear();

    QString files = QFileDialog::getOpenFileName(0, "Open Dialog", qApp->applicationDirPath(), "BU3 (*.bu3)");
    QFile file(files);

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_2) ;
        
        QString name;
        float min;
        float max;

        int n;
        stream >> n;
        
        for (int i=0;i<n;i++)
        {
            stream >> name;
            stream >> min;
            stream >> max;

            filtri<<new Filtr(&filtri);
            filtri.last()->setFiltr(name,min,max);
            LayoutFiltri->addLayout(filtri.last());
        }

        stream >> n;

        for (int i=0;i<n;i++)
        {
            stream >> min;
            stream >> max;

            sko<<new FiltrTime(&sko);
            sko.last()->setFiltr("1",min,max);
            LayoutSKO->addLayout(sko.last());
        }

        stream >> n;

        for (int i=0;i<n;i++)
        {
            stream >> min;
            stream >> max;

            timVichet<<new FiltrTime(&timVichet);
            timVichet.last()->setFiltr("1",min,max);
            LayoutTime->addLayout(timVichet.last());
        }

        stream >> n;

        for (int i=0;i<n;i++)
        {
            stream >> min;
            stream >> max;

            timTolko<<new FiltrTime(&timTolko);
            timTolko.last()->setFiltr("1",min,max);
            LayoutTime1->addLayout(timTolko.last());
        }

        QString s;
        stream>>s;
        lC->setText(s);
        setDolgCeli(s);
        stream>>s;
        fC->setText(s);
        setShirCeli(s);

        stream>>s;
        maxotklLaut->setText(s);
        setMaxOtkl(s);
        stream>>s;
        maxDalnLaut->setText(s);
        setDaln(s);

        bool b;
        stream>>b;
        Polusfer=b;
        CheckBox->setChecked(Polusfer);

        stream>>s;
        KolCelLaut->setText(s);
        setKolCel(s);
        stream>>s;
        KolParCelLaut->setText(s);
        setKolParCel(s);
        stream>>s;
        KolParObLaut->setText(s);
        setKolOsnPar(s);

        file.close();
    }
}

void obrabotka::SohrFiltra()
{
    QString files = QFileDialog::getSaveFileName(0, "Save Dialog", qApp->applicationDirPath(), "BU3 (*.bu3)");

    if (files!="")
    {
        if ( files.section( ".", -1, -1 ) != "bu3" )
                 files+= ".bu3";

        QFile file(files);
        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Qt_5_2) ;

            int n=filtri.size();
            stream << n;

            for (int i=0;i<filtri.size();i++)
            {
                stream << filtri.at(i)->name;
                stream << filtri.at(i)->min;
                stream << filtri.at(i)->max;
            }

            n=sko.size();
            stream << n;

            for (int i=0;i<sko.size();i++)
            {
                stream << sko.at(i)->min;
                stream << sko.at(i)->max;
            }

            n=timVichet.size();
            stream << n;

            for (int i=0;i<timVichet.size();i++)
            {
                stream << timVichet.at(i)->min;
                stream << timVichet.at(i)->max;
            }

            n=timTolko.size();
            stream << n;

            for (int i=0;i<timTolko.size();i++)
            {
                stream << timTolko.at(i)->min;
                stream << timTolko.at(i)->max;
            }
            
            stream<<lC->text();
            stream<<fC->text();

            stream<<maxotklLaut->text();
            stream<<maxDalnLaut->text();

            stream<<Polusfer;

            stream<<KolCelLaut->text();
            stream<<KolParCelLaut->text();
            stream<<KolParObLaut->text();

            file.close();
            if (stream.status() != QDataStream::Ok)
            {
                qDebug() << "Ошибка записи файла";
            }
        }
    }
}

void obrabotka::setDolgCeli(QString s)
{
    if (QString(s.at(0))!=QString("В") && QString(s.at(0))!=QString("З"))
    {
        lC->setText("В"+QString(s.at(0)));
        lC->setCursorPosition(2);
    }
    QString SL=lC->displayText();
    QString S;
    float L;
    S=SL.section("",9,13);
    L=S.toDouble()/60;
    S=SL.section("",6,7);
    L=(L+S.toDouble())/60;
    S=SL.section("",2,4);
    L=L+S.toDouble();
    if (QString(SL.at(0))==QString("З"))
        L=L*(-1.0);

    DolgCeli=L;
}

void obrabotka::Graf(QList<QStringList>& list)
{
    QStringList l;

    l<<"Выберети";

    for (int j=0;j<list.size();j++)
    {
        l<<list[j].at(0);
    }

    boxGrafik=new QComboBox();
    boxGrafik->addItems(l);

    connect(boxGrafik, &QComboBox::currentTextChanged,
    [&](QString s)
    {
        int n=0;

        for (int j=0;j<list.size();j++)
        {
            if (list[j].at(0).indexOf(s)>-1)
            {
                n=j;
                break;
            }
        }

        grafik=new Grafik();
        QList<float> *t=new QList<float>;
        for (int j=1;j<list[n].size();j++)
            *t<<list[n][j].toFloat();
        for (int i=0;i<list.size();i++)
        {
            QList<float>* v=new QList<float>;
            QString s=list[i][0];
            for (int j=1;j<list[i].size();j++)
                *v<<list[i][j].toFloat();
            grafik->addParametr(s, v, t);
        }
        grafik->show();

        boxGrafik->deleteLater();
    });

    boxGrafik->show();
}

void obrabotka::setShirCeli(QString s)
{

    if (QString(s.at(0))!=QString("С") && QString(s.at(0))!=QString("Ю"))
    {
        fC->setText("С"+QString(s.at(0)));
        fC->setCursorPosition(2);
    }

    QString SF=fC->displayText();
    QString S;
    float F;
    S=SF.section("",8,12);
    F=S.toDouble()/60;
    S=SF.section("",5,6);
    F=(F+S.toDouble())/60;
    S=SF.section("",2,3);
    F=F+S.toDouble();
    if (QString(SF.at(0))==QString("Ю"))
        F=F*(-1.0);

    ShirCeli=F;
}

void obrabotka::setMaxOtkl(QString s)
{
    MaxOtkl=s.toFloat();
}

void obrabotka::setDaln(QString s)
{
    MaxDaln=s.toFloat();
}

void obrabotka::setKolCel(QString s)
{
    kolcel=s.toFloat();
}

void obrabotka::setKolParCel(QString s)
{
    kolosnpar=s.toFloat();
}

void obrabotka::setKolOsnPar(QString s)
{
    NomPerOsnPar=s.toFloat();
}



void Filtr::setFiltr(QString nam,float mi,float ma)
{
    name=nam;
    Name.setCurrentText(nam);
    min=mi;
    Min.setText(QString().number(mi));
    max=ma;
    Max.setText(QString().number(ma));
}

Filtr::Filtr(QList<Filtr *>* filtr,QWidget *parent):QHBoxLayout(parent)
{
    Name.addItems(SpisPar);

    Fil=filtr;

    void (QComboBox::*Sender)(const QString &) = &QComboBox::currentIndexChanged;

    connect(&Name, Sender,
    [&](QString s)
    {
        name=s;
    });

    Min.setText("0");

    connect(&Min, &QLineEdit::editingFinished,
    [&]()
    {
        min=Min.text().toFloat();
    });

    Max.setText("1");

    min=0;
    max=1;

    connect(&Max, &QLineEdit::editingFinished,
    [&]()
    {
        max=Max.text().toFloat();
    });

    Udal.setText("Удалить");

    connect(&Udal, &QPushButton::clicked,
    [&]()
    {
        Fil->removeAll(this);
        delete this;
    });

    this->addWidget(&Name);
    this->addWidget(&Min);
    this->addWidget(&Max);
    this->addWidget(&Udal);
}

void FiltrTime::setFiltr(QString nam,float mi,float ma)
{
    name=nam;
    Name.setText(nam);
    min=mi;
    Min.setText(QString().number(mi));
    max=ma;
    Max.setText(QString().number(ma));
}

FiltrTime::FiltrTime(QList<FiltrTime *>* filtr,QWidget *parent):QHBoxLayout(parent)
{
    Name.setText("1");

    Fil=filtr;

    connect(&Name, &QLineEdit::editingFinished,
    [&]()
    {
        name=Name.text();
    });

    Min.setText("0");

    connect(&Min, &QLineEdit::editingFinished,
    [&]()
    {
        min=Min.text().toFloat();
    });

    Max.setText("1");

    min=0;
    max=1;

    connect(&Max, &QLineEdit::editingFinished,
    [&]()
    {
        max=Max.text().toFloat();
    });

    Udal.setText("Удалить");

    connect(&Udal, &QPushButton::clicked,
    [&]()
    {
        Fil->removeAll(this);
        delete this;
    });

    this->addWidget(&Name);
    this->addWidget(&Min);
    this->addWidget(&Max);
    this->addWidget(&Udal);
}

