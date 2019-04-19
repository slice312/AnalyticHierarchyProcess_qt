#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <QDialog>
#include <QStringList>
#include <QLineEdit>



namespace Ui {
class input;
}

class input : public QDialog
{
    Q_OBJECT

public:
    explicit input(int prev, int current, QWidget* parent = nullptr);
    ~input();

private:
    int count;
    Ui::input* ui;
    QList<QLineEdit*> lines;


public:
    QList<QStringList> getNames();
};

#endif // INPUT_H_INCLUDED
