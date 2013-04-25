#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QtGui>
#include <QFileSystemModel>

#define MF_WIDTH    800
#define MF_HEIGHT   480
#define TV_WIDTH    500
#define LB_WIDTH    300
#define LB_HEIGHT   100
#define LB_N_CHARS  42
#define PDF_READER  "evince"
#define SRC_READER  "gedit"
#define IMG_READER  "eog"
#define ICN_WIDTH   50
#define ICN_HEIGHT  50
#define ICN_EDIT    "imgs/icn_edit.svg"
#define ICN_QUIT    "imgs/icn_quit.svg"
#define ICN_VIEW    "imgs/icn_view.svg"
#define ICN_BUILD   "imgs/icn_build.svg"

enum Compiler {PDFLATEX, LATEXMAKE};

class MainFrame: public QWidget
{
    Q_OBJECT

    public:
        MainFrame();

    public slots:
        void selectFile(const QModelIndex &index);
        void view();
        void edit();
        void build();
        void updateCompiler(int id);

    private:
        QDir *currentDir;
        QString *selectedFilePath;
        QPushButton *btnQuit;
        QPushButton *btnBuild;
        QPushButton *btnView;
        QPushButton *btnEdit;
        QRadioButton *rdPdflatex;
        QRadioButton *rdLatexmale;
        QTreeView *treeView;
        QFileSystemModel *fileSystemModel;
        QLabel *lblFilePath;
        QLabel *lblDirPath;

        Compiler compiler;

        void setDirTree(QString path);
        QString *cleanString(QString s);
};

#endif // MAINFRAME_H
