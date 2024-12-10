#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include "pathSelect.h"
#include "textinput.h"
#include "numberinput.h"
#include "vector"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
private:
    int sNum = 1;
    QVBoxLayout *mainLayout;         // 主布局——垂直布局
    PathSelect *pathSelectWidget;    // 文件选择控件
    TextInput *moduleNameInput;      // 模块名输入框
    TextInput *factoryDirInput;      // 子类名输入框
    NumberInput *factoryNumInput;    // 工厂数量
    std::vector<TextInput*> factoryNameInput;     // 工厂名输入框
    QPushButton *generateBtn;        // 生成代码按钮
    QHBoxLayout *btnLayout;
    QString gifPath;

private slots:
    void numValueChanged();
    void handleGenerateBtn();
    void generate();
};

#endif // MAINWIDGET_H
