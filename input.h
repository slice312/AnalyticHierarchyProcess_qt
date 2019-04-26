#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <QDialog>
#include <QStringList>
#include <QLineEdit>



namespace Ui {
class Input;
}


class Input : public QDialog
{
    Q_OBJECT

private:
    Ui::Input* ui;
    QList<QLineEdit*> qlines;
    int groups;

public:
    Input(int lines, int groups, QWidget* parent = nullptr);
    ~Input();

    QList<QStringList> getNames() const;
};

#endif // INPUT_H_INCLUDED
