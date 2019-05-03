#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <QMainWindow>
#include <QSpinBox>
#include <QLineEdit>
#include <QStringList>
#include "ui_mainwindow.h"



class TreeModel;


class MainWindow : public QMainWindow,  private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();



private slots:
    void updateActions();

    void on_mAltsSpin_valueChanged(int value);
    void on_mOkButton_clicked();
    void reset();

    void insertRow();
    void insertChild();
    void removeRow();
};


#endif // MAIN_WINDOW_H_INCLUDED
