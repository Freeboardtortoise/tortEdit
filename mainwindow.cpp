#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myhighlighter.h"


#include <QFileSystemModel>
#include <QTreeView>
#include <QDockWidget>
#include <QDir>
#include <QPushButton>
#include <QSyntaxHighlighter>


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
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    highlighter = new MyHighlighter(ui->CodeEditor->document(), "/home/darion/.config/tortedit/themes/dark_theme.json");



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

    // 4. Add the dock widget to your main window (on the left)
    addDockWidget(Qt::LeftDockWidgetArea, fileDock);

    fileDock->setFloating(true);


    // add a hide button
    QAction* toggleFileDockAction = fileDock->toggleViewAction();
    ui->LeftBar->addAction(toggleFileDockAction);

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
        if (ui->projectDock->isVisible() == false) {
            ui->projectDock->setVisible(true);
            ui->projectDock->setFloating(true);
        }
    });
    connect(ui->actionFolder, &QAction::triggered, this, [this]() {
        openFolder();
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
