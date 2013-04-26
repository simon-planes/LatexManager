#ifndef SOURCECHOOSER_H
#define SOURCECHOOSER_H

#include <QtGui>
#include "mainframe.h"

#define SC_WIDTH    300
#define SC_HEIGHT   300

class SourceChooser: public QWidget
{

    Q_OBJECT

public:
    SourceChooser(MainFrame *mainFrame);

public slots:
    void selectSource(const QModelIndex &);
    void validate();
    void cancel();

private:
    MainFrame *mainFrame;
    QDir *dir;
    QListWidget *sourcesList;
    QPushButton *btnOk;
    QPushButton *btnCancel;
};

#endif // SOURCECHOOSER_H
