#ifndef CODEGENERATEUTILS_H
#define CODEGENERATEUTILS_H

#include <QCoreApplication> // 获取exe路径
#include <QDebug>
#include "string"
#include "fstream"
#include <QDir>
using namespace std;
class CodeGenerateUtils
{
public:
    CodeGenerateUtils(QString path, QString moduleName,
                      vector<QString> factroyNames, QString factoryDirName);
    ~CodeGenerateUtils();
    string factoryDir;
    string exePath;
    string folderPath;
    string oldModuleName;
    string newModuleName;
    string oldFactoryName;
    vector<string> newFactoryNames;
    QString errMessage;
    QString successMessage;
    // 模块类cpp（用于添加多个工厂）
    vector<string> moduleCpp;
    // 外层模块类路径
    vector<string> modulePath;
    // 工厂模式流程基类路径
    vector<string> basePath;
    // 工厂类路径
    vector<string> factoryPath;
    bool generate();
private:
    string getSuffix(const std::string& filePath);
    void replaceStrInLine(string &line, string oldStr, string newStr);  // 替换一行中的字符串
    bool replaceStrInFile(string filePath, string newFilePath,
                           string oldStr, string newStr);
    bool replaceStrInFile(string filePath, string newFilePath,
                          vector<string> &oldStr, vector<string> &newStr);
    string getNewFileName(string fPath, string oldModuleName, string newModuleName); // 获取新文件名
    string getBetweenSrc(string path);
    bool add_VS_Dir(string vcxprojPath, vector<string> &addPath, char op);
    bool add_VS_filter_Dir(string vcxproj_filter_path, vector<string> &addPath, char op, string newModuleName);
    void op_f(fstream &fs);
    /* 获取文件夹下所有文件名 */
    vector<string> get_files_in_dir(const string &path);
};

#endif // CODEGENERATEUTILS_H
