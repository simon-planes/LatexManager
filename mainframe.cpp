#include "mainframe.h"
#include "sourcechooser.h"

MainFrame::MainFrame()
{
    // center location
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-width()) / 2;
    int y = (screenGeometry.height()-height()) / 2;
    setGeometry(x, y, MF_WIDTH, MF_HEIGHT);
    setMaximumSize(MF_WIDTH, MF_HEIGHT);

    /* central layout */
    QHBoxLayout *centralLayout = new QHBoxLayout;
    setLayout(centralLayout);
    centralLayout->setAlignment(Qt::AlignBottom);

    /* buttons */
    QGridLayout *buttonsLayout = new QGridLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    btnBuild    = new QPushButton(QIcon(ICN_BUILD), "Build");
    btnView     = new QPushButton(QIcon(ICN_VIEW), "View");
    btnEdit     = new QPushButton(QIcon(ICN_EDIT), "Edit");
    btnClean     = new QPushButton(QIcon(ICN_CLEAN), "Clean");
    btnQuit     = new QPushButton(QIcon(ICN_QUIT), "Quit");
    btnBuild->setIconSize(QSize(ICN_WIDTH,ICN_HEIGHT));
    btnView->setIconSize(QSize(ICN_WIDTH,ICN_HEIGHT));
    btnEdit->setIconSize(QSize(ICN_WIDTH,ICN_HEIGHT));
    btnClean->setIconSize(QSize(ICN_WIDTH,ICN_HEIGHT));
    btnQuit->setIconSize(QSize(ICN_WIDTH,ICN_HEIGHT));
    btnView->setEnabled(false);
    connect(btnBuild, SIGNAL(clicked()), this, SLOT(build()));
    connect(btnView, SIGNAL(clicked()), this, SLOT(view()));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(edit()));
    connect(btnClean, SIGNAL(clicked()), this, SLOT(clean()));
    connect(btnQuit, SIGNAL(clicked()), qApp, SLOT(quit()));
    buttonsLayout->addWidget(btnBuild,0,0);
    buttonsLayout->addWidget(btnView,0,1);
    buttonsLayout->addWidget(btnEdit,1,0);
    buttonsLayout->addWidget(btnClean,1,1);
    buttonsLayout->addWidget(btnQuit,2,1);

    /* left panel */
    setDirTree("/home/"); //QDir::currentPath()
    treeView->setFixedWidth(TV_WIDTH);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(treeView);
    connect(treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(selectFile(const QModelIndex &)));
    centralLayout->addLayout(leftLayout);

    /* right layout */
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QVBoxLayout *labelsLayout = new QVBoxLayout;
    labelsLayout->setAlignment(Qt::AlignTop);
    QGroupBox *groupLabels = new QGroupBox("File info");
    groupLabels->setLayout(labelsLayout);
    lblDirPath = new QLabel("Directory:");
    lblDirPath->setMinimumWidth(LB_WIDTH);
    lblDirPath->setMaximumWidth(LB_WIDTH);
    lblDirPath->setMinimumHeight(LB_HEIGHT);
    lblDirPath->setMaximumHeight(LB_HEIGHT);
    lblFilePath = new QLabel("File:");
    labelsLayout->addWidget(lblDirPath);
    labelsLayout->addWidget(lblFilePath);

    QGroupBox *groupBuilders = new QGroupBox("Builder");
    QButtonGroup *btnGroupBuilders = new QButtonGroup();
    rdPdflatex = new QRadioButton("PDFLatex");
    rdLatexmale = new QRadioButton("latex-make");
    btnGroupBuilders->addButton(rdPdflatex);
    btnGroupBuilders->addButton(rdLatexmale);
    QHBoxLayout *radiosLayout = new QHBoxLayout;
    radiosLayout->addWidget(rdPdflatex);
    radiosLayout->addWidget(rdLatexmale);
    groupBuilders->setLayout(radiosLayout);
    rdPdflatex->setChecked(true);

    connect(btnGroupBuilders, SIGNAL(buttonClicked(int)), this, SLOT(updateBuildler(int)));

    rightLayout->addWidget(groupLabels);
    rightLayout->addWidget(groupBuilders);
    rightLayout->addLayout(buttonsLayout);

    centralLayout->addLayout(rightLayout);

    selectedFilePath = new QString("");
    sourceToBuild = new QString("");
    builder = PDFLATEX;
}

