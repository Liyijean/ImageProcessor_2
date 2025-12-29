#ifndef IMAGEPROCESSOR_2_H
#define IMAGEPROCESSOR_2_H
#include "imagetransform_1.h"

#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QImage>
#include <QLabel>
#include <QString>

class imageprocessor_2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit imageprocessor_2(QWidget *parent = nullptr);
    ~imageprocessor_2();

private slots:
    void showOpenFile();
    void zoomIn();
    void zoomOut();
    void showGeometryTransform();

private:

    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString filename);
    void updateImageView();
    ImageTransform_1 *gWin;
    QWidget  *central;
    QMenu    *fileMenu;
    QToolBar *fileTool;

    QLabel   *imgWin;
    QImage    img;
    QImage    originImg;
    QString   filename;

    QAction  *openFileAction;
    QAction  *exitAction;
    QAction  *zoomInAction;
    QAction  *zoomOutAction;
    QAction *geometryAction;
    double scaleFactor = 1.0;
};

#endif // IMAGEPROCESSOR_2_H
