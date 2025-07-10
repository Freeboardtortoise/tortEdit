#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myhighlighter.h"


#include <QFileSystemModel>
#include <QTreeView>
#include <QDockWidget>
#include <QDir>
#include <QPushButton>
#include <QSyntaxHighlighter>
#include <QStandardPaths>


#include <QFile>
#include <QTextStream>

void MainWindow::loadFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Handle error — maybe show a message box
        qWarning() << "Could not open file:" << filePath;
        return;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    ui->CodeEditor->setPlainText(fileContent);
}
void MainWindow::saveFile(const QString& filePath) {

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return;
    }

    QTextStream out(&file);
    out << ui->CodeEditor->toPlainText();
    file.close();
}
void MainWindow::openFolder() {
    QFileSystemModel* fileModel2 = new QFileSystemModel(this);

    // Set what to show (files + directories)
    fileModel2->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    // Set root path (e.g., user home or project folder)
    QString rootPath = QDir::homePath();
    fileModel2->setRootPath(rootPath);

    // 2. Create the tree view and assign the model
    QTreeView* fileTreeView = new QTreeView(this);
    fileTreeView->setModel(fileModel2);

    // Set the root index so it shows the directory you want
    fileTreeView->setRootIndex(fileModel2->index(rootPath));

    // Optional: hide unwanted columns (e.g., only show file name)
    fileTreeView->setColumnHidden(1, true); // Size column
    fileTreeView->setColumnHidden(2, true); // Type column
    fileTreeView->setColumnHidden(3, true); // Date modified

    // 3. Create a dock widget to hold the tree
    QDockWidget* fileDock2 = new QDockWidget(tr("File Explorer"), this);
    fileDock2->setWidget(fileTreeView);

    // Allow docking on left or right side
    fileDock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    addDockWidget(Qt::LeftDockWidgetArea, fileDock2);

    fileDock2->setFloating(true);
    fileDock2->setMinimumHeight(400);
    fileDock2->setMaximumHeight(500);

    fileDock2->setMaximumWidth(600);
    fileDock2->setMinimumWidth(1000);

    connect(fileTreeView, &QTreeView::clicked, this, [this, fileModel2, fileDock2](const QModelIndex &index) {
        if (!fileModel2->isDir(index)) {
            QString filePath = fileModel2->filePath(index);
            // TODO: Load the file content into your editor
            qDebug() << "File clicked:" << filePath;
            loadFile(filePath);
            currentFilePath = filePath;
            highlighter->rehighlight();
            ui->CodeEditor->setPlainText(ui->CodeEditor->toPlainText());
            fileDock2->close();

        }
    });
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Use portable config path instead of hard-coded path
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QString themePath = configPath + "/tortedit/themes/dark_theme.json";
    
    highlighter = new MyHighlighter(ui->CodeEditor->document(), themePath);



    // 1. Create the file system model
    QFileSystemModel* fileModel = new QFileSystemModel(this);

    // Set what to show (files + directories)
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    // Set root path (e.g., user home or project folder)
    QString rootPath = QDir::homePath();
    fileModel->setRootPath(rootPath);

    // 2. Create the tree view and assign the model
    QTreeView* fileTreeView = new QTreeView(this);
    fileTreeView->setModel(fileModel);

    // Set the root index so it shows the directory you want
    fileTreeView->setRootIndex(fileModel->index(rootPath));

    // Optional: hide unwanted columns (e.g., only show file name)
    fileTreeView->setColumnHidden(1, true); // Size column
    fileTreeView->setColumnHidden(2, true); // Type column
    fileTreeView->setColumnHidden(3, true); // Date modified

    // 3. Create a dock widget to hold the tree
    QDockWidget* fileDock = new QDockWidget(tr("File Explorer"), this);
    fileDock->setWidget(fileTreeView);

    // Allow docking on left or right side
    fileDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    fileDock->setFloating(false);

    // 4. Add the dock widget to your main window (on the left)
    addDockWidget(Qt::LeftDockWidgetArea, fileDock);

    fileDock->setFloating(false);


    // add a hide button
    QAction* toggleFileDockAction = fileDock->toggleViewAction();
    ui->LeftBar->addAction(toggleFileDockAction);

    projectManager->newOpenInterface();

    std::string path = "";
    //opening of files
    connect(fileTreeView, &QTreeView::clicked, this, [this, fileModel](const QModelIndex &index) {
        if (!fileModel->isDir(index)) {
            QString filePath = fileModel->filePath(index);
            // TODO: Load the file content into your editor
            qDebug() << "File clicked:" << filePath;
            loadFile(filePath);
            currentFilePath = filePath;
            highlighter->rehighlight();
            ui->CodeEditor->setPlainText(ui->CodeEditor->toPlainText());

        }
    });

    // menue buttons
    // save button
    connect(ui->actionsave, &QAction::triggered, this, [this]() {
        saveFile(currentFilePath);
    });
    connect(ui->actionProjectManager, &QAction::triggered, this, [this]() {
        projectManager->newOpenInterface();
    });
    connect(ui->actionFile, &QAction::triggered, this, [this]() {
        MainWindow::openFolder();
    });

    //quit button
    connect(ui->actionquit_without_saving, &QAction::triggered, this, [this]() {
        delete ui;
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}
