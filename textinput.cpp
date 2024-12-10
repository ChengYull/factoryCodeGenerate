#include "textinput.h"

TextInput::TextInput(QWidget *parent, QString lab)
    : QWidget{parent}
{
    label = new QLabel(lab, this);
    lineEdit = new QLineEdit(this);
    // 验证器 以字母开头（可以包括字母和下划线）
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[A-Za-z][A-Za-z0-9_]*$"), this); // 只接受字母和数字
    lineEdit->setValidator(validator);

    lineEdit->setPlaceholderText("请输入...(以字母为起始)");
    layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
}

TextInput::~TextInput(){
    delete label;
    delete lineEdit;
    delete layout;
}

QString TextInput::getText(){
    return lineEdit->text();
}
