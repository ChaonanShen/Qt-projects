#include "MainWindow.h"
#include <QStatusBar>
#include <QIcon>
#include <QSize>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mainEditor(this)
{
    resize(1000, 600);
    initMenu();
    initToolBar();
    initStatusBar();
    initMainEdit();
}


void MainWindow::initMainEdit(){
    setCentralWidget(&mainEditor);
}

void MainWindow::initStatusBar(){
    QStatusBar *sb = this->statusBar();
    QLabel *label = new QLabel("Notepad");
    label->setMinimumWidth(200);
    label->setAlignment(Qt::AlignHCenter);

    statusLabel.setMinimumWidth(200);
    statusLabel.setAlignment(Qt::AlignHCenter);
    statusLabel.setText("Ln: 1   Col: 1");

    sb->addPermanentWidget(&statusLabel);
    sb->addPermanentWidget(label);
}


void MainWindow::initToolBar(){
    QToolBar *tb = addToolBar("Tool Bar");
    tb->setFloatable(false);
    tb->setMovable(false);
    tb->setIconSize(QSize(24, 24));

    tb->addAction(QIcon(":/Res/pic/new.png"), "New File");
    tb->addAction(QIcon(":/Res/pic/open.png"), "Open File");
    tb->addAction(QIcon(":/Res/pic/save.png"), "Save");
    tb->addAction(QIcon(":/Res/pic/saveas.png"), "Save As");
    tb->addAction(QIcon(":/Res/pic/print.png"), "Print");
    tb->addSeparator();
    tb->addAction(QIcon(":/Res/pic/redo.png"), "Redo");
    tb->addAction(QIcon(":/Res/pic/undo.png"), "Undo");
    tb->addAction(QIcon(":/Res/pic/cut.png"), "Cut");
    tb->addAction(QIcon(":/Res/pic/copy.png"), "Copy");
    tb->addAction(QIcon(":/Res/pic/paste.png"), "Paste");
    tb->addSeparator();
    tb->addAction(QIcon(":/Res/pic/find.png"), "Find");
    tb->addAction(QIcon(":/Res/pic/replace.png"), "Replace");
}

QAction *MainWindow::makeAction(const QString &name, int shortcut){
    QAction *action = new QAction(name, NULL); //第二个参数是指定父对象，不过在addAction中会自动指定父对象
    action->setShortcut(QKeySequence(shortcut));
    return action;
}

QAction *MainWindow::makeAction(const QString &name){
    QAction *action = new QAction(name, NULL); //第二个参数是指定父对象，不过在addAction中会自动指定父对象
    return action;
}

void MainWindow::initMenu(){
    QMenuBar *mb = menuBar();

    //file menu
    QMenu *fileMenu = new QMenu("File", mb); //这里必须直接指定parent，只靠addMenu不能设置（感觉是Qt的bug）
    fileMenu->addAction(makeAction("New File", Qt::CTRL+Qt::Key_N));
    fileMenu->addAction(makeAction("Open File", Qt::CTRL+Qt::Key_O));
    fileMenu->addAction(makeAction("Save File", Qt::CTRL+Qt::Key_S));
    fileMenu->addAction(makeAction("Save As", Qt::CTRL+Qt::ALT+Qt::Key_S));
    fileMenu->addAction(makeAction("Close File", Qt::CTRL+Qt::Key_W));
    fileMenu->addSeparator();
    fileMenu->addAction(makeAction("Print", Qt::CTRL+Qt::Key_P));
    fileMenu->addSeparator();
    fileMenu->addAction(makeAction("Exit", Qt::CTRL+Qt::Key_X));
    mb->addMenu(fileMenu);

    //edit menu
    QMenu *editMenu = new QMenu("Edit", mb);
    editMenu->addAction((makeAction("Undo", Qt::CTRL+Qt::Key_Z)));
    editMenu->addAction((makeAction("Redo", Qt::CTRL+Qt::Key_Y)));
    editMenu->addSeparator();
    editMenu->addAction((makeAction("Cut", Qt::CTRL+Qt::Key_X)));
    editMenu->addAction((makeAction("Copy", Qt::CTRL+Qt::Key_C)));
    editMenu->addAction((makeAction("Paste", Qt::CTRL+Qt::Key_V)));
    editMenu->addSeparator();
    editMenu->addAction((makeAction("Wrap")));
    mb->addMenu(editMenu);

    //search menu
    QMenu *searchMenu = new QMenu("Search", mb);
    searchMenu->addAction((makeAction("Find", Qt::CTRL+Qt::Key_F)));
    searchMenu->addAction((makeAction("Find Next", Qt::Key_F3)));
    searchMenu->addAction((makeAction("Find Prev", Qt::SHIFT+Qt::Key_F3)));
    searchMenu->addAction((makeAction("Replace", Qt::CTRL+Qt::Key_H)));
    mb->addMenu(searchMenu);
}

MainWindow::~MainWindow()
{
    
}
