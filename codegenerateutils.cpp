#include "codegenerateutils.h"
#include "stack"
#include "get_uuid.h"

CodeGenerateUtils::CodeGenerateUtils(QString path, QString moduleName,
                                     vector<QString> factoryNames, QString factoryDirName) {
    oldModuleName = "xxxxxxxx";
    oldFactoryName = "factoryName";
    newModuleName = moduleName.toStdString();
    for(QString &name : factoryNames){
        newFactoryNames.push_back(name.toStdString());
    }
    folderPath = path.append("//").toStdString();
    exePath = QCoreApplication::applicationDirPath().append("\\").toStdString();
    errMessage = "";
    // 外层模块类路径
    string Module_Path_h = exePath + "Module_Template\\template\\qtmALGO_xxxxxxxx.h";
    string Modele_Path_c = exePath + "Module_Template\\template\\qtmALGO_xxxxxxxx.cpp";
    modulePath.push_back(Module_Path_h);
    modulePath.push_back(Modele_Path_c);
    // 工厂模式流程基类路径
    string tmpPath = exePath + "Module_Template\\template\\xxxxxxxx\\";
    basePath.push_back(tmpPath + "qtmALGO_xxxxxxxx_Base.h");
    basePath.push_back(tmpPath + "qtmALGO_xxxxxxxx_Base.cpp");
    basePath.push_back(tmpPath + "qtmALGO_xxxxxxxx_ProcessBase.h");
    basePath.push_back(tmpPath + "qtmALGO_xxxxxxxx_ProcessBase.cpp");
    // 工厂类路径
    string factoryDir = tmpPath + "factory\\";
    factoryPath.push_back(factoryDir + "qtmALGO_factoryName.h");
    factoryPath.push_back(factoryDir + "qtmALGO_factoryName.cpp");
    // 设置子类文件夹
    this->factoryDir = factoryDirName.toStdString();
}

