#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add_new_map.h"
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QGraphicsItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir bin("./");
    QString errors("");
    QDir resources("./resources");
    QStringList resdir = resources.entryList();
    resdir.removeFirst();
    resdir.removeFirst();
    if(!bin.entryList().contains("database"))
        errors.append("There is no database installed\n");
    if(!bin.entryList().contains("fste.out"))
        errors.append("There is no fste.out");
    for(int i = 0; i < resdir.count(); i++){
        QDir curRes("./resources/"+resdir.at(i));
        if(!curRes.entryList().contains("test.bmp"))
            errors.append("There is no test.bmp in " + resources.entryList().at(i) + "resources folder \n");
        if(!curRes.entryList().contains("coor.txt"))
            errors.append("There is no coor.txt in " + resources.entryList().at(i) + "resources folder \n");
    }
    if(errors != "")
        QMessageBox::critical(this, "ERROR", errors);
    ui->setupUi(this);
    ui -> handler -> append("Welcome to the control centre!");
    //ui -> statusBar -> showMessage("0");
    QImage logo("./logo.bmp");
    scene -> addPixmap(QPixmap::fromImage(logo));
    ui -> map -> setScene(scene);
    ui -> comboBox -> addItems(resdir);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer -> start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clr_button_clicked()
{
    QDir errdir("./errors");
    for(int i = 2; i < errdir.entryList().count(); i++){
        QFile file("./errors/" + errdir.entryList().at(i));
        if(!file.remove()){
            ui -> statusBar -> showMessage("error removing ./errors" + errdir.entryList().at(i));
        }
    }
}

void MainWindow::update(){
    //QDir resources("./resources");
    QString curItem = ui -> comboBox -> currentText();
    QGraphicsItem *item = scene -> items().at(0);
    scene -> removeItem(item);
    delete item;
    QImage map("./resources/"+curItem+"/test_1.bmp");
    scene -> addPixmap(QPixmap::fromImage(map));
    ui -> map -> fitInView(scene -> sceneRect(), Qt::KeepAspectRatio);
    ui -> map -> setScene(scene);
    //qDebug() << "scene set";
    if(ui -> RTUpdates -> isChecked()){
        realTimeProcess();
    }
    handleErr();
    system("./database");
    QString prompt("./fste.out ./resources/"+curItem);
    QByteArray ba = prompt.toLatin1();
    const char *c_str = ba.data();
    system(c_str);
}

void MainWindow::on_show_button_clicked()
{
    ui -> RTUpdates -> setChecked(false);
    QFile indexFile("indexTime.txt");
    if (!indexFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this, "Error", "Could not find indexFile directory");
        //ui -> textBrowser -> append("Could not open index file");
        return;
    }
    QString IMEI("");
    QString Date("");
    QString fromTime("");
    QString toTime("");
    if(ui -> IMEI_chk -> isChecked()){
        IMEI = ui -> IMEI_ln -> text();
        qDebug() << "IMEI =" << IMEI;
    }
    if(ui -> Time_chk -> isChecked()){
        fromTime = ui -> timeEditFrom -> time().toString();
        toTime = ui -> timeEditTo -> time().toString();
        qDebug() << "Time =" << fromTime << "-" << toTime;
    }
    if(ui -> Date_chk -> isChecked()){
        Date = ui -> dateEdit -> date().toString("dd.MM.yyyy");
        qDebug() << "Date =" << Date;
    }
    QFile outf("indexRequest.txt");
    if(outf.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&outf);
        QStringList prompt;
        while(!indexFile.atEnd()){
            QString curline(indexFile.readLine());
            //parse the query with TIME, DATE and IMEI statements
            QStringList parts = curline.split("/");
            if(((parts.at(2) == IMEI) || (IMEI == "")) && ((parts.at(3) == Date) || (Date == "")) && (((parts.at(4) >= fromTime + ".txt\n") || (fromTime == "")) && ((parts.at(4) <= toTime + ".txt\n") || (toTime == "")))){
                //ui -> handler -> append(curline.chopped(1));
                prompt.append(curline.chopped(1) + "\n");
                //out << curline.chopped(1) << "\n";
            }
        }
        //qDebug()<<prompt;
        out << prompt.size() << "\n";
        out << prompt.join("");
    }
    //ui -> handler -> append("#######################################");
}

void MainWindow::handleErr(){
    ui -> handler -> setText("");
    //qDebug() << "in handle err";
    QDir directory("./errors");
    QStringList dirlist = directory.entryList();
    //qDebug() << dirlist;
    //error file list
    for(int i = 2; i < dirlist.count(); ++i){
        QFile file("./errors/" + dirlist.at(i));
        //qDebug() << "file opened";
        file.open(QFile::ReadOnly | QFile::Truncate);
        QString currentstr = file.readLine();
        //qDebug() << currentstr;
        //parse the error string and feed it to QDateTime for it to convert to time type
        QStringList chunks = currentstr.split("_");
        QDateTime datetime = QDateTime::fromString(chunks.at(2).mid(0, 17), "yy/MM/dd,hh:mm:ss");
        //add 7 hours(Krasnoyarsk is GMT+7)
        datetime.addSecs(7*60*60);
        if(chunks.at(1) == "DATA"){
            ui -> handler -> append("This is a data error at #" + chunks.at(3).chopped(1) + " at " + datetime.toString("yy/MM/dd,hh:mm:ss") + " GMT+7");
        }
        else if(chunks.at(1) == "SAT"){
            ui -> handler -> append("This is a sattelite error at #" + chunks.at(3).chopped(1) + " at " + datetime.toString("yy/MM/dd,hh:mm:ss") + " GMT+7");
        }
    }
}


void MainWindow::realTimeProcess(){
    QFile indReq("indexRequest.txt");
    QFile indTime("indexTime.txt");
    indReq.open(QFile::WriteOnly | QFile::Truncate);
    indTime.open(QFile::ReadOnly | QFile::Truncate);
    QTextStream ReqSt(&indReq);
    QString data = indTime.readAll();
    QStringList datal = data.split("\n");
    ReqSt << datal.count() << "\n";
    ReqSt << data;
}

void MainWindow::on_pushButton_clicked()
{
    add_new_map window;
    window.setModal(true);
    window.exec();
}
