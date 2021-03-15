#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QPlainTextEdit m_mainEditor; //经常要使用的就直接声明为成员变量
    QLabel m_statusLabel; //显示行列

    QString m_currentFilePath; //当前打开的文件路径
    void updateTitle(); //让标题随着m_currentFilePath而变化

    bool m_isTextChanged;
    void saveCurrentData(QString filepath=""); //保存后m_isTextChanged设为false
    void preEditorChange();

    void initMenu();
    void initToolBar();
    void initStatusBar();
    void initMainEdit();

    QString showOpenFileDialog(QFileDialog::AcceptMode mode, QString title); //打开文件选择对话框选择一个文件
    void showErrorMessage(QString Message); //show messege using QMessageBox
    int showQueryMessage(QString Message); //show query message
    void showMessage(QString Message);

private slots: //这个函数实现文件要与UI的实现分开
    void onTextChanged();

    void onOpenFile();
    void onNewFile();
    void onSaveFile();
    void onSaveAs();
    void onCloseFile();
    void onPrint();
    void onExit();

    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();
    void onWrap();

    void onFind();
    void onFindNext();
    void onFindPrev();
    void onReplace();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
