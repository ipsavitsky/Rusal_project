#ifndef ADD_NEW_MAP_H
#define ADD_NEW_MAP_H

#include <QDialog>

namespace Ui {
class add_new_map;
}

class add_new_map : public QDialog
{
    Q_OBJECT

public:
    explicit add_new_map(QWidget *parent = 0);
    ~add_new_map();

private slots:
    void on_choose_file_button_clicked();

    void on_buttonBox_accepted();

private:
    Ui::add_new_map *ui;
};

#endif // ADD_NEW_MAP_H
