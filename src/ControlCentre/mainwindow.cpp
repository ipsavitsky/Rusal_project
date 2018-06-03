#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir resources1("./resources/map1");
    QDir resources2("./resources/map2");
    QDir resources3("./resources/map3");
    QDir bin("./");
    QString errors("");
    if(!bin.entryList().contains("database"))
        errors.append("There is no database installed\n");
    if(!bin.entryList().contains("fste.out"))
        errors.append("There is no fste.out");
    //if(!resources1.entryList().contains("fste.out"))
    //    errors.append("There is no fste.out for map_1 installed\n");
    //if(!resources2.entryList().contains("fste.out"))
    //    errors.append("There is no fste.out for map_2 installed\n");
    //if(!resources3.entryList().contains("fste.out"))
    //    errors.append("There is no fste.out for map_3 installed\n");
    if(!resources1.entryList().contains("test.bmp"))
        errors.append("There is no base map for map_1\n");
    if(!resources2.entryList().contains("test.bmp"))
        errors.append("There is no base map for map_2\n");
    if(!resources3.entryList().contains("test.bmp"))
        errors.append("There is no base map for map_3\n");
    if(!resources1.entryList().contains("coor.txt"))
        errors.append("There is no coordinates configuration for map_1\n");
    if(!resources2.entryList().contains("coor.txt"))
        errors.append("There is no coordinates configuration for map_2\n");
    if(!resources3.entryList().contains("coor.txt"))
        errors.append("There is no coordinates configuration for map_3\n");
    if(errors != "")
        QMessageBox::critical(this, "ERROR", errors);
    ui->setupUi(this);
    ui -> handler -> append("Welcome to the control centre!");
    //ui -> statusBar -> showMessage("0");
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
    QImage map1("./resources/map1/test_1.bmp");
    QImage map2("./resources/map2/test_1.bmp");
    QImage map3("./resources/map3/test_1.bmp");
    QGraphicsScene *scene = new QGraphicsScene(this);
    if(ui -> map_1_radio -> isChecked()){
        scene -> addPixmap(QPixmap::fromImage(map1));
        ui -> statusBar -> showMessage("map 1 set");
    }
    if(ui -> map_2_radio -> isChecked()){
        scene -> addPixmap(QPixmap::fromImage(map2));
        ui -> statusBar -> showMessage("map 2 set");
    }
    if(ui -> map_3_radio -> isChecked()){
        scene -> addPixmap(QPixmap::fromImage(map3));
        ui -> statusBar -> showMessage("map 3 set");
    }
    ui -> map -> setScene(scene);
    if(ui -> RTUpdates -> isChecked()){
        realTimeProcess();
    }
    handleErr();
    system("./database");
    if(ui -> map_1_radio -> isChecked()){
        system("./fste.out 1");
    }
    if(ui -> map_2_radio -> isChecked()){
        system("./fste.out 2");
    }
    if(ui -> map_3_radio -> isChecked()){
        system("./fste.out 3");
    }
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
    QString Time("");
    if(ui -> IMEI_chk -> isChecked()){
        IMEI = ui -> IMEI_ln -> text();
        qDebug() << "IMEI =" << IMEI;
    }
    if(ui -> Time_chk -> isChecked()){
        Time = ui -> timeEdit -> time().toString();
        qDebug() << "Time =" << Time;
    }
    if(ui -> Date_chk -> isChecked()){
        Date = ui -> dateEdit -> date().toString("dd.MM.yyyy");
        qDebug() << "Date =" << Date;
    }
    QFile outf("indexRequest.txt");
    if(outf.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&outf);
        while(!indexFile.atEnd()){
            QString curline(indexFile.readLine());
            //parse the query with TIME, DATE and IMEI statements
            QStringList parts = curline.split("/");
            QString prompt("");
            if(((parts.at(2) == IMEI) || (IMEI == "")) && ((parts.at(3) == Date) || (Date == "")) && ((parts.at(4) == Time + ".txt\n") || (Time == ""))){
                //ui -> handler -> append(curline.chopped(1));
                prompt.append(curline.chopped(1) + "\n");
                //out << curline.chopped(1) << "\n";
            }
            out << curline.count() << "\n";
            out << prompt;
        }
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
        //if the checkbox is checked, erase the directory after displaying the entrys
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
