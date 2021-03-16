#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QCloseEvent>
#include <QList>
#include <QUrl>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>

//可以感受到Qt本身还是帮忙做了很多事的
//有没有办法让WindowTitle自动随着m_currentFilePath变化

void MainWindow::onTextChanged(){
    if(!m_isTextChanged) {
        m_isTextChanged = true;
        setWindowTitle("*" + windowTitle()); //修改时标题也变化
    }
}

void MainWindow::saveCurrentData(QString filepath){ //如果传入filepath为空那就是生成一个新文件
    if(filepath == ""){ //若还未打开文件，就新建一个文件
        filepath = showOpenFileDialog(QFileDialog::AcceptSave, "Save");
    }

    if(filepath != "") {
        QFile file(filepath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){ //使用文件流写入
            QTextStream out(&file);
            out << m_mainEditor.toPlainText();
            file.close();
            m_currentFilePath = filepath;
            updateTitle();
            m_isTextChanged = false; //特别注意保存了之后要修改这个标志
        }
        else{ //弹出一个对话框告知打开失败
            showErrorMessage("Save File Error");
        }
    }
}

void MainWindow::preEditorChange() { //在需要改动mainEditor时候先调用，主要在open和new这两个会改变mainEditor的时候
    if(m_isTextChanged) { //如果有未保存文件要先保存，保存的逻辑就是点击save按钮的逻辑
        int r = showQueryMessage("Would you like to save current file?"); //三个按钮 yes no cancel

        switch(r){
        case QMessageBox::Yes:
            saveCurrentData(m_currentFilePath);
            break;
        case QMessageBox::No:
            m_isTextChanged = false; //直接放弃当前数据
            break;
        case QMessageBox::Cancel:
            /*
              啥都不干，也不再执行后续操作（要让后续操作只能在m_isTextChanged=true才能操作）
              这点特别注意，preEditorChange后面的操作必须判断m_isTextChanged=false才能继续
              这点最后要检查下！看看有没有忘了判断
            */
            break;
        }
    }
}

void MainWindow::openFileToEditor(QString filepath){
    if(filepath != "") {
        QFile file(filepath);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            m_mainEditor.setPlainText(QString(file.readAll()));
            m_currentFilePath = filepath;
            updateTitle();
            m_isTextChanged = false;
            file.close();
        }
        else{ //弹出一个对话框告知打开失败
            showErrorMessage(QString().sprintf("can't open file\n%s", filepath.toStdString().c_str()));
        }
    }
}

void MainWindow::onOpenFile(){
    qDebug() << "onOpenFile";

    preEditorChange(); //要修改mainEditor前调用
    if(!m_isTextChanged){
        QString fpath = showOpenFileDialog(QFileDialog::AcceptOpen, "Open");
        openFileToEditor(fpath);
    }
}

void MainWindow::onNewFile(){ //保存当前文件，然后清空屏幕恢复初始状态
    qDebug() << "onNewFile";

    preEditorChange(); //要修改mainEditor前调用
    //初始化
    if(!m_isTextChanged) {
        m_mainEditor.setPlainText("");
        m_isTextChanged = false;
        m_currentFilePath = "";
        updateTitle();
    }
}

//现在要把save和saveas抽象出底层逻辑

void MainWindow::onSaveFile(){ //如果是一个已存在文件就直接save，如果是新文件就新建一个文件再save
    qDebug() << "onSaveFile";
    saveCurrentData(m_currentFilePath);
}

void MainWindow::onSaveAs(){
    qDebug() << "onSaveAs";
    saveCurrentData(); //新建一个文件再save
}

void MainWindow::closeEvent(QCloseEvent *e){
    preEditorChange(); //要修改MainEditor前调用
    if(!m_isTextChanged){
        QMainWindow::closeEvent(e); //直接正常关闭
    }
    else{ //用户选择cancel就不会关闭
        e->ignore(); //忽略当前修改操作
    }
}

void MainWindow::onCloseFile(){
    qDebug() << "onCloseFile";
}

