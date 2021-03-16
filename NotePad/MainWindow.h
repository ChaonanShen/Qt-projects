#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QHash>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QPlainTextEdit m_mainEditor; //经常要使用的就直接声明为成员变量
    QLabel m_statusLabel; //显示行列

    QToolBar *m_toolBar; //menuBar直接有函数可以得到，toolBar就自己记下

    QHash<QString, QAction*> mapToolBar, mapMenuBar;

    QString m_currentFilePath; //当前打开的文件路径
    void updateTitle(); //让标题随着m_currentFilePath而变化

    void updateRowCol(int row, int col); //更新状态栏的行列标记

    bool m_isTextChanged;
    void saveCurrentData(QString filepath=""); //保存后m_isTextChanged设为false
    void preEditorChange();
    void openFileToEditor(QString filepath);

    void initMenu();
    void initToolBar();
    void initStatusBar();
    void initMainEdit();

    void initStr2QAction(); //初始化mapToolBar, mapMenuBar;

    QString showOpenFileDialog(QFileDialog::AcceptMode mode, QString title); //打开文件选择对话框选择一个文件
    void showErrorMessage(QString Message); //show messege using QMessageBox
    int showQueryMessage(QString Message); //show query message
    void showMessage(QString Message);

private slots: //这个函数实现文件要与UI的实现分开
    void onTextChanged();
    void onCursorPositonChanged();

    void onCopyAvailable(bool);
    void onRedoAvailable(bool);
    void onUndoAvailable(bool);

    void onOpenFile();
    void onNewFile();
    void onSaveFile();
    void onSaveAs();

    void onPrint();
    void onExit();

    void onFind();
    void onFindNext();
    void onFindPrev();
    void onReplace();

protected:
    void closeEvent(QCloseEvent *); //重写父类QWidget的一个事件函数
    //重写拖放事件
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