QDir *MainFrame::getCurrentDir()
{
    return currentDir;
}

void MainFrame::setSourceToBuild(QString source)
{
    sourceToBuild = new QString(source);
}

/**
  * Return a tree view of the directory entries
  */
void MainFrame::setDirTree(QString path)
{
    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setRootPath(path);
    treeView = new QTreeView();
    treeView->setModel(fileSystemModel);
    treeView->setRootIndex(fileSystemModel->index((path)));
    treeView->setSortingEnabled(true);
    treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setColumnWidth(0,250);
    treeView->hideColumn(1);
}

/**
  * Return a string equivalent to s,
  * but with a space every LB_N_CHARS character
  */
QString *MainFrame::cleanString(QString s)
{
    QString *newString = new QString(s);
    newString->insert(0, "<br/>");
    int c = newString->length();
    if( c > LB_N_CHARS)
    {
        for(int i = 1; i <= c/LB_N_CHARS; i++)
        {
            int n = i * LB_N_CHARS;
            newString->insert(n, "<br/>");
        }
    }
    QString str("<i>");
    str = (str.append(newString)).append("</i>");
    return new QString(str);
}

/**************************************************/
/********************* SLOTS **********************/
/**************************************************/

void MainFrame::selectFile(const QModelIndex &index)
{
    *selectedFilePath = fileSystemModel->filePath(index);
    btnView->setEnabled(selectedFilePath->endsWith(".pdf")
                        || selectedFilePath->endsWith(".tex")
                        || selectedFilePath->endsWith(".png")
                        || selectedFilePath->endsWith(".jpg"));
    int idx = selectedFilePath->lastIndexOf("/");
    QString dirName;
    if (QDir(*selectedFilePath).exists())
        dirName = *selectedFilePath;
    else
        dirName = selectedFilePath->mid(0,idx);
    QString fileName = selectedFilePath->mid(idx+1);
    lblDirPath->setText("Directory:" + *cleanString(dirName));
    lblFilePath->setText("File:" + *cleanString(fileName));
    currentDir = new QDir(dirName);
}

void MainFrame::view()
{
    if (selectedFilePath->compare("") != 0)
    {
        QString cmd;
        if (selectedFilePath->endsWith(".pdf"))
            cmd = QString(PDF_READER) + " " + *selectedFilePath + " &";
        else if (selectedFilePath->endsWith(".tex"))
            cmd = QString(SRC_READER) + " " + *selectedFilePath + " &";
        else if (selectedFilePath->endsWith(".png") ||
                 selectedFilePath->endsWith(".jpg"))
            cmd = QString(IMG_READER) + " " + *selectedFilePath + " &";
        system(cmd.toStdString().c_str());
    }
}

void MainFrame::edit()
{
    //ASSERT()
    if (selectedFilePath->compare("") != 0)
    {
        QString cmd = QString(SRC_READER) + " " + *selectedFilePath + " &";
        system(cmd.toStdString().c_str());
    }
}

void MainFrame::updateBuildler(int id)
{
    if (rdPdflatex->isChecked())
        builder = PDFLATEX;
    else
        builder = LATEXMAKE;
}

void MainFrame::build()
{
    if (builder == PDFLATEX)
    {
        /*SourceChooser *sourceChooser = new SourceChooser(this);
        sourceChooser->show();*/
        if (selectedFilePath->endsWith(".tex"))
        {
            QString cmd = "cd " + QString(currentDir->absolutePath()) +
                    "; pdflatex " + *selectedFilePath +"; cd -";
            system(cmd.toStdString().c_str());
        }
        else
            QMessageBox::critical(this, "No tex source", "Select a tex format file.");
    }
}

void MainFrame::clean()
{
    QString cmd = "cd " + QString(currentDir->absolutePath()) +
            "; rm -f *.aux *log *.backup *.toc *.out *.nav *.snm *.bbl *.blg *pdf.mk *~; cd -";
    system(cmd.toStdString().c_str());
}
