#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <QMainWindow>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <QPair>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT


private:
    Ui::MainWindow* ui;
    QList<QSpinBox*> spins;
    QList<QToolButton*> attachments;
    QList<QLineEdit*> lineEdits;

    QList<QList<QStringList>> critNames;


public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_resetButton_clicked();
    void on_okButton_clicked();
    void on_altsSpin_valueChanged(int value);


    void toolButton();

};









#endif // MAIN_WINDOW_H_INCLUDED
