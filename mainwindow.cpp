#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QProcess"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    obr=new obrabotka(ui->lineEdit_7,ui->lineEdit_9,ui->lineEdit_8,ui->lineEdit_3,ui->lineEdit_4,
                      ui->lineEdit_5,ui->lineEdit_6,ui->widget,ui->widget_2,ui->widget_3,ui->widget_4,ui->checkBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(obr->ZagrDan())
        ui->pushButton_3->setEnabled(true);
    else
    {
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    obr->DobFiltr();
}

//void MainWindow::on_pushButton_7_clicked()
//{
//    obr->DobSKO();
//}

void MainWindow::on_pushButton_6_clicked()
{
    obr->SohrFiltra();
}

void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    obr->setDolgCeli(arg1);
}

void MainWindow::on_lineEdit_4_textEdited(const QString &arg1)
{
    obr->setShirCeli(arg1);
}

void MainWindow::on_pushButton_5_clicked()
{
    obr->ZagrFiltra();
//    if(obr->ZagrDan())
//        ui->pushButton_3->setEnabled(true);
//    else
//        ui->pushButton_3->setEnabled(false);
//    if (obr->Filracia())
//        ui->pushButton_4->setEnabled(true);
//    else
//        ui->pushButton_4->setEnabled(false);
//    obr->Graf();
}

void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)
{
    obr->setMaxOtkl(arg1);
}

void MainWindow::on_lineEdit_6_textEdited(const QString &arg1)
{
    obr->setDaln(arg1);
}

void MainWindow::on_lineEdit_7_textEdited(const QString &arg1)
{
    obr->setKolCel(arg1);
}

void MainWindow::on_lineEdit_9_textEdited(const QString &arg1)
{
    obr->setKolParCel(arg1);
}

void MainWindow::on_lineEdit_8_textEdited(const QString &arg1)
{
    obr->setKolOsnPar(arg1);
}

void MainWindow::on_pushButton_9_clicked()
{
    obr->DobTimeFilVichet();
}

void MainWindow::on_pushButton_8_clicked()
{
    obr->DobTimeFilTolko();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    obr->SetPolusfer(checked);
}

void MainWindow::on_pushButton_3_clicked()
{
    if (obr->Filracia())
    {
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
    }
    else
    {
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    obr->Graf(obr->otchparametri);
    //obr->Graf(obr->dvoenie);
}

void MainWindow::on_pushButton_7_clicked()
{
    system(qPrintable(QString(qApp->applicationDirPath()+"/open.exe")));
}
