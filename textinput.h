#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegExpValidator>
class TextInput : public QWidget
{
    Q_OBJECT
public:
    explicit TextInput(QWidget *parent = nullptr, QString lab = "");
    ~TextInput();
    QString getText();
private:
    QHBoxLayout *layout;
    QLabel *label;
    QLineEdit *lineEdit;
    QString text;

private slots:

};

#endif // TEXTINPUT_H
