#include "pathSelect.h"

PathSelect::PathSelect(QWidget *parent)
    : QWidget{parent}
{
    label = new QLabel("选择文件夹", this);
    // 水平布局
    pathSelectLayout = new QHBoxLayout(this);
    pathSelectLayout->setSpacing(10);
    pathText = new QLineEdit(this);
    pathText->setFixedWidth(280);
    pathText->setPlaceholderText("保证文件夹下有.vcxproj与.filters文件");
    pathSelectBtn = new QPushButton("浏览", this);
    pathSelectLayout->addWidget(label);
    pathSelectLayout->addWidget(pathText);
    pathSelectLayout->addWidget(pathSelectBtn);
    // 绑定选择路径槽函数
    // 按钮按下
    connect(pathSelectBtn, &QPushButton::clicked, this, &PathSelect::pathSelect);
}
PathSelect::~PathSelect(){
    delete pathSelectLayout;
    delete pathText;
    delete pathSelectBtn;
    delete label;
}
void PathSelect::pathSelect(){
    this->pathText->setText(QFileDialog::getExistingDirectory());
}

QString PathSelect::getPath(){
    return pathText->text();
}
