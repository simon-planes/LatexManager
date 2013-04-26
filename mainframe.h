#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QtGui>

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
#define ICN_CLEAN   "imgs/icn_clean.svg"

enum Builder {PDFLATEX, LATEXMAKE};

class MainFrame: public QWidget
{
    Q_OBJECT

    public:
        MainFrame();
        QDir *getCurrentDir();
        void setSourceToBuild(QString source);

    public slots:
        void selectFile(const QModelIndex &index);
        void view();
        void edit();
        void build();
        void clean();
        void updateBuildler(int id);

    private:
        QDir *currentDir;
        QString *selectedFilePath;
        QString *sourceToBuild;
        QPushButton *btnQuit;
        QPushButton *btnBuild;
        QPushButton *btnView;
        QPushButton *btnEdit;
        QPushButton *btnClean;
        QRadioButton *rdPdflatex;
        QRadioButton *rdLatexmale;
        QTreeView *treeView;
        QFileSystemModel *fileSystemModel;
        QLabel *lblFilePath;
        QLabel *lblDirPath;

        Builder builder;

        void setDirTree(QString path);
        QString *cleanString(QString s);
};

#endif // MAINFRAME_H
