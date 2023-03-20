#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QEvent>
#include <QFileDialog>
#include <QPainter>
#include <QWheelEvent>
#include <QApplication>
#include <QLineEdit>

class Widget : public QWidget
{
    enum  Type {
        None = 0,
        Amplification,
        Shrink,
        Lift,
        Right,
        Up,
        Down,
        Move,
        Reset,
        Positioning
    };

    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);

signals:
private :
    QPixmap  pix;
    QPixmap  crtPix;
    int action;          //动作(放大,缩小,移动...)
    int pixW;            //图片宽
    int pixH;            //图片高
    QWidget Paint;         //绘画区域
    QImage image;        //打开的图片
    float ratio;              //缩放比例
    QPoint offset;           //一次的图片偏移值
    QPoint Alloffset;          //总偏移
    QLabel label;
    QLineEdit inputTectF;

    QPushButton  BiBtn;  //放大
    QPushButton  LeBtn;  //缩小
    QPushButton  LBtn;   //左移
    QPushButton  RBtn;   //右移
    QPushButton  UBtn;   //上移
    QPushButton  DBtn;   //下移
    QPushButton  ResetBtn;
    QPushButton  OpenBtn;  //打开

    void AddComboItem(QComboBox* cmbo);
    bool event(QEvent * event);
    void wheelEvent(QWheelEvent* e);     //鼠标滑轮事件
    QRectF rec = {10,40,60,40};

    qint64 displacementCompensationW = 0; //定位补偿
    qint64 displacementCompensationH = 0; //定位补偿

private slots:
    void onUpClicked();
    void onDownClicked();
    void onResetClicked();
    void OnLeftClicked();
    void OnRightClicked(qint64 step = 20);
    void onLittleClicked();
    void onBigClicked();
    void onOpenClicked();
    void paintEvent(QPaintEvent *event);
    void onChangeImagePosition();
};

#endif // WIDGET_H
