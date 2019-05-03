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

private:

    QList<QLineEdit*> lineEdits;   //для альтернатив

    QList<QList<QStringList>> critNames; //имена критериев и альтернатив


public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateActions();

private slots:
    void on_resetButton_clicked();
    void on_altsSpin_valueChanged(int value);
    void on_mOkButton_clicked();

    void insertRow();
    void insertChild();
    void removeRow();

};


#endif // MAIN_WINDOW_H_INCLUDED
