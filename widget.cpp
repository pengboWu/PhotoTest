#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      BiBtn("放大",this),
      LeBtn("缩小",this),
      LBtn("向左",this),
      RBtn("向右",this),
      UBtn("向上",this),
      DBtn("向下",this),
      ResetBtn("还原",this),
      OpenBtn("打开文件",this),
      Alloffset(0,0),
      label("100%",this),
      inputTectF(this)
{
    ratio = 1.0;             //初始化图片缩放比例
    action = Widget::None;

    //新建栅格布局
    QGridLayout *layout = new QGridLayout;

    //设置按钮尺寸
    BiBtn.setMaximumSize(100, 40);
    LeBtn.setMaximumSize(100, 40);
    LBtn.setMaximumSize(100, 40);
    RBtn.setMaximumSize(100, 40);
    UBtn.setMaximumSize(100, 40);
    DBtn.setMaximumSize(100, 40);
    ResetBtn.setMaximumSize(100, 40);
    OpenBtn.setMaximumSize(100, 40);
    Paint.setMinimumSize(600, 600);
    label.setMaximumSize(50, 40);
    inputTectF.setMaximumSize(100,40);

    //设置按钮尺寸
    BiBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    LeBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    LBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    RBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    UBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    DBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ResetBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    OpenBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //向布局管理器添加组件
    layout->addWidget(&Paint, 0, 0, -1, 1);
    layout->addWidget(&BiBtn, 0, 1, 1, 1);
    layout->addWidget(&LeBtn, 1, 1, 1, 1);
    layout->addWidget(&LBtn, 2, 1, 1, 1);
    layout->addWidget(&RBtn,3, 1, 1, 1);
    layout->addWidget(&UBtn,4, 1, 1, 1);
    layout->addWidget(&DBtn, 5, 1, 1, 1);
    layout->addWidget(&ResetBtn, 6, 1, 1, 1);
    layout->addWidget(&OpenBtn,7, 1, 1, 1);
    layout->addWidget(&inputTectF, 8, 1, 1, 1);
    layout->addWidget(&label, 9, 1, 1, 1);


    inputTectF.setEnabled(false);

    setLayout(layout);           //为部件设置布局管理器

    //     BiBtn.setGeometry(822,10,60,25);
    connect(&BiBtn,SIGNAL(clicked()),this,SLOT(onBigClicked()));

    //     LeBtn.setGeometry(822,40,60,25);
    connect(&LeBtn,SIGNAL(clicked()),this,SLOT(onLittleClicked()));

    //     LBtn.setGeometry(822,70,60,25);
    connect(&LBtn,SIGNAL(clicked()),this,SLOT(OnLeftClicked()));
    //     RBtn.setGeometry(822,100,60,25);
    connect(&RBtn,SIGNAL(clicked()),this,SLOT(OnRightClicked()));
    //     UBtn.setGeometry(822,130,60,25);
    connect(&UBtn,SIGNAL(clicked()),this,SLOT(onUpClicked()));
    //     DBtn.setGeometry(822,160,60,25);
    connect(&DBtn,SIGNAL(clicked()),this,SLOT(onDownClicked()));


    //     ResetBtn.setGeometry(822,190,60,25);
    connect(&ResetBtn,SIGNAL(clicked()),this,SLOT(onResetClicked()));

    //     OpenBtn.setGeometry(822,220,60,25);
    connect(&OpenBtn,SIGNAL(clicked()),this,SLOT(onOpenClicked()));

    connect(&inputTectF, SIGNAL(editingFinished()), this, SLOT(onChangeImagePosition()));

    //     label.move(840,260);
    resize(600,600);    //调整窗口大小

    this->setWindowTitle("图片浏览器(请打开文件)");//设置窗口标题
}