CodeGenerateUtils::~CodeGenerateUtils(){

}
/* 获取路径后缀 */
string CodeGenerateUtils::getSuffix(const std::string& filePath) {
    size_t lastDotPos = filePath.rfind('.');
    if (lastDotPos != std::string::npos) {
        return filePath.substr(lastDotPos + 1);
    }
    return ""; // 如果没有找到点，则返回空字符串
}
/* 替换一行中字符串 */
void CodeGenerateUtils::replaceStrInLine(string &line, string oldStr, string newStr){
    if (line.find(oldStr) == string::npos)
        return;
    size_t pos = 0;
    while ((pos = line.find(oldStr, pos)) != std::string::npos){
        line.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}
/* 替换文件中的字符串并生成新文件（不改变源文件） */
bool CodeGenerateUtils::replaceStrInFile(string filePath, string newFilePath, string oldStr, string newStr){
    if (oldStr.size() == 0 || newStr.size() == 0){
        errMessage = "replace param error!";
        return false;
    }
    ifstream ifs(filePath, ios::in);
    if (!ifs.is_open()){
        errMessage = "can't find :" + QString::fromStdString(filePath);
        return false;
    }
    ofstream ofs(newFilePath, ios::out);
    if (!ofs.is_open()){
        errMessage = "no such path:" + QString::fromStdString(newFilePath);
        return false;
    }
    string lineBuffer;
    while (getline(ifs, lineBuffer)){
        for (unsigned long long i = 0; i < oldStr.size(); ++i){
            replaceStrInLine(lineBuffer, oldStr, newStr);
        }

        ofs << lineBuffer << endl;
    }
    return true;
}
/* 替换文件中的字符串并生成新文件（不改变源文件） */
bool CodeGenerateUtils::replaceStrInFile(string filePath, string newFilePath, vector<string> &oldStr, vector<string> &newStr){
    if (oldStr.size() == 0 || (oldStr.size() != newStr.size())){
        cout << "replace param error!" << endl;
        return false;
    }
    ifstream ifs(filePath, ios::in);
    if (!ifs.is_open()){
        cout << "can't find :" << filePath << endl;
        return false;
    }
    ofstream ofs(newFilePath, ios::out);
    if (!ofs.is_open()){
        cout << "no such path:" << newFilePath << endl;
        return false;
    }
    string lineBuffer;
    while (getline(ifs, lineBuffer)){
        for (size_t i = 0; i < oldStr.size(); ++i){
            replaceStrInLine(lineBuffer, oldStr[i], newStr[i]);
        }

        ofs << lineBuffer << endl;
    }
    return true;
}

/* 获取新文件名 */
string CodeGenerateUtils::getNewFileName(string fPath, string oldModuleName, string newModuleName){
    size_t pos1 = fPath.find_last_of('\\');
    if (pos1 == string::npos) return "";
    string fileName = fPath.substr(pos1 + 1, fPath.size() - pos1);

    size_t pos2 = fileName.find(oldModuleName);
    if (pos2 == string::npos) return "";
    string newFileName = fileName.replace(pos2, oldModuleName.length(), newModuleName);
    return newFileName;
}
/* 获取src与最后\\ 之间的内容 */
string CodeGenerateUtils::getBetweenSrc(string path){
    size_t sta = path.find("src");
    size_t ed = path.find_last_of('\\');
    if (sta == string::npos || ed == string::npos)
        return "";
    return path.substr(sta + 3, ed - sta - 3);
}

/* 添加VS目录结构
    string vcxprojPath					：.vcxproj文件路径
    vector<string> &addPath				：要添加的路径数组（可添加多个）
    char op								：操作类型 h - 添加.h文件  c - 添加.cpp文件
*/
bool CodeGenerateUtils::add_VS_Dir(string vcxprojPath, vector<string> &addPath, char op){
    int add_switch = 0;
    switch (op){
    case 'h':
        add_switch = 2;
        break;
    case 'c':
        add_switch = 3;
        break;
    default:
        errMessage = "addVSDir op is not 'h' or 'c'";
        return false;
    }
    bool bRes = false;
    fstream fs(vcxprojPath, ios::out | ios :: in);
    if (!fs.is_open()){
        errMessage = "can't find vcxproj file:" + QString::fromStdString(vcxprojPath);
        return bRes;
    }

    // 获取文件大小
    fs.seekg(0, std::ios::end);
    streampos fileSize = fs.tellg();

    // 从文件末尾开始往前查找 </ItemGroup>
    bool found = false;
    // 找到标志的个数
    int target_count = 0;
    // 把末尾行内容存入栈中
    stack<string> lines_stack;

    streampos pos = fileSize;
    // 从文件末尾开始，每次向前读取一行 读取到第二个</ItemGroup>为止
    while (pos > 0) {
        // 回到上一行换行符之前
        pos -= 2;
        bool newlineFound = false;
        // 向前搜索换行符
        while (pos > 0 && !newlineFound) {
            pos -= 1; // 向前移动一个字符
            fs.seekg(pos); // 定位到新位置
            char ch;
            fs.get(ch); // 读取当前字符

            if (ch == '\n') {
                newlineFound = true;
                pos += 1; // 跳过换行符 到下一行开头
            }
        }

        fs.seekg(pos);
        string line_bf = "";
        // 获取该行内容
        if (getline(fs, line_bf)) {
            lines_stack.push(line_bf);

            // 找到倒数第二个 </ItemGroup>
            if (line_bf.find("</ItemGroup>") != string::npos ){
                target_count++;
                if (target_count < add_switch){

                    continue;
                }
                found = true;
                break;
            }
        }
    }
    if (!found){
        errMessage = "not found target '</ItemGroup>' in vcxproj file.";
        return bRes;
    }

    fs.seekp(pos);
    string tar_begin = "ClInclude";
    if ('c' == op)
        tar_begin = "ClCompile";
    for (string &path : addPath){
        string suf = getSuffix(path);
        if (suf.empty() || suf[0] != op){
            errMessage = "path suffix is not matching add switch";
            return false;
        }
        fs << "   <" << tar_begin << " Include=\"" << path << "\" />" << endl;
    }

    while (!lines_stack.empty()){
        string line_str = lines_stack.top();
        lines_stack.pop();
        if (line_str.empty()) continue;
        fs << line_str << endl;
    }
    fs.close();
    //cout << "add success" << endl;
    bRes = true;
    return bRes;
}
/* 添加VS目录文件夹结构
string vcxproj_filter_path					：.vcxproj文件路径
vector<string> &addPath				：要添加的路径数组（可添加多个）
char op								：操作类型 h - 添加.h文件  c - 添加.cpp文件 f - 添加筛选器（文件夹）
*/
bool CodeGenerateUtils::add_VS_filter_Dir(string vcxproj_filter_path, vector<string> &addPath, char op, string newModuleName){
    int add_switch = 0;
    switch (op){
    case 'h':
        add_switch = 2;
        break;
    case 'c':
        add_switch = 3;
        break;
    case 'f':
        add_switch = 4;
        break;
    default:
        errMessage = "addVSFilterDir op is not 'h' or 'c' or 'f'";
        return false;
    }
    bool bRes = false;
    fstream fs(vcxproj_filter_path, ios::out | ios::in);
    if (!fs.is_open()){
        errMessage = "can't find vcxproj.filter file:" + QString::fromStdString(vcxproj_filter_path);
        return bRes;
    }

    // 获取文件大小
    fs.seekg(0, std::ios::end);
    streampos fileSize = fs.tellg();

    // 从文件末尾开始往前查找 </ItemGroup>
    bool found = false;
    // 找到标志个数
    int target_count = 0;
    // 把末尾行内容存入栈中
    stack<string> lines_stack;

    streampos pos = fileSize;
    // 从文件末尾开始，每次向前读取一行 读取到第二个</ItemGroup>为止
    while (pos > 0) {
        // 回到上一行换行符之前
        pos -= 2;
        bool newlineFound = false;
        // 向前搜索换行符
        while (pos > 0 && !newlineFound) {
            pos -= 1; // 向前移动一个字符
            fs.seekg(pos); // 定位到新位置
            char ch;
            fs.get(ch); // 读取当前字符

            if (ch == '\n') {
                newlineFound = true;
                pos += 1; // 跳过换行符 到下一行开头
            }
        }

        fs.seekg(pos);
        string line_bf = "";
        // 获取该行内容
        if (getline(fs, line_bf)) {
            lines_stack.push(line_bf);
            //cout << line_bf << endl;

            // 找到倒数第 add_switch 个 </ItemGroup>
            if (line_bf.find("</ItemGroup>") != string::npos){
                target_count++;
                if (target_count < add_switch){
                    continue;
                }
                //cout << "found target" << endl;
                found = true;
                break;
            }
        }
    }
    if (!found){
        errMessage = "not found target '</ItemGroup>' in vcxproj file.";
        return bRes;
    }
    fs.seekp(pos);

    if ('f' == op){
        // 添加筛选器
        op_f(fs);

    }
    else{
        // 路径开头
        string tar_begin = "";
        // 筛选器开头
        string filter_begin = "";
        if ('c' == op){
            filter_begin = "Source Files\\qtmALGO";
            tar_begin = "ClCompile";
        }

        else{
            filter_begin = "Header Files";
            tar_begin = "ClInclude";
        }

        for (string &path : addPath){
            string suf = getSuffix(path);
            if (suf.empty() || suf[0] != op){
                errMessage = "path suffix is not matching add switch";
                return false;
            }
            string m_path = getBetweenSrc(path);

            fs << "    <" << tar_begin << " Include=\"" << path << "\">" << endl;
            fs << "      <Filter>" << filter_begin << m_path << "</Filter>" << endl;
            fs << "    </" << tar_begin << ">" << endl;
        }
    }

    // 填充被覆盖的内容
    while (!lines_stack.empty()){
        string line_str = lines_stack.top();
        lines_stack.pop();
        if (line_str.empty()) continue;
        fs << line_str << endl;
    }
    //cout << "add success" << endl;
    fs.close();
    bRes = true;
    return bRes;
}
/* 操作 f 输入filter */
void CodeGenerateUtils::op_f(fstream &fs){
    fs << "    <Filter Include=\"Header Files\\" << factoryDir << "\">" << endl;
    fs << "      <UniqueIdentifier>{" << get_uuid() << "}</UniqueIdentifier>" << endl;
    fs << "    </Filter>" << endl;

    fs << "    <Filter Include=\"Header Files\\" << factoryDir << "\\factory\">" << endl;
    fs << "      <UniqueIdentifier>{" << get_uuid() << "}</UniqueIdentifier>" << endl;
    fs << "    </Filter>" << endl;

    fs << "    <Filter Include=\"Source Files\\qtmALGO\\" << factoryDir << "\">" << endl;
    fs << "      <UniqueIdentifier>{" << get_uuid() << "}</UniqueIdentifier>" << endl;
    fs << "    </Filter>" << endl;

    fs << "    <Filter Include=\"Source Files\\qtmALGO\\" << factoryDir << "\\factory\">" << endl;
    fs << "      <UniqueIdentifier>{" << get_uuid() << "}</UniqueIdentifier>" << endl;
    fs << "    </Filter>" << endl;
}
/* 获取文件夹下所有文件名 */
vector<string> CodeGenerateUtils::get_files_in_dir(const string &path){
    QDir dir(QString::fromStdString(path));
    QStringList files = dir.entryList(QDir::Files); // 获取所有文件，不包括目录
    vector<string> res;
    foreach (const QString &file, files) {
        res.push_back(file.toStdString());
    }
    return res;
}
/* 生成代码 */
bool CodeGenerateUtils::generate(){
    bool bRes = false;
    // 查询目录下所有文件
    vector<string> fileNameInDir = get_files_in_dir(folderPath);
    if (fileNameInDir.empty()){
        errMessage = "The directory is empty.";
        return bRes;
    }

    // .vcxproj文件找到标志
    bool v_find = false;
    // .vcxproj.filters 文件找到标志
    bool v_f_find = false;
    // 设置VS目录配置文件路径
    string vcxprojPath = "";
    string vcxproj_filtersPath = "";
    for (string &f : fileNameInDir){
        if (f.find(".vcxproj") != string::npos && !v_find){
            v_find = true;
            vcxprojPath = folderPath + f;
        }
        if (f.find(".vcxproj.filters") != string::npos && !v_f_find){
            v_f_find = true;
            vcxproj_filtersPath = folderPath + f;
        }
    }
    if (!v_f_find || !v_find){
        errMessage = "Cannot find the .vcxproj file or .vcxproj.filters file.";
        return bRes;
    }
    string rootPath = folderPath;
    folderPath += "src\\";
    // 尝试创建文件夹 如果已存在也无需处理继续执行即可
    QDir().mkdir(QString::fromStdString(folderPath));

    vector<string> c_path;
    vector<string> h_path;

    // 要替换的内容
    string oldStr = oldModuleName;
    string newStr = newModuleName;
    // 生成文件计数
    int count_file = 0;
    string moduleCppPath = "";
    // 生成外层模块类
    for (string &path : modulePath){
        string newFileName = getNewFileName(path, oldModuleName, newModuleName);
        string savePath = folderPath + newFileName;
        int srcPos = savePath.find("src");
        if ('h' == getSuffix(savePath)[0])
            h_path.push_back(savePath.substr(srcPos, savePath.size() - srcPos));
        else{
            moduleCppPath = savePath;
            c_path.push_back(savePath.substr(srcPos, savePath.size() - srcPos));
        }
        // 替换模块名以及工厂名
        if (replaceStrInFile(path, savePath,oldStr, newStr)){
            // cout << newFileName << " generate success" << endl;
            count_file++;
        }
        else{
            errMessage.append(QString::fromStdString(newFileName + " generate fail"));
            continue;
        }
    }

    folderPath += factoryDir + "\\";
    // 生成工厂模式基类文件夹
    bool dirCreate = QDir().mkdir(QString::fromStdString(folderPath));
    if(!dirCreate){
        errMessage = "子类文件夹已存在!";
        return bRes;
    }
    // 生成工厂模式基类
    for (string &path : basePath){
        string newFileName = getNewFileName(path, oldModuleName, newModuleName);
        string savePath = folderPath + newFileName;
        int srcPos = savePath.find("src");
        if ('h' == getSuffix(savePath)[0])
            h_path.push_back(savePath.substr(srcPos, savePath.size() - srcPos));
        else
            c_path.push_back(savePath.substr(srcPos, savePath.size() - srcPos));
        if (replaceStrInFile(path, savePath,oldStr, newStr)){
            // cout << newFileName << " generate success" << endl;
            count_file++;
        }
        else{
            errMessage.append(QString::fromStdString(newFileName + " generate fail\n"));
            continue;
        }
    }

    // 生成工厂类factory目录
    folderPath += "factory\\";
    bool factoryDirCreate = QDir().mkdir(QString::fromStdString(folderPath));
    if(!factoryDirCreate){
        errMessage = "create factory base directory fail!";
        return bRes;
    }
    // 生成工厂子类
    for (string &path : factoryPath){
        // 多个子类
        for(string &newFactoryName : newFactoryNames){
            string newFileName = getNewFileName(path, oldFactoryName, newFactoryName);
            string savePath = folderPath + newFileName;
            int srcPos = savePath.find("src");
            if ('h' == getSuffix(savePath)[0])
                h_path.push_back(savePath.substr(srcPos, savePath.size() - srcPos));
            else
                c_path.push_back(savePath.substr(srcPos, savePath.size() - srcPos));
            vector<string> oldTmp = {oldStr, oldFactoryName};
            vector<string> newTmp = {newStr, newFactoryName};
            if(replaceStrInFile(path, savePath, oldTmp, newTmp)){
                // cout << newFileName << " generate success" << endl;
                count_file++;
            }
            else{
                errMessage.append(QString::fromStdString(newFileName + " generate fail\n"));
                continue;
            }
        }
    }
    // 模块类中 注册工厂
    fstream fs(moduleCppPath, ios::in | ios::out);
    if(!fs.is_open()){
        errMessage.append("\nmodule cpp open failed!");
        return bRes;
    }
    string lineBuf;
    while(getline(fs, lineBuf)){
        moduleCpp.push_back(lineBuf);
    }
    // 清除EOF标志
    fs.clear();
    // 重新定位到文件开始
    fs.seekp(0, ios::beg);
    for(string &line : moduleCpp){

        size_t pos1 = line.find("factoryName.h");
        size_t pos2 = line.find("factoryName>");
        // 添加头文件
        if(pos1 != string::npos){

            for(string &newFactoryName : newFactoryNames){
                string strTmp = line;
                strTmp.replace(pos1, oldFactoryName.length(), newFactoryName);
                // 添加头文件路径修改为新增 的自定义子类文件夹
                strTmp.replace(strTmp.find(newModuleName), newModuleName.length(), factoryDir);
                fs << strTmp << endl;
            }
        }
        // 注册对应的子类
        else if(pos2 != string::npos){
            for(string &newFactoryName : newFactoryNames){
                string strTmp = line;
                strTmp.replace(pos2, oldFactoryName.length(), newFactoryName);
                fs << strTmp << endl;
            }
        }
        else{
            fs << line << endl;
        }
    }
    fs.close();
    // vcxproj 添加
    if (!add_VS_Dir(vcxprojPath, c_path, 'c') || !add_VS_Dir(vcxprojPath, h_path, 'h')){
         errMessage.append("\nadd VS Directory fail!");
        return bRes;
    }

    // vcxproj_filters 添加
    // 添加筛选器
    if (!add_VS_filter_Dir(vcxproj_filtersPath, c_path, 'f', newModuleName)){
         errMessage.append("\nadd VS Directory filter fail!");
        return bRes;
    }
    // 添加筛选器文件
    if (!add_VS_filter_Dir(vcxproj_filtersPath, c_path, 'c', newModuleName) ||
        !add_VS_filter_Dir(vcxproj_filtersPath, h_path, 'h', newModuleName)
        ){
         errMessage.append("\nadd VS Directory filter file fail!");
        return bRes;
    }
    if(!errMessage.isEmpty())
        return bRes;
    successMessage = "generate success ";
    successMessage.append(QString::number(count_file)).append(" files.\n")
        .append("file path: ").append(QString::fromStdString(rootPath));
    bRes = true;
    return bRes;
}
