#include "mainwidget.h"
#include <QMovie>
#include "codegenerateutils.h"
MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    gifPath = QCoreApplication::applicationDirPath().append("//data//img//tired.gif");
    factoryNameInput.resize(1);
    this->resize(400, 200);
    // 主布局
    mainLayout = new QVBoxLayout(this);
    // 文件选择控件
    pathSelectWidget = new PathSelect(this);
    // 模块名输入框控件
    moduleNameInput = new TextInput(this, "模块名:");
    // 子类文件夹输入框
    factoryDirInput = new TextInput(this, "子类文件夹：");
    // 工厂数量选择
    factoryNumInput = new NumberInput(this, "子类数量:");
    // 工厂名输入框
    factoryNameInput[0] = new TextInput(this, "子类名:");


    // 生成按钮
    generateBtn = new QPushButton("生成代码", this);
    generateBtn->setMaximumWidth(100);
    // 按钮居中布局
    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(generateBtn, 0, Qt::AlignHCenter); // 水平居中
    // 将所有控件添加到主布局

    mainLayout->addWidget(pathSelectWidget);
    mainLayout->addWidget(moduleNameInput);
    mainLayout->addWidget(factoryDirInput);
    mainLayout->addWidget(factoryNumInput);
    mainLayout->addWidget(factoryNameInput[0]);
    mainLayout->addLayout(btnLayout);

    // 连接槽函数
    // 子类数量发生改变
    connect(factoryNumInput, &NumberInput::valueChanged,
            this, &MainWidget::numValueChanged);
    // 生成按钮
    connect(generateBtn, &QPushButton::clicked,
            this, &MainWidget::handleGenerateBtn);
}

MainWidget::~MainWidget(){
    delete mainLayout;
    delete pathSelectWidget;
    delete moduleNameInput;
    delete factoryNumInput;

    delete btnLayout;
}

void MainWidget::numValueChanged(){
    mainLayout->removeItem(btnLayout);
    int getNum = factoryNumInput->getValue();

    if(sNum <= getNum){
        for(int i = 0; i < getNum - sNum; ++i){
            TextInput * nameInput = new TextInput(this, "子类名:");
            factoryNameInput.push_back(nameInput);
            mainLayout->addWidget(nameInput);
        }
    }else{
        for(int i = sNum - 1; i > getNum - 1; --i){
            TextInput * nameInput = factoryNameInput[i];
            mainLayout->removeWidget(nameInput);
            delete nameInput;
        }
        factoryNameInput.resize(getNum);
    }
    sNum = getNum;
    mainLayout->addLayout(btnLayout);
    // 更新大小
    this->adjustSize();
}
void MainWidget::handleGenerateBtn(){
    // 卡控为空的情况
    if(pathSelectWidget->getPath().isEmpty() || moduleNameInput->getText().isEmpty()
        || factoryDirInput->getText().isEmpty()){
        QMessageBox::critical(this, "填写不完整", "表单有空项，填写完整后重试！");
        return;
    }
    for(auto fn : factoryNameInput){
        if(fn->getText().isEmpty()){
            QMessageBox::critical(this, "填写不完整", "表单有空项，填写完整后重试！");
            return;
        }
    }

    // 确认页面
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *dLayout = new QVBoxLayout(dialog);
    QString lab1 = "生成目录：";
    lab1.append(pathSelectWidget->getPath());
    QLabel *dLabel1 = new QLabel(lab1, dialog);
    QString lab2 = "模块名：";
    lab2.append(moduleNameInput->getText());
    QLabel *dLabel2 = new QLabel(lab2, dialog);
    QString lab3 = "子类文件夹名称：";
    lab3.append(factoryDirInput->getText());
    QLabel *dLabel3 = new QLabel(lab3, dialog);


    dialog->setWindowTitle("确认");
    dialog->resize(300, 200);
    dLayout->addWidget(dLabel1);
    dLayout->addWidget(dLabel2);
    dLayout->addWidget(dLabel3);

    for(size_t i = 0; i < factoryNameInput.size(); ++i){
        QString lab = "子类";
        lab.append(QString::number(i + 1)).append(":");
        lab.append(factoryNameInput[i]->getText());
        QLabel *dLab = new QLabel(lab, dialog);
        dLayout->addWidget(dLab);
    }
    QPushButton *dBtn = new QPushButton("确定生成", dialog);
    // 关闭对话框
    connect(dBtn, &QPushButton::clicked, dialog, &QDialog::close);
    // 开始生成
    connect(dBtn, &QPushButton::clicked, this, &MainWidget::generate);
    dLayout->addWidget(dBtn);
    dialog->show();
}
void MainWidget::generate(){
    QDialog *tmpDlg = new QDialog(this);
    // 禁用关闭窗口
    tmpDlg->setWindowFlags(windowFlags() &~(Qt::WindowCloseButtonHint
                            | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint));
    tmpDlg->setWindowTitle("代码生成中...");
    tmpDlg->resize(200, 50);
    QLabel * lab = new QLabel(tmpDlg);
    lab->setText("代码生成中，请稍后...");

    tmpDlg->show();
    vector<QString> factoryNames;
    for(auto fn : factoryNameInput){
        factoryNames.push_back(fn->getText());
    }
    // 执行代码生成
    CodeGenerateUtils *generater =new CodeGenerateUtils
        (pathSelectWidget->getPath(), moduleNameInput->getText(), factoryNames,
         factoryDirInput->getText());
    bool res = generater->generate();
    tmpDlg->close();
    if(!res){
        QMessageBox::critical(this, "生成失败", generater->errMessage);
        return;
    }
    QDialog *successDlg = new QDialog(this);
    successDlg->setWindowTitle("生成成功！");
    // 成功后打开新对话框
    QVBoxLayout *dlgLayout = new QVBoxLayout(successDlg);
    QLabel *sLab = new QLabel(successDlg);
    sLab->setText(generater->successMessage);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *openDirBtn = new QPushButton("打开文件夹", successDlg);
    QPushButton *closeBtn = new QPushButton("关闭", successDlg);
    // 创建一个QLabel用于显示GIF
    QLabel *gifLabel = new QLabel(successDlg);
    gifLabel->setAlignment(Qt::AlignCenter); // 居中显示
    // 创建QMovie对象并设置GIF文件路径
    QMovie *movie = new QMovie(gifPath);
    gifLabel->setMovie(movie);
    // 开始播放GIF
    movie->start();
    btnLayout->addWidget(openDirBtn);
    btnLayout->addWidget(closeBtn);
    dlgLayout->addWidget(sLab);
    dlgLayout->addWidget(gifLabel);
    dlgLayout->addLayout(btnLayout);
    successDlg->show();
    connect(closeBtn, &QPushButton::clicked, successDlg, &QDialog::close);
    connect(openDirBtn, &QPushButton::clicked, successDlg, &QDialog::close);
    connect(openDirBtn, &QPushButton::clicked, successDlg, [=](){
        // 打开对应文件夹
        QUrl url = QUrl::fromLocalFile(pathSelectWidget->getPath());
        QDesktopServices::openUrl(url);
    });
}