//鼠标按下拖动事件
bool Widget::event(QEvent * event)
{
    static bool press=false;
    static QPoint PreDot;
    //检测鼠标按下
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

        //判断鼠标是否是左键按下,且鼠标位置是否在绘画区域
        //        if(mouse->button()==Qt::LeftButton && Paint.contains(mouse->pos())){
        if (mouse->button() == Qt::LeftButton) {
            press = true;
            QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式
            PreDot = mouse->pos();
        }
    } else if(event->type() == QEvent::MouseButtonRelease){//检测鼠标松开
        QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

        //判断鼠标是否是左键释放,且之前是在绘画区域
        if (mouse->button() == Qt::LeftButton && press ) {
            QApplication::setOverrideCursor(Qt::ArrowCursor); //改回鼠标样式
            press = false;
        }
    }
    //检测鼠标移动
    if (event->type() == QEvent::MouseMove) {//移动图片
        if (press) {
            QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);

            offset.setX(mouse->position().x() - PreDot.x());
            offset.setY(mouse->position().ry() - PreDot.y());
            PreDot = mouse->pos();
            action = Widget::Move;

            this->update();
        }
    }
    return QWidget::event(event);
}

//鼠标滑轮事件
void Widget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {      //上滑,缩小

        action=Widget::Shrink;
        this->update();
    } else {                    //下滑,放大
        action=Widget::Amplification;
        this->update();
    }

    event->accept();
}

//重写绘图事件
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //设置渲染方式
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框

    if (image.isNull()) {
        return;
    }

    inputTectF.setEnabled(true);

    int NowW = ratio * pixW;
    int NowH = ratio * pixH;

    if (action == Widget::Shrink)   //缩小
    {
        ratio -= 0.05 * ratio;
        if (ratio < 0.018) {
            ratio = 0.01;
        }

        /*显示比例*/
        QString str;
        str=str.setNum(ratio * 100) + "%";
        //str.sprintf("%.0f%",ratio*100);
        label.setText(str) ;
        qDebug() << "缩小:" <<ratio;
    } else  if (action == Widget::Amplification) { //放大
        ratio += 0.05 * ratio;
        if(ratio > 4.5)
            ratio = 5.000;

        /*显示比例*/
        QString str;
        str=str.setNum(ratio * 100) + "%";;
        //str.sprintf("%.0f%",ratio*100);
        label.setText(str);
        qDebug()<<"放大:"<<ratio;
    }

    if (action == Widget::Amplification || action == Widget::Shrink || action == Widget::Reset) { //放大，缩小，更新图片
        NowW = ratio * pixW;
        NowH = ratio * pixH;

        crtPix = pix.scaled(NowW, NowH, Qt::KeepAspectRatio, Qt::SmoothTransformation); //重新装载
        action = Widget::None;
    }

    if (action == Widget::Move ) { //移动
        int offsetx=Alloffset.x() + offset.x();
        Alloffset.setX(offsetx);

        int offsety=Alloffset.y() + offset.y();
        Alloffset.setY(offsety);

        action = Widget::None;
    }

    if (abs(Alloffset.x()) >= (Paint.width() / 2 + NowW / 2 - 10)) {//限制X偏移值
        if (Alloffset.x() > 0) {
            Alloffset.setX(Paint.width() / 2 + NowW / 2 - 10);
        } else {
            Alloffset.setX(-Paint.width() / 2 + -NowW / 2 + 10);
        }
    }

    if (abs(Alloffset.y()) >= (Paint.height() / 2 + NowH / 2 -10)) {//限制Y偏移值
        if (Alloffset.y() > 0) {
            Alloffset.setY(Paint.height() / 2 + NowH / 2 -10);
        } else {
            Alloffset.setY(-Paint.height() / 2 + -NowH / 2 + 10);
        }
    }

    int x = Paint.width() / 2 + Alloffset.x() - NowW / 2;//确定draw起始点x坐标

    if (x < 0) {
        x = 0;
    }

    int y = Paint.height() / 2 + Alloffset.y() - NowH / 2;//确定draw起始点y坐标
    if (y < 0) {
        y = 0;
    }

    int  sx = NowW / 2 - Paint.width() / 2 - Alloffset.x();
    if (sx < 0) {
        sx = 0;
    }

    int  sy = NowH / 2 - Paint.height() / 2 - Alloffset.y();
    if (sy < 0) {
        sy=0;
    }

    int w =(NowW - sx)>Paint.width() ? Paint.width() : (NowW - sx);
    if (w > (Paint.width() - x)) {
        w = Paint.width() - x;
    }

    int h =(NowH - sy) > Paint.height() ? Paint.height() : (NowH - sy);
    if (h > (Paint.height() - y)) {
        h = Paint.height() - y;
    }

    painter.drawTiledPixmap(x + Paint.x(), y + Paint.y(), w, h, crtPix, sx, sy);  //绘画图形

    qDebug() << "Paint.rect.width = " << Paint.rect().width();
    qDebug() << "Paint.rect.height = " << Paint.rect().height();

    QRectF selectSCope;

    selectSCope = {(Paint.width() / 2 - rec.width()* ratio / 2  + Paint.x() + Alloffset.x()) - displacementCompensationW * ratio, \
                   (Paint.height() / 2 -  rec.height()* ratio / 2 + Paint.y() + Alloffset.y()) - displacementCompensationH * ratio, \
                   rec.width() * ratio, rec.height() * ratio};

    painter.drawRect(selectSCope); //绘制选择框
}

