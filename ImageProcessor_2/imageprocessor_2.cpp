#include "imageprocessor_2.h"

#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QColor>
#include <cstdio>
#include "imagetransform_1.h"

imageprocessor_2::imageprocessor_2(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("影像處理"));

    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300, 200);
    gWin = new ImageTransform_1();
    initPixmap->fill(QColor(255, 255, 255));
    imgWin->resize(300, 200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);

    mainLayout->addWidget(imgWin);
    setCentralWidget(central);

    createActions();
    createMenus();
    createToolBars();
}

imageprocessor_2::~imageprocessor_2()
{
}

void imageprocessor_2::createActions()
{
    // Open
    openFileAction = new QAction(QStringLiteral("開啟檔案&O"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(QStringLiteral("開啟影像檔案"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(showOpenFile()));

    // Exit
    exitAction = new QAction(QStringLiteral("結束&Q"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(QStringLiteral("退出程式"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    geometryAction = new QAction(QStringLiteral("幾何轉換"), this);
    geometryAction->setShortcut(tr("Ctrl+G"));
    geometryAction->setStatusTip(QStringLiteral("影像幾何轉換"));
    connect(geometryAction, SIGNAL(triggered()), this, SLOT(showGeometryTransform()));
    connect(exitAction, SIGNAL(triggered()), gWin, SLOT(close()));

    // Zoom In
    zoomInAction = new QAction(QStringLiteral("放大"), this);
    zoomInAction->setShortcut(tr("Ctrl+]"));
    zoomInAction->setStatusTip(QStringLiteral("放大影像"));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    // Zoom Out
    zoomOutAction = new QAction(QStringLiteral("縮小"), this);
    zoomOutAction->setShortcut(tr("Ctrl+["));
    zoomOutAction->setStatusTip(QStringLiteral("縮小影像"));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
}

void imageprocessor_2::createMenus()
{
    fileMenu = menuBar()->addMenu(QStringLiteral("檔案&F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(geometryAction);
    fileMenu->addAction(exitAction);

    QMenu *toolsMenu = menuBar()->addMenu(QStringLiteral("工具&T"));
    toolsMenu->addAction(zoomInAction);
    toolsMenu->addAction(zoomOutAction);
}

void imageprocessor_2::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);
    fileTool->addAction(geometryAction);
    fileTool->addAction(zoomInAction);
    fileTool->addAction(zoomOutAction);
}

void imageprocessor_2::loadFile(QString filename)
{
    qDebug() << QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s\n", (char *)ba.data());

    originImg.load(filename);
    img = originImg;
    scaleFactor = 1.0;

    updateImageView();
}

void imageprocessor_2::showOpenFile()
{

    filename = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("開啟影像"),
        tr("."),
        "bmp (*.bmp);;png (*.png);;jpeg (*.jpg)"
        );

    if (!filename.isEmpty())
    {
        loadFile(filename);
    }
}

void imageprocessor_2::showGeometryTransform()
{
    if (!img.isNull()) {
        gWin->srcImg = img;
        gWin->inWin->setPixmap(QPixmap::fromImage(gWin->srcImg));
        gWin->show();
    }
}

void imageprocessor_2::zoomIn()
{
    if (originImg.isNull()) return;

    scaleFactor *= 1.25;
    if (scaleFactor > 5.0) scaleFactor = 5.0;

    updateImageView();
}

void imageprocessor_2::zoomOut()
{
    if (originImg.isNull()) return;

    scaleFactor /= 1.25;
    if (scaleFactor < 0.2) scaleFactor = 0.2;

    updateImageView();
}

void imageprocessor_2::updateImageView()
{
    if (originImg.isNull()) return;

    int w = int(originImg.width()  * scaleFactor);
    int h = int(originImg.height() * scaleFactor);

    QImage scaled = originImg.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imgWin->setPixmap(QPixmap::fromImage(scaled));
}

