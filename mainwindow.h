#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myhighlighter.h"
#include "projects.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadFile(const QString& filePath);
    void saveFile(const QString& filePath);
    void openFolder();
    void ProjectManagerOpen();
    QString currentFolderPath;

private:
    Ui::MainWindow *ui;
    QString currentFilePath;
    MyHighlighter *highlighter;
    Projects *projectManager;

};
#endif // MAINWINDOW_H
