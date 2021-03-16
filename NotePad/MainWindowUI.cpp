#include "MainWindow.h"
#include <QStatusBar>
#include <QIcon>
#include <QSize>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QKeySequence>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_mainEditor(this), m_currentFilePath(""), m_isTextChanged(false)
{
    resize(1000, 600);
    initMenu();
    initToolBar();
    initStatusBar();
    initMainEdit();
    updateTitle();

    setAcceptDrops(true);// 接受拖放事件
}

void MainWindow::updateTitle(){
    if(m_currentFilePath == "")
        setWindowTitle("NotePad - [New File]");
    else
        setWindowTitle("NotePad - [" + m_currentFilePath + "]");
}

void MainWindow::initMainEdit(){
    setCentralWidget(&m_mainEditor);
    connect(&m_mainEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void MainWindow::initStatusBar(){
    QStatusBar *sb = this->statusBar();
    QLabel *label = new QLabel("Notepad");
    label->setMinimumWidth(200);
    label->setAlignment(Qt::AlignHCenter);

    m_statusLabel.setMinimumWidth(200);
    m_statusLabel.setAlignment(Qt::AlignHCenter);
    m_statusLabel.setText("Ln: 1   Col: 1");

    sb->addPermanentWidget(&m_statusLabel);
    sb->addPermanentWidget(label);
}


void MainWindow::initToolBar(){
    QToolBar *tb = addToolBar("Tool Bar");
    tb->setFloatable(false);
    tb->setMovable(false);
    tb->setIconSize(QSize(24, 24));

    QAction *action = NULL;

    action = tb->addAction(QIcon(":/Res/pic/new.png"), "New File", this, SLOT(onNewFile()));
    action = tb->addAction(QIcon(":/Res/pic/open.png"), "Open File", this, SLOT(onOpenFile()));
    action = tb->addAction(QIcon(":/Res/pic/save.png"), "Save", this, SLOT(onSaveFile()));
    action = tb->addAction(QIcon(":/Res/pic/saveas.png"), "Save As", this, SLOT(onSaveAs()));
    action = tb->addAction(QIcon(":/Res/pic/print.png"), "Print", this, SLOT(onPrint()));
    tb->addSeparator();
    action = tb->addAction(QIcon(":/Res/pic/redo.png"), "Redo", this, SLOT(onRedo()));
    action = tb->addAction(QIcon(":/Res/pic/undo.png"), "Undo", this, SLOT(onUndo()));
    action = tb->addAction(QIcon(":/Res/pic/cut.png"), "Cut", this, SLOT(onCut()));
    action = tb->addAction(QIcon(":/Res/pic/copy.png"), "Copy", this, SLOT(onCopy()));
    action = tb->addAction(QIcon(":/Res/pic/paste.png"), "Paste", this, SLOT(onPaste()));
    tb->addSeparator();
    action = tb->addAction(QIcon(":/Res/pic/find.png"), "Find", this, SLOT(onFind()));
    action = tb->addAction(QIcon(":/Res/pic/replace.png"), "Replace", this, SLOT(onReplace()));
}

void MainWindow::initMenu(){
    QMenuBar *mb = menuBar();
    //file menu
    QMenu *fileMenu = new QMenu("File", mb); //这里必须直接指定parent，只靠addMenu不能设置（感觉是Qt的bug）
    fileMenu->addAction("New File", this, SLOT(onNewFile()), QKeySequence(Qt::CTRL+Qt::Key_N)); //连接的是triggered信号
    fileMenu->addAction("Open File", this, SLOT(onOpenFile()), QKeySequence(Qt::CTRL+Qt::Key_O));
    fileMenu->addAction("Save File", this, SLOT(onSaveFile()), QKeySequence(Qt::CTRL+Qt::Key_S));
    fileMenu->addAction("Save As", this, SLOT(onSaveAs()), QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_S));
    fileMenu->addAction("Close File", this, SLOT(onCloseFile()), QKeySequence(Qt::CTRL+Qt::Key_W));
    fileMenu->addSeparator();
    fileMenu->addAction("Print", this, SLOT(onPrint()), QKeySequence(Qt::CTRL+Qt::Key_P));
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, SLOT(onExit()), QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_X));
    mb->addMenu(fileMenu);

    //edit menu
    QMenu *editMenu = new QMenu("Edit", mb);
    editMenu->addAction("Undo", this, SLOT(onUndo()), QKeySequence(Qt::CTRL+Qt::Key_Z));
    editMenu->addAction("Redo", this, SLOT(onRedo()), QKeySequence(Qt::CTRL+Qt::Key_Y));
    editMenu->addSeparator();
    editMenu->addAction("Cut", this, SLOT(onCut()), QKeySequence(Qt::CTRL+Qt::Key_X));
    editMenu->addAction("Copy", this, SLOT(onCopy()), QKeySequence(Qt::CTRL+Qt::Key_C));
    editMenu->addAction("Paste", this, SLOT(onPaste()), QKeySequence(Qt::CTRL+Qt::Key_V));
    editMenu->addSeparator();
    editMenu->addAction("Wrap", this, SLOT(onWrap()));
    mb->addMenu(editMenu);

    //search menu
    QMenu *searchMenu = new QMenu("Search", mb);
    searchMenu->addAction("Find", this, SLOT(onFind()), QKeySequence(Qt::CTRL+Qt::Key_F));
    searchMenu->addAction("Find Next", this, SLOT(onFindNext()), QKeySequence(Qt::Key_F3));
    searchMenu->addAction("Find Prev", this, SLOT(onFindPrev()), QKeySequence(Qt::SHIFT+Qt::Key_F3));
    searchMenu->addAction("Replace", this, SLOT(onReplace()), QKeySequence(Qt::CTRL+Qt::Key_H));
    mb->addMenu(searchMenu);
}

MainWindow::~MainWindow()
{
    
}
