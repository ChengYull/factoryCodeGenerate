#include "numberinput.h"

NumberInput::NumberInput(QWidget *parent, QString lab)
    : QWidget{parent}
{
    layout = new QHBoxLayout(this);
    label = new QLabel(lab, this);
    spin = new QSpinBox(this);
    spin->setRange(1,28);   // 设置范围
    spin->setValue(1);      // 设置初始值
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal); // 设置滑块为横向
    slider->setRange(1,28); // 设置范围
    slider->setValue(1);    // 设置初始值


    // 连接信号和槽
    connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);
    connect(slider, &QSlider::valueChanged, spin, &QSpinBox::setValue);
    // spin改变时发出自定义信号
    connect(spin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &NumberInput::onSpinValueChanged);
    // 添加到布局中
    layout->addWidget(label);
    layout->addWidget(spin);
    layout->addWidget(slider);
}

NumberInput::~NumberInput(){
    delete spin;
    delete slider;
    delete layout;
}

void NumberInput::onSpinValueChanged() {
    // 当spin的值改变时，发出自定义信号
    emit valueChanged();
}


int NumberInput::getValue(){
    return spin->value();
}
