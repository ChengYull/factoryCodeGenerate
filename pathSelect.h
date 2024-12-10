#ifndef PATHSELECT_H
#define PATHSELECT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
class PathSelect : public QWidget
{
    Q_OBJECT
public:
    explicit PathSelect(QWidget *parent = nullptr);
    ~PathSelect();
    QString getPath();
private:
    QLabel *label;                   // 文件夹选择label
    QHBoxLayout *pathSelectLayout;   // 文件夹选择布局
    QLineEdit *pathText;             // 文件夹路径显示
    QPushButton *pathSelectBtn;      // 文件夹选择按钮
signals:

private slots:
    void pathSelect();
};

#endif // PATHSELECT_H