//缩小按钮响应槽函数
void  Widget::onLittleClicked()
{
    action = Widget::Shrink;
    this->update();
}

void  Widget::onBigClicked()
{
    action = Widget::Amplification;
    this->update();
}

//打开按钮响应槽函数
void Widget::onOpenClicked()
{
    QString str = QFileDialog::getOpenFileName(this,
                                               "open",
                                               "D:",
                                               "img (*.png *.jpg)");

    if (!str.isNull()) {
        image.load(str);
        pix = pix.fromImage(image);

        crtPix = pix;
        pixW = image.width();            //图片宽
        pixH = image.height();           //图片高
        qDebug() << str << pixW << pixH;
        this->setWindowTitle("图片浏览器("+str+")");
        onResetClicked();
    }
}

//上移按钮响应函数
void Widget::onUpClicked()
{
    action = Widget::Move;
    offset.setX(0);
    offset.setY(-20);

    this->update();
}

//下移按钮响应槽函数
void Widget::onDownClicked()
{
    action=Widget::Move;
    offset.setX(0);
    offset.setY(20);
    this->update();
}

//重置按钮响应槽函数
void Widget::onResetClicked()
{
    action = Widget::Reset;
    Alloffset.setX(0);
    Alloffset.setY(0);
    ratio = 1.000;
    label.setText("100%");
    this->update();
}

//左移按钮响应槽函数
void Widget::OnLeftClicked()
{
    action = Widget::Move;
    offset.setX(-20);
    offset.setY(0);

    this->update();
}

//右移按钮响应槽函数
void Widget::OnRightClicked(qint64 step)
{
    action = Widget::Move;
    offset.setX(step) ;
    offset.setY(0) ;

    this->update();
}

//收到定位信号
void Widget::onChangeImagePosition() {
    ratio = 1.000;
    label.setText("100%");

    QStringList rectf = inputTectF.text().split(",");
    qDebug() << "line edit input par =" << inputTectF.text();

    rec = {rectf.at(0).toFloat(), rectf.at(1).toFloat(), rectf.at(2).toFloat(), rectf.at(3).toFloat()};

    if (rec.x() < pixW / 2) {
        Alloffset.setX(pixW / 2 - rec.x());
         displacementCompensationW = (pixW / 2 - rec.x());
    } else if (rec.x() > pixW / 2) {
        Alloffset.setX(rec.x() - pixW / 2);
        displacementCompensationW = (rec.x() - pixW / 2);
    } else {
        Alloffset.setX(0);
    }

    if (rec.y() < pixH / 2) {
        Alloffset.setY(pixH / 2 - rec.y());
        displacementCompensationH = (pixH / 2 - rec.y());
    } else if (rec.y() > pixH / 2) {
        Alloffset.setY(rec.y() - pixH / 2);
        displacementCompensationH = (rec.y() - pixH / 2);
    } else {
        Alloffset.setY(0);
    }
    action = Widget::Reset;

    this->update();
}



