#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <QMainWindow>
#include <QSpinBox>
#include <QLineEdit>
#include <QStringList>




namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT


private:
    Ui::MainWindow* ui;

    QList<QSpinBox*> spins;        //спины с кол-вом критериев на уровне
    QList<QToolButton*> attachments;

    QList<QLineEdit*> lineEdits;   //для альтернатив

    QList<QList<QStringList>> critNames; //имена критериев и альтернатив


public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_resetButton_clicked();
    void on_addButton_clicked();
    void on_toolbut_clicked();
    void on_altsSpin_valueChanged(int value);
    void on_okButton_clicked();
};


#endif // MAIN_WINDOW_H_INCLUDED