void MainWindow::onPrint(){
    qDebug() << "onPrint";
}

void MainWindow::onExit(){
    qDebug() << "onExit";
}

void MainWindow::onUndo(){
    qDebug() << "onUndo";
}

void MainWindow::onRedo(){
    qDebug() << "onRedo";
}

void MainWindow::onCut(){
    qDebug() << "onCut";
}

void MainWindow::onCopy(){
    qDebug() << "onCopy";
}

void MainWindow::onPaste(){
    qDebug() << "onPaste";
}

void MainWindow::onWrap(){
    qDebug() << "onWrap";
}

void MainWindow::onFind(){
    qDebug() << "onFind";
}

void MainWindow::onFindNext(){
    qDebug() << "onFindNext";
}

void MainWindow::onFindPrev(){
    qDebug() << "onFindPrev";
}

void MainWindow::onReplace(){
    qDebug() << "onReplace";
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e){
    if(e->mimeData()->hasUrls()){
        e->acceptProposedAction();
    }
    else{
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *e){
    if(e->mimeData()->hasUrls()){
        QList<QUrl> urls = e->mimeData()->urls();
        QString path = urls[0].toLocalFile();
        QFileInfo finfo(path);
        if(finfo.isFile()) {
            preEditorChange(); //有必要先保存当前文件
            if(!m_isTextChanged){
                openFileToEditor(path); //目前只能打开一个文件，以后会实现多文件
            }
        } else{
            showErrorMessage(path + " is not a text file!");
        }
    }
    else{
        e->ignore();
    }
}

//Title-Error or Warning
void MainWindow::showErrorMessage(QString Message){ //只是显示下信息
    QMessageBox msg(this);
    msg.setWindowTitle("Error");
    msg.setText(Message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MainWindow::showMessage(QString Message){ //只是显示下信息
    QMessageBox msg(this);
    msg.setWindowTitle("Information");
    msg.setText(Message);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

int MainWindow::showQueryMessage(QString Message){ //需要用户选择 yes/no/cancel
    QMessageBox msg(this);
    msg.setWindowTitle("Query");
    msg.setText(Message);
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    return msg.exec();
}

//mode可以是open或save
QString MainWindow::showOpenFileDialog(QFileDialog::AcceptMode mode, QString title){ //通过打开文件对话框返回文件路径，错误则返回空串
    QFileDialog fd(this); //打开文件选择对话框

    QStringList filters; //过滤文件
    QMap<QString, QString> suffixMap;

    const char* filterArr[][2] = {
        {"Text Files (*.txt)", ".txt"},
        {"Cpp Files (*.cpp)", ".cpp"},
        {"C Files (*.c)", ".c"},
        {"All Files (*)", "*"},
        {NULL, NULL}
    };

    for(int i = 0; filterArr[i][0] != NULL; i++){
        filters.append(filterArr[i][0]);
        suffixMap[filterArr[i][0]] = filterArr[i][1];
    }

    fd.setWindowTitle(title);
    fd.setAcceptMode(mode);
    fd.setFilters(filters);

    if(mode == QFileDialog::AcceptOpen) {
        fd.setFileMode(QFileDialog::ExistingFile); //只有在open情况下才需要文件本身已经存在，如果是save的话文件不存在就新建一个
    }

    QString ret = "";
    if(fd.exec() == QFileDialog::Accepted){
        ret = fd.selectedFiles()[0];

        if(mode == QFileDialog::AcceptSave){ //保存模式看是否要加上后缀
            QString suffix = suffixMap[fd.selectedNameFilter()]; //用户选择的文件类型对应的后缀
            if(suffix != "*" && !ret.endsWith(suffix)){ //若是用户没有添加后缀就帮忙加上
                ret += suffix; //有一个问题，这里只是在返回的文件名中加上了后缀，为什么保存的文件中也加上了后缀？难道不是这个函数中就保存了的？
            }
        }
    }

    return ret;
}
