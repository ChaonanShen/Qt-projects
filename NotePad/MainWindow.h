#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    QPlainTextEdit mainEditor; //经常要使用的就直接声明为成员变量
    QLabel statusLabel; //显示行列
    void initMenu();
    void initToolBar();
    void initStatusBar();
    void initMainEdit();

    QAction *makeAction(const QString &name, int shortcut); // for menu
    QAction *makeAction(const QString &name); //for menu
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
