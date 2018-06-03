#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void handleErr();
    void realTimeProcess();

private slots:

    void on_clr_button_clicked();
    void update();
    void on_show_button_clicked();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
