#include "projects.h"

#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QString>
#include <QDockWidget>

Projects::Projects() {
        QString currentProjectPath = "";
}
void Projects::newOpenInterface(){
        // setting objects
        QLabel* TitleLabel = new QLabel();
        QPushButton* openButton = new QPushButton();
        QPushButton* cancleButton = new QPushButton();
        QDockWidget* ProjectManagerDock = new QDockWidget();

        //layout
        QVBoxLayout* layout = new QVBoxLayout;
        QHBoxLayout* buttonsLayout = new QHBoxLayout;
        // texts
        TitleLabel->setText("TortEdit projectManager");
        openButton->setText("Open");
        cancleButton->setText("Close");

        // other settings

        ProjectManagerDock->setFloating(true);

        //max and min size
        ProjectManagerDock->setMinimumSize(QSize(335,600));
        ProjectManagerDock->setMaximumSize(QSize(400,700));

        //setting the layout
        buttonsLayout->addWidget(openButton);
        buttonsLayout->addWidget(cancleButton);

        layout->addWidget(TitleLabel);
        layout->addLayout(buttonsLayout);

        //adding the layout
        ProjectManagerDock->setLayout(layout);
};
