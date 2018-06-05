#include "add_new_map.h"
#include "ui_add_new_map.h"
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

add_new_map::add_new_map(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_new_map)
{
    ui->setupUi(this);
}

add_new_map::~add_new_map()
{
    delete ui;
}

void add_new_map::on_choose_file_button_clicked()
{
    QString bmpFilename = QFileDialog::getOpenFileName(this, "Open file...", "~", "Windows bitmap pictures (*.bmp);; All files (*.*)");
    ui -> filename_edit -> setText(bmpFilename);
    QImage selectedImg(bmpFilename);
    ui -> height -> setValue(selectedImg.height());
    ui -> width -> setValue(selectedImg.width());
}

void add_new_map::on_buttonBox_accepted()
{
    QString bmpFilename = ui -> filename_edit -> text();
    QString mapName = ui -> map_name_edit -> text();
    int height = ui -> height -> cleanText().toInt();
    int width = ui -> width -> cleanText().toInt();
    double lat1 = ui -> latitude1_edit -> text().toDouble();
    double lat2 = ui -> latitude2_edit -> text().toDouble();
    double lon1 = ui -> longitude1_edit -> text().toDouble();
    double lon2 = ui -> longitude2_edit -> text().toDouble();
    double lat3 = ui -> latitude3_edit -> text().toDouble();
    double lon3 = ui -> longitude3_edit -> text().toDouble();
    double lat4 = ui -> latitude4_edit -> text().toDouble();
    double lon4 = ui -> longitude4_edit -> text().toDouble();
    QDir mapdir("./resources/"+mapName);
    if(!mapdir.exists()){
        mapdir.mkpath(".");
    }
    QFile coor("./resources/"+mapName+"/coor.txt");
    QString prompt("cp "+bmpFilename+" ./resources/" + mapName + "/test.bmp");
    QByteArray ba = prompt.toLatin1();
    const char *c_str = ba.data();
    system(c_str);
    if(coor.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream coorStr(&coor);
        coorStr << width << " " << height << qSetRealNumberPrecision(10) << "\n" << lat1 << " " << lon1 << "\n" << lat3 << " " << lon3 << "\n" << lat2 << " " << lon2 << "\n" << lat4 << " " << lon4 << "\n";
    }
    QMessageBox::warning(this, "Warning", "Restart the program to apply changes");
}
