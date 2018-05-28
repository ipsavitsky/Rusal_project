#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui -> requestButton, SIGNAL(clicked()), this, SLOT(processRequest()));
    connect(ui -> lineEdit, SIGNAL(returnPressed()), this, SLOT(processRequest()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clearButton_clicked()//if clearButton clicked
{
    ui -> textBrowser -> setText("");
    ui -> lineEdit -> setText("");
}

void MainWindow::processRequest(){ //triggered when enter in the line pressed or requestButton clicked
    QFile indexFile("indexTime.txt");
    if (!indexFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this, "Error", "Could not find indexFile directory");
        //ui -> textBrowser -> append("Could not open index file");
        return;
    }
    QString prompt(ui -> lineEdit -> text());
    if(!prompt.contains("SELECT")){
        QMessageBox::critical(this, "Error", "Parsing error: no SELECT statement");
        return;
    }
    QStringList chunks = prompt.split(" ");
    QString IMEI("");
    QString Date("");
    QString Time("");
    if(chunks.contains("IMEI")){
        int position = chunks.indexOf("IMEI");
        IMEI = chunks.at(position+1);
        qDebug() << "IMEI =" << IMEI;
    }
    if(chunks.contains("TIME")){
        int position = chunks.indexOf("TIME");
        Time = chunks.at(position+1);
        qDebug() << "Time =" << Time;
    }
    if(chunks.contains("DATE")){
        int position = chunks.indexOf("DATE");
        Date = chunks.at(position+1);
        qDebug() << "Date =" << Date;
    }
    while(!indexFile.atEnd()){
        QString curline(indexFile.readLine());
        //parse the query with TIME, DATE and IMEI statements
        QStringList parts = curline.split("/");
        if(((parts.at(2) == IMEI) || (IMEI == "")) && ((parts.at(3) == Date) || (Date == "")) && ((parts.at(4) == Time + ".txt\n") || (Time == ""))){
            ui -> textBrowser -> append(curline.chopped(1));
        }
    }
    ui -> textBrowser -> append("#######################################");
}
