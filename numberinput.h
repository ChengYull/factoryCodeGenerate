#ifndef NUMBERINPUT_H
#define NUMBERINPUT_H

#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>
class NumberInput : public QWidget
{
    Q_OBJECT
public:
    explicit NumberInput(QWidget *parent = nullptr, QString lab = "");
    ~NumberInput();
    int getValue();
private:
    QHBoxLayout *layout;
    QLabel *label;
    QSpinBox *spin;
    QSlider *slider;
signals:
    void valueChanged();
private slots:
    void onSpinValueChanged();
};

#endif // NUMBERINPUT_H
