#include "imagetransform_1.h"
#include<QPixmap>
#include<QPainter>
#include <QFileDialog>
#include <QMessageBox>

ImageTransform_1::ImageTransform_1(QWidget *parent)
    : QWidget(parent)
{

    mainLayout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();

    mirrorGroup = new QGroupBox(QStringLiteral("鏡射"), this);
    groupLayout = new QVBoxLayout(mirrorGroup);

    hCheckBox = new QCheckBox(QStringLiteral("水平"), mirrorGroup);
    vCheckBox = new QCheckBox(QStringLiteral("垂直"), mirrorGroup);
    mirrorButton = new QPushButton(QStringLiteral("執行"), mirrorGroup);

    // 這裡建議不要用 setGeometry，因為這會跟 Layout 衝突，交給 groupLayout 管理就好
    groupLayout->addWidget(hCheckBox);
    groupLayout->addWidget(vCheckBox);
    groupLayout->addWidget(mirrorButton);

    leftLayout->addWidget(mirrorGroup);

    rotateDial = new QDial(this);
    rotateDial->setNotchesVisible(true);
    leftLayout->addWidget(rotateDial); // 先加 Dial

    // 2. 必須先 new，才能 addItem
    vSpacer = new QSpacerItem(20, 58, QSizePolicy::Minimum, QSizePolicy::Expanding);
    leftLayout->addItem(vSpacer); // 後加 Spacer 把東西往上推

    // 3. 插入存檔按鈕
    saveButton = new QPushButton(QStringLiteral("存檔"), this);
    leftLayout->addWidget(saveButton);

    mainLayout->addLayout(leftLayout);

    inWin = new QLabel(this);
    inWin->setScaledContents(true);

    // 初始化圖片繪製
    QPixmap *initPixmap = new QPixmap(300, 200);
    initPixmap->fill(QColor(255, 255, 255));

    QPainter paint(initPixmap); // 改用 stack 物件比較安全
    paint.setPen(Qt::black);
    paint.drawRect(15, 15, 60, 40);
    paint.end();

    srcImg = initPixmap->toImage();
    inWin->setPixmap(*initPixmap);
    inWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout->addWidget(inWin);

    // 4. 最後這行最重要：一次性設定視窗的頂層 Layout
    this->setLayout(mainLayout);

    // 連接訊號
    connect(mirrorButton, SIGNAL(clicked()), this, SLOT(mirroredImage()));
    connect(rotateDial, SIGNAL(valueChanged(int)), this, SLOT(rotatedImage()));
    connect(saveButton, &QPushButton::clicked, this, &ImageTransform_1::saveImage);
}
/*ImageTransform_1::ImageTransform_1(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout(this);
    mirrorGroup = new QGroupBox(QStringLiteral("鏡射"), this);
    groupLayout = new QVBoxLayout(mirrorGroup);

    hCheckBox = new QCheckBox(QStringLiteral("水平"), mirrorGroup);
    vCheckBox = new QCheckBox(QStringLiteral("垂直"), mirrorGroup);
    mirrorButton = new QPushButton(QStringLiteral("執行"), mirrorGroup);

    hCheckBox->setGeometry(QRect(13, 28, 87, 19));
    vCheckBox->setGeometry(QRect(13, 54, 87, 19));
    mirrorButton->setGeometry(QRect(13, 80, 93, 28));

    groupLayout->addWidget(hCheckBox);
    groupLayout->addWidget(vCheckBox);
    groupLayout->addWidget(mirrorButton);

    leftLayout->addWidget(mirrorGroup);

    rotateDial = new QDial(this);
    rotateDial->setNotchesVisible(true);
    leftLayout->addItem(vSpacer);

    vSpacer = new QSpacerItem(20, 58, QSizePolicy::Minimum,
                              QSizePolicy::Expanding);

    leftLayout->addWidget(rotateDial);


    // --- 在這裡插入存檔按鈕 ---
    saveButton = new QPushButton(QStringLiteral("存檔"), this);
    leftLayout->addWidget(saveButton);

    // -----------------------
    mainLayout->addLayout(leftLayout);

    inWin = new QLabel(this);
    inWin->setScaledContents(true);

    QPixmap *initPixmap = new QPixmap(300, 200);
    initPixmap->fill(QColor(255, 255, 255));
    //-----------------------------------------------------------------
    QPainter *paint = new QPainter(initPixmap);
    paint->setPen(*(new QColor(0,0,0)));
    paint->begin(initPixmap);
    paint->drawRect(15,15,60,40);
    paint->end();
    if (srcImg.isNull())
    {
        srcImg = initPixmap->toImage();
    }
    //-----------------------------------------------------------------
    inWin->setPixmap(*initPixmap);

    inWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (srcImg.isNull())
    {
        QPixmap *initPixmap = new QPixmap(300, 200);
        initPixmap->fill(QColor(255, 255, 255));
        inWin->setPixmap(*initPixmap);
    }

    mainLayout->addWidget(inWin);

    connect(mirrorButton, SIGNAL(clicked()), this, SLOT(mirroredImage()));
    connect(rotateDial, SIGNAL(valueChanged(int)), this, SLOT(rotatedImage()));
    connect(saveButton, &QPushButton::clicked, this, &ImageTransform_1::saveImage);

}*/

ImageTransform_1::~ImageTransform_1()
{

}

void ImageTransform_1::mirroredImage()
{
    bool H, V;

    if (srcImg.isNull())
        return;

    H = hCheckBox->isChecked();
    V = vCheckBox->isChecked();

    dstImg = srcImg.mirrored(H, V);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
    srcImg = dstImg;

}

void ImageTransform_1::rotatedImage()
{
    QTransform tran;
    int angle;

    if (srcImg.isNull())
        return;

    angle = rotateDial->value();
    tran.rotate(angle);

    dstImg = srcImg.transformed(tran);
    inWin->setPixmap(QPixmap::fromImage(dstImg));

}

/*void ImageTransform_1::saveImage()
{
    // 直接抓取畫面上目前的 Pixmap (包含鏡射或旋轉後的樣子)
    const QPixmap *currentPixmap = inWin->pixmap();

    if (!currentPixmap || currentPixmap->isNull()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("沒有圖案可以儲存"));
        return;
    }

    // 彈出視窗讓使用者選路徑與檔名
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("儲存圖案"),
                                                    "",
                                                    "PNG (*.png);;JPG (*.jpg);;BMP (*.bmp)");

    if (!fileName.isEmpty()) {
        if (currentPixmap->save(fileName)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("存檔完成！"));
        } else {
            QMessageBox::warning(this, QStringLiteral("失敗"), QStringLiteral("存檔失敗，請檢查路徑或權限。"));
        }
    }
}*/
void ImageTransform_1::saveImage()
{
    // 修改這裡：直接用 QPixmap 接收，不要用指標
    QPixmap currentPixmap = inWin->pixmap();

    // 修改判斷方式：直接檢查物件是否為空
    if (currentPixmap.isNull()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("沒有圖案可以儲存"));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("儲存目前的圖案"),
                                                    "",
                                                    "PNG Files (*.png);;JPG Files (*.jpg);;BMP Files (*.bmp)");

    if (!fileName.isEmpty()) {
        // 修改這裡：使用點運算子 (.) 而不是箭頭 (->)
        if (currentPixmap.save(fileName)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("存檔成功！"));
        } else {
            QMessageBox::warning(this, QStringLiteral("失敗"), QStringLiteral("存檔失敗"));
        }
    }
}
//.cpp
