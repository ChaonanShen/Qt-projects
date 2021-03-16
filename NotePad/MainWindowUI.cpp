#include "MainWindow.h"
#include <QStatusBar>
#include <QIcon>
#include <QSize>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QKeySequence>
#include <QObjectList>
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
    initStr2QAction();

    setAcceptDrops(true);// 接受拖放事件

    //打印两个map看看，奇怪的是最开始都有一个空字符串，但菜单栏和工具栏的图标都不应该有空字符串的呀
//    int cnt = 0;
//    for(QHash<QString, QAction*>::iterator it = mapToolBar.begin(); it != mapToolBar.end(); it++){
//        qDebug() << cnt++ << it.key() << "->" << it.value();
//    }
//    cnt = 0;
//    for(QHash<QString, QAction*>::iterator it = mapMenuBar.begin(); it != mapMenuBar.end(); it++){
//        qDebug() << cnt++ << it.key() << "->" << it.value();
//    }
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
    connect(&m_mainEditor, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositonChanged()));
    connect(&m_mainEditor, SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(&m_mainEditor, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(&m_mainEditor, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
}

void MainWindow::initStatusBar(){
    QStatusBar *sb = this->statusBar();
    QLabel *label = new QLabel("Notepad");
    label->setMinimumWidth(200);
    label->setAlignment(Qt::AlignHCenter);

    m_statusLabel.setMinimumWidth(200);
    m_statusLabel.setAlignment(Qt::AlignHCenter);

    updateRowCol(1, 1);

    sb->addPermanentWidget(&m_statusLabel);
    sb->addPermanentWidget(label);
}


void MainWindow::initToolBar(){
    QToolBar *tb = addToolBar("Tool Bar");
    tb->setFloatable(false);
    tb->setMovable(false);
    tb->setIconSize(QSize(24, 24));

    tb->addAction(QIcon(":/Res/pic/new.png"), "New File", this, SLOT(onNewFile()));
    tb->addAction(QIcon(":/Res/pic/open.png"), "Open File", this, SLOT(onOpenFile()));
    tb->addAction(QIcon(":/Res/pic/save.png"), "Save", this, SLOT(onSaveFile()));
    tb->addAction(QIcon(":/Res/pic/saveas.png"), "Save As", this, SLOT(onSaveAs()));
    tb->addAction(QIcon(":/Res/pic/print.png"), "Print", this, SLOT(onPrint()));
    tb->addSeparator();
    tb->addAction(QIcon(":/Res/pic/redo.png"), "Redo", &m_mainEditor, SLOT(redo()))->setEnabled(false);
    tb->addAction(QIcon(":/Res/pic/undo.png"), "Undo", &m_mainEditor, SLOT(undo()))->setEnabled(false);
    tb->addAction(QIcon(":/Res/pic/cut.png"), "Cut", &m_mainEditor, SLOT(cut()))->setEnabled(false);
    tb->addAction(QIcon(":/Res/pic/copy.png"), "Copy", &m_mainEditor, SLOT(copy()))->setEnabled(false);
    tb->addAction(QIcon(":/Res/pic/paste.png"), "Paste", &m_mainEditor, SLOT(paste()))->setEnabled(true); //最初只有paste才是enabled
    tb->addSeparator();
    tb->addAction(QIcon(":/Res/pic/find.png"), "Find", this, SLOT(onFind()));
    tb->addAction(QIcon(":/Res/pic/replace.png"), "Replace", this, SLOT(onReplace()));

    m_toolBar = tb;
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
    editMenu->addAction("Undo", &m_mainEditor, SLOT(undo()), QKeySequence(Qt::CTRL+Qt::Key_Z))->setEnabled(false);
    editMenu->addAction("Redo", &m_mainEditor, SLOT(redo()), QKeySequence(Qt::CTRL+Qt::Key_Y))->setEnabled(false);
    editMenu->addSeparator();
    editMenu->addAction("Cut", &m_mainEditor, SLOT(cut()), QKeySequence(Qt::CTRL+Qt::Key_X))->setEnabled(false);
    editMenu->addAction("Copy", &m_mainEditor, SLOT(copy()), QKeySequence(Qt::CTRL+Qt::Key_C))->setEnabled(false);
    editMenu->addAction("Paste", &m_mainEditor, SLOT(paste()), QKeySequence(Qt::CTRL+Qt::Key_V))->setEnabled(true);
    editMenu->addSeparator();
    editMenu->addAction("Select All", &m_mainEditor, SLOT(selectAll()), QKeySequence(Qt::CTRL+Qt::Key_A))->setEnabled(false);
    mb->addMenu(editMenu);

    //search menu
    QMenu *searchMenu = new QMenu("Search", mb);
    searchMenu->addAction("Find", this, SLOT(onFind()), QKeySequence(Qt::CTRL+Qt::Key_F));
    searchMenu->addAction("Find Next", this, SLOT(onFindNext()), QKeySequence(Qt::Key_F3));
    searchMenu->addAction("Find Prev", this, SLOT(onFindPrev()), QKeySequence(Qt::SHIFT+Qt::Key_F3));
    searchMenu->addAction("Replace", this, SLOT(onReplace()), QKeySequence(Qt::CTRL+Qt::Key_H));
    mb->addMenu(searchMenu);
}

void MainWindow::initStr2QAction(){
    const QObjectList menuList = menuBar()->children();
    for(QObjectList::const_iterator it = menuList.begin(); it != menuList.end(); it++){
        QMenu *menu = dynamic_cast<QMenu*>(*it);
        if(menu != NULL){ //转换成功
            QList<QAction*> actions = menu->actions();
            for(QList<QAction*>::const_iterator i = actions.begin(); i != actions.end(); i++){
                QAction * const action = *i;
                mapMenuBar[action->text()] = action;
            }
        }
    }

    QList<QAction*> actions = m_toolBar->actions();
    for(QList<QAction*>::const_iterator i = actions.begin(); i != actions.end(); i++){
        QAction * const action = *i;
        mapToolBar[action->text()] = action;
    }
}

MainWindow::~MainWindow()
{
    
}
