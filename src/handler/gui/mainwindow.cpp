#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() // this code executes when the button is pushed
{
    ui -> textBrowser -> setText("");
    QDir directory("./errors");
    qDebug() << "got directory";
    QStringList dirlist = directory.entryList();
    //error file list
    qDebug() << "got directory list" << dirlist;
    if(dirlist.count() == 2){
        QMessageBox::critical(this, "Error", "There are no error files in ./errors directory");
        return;
    }
    for(int i = 2; i < dirlist.count(); ++i){
        qDebug() << "###########################################";
        qDebug() << "processing " << dirlist.at(i);
        QFile file("./errors/" + dirlist.at(i));
        qDebug() << "file opened";
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
               qDebug() << "Entering error handeling";
               return;
        }
        QTextStream in(&file);
        QString currentstr = in.readLine();
        qDebug() << "string: " << currentstr;
        //parse the error sstring and feed it to QDateTime for it to convert to time type
        QStringList chunks = currentstr.split("_");
        QDateTime datetime = QDateTime::fromString(chunks.at(2).mid(0, 17), "yy/MM/dd,hh:mm:ss");
        //add 7 hours(Krasnoyarsk is GMT+7)
        datetime.addSecs(7*60*60);
        qDebug() << datetime.toString();
        //if the checkbox is checked, erase the directory after displaying the entrys
        if(chunks.at(1) == "DATA"){
            ui -> textBrowser -> append("This is a data error at #" + chunks.at(3) + " at " + datetime.toString() + " GMT+7");
        }
        else if(chunks.at(1) == "SAT"){
            ui -> textBrowser -> append("This is a sattelite error at #" + chunks.at(3) + " at " + datetime.toString() + " GMT+7");
        }
        if(ui -> checkBox -> isChecked()){
            if(!file.remove()){
                qDebug() << "Error removing file";
            }
        }
    }
}
