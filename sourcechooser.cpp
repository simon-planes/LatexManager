#include "sourcechooser.h"

SourceChooser::SourceChooser(MainFrame *mainFrame)
{
    this->mainFrame = mainFrame;
    dir = mainFrame->getCurrentDir();
    // center location
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-width()) / 2;
    int y = (screenGeometry.height()-height()) / 2;
    setGeometry(x, y, SC_WIDTH, SC_HEIGHT);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    sourcesList = new QListWidget;
    QStringList list = dir->entryList();
    QStringList::const_iterator constIterator;
    for (constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator)
       sourcesList->addItem(QString((*constIterator).toLocal8Bit().constData()));
    mainLayout->addWidget(sourcesList);
    connect(sourcesList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(selectSource(const QModelIndex &)));

    btnOk = new QPushButton("OK");
    btnCancel = new QPushButton("Cancel");
    connect(btnOk, SIGNAL(clicked()), this, SLOT(validate()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(btnOk);
    buttonsLayout->addWidget(btnCancel);
    mainLayout->addLayout(buttonsLayout);

    this->setWindowModality(Qt::ApplicationModal);

}


/**************************************************/
/********************* SLOTS **********************/
/**************************************************/

void SourceChooser::selectSource(const QModelIndex &)
{
}

void SourceChooser::validate()
{
    QList<QListWidgetItem*> list = sourcesList->selectedItems();
    if (list.isEmpty())
        QMessageBox::critical(this, "No tex source", "There is no tex source in the directory.");
    else
    {
        QListWidgetItem *it = list.first();
        qDebug()<<it->text()<<endl;
    }

}

void SourceChooser::cancel()
{

}
