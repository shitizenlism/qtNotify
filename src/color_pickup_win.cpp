#define  COLOR_PICKUP_WIN_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollBar>
#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <glog/logging.h>
#include <vector>
#include "auto_label_algorithm.hh"
#include "color_pickup_win.hh"
#include "ui_color_pickup_win.h"

/** @defgroup COLOR_PICKUP_WIN
  * @brief COLOR_PICKUP_WIN system modules
  * @{
  */

/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const char* DT_POS_NAME[] = {"Dragon", "Tiger"};
static const char* BAC_POS_NAME[] = {"Player-1", "Bancker-1", "Player-2", "Bancker-2", "Player", "Bancker"};
static const char* VNN_POS_NAME[] = {"Silver-1", "Silver-2", "Silver-3", "Silver-4", "Silver-5", "Gold-1", "Gold-2", "Gold-3", "Gold-4", "Gold-5"};
static const char* NN4_POS_NAME[] = {"First", "Bancker-1", "Bancker-2", "Bancker-3", "Bancker-4", "Bancker-5", "Player1-1", "Player1-2", "Player1-3", "Player1-4", "Player1-5", "Player2-1", "Player2-2", "Player2-3", "Player2-4", "Player2-5", "Player3-1", "Player3-2", "Player3-3", "Player3-4", "Player3-5"};
static const char* VBR_POS_NAME[] = {"Black-1", "Black-2", "Black-3", "Red-1", "Red-2", "Red-3"};
static const char* DZ_POS_NAME[] = {"Bull-1", "Bull-2", "Cowboy-1", "Cowboy-2", "Flop-1", "Flop-2", "Flop-3", "Turn", "River"};
static const char* TEB_POS_NAME[] = {"Banker-1", "Banker-2", "Shun-1", "Shun-2", "Sky-1", "Sky-2", "Groud-1", "Groud-2"};



/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/


/** @defgroup CImageView_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup CImageView_CLASS_Functions
  * @{
  */


CImageView::CImageView(QWidget *parent)
    : QGraphicsView (parent)
{
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_iGraphicsScene = new QGraphicsScene();

    m_iGraphicsScene->setSceneRect(0, 0, viewport()->width(), viewport()->height());
    setScene(m_iGraphicsScene);
    m_iImgItem = nullptr;
    m_bDrawingRectangle = false;
    m_MouseActionStat = EN_MouseActionStat_IDLE;
    m_iCurRectItem = nullptr;
    m_nImgWidth = 0;
    m_nImgHeight = 0;
}

CImageView::~CImageView()
{
    QMap<int, QGraphicsRectItem *>::const_iterator iter = m_RectMap.constBegin();
    while (iter != m_RectMap.constEnd()) {
        m_iGraphicsScene->removeItem(iter.value());
        delete iter.value();
        ++iter;
    }
    if(m_iImgItem != nullptr){
        m_iGraphicsScene->removeItem(m_iImgItem);
        delete m_iImgItem;
    }
    m_RectMap.clear();
    delete m_iGraphicsScene;
}

void CImageView::Show_BackgroundImage(QString filename)
{
    m_BackgroundImageFile = filename;
    QFile imageFile(filename);
    if(imageFile.exists()){
        QImage image;
        if(image.load(filename)){
            m_nImgWidth = image.width();
            m_nImgHeight = image.height();
            if(m_iImgItem != nullptr){
                m_iGraphicsScene->removeItem(m_iImgItem);
                delete m_iImgItem;
                m_iImgItem = nullptr;
            }
            QPixmap pixmap = QPixmap::fromImage(image);
            QPixmap scaledPixmap = pixmap.scaled(viewport()->width(), viewport()->height());
            m_iGraphicsScene->setSceneRect(0, 0, viewport()->width(), viewport()->height());
            m_iImgItem = m_iGraphicsScene->addPixmap(scaledPixmap);
    //        ui->m_iGraphicsView->setScene(scene);
    //        ui->m_iGraphicsView->resize(image.width(), image.height());

//            m_iGraphicsScene->setSceneRect(-int(width()/2), -int(height()/2), width(), height());
//             m_iGraphicsScene->setBackgroundBrush(scaledPixmap);
//             fitInView(m_iImgItem);
            show();
        } else {
            qDebug() << "File:" << filename << " is not a valid image file";
            LOG(WARNING) << "File:" << filename.toStdString() << " is not a valid image file";
        }
    } else {
        qDebug() << "File:" << filename << " dosen't exist";
        LOG(WARNING) << "File:" << filename.toStdString() << " dosen't exist";
    }
}

void CImageView::Show_Label(const QRect &rect, const QString &title)
{
    qDebug() << "Show_Label: title=" << title << ", (x=" << rect.x() << ",y=" << rect.y() << ",w=" << rect.width() << ",h=" << rect.height() << ")";
    qreal viewWidth = viewport()->width();
    qreal viewHeight = viewport()->height();
    int real_x = static_cast<int>(rect.x() * viewWidth / m_nImgWidth);
    int real_y = static_cast<int>(rect.y() * viewHeight / m_nImgHeight);
    int real_w = static_cast<int>(rect.width() * viewWidth / m_nImgWidth);
    int real_h = static_cast<int>(rect.height() * viewHeight / m_nImgHeight);
    qDebug() << "Show_Label: scene_rect" << ", (x=" << real_x << ",y=" << real_y << ",w=" << real_w << ",h=" << real_h << ")";
    QMap<QString, CLabelItem *>::iterator iter = m_LabelsItemMap.find(title);
    if (iter != m_LabelsItemMap.end()) {
        delete iter.value();
        m_LabelsItemMap.erase(iter);
    }
    CLabelItem *pLabelItem = new CLabelItem;
    pLabelItem->m_Title = title;
    pLabelItem->m_PosRect = rect;
    QGraphicsRectItem* pRectItem = new QGraphicsRectItem();
    pRectItem->setRect(real_x, real_y, real_w, real_h);
    m_iGraphicsScene->addItem(pRectItem);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0, 0, 255));
    pRectItem->setPen(pen);
    pRectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    pLabelItem->m_iRectItem = pRectItem;

    QGraphicsTextItem *pTextItem = new QGraphicsTextItem(title);
    pTextItem->setDefaultTextColor(QColor(Qt::red));
    pTextItem->setFont(QFont("Courier New", 8));
    pTextItem->setPos(real_x, real_y);
    m_iGraphicsScene->addItem(pTextItem);
    pLabelItem->m_iTextItem = pTextItem;

    m_LabelsItemMap.insert(title, pLabelItem);
}

void CImageView::Clear()
{
    if(m_RectMap.size() > 0){
        QMap<int, QGraphicsRectItem *>::const_iterator iter = m_RectMap.constBegin();
        while (iter != m_RectMap.constEnd()) {
            m_iGraphicsScene->removeItem(iter.value());
            delete iter.value();
            m_RectMap.remove(iter.key());
            emit Del_Rect(iter.key());
            ++iter;
        }
    }

    if(m_LabelsItemMap.size() > 0){
        QMap<QString, CLabelItem*>::const_iterator label_iter = m_LabelsItemMap.constBegin();
        while (label_iter != m_LabelsItemMap.constEnd()) {
            m_iGraphicsScene->removeItem(label_iter.value()->m_iRectItem);
            delete label_iter.value()->m_iRectItem;
            m_iGraphicsScene->removeItem(label_iter.value()->m_iTextItem);
            delete label_iter.value()->m_iTextItem;
            m_LabelsItemMap.remove(label_iter.key());
            ++label_iter;
        }
    }

    if(m_iImgItem != nullptr){
        m_iGraphicsScene->removeItem(m_iImgItem);
        delete m_iImgItem;
        m_iImgItem = nullptr;
    }
}

void CImageView::Clear_Label()
{
    if(m_LabelsItemMap.size() > 0){
        QMap<QString, CLabelItem*>::const_iterator label_iter = m_LabelsItemMap.constBegin();
        while (label_iter != m_LabelsItemMap.constEnd()) {
            m_iGraphicsScene->removeItem(label_iter.value()->m_iRectItem);
            delete label_iter.value()->m_iRectItem;
            m_iGraphicsScene->removeItem(label_iter.value()->m_iTextItem);
            delete label_iter.value()->m_iTextItem;
            m_LabelsItemMap.remove(label_iter.key());
            ++label_iter;
        }
    }
}


void CImageView::resizeEvent(QResizeEvent *event)
{
    if(m_iImgItem != nullptr){
        QFile imageFile(m_BackgroundImageFile);
        if(imageFile.exists()){
            QImage image;
            if(image.load(m_BackgroundImageFile))
            {
                QPixmap pixmap = QPixmap::fromImage(image);
                QPixmap scaledPixmap = pixmap.scaled(viewport()->width(), viewport()->height());
                qreal last_width = m_iGraphicsScene->width();
                qreal last_height = m_iGraphicsScene->height();
                m_iGraphicsScene->setSceneRect(0, 0, viewport()->width(), viewport()->height());
                m_iImgItem->setPixmap(scaledPixmap);
                QMap<int, QGraphicsRectItem *>::const_iterator iter = m_RectMap.constBegin();
                while (iter != m_RectMap.constEnd()) {
                    qreal cur_x = iter.value()->rect().x() * viewport()->width()/last_width;
                    qreal cur_y = iter.value()->rect().y() * viewport()->height()/last_height;
                    qreal cur_width = iter.value()->rect().width() * viewport()->width()/last_width;
                    qreal cur_height = iter.value()->rect().height() * viewport()->height()/last_height;
                    iter.value()->setRect(cur_x, cur_y, cur_width, cur_height);
                    ++iter;
                }

                QMap<QString, CLabelItem*>::const_iterator label_iter = m_LabelsItemMap.constBegin();
                while (label_iter != m_LabelsItemMap.constEnd()) {
                    qreal cur_x = label_iter.value()->m_iRectItem->rect().x() * viewport()->width()/last_width;
                    qreal cur_y = label_iter.value()->m_iRectItem->rect().y() * viewport()->height()/last_height;
                    qreal cur_width = label_iter.value()->m_iRectItem->rect().width() * viewport()->width()/last_width;
                    qreal cur_height = label_iter.value()->m_iRectItem->rect().height() * viewport()->height()/last_height;
                    label_iter.value()->m_iRectItem->setRect(cur_x, cur_y, cur_width, cur_height);
                    cur_x = label_iter.value()->m_iTextItem->pos().x() * viewport()->width()/last_width;
                    cur_y = label_iter.value()->m_iTextItem->pos().y() * viewport()->width()/last_width;
                    label_iter.value()->m_iTextItem->setPos(cur_x, cur_y);
                    ++label_iter;
                }
//                 m_iImgItem = m_iGraphicsScene->addPixmap(scaledPixmap);
        //        ui->m_iGraphicsView->setScene(scene);
        //        ui->m_iGraphicsView->resize(image.width(), image.height());

    //            m_iGraphicsScene->setSceneRect(-int(width()/2), -int(height()/2), width(), height());
    //             m_iGraphicsScene->setBackgroundBrush(scaledPixmap);
    //            fitInView(m_iImgItem);
                show();
            }
        }
    }
}

void CImageView::wheelEvent(QWheelEvent *event)
{
    QPoint cursorPoint = event->pos();
    QPointF scenePos = mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));

    qreal viewWidth = viewport()->width();
    qreal viewHeight = viewport()->height();

    qreal hScale = cursorPoint.x() / viewWidth;
    qreal vScale = cursorPoint.y() / viewHeight;

    //qreal scaleFactor = matrix().m11();
    int wheelDeltaValue = event->delta();
    if (wheelDeltaValue > 0)
    {
        scale(1.2, 1.2);
    }
    else
    {
        scale(1.0 / 1.2, 1.0 / 1.2);
    }

    QPointF viewPoint = matrix().map(scenePos);
    horizontalScrollBar()->setValue(int(viewPoint.x() - viewWidth * hScale));
    verticalScrollBar()->setValue(int(viewPoint.y() - viewHeight * vScale));
}

void CImageView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(m_MouseActionStat == EN_MouseActionStat_IDLE){
            QPointF relative_pos = mapToScene(QPoint(event->pos().x(), event->pos().y()));
            qreal viewWidth = viewport()->width();
            qreal viewHeight = viewport()->height();
            if(relative_pos.x() < 0){
                relative_pos.setX(0);
            }
            if(relative_pos.y() < 0){
                relative_pos.setY(0);
            }
            if(relative_pos.x() >= viewWidth){
                relative_pos.setY(viewWidth - 1);
            }
            if(relative_pos.y() >= viewHeight){
                relative_pos.setY(viewHeight - 1);
            }
            QMap<int, QGraphicsRectItem *>::const_iterator iter = m_RectMap.constBegin();
            while (iter != m_RectMap.constEnd()) {
                if(iter.value()->contains(relative_pos)){
                    QRectF curRectF = iter.value()->rect();
                    qreal curLeftupPointx = curRectF.width() >= 0 ? curRectF.x():(curRectF.x() + curRectF.width());
                    qreal curLeftupPointy = curRectF.height() >= 0 ? curRectF.y():(curRectF.y() + curRectF.height());
                    if(m_iCurRectItem != nullptr){
                        QRectF lastRectF = m_iCurRectItem->rect();
                        qreal lastLeftupPointx = lastRectF.width() >= 0 ? lastRectF.x():(lastRectF.x() + lastRectF.width());
                        qreal lastLeftupPointy = lastRectF.height() >= 0 ? lastRectF.y():(lastRectF.y() + lastRectF.height());
                        if(curLeftupPointx <= lastLeftupPointx){
                            if(curLeftupPointy <= lastLeftupPointy){
                                // do nothing
                                iter.value()->setSelected(false);
                            } else {
                                m_iCurRectItem->setSelected(false);
                                m_iCurRectItem = iter.value();
                                m_iCurRectItem->setSelected(true);
                            }
                        } else {
                            m_iCurRectItem->setSelected(false);
                            m_iCurRectItem = iter.value();
                            m_iCurRectItem->setSelected(true);
                        }
                    } else {
                        m_iCurRectItem = iter.value();
                        m_iCurRectItem->setSelected(true);
                    }
                    m_MouseActionStat = EN_MouseActionStat_MoveRect;
                } else {
                    iter.value()->setSelected(false);
                }
                ++iter;
            }
            if(m_MouseActionStat == EN_MouseActionStat_MoveRect){
                m_StartPos = relative_pos;
                return;
            }
            iter = m_RectMap.constBegin();
            while (iter != m_RectMap.constEnd()) {
                iter.value()->setSelected(false);
                ++iter;
            }
            m_MouseActionStat = EN_MouseActionStat_DrawingRect;
            m_bDrawingRectangle = true;
            m_StartPos = relative_pos;
        }
    }
    event->accept();
}

void CImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QPointF curPos = mapToScene(QPoint(event->pos().x(), event->pos().y()));
        qreal viewWidth = viewport()->width();
        qreal viewHeight = viewport()->height();
        if(m_MouseActionStat == EN_MouseActionStat_DrawingRect){
            m_MouseActionStat = EN_MouseActionStat_IDLE;
            qreal width_resize = 0;
            if(curPos.x() <= m_StartPos.x()){
                width_resize = 0;
            } else {
                if(curPos.x() < 0){
                    width_resize = -m_StartPos.x();
                } else if(curPos.x() >= viewWidth){
                    width_resize = viewWidth-m_StartPos.x();
                } else {
                    width_resize = curPos.x()-m_StartPos.x();
                }
            }

            qreal height_resize = 0;
            if(curPos.y() <= m_StartPos.y()){
                height_resize = 0;
            } else {
                if(curPos.y() < 0){
                    height_resize = -m_StartPos.y();
                } else if(curPos.y() >= viewHeight){
                    height_resize = viewHeight-m_StartPos.y();
                } else {
                    height_resize = curPos.y()-m_StartPos.y();
                }
            }
            if(m_StartPos != curPos){
                if(m_iCurRectItem == nullptr){
                    m_iCurRectItem = new QGraphicsRectItem();
                    m_iCurRectItem->setRect(m_StartPos.x(), m_StartPos.y(), width_resize, height_resize);
                    m_iGraphicsScene->addItem(m_iCurRectItem);
                    QPen pen;
                    pen.setWidth(1);
                    pen.setColor(QColor(255, 0, 0));
                    m_iCurRectItem->setPen(pen);
                    m_iCurRectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
                } else {
                    m_iCurRectItem->setRect(m_StartPos.x(), m_StartPos.y(), width_resize, height_resize);
                }
                int index = 0;
                QMap<int, QGraphicsRectItem *>::iterator iter;
                do{
                    index++;
                    iter = m_RectMap.find(index);
                }while(iter != m_RectMap.end());
                m_RectMap.insert(index, m_iCurRectItem);

                QRect rect;
                rect.setX(int(m_iCurRectItem->rect().x()*m_nImgWidth/viewWidth));
                rect.setY(int(m_iCurRectItem->rect().y()*m_nImgHeight/viewHeight));
                rect.setWidth(int(m_iCurRectItem->rect().width()*m_nImgWidth/viewWidth));
                rect.setHeight(int(m_iCurRectItem->rect().height()*m_nImgHeight/viewHeight));
                Add_Rect(index, rect);
                m_iCurRectItem = nullptr;
            } else {
                if(m_iCurRectItem != nullptr){
                    m_iGraphicsScene->removeItem(m_iCurRectItem);
                    delete m_iCurRectItem;
                    m_iCurRectItem = nullptr;
                }
            }
        } else if(m_MouseActionStat == EN_MouseActionStat_MoveRect) {
            m_MouseActionStat = EN_MouseActionStat_IDLE;
//            QPointF curPos = mapToScene(QPoint(event->pos().x(), event->pos().y()));
//            if(m_StartPos != curPos){
//                m_iRectItem->moveBy(curPos.x(), curPos.y());
//            }
            m_MouseActionStat = EN_MouseActionStat_IDLE;
            m_iCurRectItem = nullptr;
        }
    }
    event->accept();
}

void CImageView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_MouseActionStat == EN_MouseActionStat_DrawingRect){
        QPointF curPos = mapToScene(QPoint(event->pos().x(), event->pos().y()));
        qreal viewWidth = viewport()->width();
        qreal viewHeight = viewport()->height();
        qreal width_resize = 0;
        if(curPos.x() <= m_StartPos.x()){
            width_resize = 0;
        } else {
            if(curPos.x() < 0){
                width_resize = -m_StartPos.x();
            } else if(curPos.x() >= viewWidth){
                width_resize = viewWidth-m_StartPos.x();
            } else {
                width_resize = curPos.x()-m_StartPos.x();
            }
        }

        qreal height_resize = 0;
        if(curPos.y() <= m_StartPos.y()){
            height_resize = 0;
        } else {
            if(curPos.y() < 0){
                height_resize = -m_StartPos.y();
            } else if(curPos.y() >= viewHeight){
                height_resize = viewHeight-m_StartPos.y();
            } else {
                height_resize = curPos.y()-m_StartPos.y();
            }
        }

        if(m_StartPos != curPos){
            if(m_iCurRectItem == nullptr){
                m_iCurRectItem = new QGraphicsRectItem();
                m_iCurRectItem->setRect(m_StartPos.x(), m_StartPos.y(), width_resize, height_resize);
                m_iGraphicsScene->addItem(m_iCurRectItem);
                QPen pen;
                pen.setWidth(1);
                pen.setColor(QColor(255, 0, 0));
                m_iCurRectItem->setPen(pen);
                m_iCurRectItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
            } else {
                m_iCurRectItem->setRect(m_StartPos.x(), m_StartPos.y(), width_resize, height_resize);
            }
        }
    } else if(m_MouseActionStat == EN_MouseActionStat_MoveRect){
        QPointF curPos = mapToScene(QPoint(event->pos().x(), event->pos().y()));
        if(m_StartPos != curPos){
            if(m_iCurRectItem != nullptr){
                qreal start_x = m_iCurRectItem->rect().x() + curPos.x() - m_StartPos.x();
                qreal start_y = m_iCurRectItem->rect().y() + curPos.y() - m_StartPos.y();
                m_StartPos = curPos;
                m_iCurRectItem->setRect(start_x, start_y, m_iCurRectItem->rect().width(), m_iCurRectItem->rect().height());
            }
        }
    }
    event->accept();
}

void CImageView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete){
        QMap<int, QGraphicsRectItem *>::const_iterator iter = m_RectMap.constBegin();
        while (iter != m_RectMap.constEnd()) {
            if(iter.value()->isSelected()){
                m_iGraphicsScene->removeItem(iter.value());
                delete iter.value();
                m_RectMap.remove(iter.key());
                emit Del_Rect(iter.key());
                return;
            }
            ++iter;
        }
    }
}

/**
  * @}
  */


/** @defgroup CColorPickupWin_Private_Functions
  * @{
  */

/**
  * @}
  */

/** @defgroup CColorPickupWin_CLASS_Functions
  * @{
  */

CColorPickupWin::CColorPickupWin(QWidget *parent)
    : CBaseTabWin (parent), ui(new Ui::CColorPickupWin),
      m_nBestMinH(65535),m_nBestMinS(65535),m_nBestMinV(65535),m_nBestMaxH(0), m_nBestMaxS(0), m_nBestMaxV(0)
{
    ui->setupUi(this);
//    qDebug() << "CColorPickupWin class construct:" << img_filename;
//    m_ImgFilename = img_filename;

    connect(ui->m_iImageFileChooseBtn, SIGNAL(clicked()), this, SLOT(On_ImageFile_Choose()));

    connect(ui->m_iImageView, SIGNAL(Add_Rect(int, const QRect &)), this, SLOT(On_Rect_Add(int, const QRect &)));
    connect(ui->m_iImageView, SIGNAL(Del_Rect(int)), this, SLOT(On_Rect_Del(int)));

    ui->m_iColorAreaView->resizeColumnsToContents();
    m_iColorAreaModel = new QStandardItemModel(this);
    QStringList labels;
    labels << tr("ID") << tr("区域坐标") << tr("区域的最小HSV值") << tr("区域的最大HSV值");
    m_iColorAreaModel->setHorizontalHeaderLabels(labels);
    ui->m_iColorAreaView->setModel(m_iColorAreaModel);

    connect(ui->m_iShowAutoLabelParamCBox, SIGNAL(stateChanged(int)), this, SLOT(On_Show_AutoLabel_Param(int)));
    ui->m_iAutoLabelParamWin->setVisible(false);

    connect(ui->m_iAutoLabelBtn, SIGNAL(clicked()), this, SLOT(On_Auto_Label()));

    ui->m_iDetectAreaNameView->resizeColumnsToContents();
    m_iDetectAreaNameModel = new QStandardItemModel(this);
    QStringList labels1;
    labels1 << tr("区域名称") << tr("区域别名");
    m_iDetectAreaNameModel->setHorizontalHeaderLabels(labels1);
    ui->m_iDetectAreaNameView->setModel(m_iDetectAreaNameModel);
}

CColorPickupWin::~CColorPickupWin(){
    m_ColorMinMap.clear();
    m_ColorMaxMap.clear();

    int rows = m_iColorAreaModel->rowCount();
    while(rows > 0){
        QList<QStandardItem *> list = m_iColorAreaModel->takeRow(rows - 1);
        while (!list.isEmpty()){
            delete list.takeFirst();
        }
        rows = m_iColorAreaModel->rowCount();
    }
    delete m_iColorAreaModel;
}

void CColorPickupWin::Get_BestMinColor(int &minh, int &mins, int &minv)
{
    if(m_ImgFilename.size() == 0){
        return;
    }
    int sum_h = 0, sum_s = 0, sum_v = 0;
    QMap<int, cv::Scalar_<int>>::const_iterator iter = m_ColorMinMap.constBegin();
    while (iter != m_ColorMinMap.constEnd()) {
        sum_h = sum_h + int(iter.value()[0]);
        sum_s = sum_s + int(iter.value()[1]);
        sum_v = sum_v + int(iter.value()[2]);
        ++iter;
    }
    minh = int(sum_h/m_ColorMinMap.size());
    mins = int(sum_s/m_ColorMinMap.size());
    minv = int(sum_v/m_ColorMinMap.size());
}

void CColorPickupWin::Get_BestMaxColor(int &maxh, int &maxs, int &maxv)
{
    if(m_ImgFilename.size() == 0){
        return;
    }
    int sum_h = 0, sum_s = 0, sum_v = 0;
    QMap<int, cv::Scalar_<int>>::const_iterator iter = m_ColorMaxMap.constBegin();
    while (iter != m_ColorMaxMap.constEnd()) {
        sum_h = sum_h + int(iter.value()[0]);
        sum_s = sum_s + int(iter.value()[1]);
        sum_v = sum_v + int(iter.value()[2]);
        ++iter;
    }
    maxh = int(sum_h/m_ColorMaxMap.size());
    maxs = int(sum_s/m_ColorMaxMap.size());
    maxv = int(sum_v/m_ColorMaxMap.size());
}

void CColorPickupWin::Select()
{
    ui->m_iAutoLabelParamWin->Refresh();
}

void CColorPickupWin::UnSelect()
{

}

static cv::Mat __doubleCols(cv::Mat &a){
    int totalCols = a.cols * 2;
    cv::Mat mergeMat(a.rows, totalCols, a.type());
    cv::Mat subMat = mergeMat.colRange(0, a.cols);
    a.copyTo(subMat);
    subMat = mergeMat.colRange(a.cols, totalCols);
    a.copyTo(subMat);
    return mergeMat;
}

static cv::Mat __doubleRows(cv::Mat &a){
    int totalRows = a.rows * 2;
    cv::Mat mergeMat(totalRows, a.cols, a.type());
    cv::Mat subMat = mergeMat.rowRange(0, a.rows);
    a.copyTo(subMat);
    subMat = mergeMat.colRange(a.rows, totalRows);
    a.copyTo(subMat);
    return mergeMat;
}

void CColorPickupWin::__Calc_Color_Range(int id, QRect rect, int &minh, int &mins, int &minv, int &maxh, int &maxs, int &maxv)
{
    cv::Mat image = cv::imread(m_ImgFilename.toLocal8Bit().data());
    if(rect.x() < 0 || rect.x() > image.cols ||
       rect.y() < 0 || rect.y() > image.rows){
        return;
    }
    cv::Mat sub_image;
    qDebug() << QString("x=%1, y=%2,w=%3,h=%4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
    if(rect.width() == 0 && rect.height() == 0){
        sub_image.create(2, 2, image.type());
        sub_image.at<cv::Vec3b>(0, 0) = image.at<cv::Vec3b>(rect.y(), rect.x());
        sub_image.at<cv::Vec3b>(0, 1) = image.at<cv::Vec3b>(rect.y(), rect.x());
        sub_image.at<cv::Vec3b>(1, 0) = image.at<cv::Vec3b>(rect.y(), rect.x());
        sub_image.at<cv::Vec3b>(1, 1) = image.at<cv::Vec3b>(rect.y(), rect.x());
    } else if (rect.width() == 0 && rect.height() != 0) {
        sub_image.create(rect.height(), 2, image.type());
        for (int iLoop = 0; iLoop < rect.height(); iLoop++){
            sub_image.at<cv::Vec3b>(iLoop, 0) = image.at<cv::Vec3b>(rect.y()+iLoop, rect.x());
            sub_image.at<cv::Vec3b>(iLoop, 1) = image.at<cv::Vec3b>(rect.y()+iLoop, rect.x());
        }
    } else if (rect.width() != 0 && rect.height() == 0) {
        sub_image.create(2, rect.width(), image.type());
        for (int iLoop = 0; iLoop < rect.width(); iLoop++){
            sub_image.at<cv::Vec3b>(0, iLoop) = image.at<cv::Vec3b>(rect.y(), rect.x()+iLoop);
            sub_image.at<cv::Vec3b>(1, iLoop) = image.at<cv::Vec3b>(rect.y(), rect.x()+iLoop);
        }
    } else {
        sub_image = image(cv::Rect(rect.x(), rect.y(), rect.width(), rect.height()));
    }

    if(sub_image.empty()){
        return;
    }

    qDebug() << QString("sub_image rows=%1, cols=%2").arg(sub_image.rows).arg(sub_image.cols);
    //QString sub_image_filename = "./config/sub-" + QString::number(id) + ".jpg";
    //cv::imwrite(sub_image_filename.toStdString(), sub_image);
    cv::Mat image_hsv;
    cv::cvtColor(sub_image, image_hsv, cv::COLOR_BGR2HSV);
    cv::MatND hist;
    minh = 0;maxh = 0;mins = 0;maxs = 0;minv = 0;maxv = 0;
    int channels = 0;
    int histsize[] = { 1024 };
    float midranges[] = { 0,1023 };
    const float *ranges[] = { midranges };
    calcHist(&image_hsv, 1, &channels, cv::Mat(), hist, 1, histsize, ranges, true, false);
    for(int iLoop = 0; iLoop < 1024; iLoop++) {
        if(hist.at<float>(iLoop) > 0){
            if(minh == 0){
                minh = iLoop;
            }
            if(iLoop > maxh){
                maxh = iLoop;
            }
        }
    }

    channels = 1;
    calcHist(&image_hsv, 1, &channels, cv::Mat(), hist, 1, histsize, ranges, true, false);
    for(int iLoop = 0; iLoop < 1024; iLoop++) {
        if(hist.at<float>(iLoop) > 0){
            if(mins == 0){
                mins = iLoop;
            }
            if(iLoop > maxs){
                maxs = iLoop;
            }
        }
    }

    channels = 2;
    calcHist(&image_hsv, 1, &channels, cv::Mat(), hist, 1, histsize, ranges, true, false);
    for(int iLoop = 0; iLoop < 1024; iLoop++) {
        if(hist.at<float>(iLoop) > 0){
            if(minv == 0){
                minv = iLoop;
            }
            if(iLoop > maxv){
                maxv = iLoop;
            }
        }
    }
}

void CColorPickupWin::__Update_Label()
{
    while(m_iDetectAreaNameModel->rowCount() > 0){
        QList<QStandardItem *> pRowItemsList = m_iDetectAreaNameModel->takeRow(m_iDetectAreaNameModel->rowCount() - 1);
        while (!pRowItemsList.isEmpty()){
          delete pRowItemsList.takeFirst();
        }
    }
    ui->m_iImageView->Clear_Label();
    m_LabelPosMap.clear();
    CAnnotation* pAnnotation = Configs::GetInstance()->GetAnnotation();
    if(pAnnotation != nullptr){
        LOG(INFO) << "pAnnotation->m_nGameType=" << pAnnotation->m_nGameType;
        LOG(INFO) << "pAnnotation->m_LabelsObjectMap.size()=" << pAnnotation->m_LabelsObjectMap.size();
        QMap<QString, cv::Rect>::const_iterator iter = pAnnotation->m_LabelsObjectMap.constBegin();
        while (iter != pAnnotation->m_LabelsObjectMap.constEnd()) {
            QString labelText;
            int index = iter.key().split("_")[1].toInt();
            if(pAnnotation->m_LabelsObjectMap.size() == 6){
                if(pAnnotation->m_nGameType == 4){
                    labelText = BAC_POS_NAME[index - 1];
                } else if(pAnnotation->m_nGameType == 6) {
                    labelText = VBR_POS_NAME[index - 1];
                } else {
                    LOG(ERROR) << "unkown gametype:" << pAnnotation->m_nGameType;
                }
            } else if(pAnnotation->m_LabelsObjectMap.size() == 2){
                labelText = DT_POS_NAME[index - 1];
            } else if(pAnnotation->m_LabelsObjectMap.size() == 21){
                labelText = NN4_POS_NAME[index];
            } else if(pAnnotation->m_LabelsObjectMap.size() == 10){
                labelText = VNN_POS_NAME[index - 1];
            } else if(pAnnotation->m_LabelsObjectMap.size() == 9){
                labelText = DZ_POS_NAME[index - 1];
            } else if(pAnnotation->m_LabelsObjectMap.size() == 8){
                labelText = TEB_POS_NAME[index - 1];
            }
            m_LabelPosMap[labelText] = iter.value();
            QRect pos;
            pos.setX(iter.value().x);
            pos.setY(iter.value().y);
            pos.setWidth(iter.value().width);
            pos.setHeight(iter.value().height);
            ui->m_iImageView->Show_Label(pos, labelText);

            QStandardItem* pItem = nullptr;
            int rownum = m_iDetectAreaNameModel->rowCount();
            pItem = new QStandardItem(iter.key());
            m_iDetectAreaNameModel->setItem(rownum,0,pItem);
            pItem = new QStandardItem(labelText);
            m_iDetectAreaNameModel->setItem(rownum,1,pItem);
            ui->m_iDetectAreaNameView->resizeColumnsToContents();
            ++iter;
        }
    }
}

void CColorPickupWin::On_Rect_Add(int id, const QRect &rect)
{
    qDebug() << "On_Rect_Add";
    int minh = 0,maxh = 0,mins = 0,maxs = 0,minv = 0,maxv = 0;
    __Calc_Color_Range(id, rect, minh,mins,minv,maxh,maxs,maxv);
    QStandardItem* pItem = nullptr;
    int rownum = m_iColorAreaModel->rowCount();
    pItem = new QStandardItem(QString::number(id));
    m_iColorAreaModel->setItem(rownum,0,pItem);
    pItem = new QStandardItem(QString("(%1,%2,%3,%4)").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));
    m_iColorAreaModel->setItem(rownum,1,pItem);
    pItem = new QStandardItem(QString("HSV(%1, %2, %3)").arg(minh).arg(mins).arg(minv));
    m_iColorAreaModel->setItem(rownum,2,pItem);
    pItem = new QStandardItem(QString("HSV(%1, %2, %3)").arg(maxh).arg(maxs).arg(maxv));
    m_iColorAreaModel->setItem(rownum,3,pItem);
    ui->m_iColorAreaView->resizeColumnsToContents();
    QMap<int, cv::Scalar_<int>>::iterator iter;
    iter = m_ColorMinMap.find(id);
    cv::Scalar_<int> min_hsv;
    min_hsv[0] = minh;
    min_hsv[1] = mins;
    min_hsv[2] = minv;
    if(iter != m_ColorMinMap.end()){
        m_ColorMinMap[id] = min_hsv;
    } else {
        m_ColorMinMap.insert(id, min_hsv);
    }
    m_nBestMinH = 65535;
    m_nBestMinS = 65535;
    m_nBestMinV = 65535;
    QMap<int, cv::Scalar_<int>>::const_iterator const_iter = m_ColorMinMap.constBegin();
    while (const_iter != m_ColorMinMap.constEnd()) {
        if(m_nBestMinH > const_iter.value()[0]){
            m_nBestMinH = const_iter.value()[0];
        }
        if(m_nBestMinS > const_iter.value()[1]){
            m_nBestMinS = const_iter.value()[1];
        }
        if(m_nBestMinV > const_iter.value()[2]){
            m_nBestMinV = const_iter.value()[2];
        }
        qDebug() << "min_hsv id=" <<  const_iter.key() << ",minH=" << const_iter.value()[0] << ",minS=" << const_iter.value()[1] << ",minV=" << const_iter.value()[2];
        ++const_iter;
    }
    cv::Scalar_<int> max_hsv;
    max_hsv[0] = maxh;
    max_hsv[1] = maxs;
    max_hsv[2] = maxv;
    iter = m_ColorMaxMap.find(id);
    if(iter != m_ColorMaxMap.end()){
        m_ColorMaxMap[id] = max_hsv;
    } else {
        m_ColorMaxMap.insert(id, max_hsv);
    }
    m_nBestMaxH = 0;
    m_nBestMaxS = 0;
    m_nBestMaxV = 0;
    const_iter = m_ColorMaxMap.constBegin();
    while (const_iter != m_ColorMaxMap.constEnd()) {
        if(m_nBestMaxH < const_iter.value()[0]){
            m_nBestMaxH = const_iter.value()[0];
        }
        if(m_nBestMaxS < const_iter.value()[1]){
            m_nBestMaxS = const_iter.value()[1];
        }
        if(m_nBestMaxV < const_iter.value()[2]){
            m_nBestMaxV = const_iter.value()[2];
        }
        qDebug() << "max_hsv id=" <<  const_iter.key() << ",maxH=" << const_iter.value()[0] << ",maxS=" << const_iter.value()[1] << ",maxV=" << const_iter.value()[2];
        ++const_iter;
    }
    QString title = tr("颜色区域的HSV信息:");
    title = title + "(minH=" + QString::number(m_nBestMinH) + ",minS=" + QString::number(m_nBestMinS) + ",minV=" + QString::number(m_nBestMinV) + ")";
    title = title + "(maxH=" + QString::number(m_nBestMaxH) + ",maxS=" + QString::number(m_nBestMaxS) + ",maxV=" + QString::number(m_nBestMaxV) + ")";
    ui->m_iColorAreaGBox->setTitle(title);
    ui->m_iAutoLabelParamWin->ShowColorThreshold(m_nBestMinH, m_nBestMinS, m_nBestMinV, m_nBestMaxH, m_nBestMaxS, m_nBestMaxV);
    qDebug() << "On_Rect_Add finished";
}

void CColorPickupWin::On_Rect_Del(int id)
{
    int rows = m_iColorAreaModel->rowCount();
    int iLoop = 0;
    for (iLoop = 0; iLoop < rows; ++iLoop)
    {
        QStandardItem* item = m_iColorAreaModel->item(iLoop, 0);//1 1
        if (item)
        {
            if (item->text().toInt() == id)
            {
                break;
            }
        }
    }
    if(iLoop < rows){
        QList<QStandardItem *> list = m_iColorAreaModel->takeRow(iLoop);
        while (!list.isEmpty()){
            delete list.takeFirst();
        }
    }
    ui->m_iColorAreaView->resizeColumnsToContents();

    QMap<int, cv::Scalar_<int>>::iterator iter;
    iter = m_ColorMinMap.find(id);
    if(iter != m_ColorMinMap.end()){
        m_ColorMinMap.remove(id);
    }

    iter = m_ColorMaxMap.find(id);
    if(iter != m_ColorMaxMap.end()){
        m_ColorMaxMap.remove(id);
    }

    QMap<int, cv::Scalar_<int>>::const_iterator const_iter = m_ColorMinMap.constBegin();
    while (const_iter != m_ColorMinMap.constEnd()) {
        if(m_nBestMinH > const_iter.value()[0]){
            m_nBestMinH = const_iter.value()[0];
        }
        if(m_nBestMinS > const_iter.value()[1]){
            m_nBestMinS = const_iter.value()[1];
        }
        if(m_nBestMinV > const_iter.value()[2]){
            m_nBestMinV = const_iter.value()[2];
        }
        qDebug() << "min_hsv id=" <<  const_iter.key() << ",minH=" << const_iter.value()[0] << ",minS=" << const_iter.value()[1] << ",minV=" << const_iter.value()[2];
        ++const_iter;
    }

    m_nBestMaxH = 0;
    m_nBestMaxS = 0;
    m_nBestMaxV = 0;
    const_iter = m_ColorMaxMap.constBegin();
    while (const_iter != m_ColorMaxMap.constEnd()) {
        if(m_nBestMaxH < const_iter.value()[0]){
            m_nBestMaxH = const_iter.value()[0];
        }
        if(m_nBestMaxS < const_iter.value()[1]){
            m_nBestMaxS = const_iter.value()[1];
        }
        if(m_nBestMaxV < const_iter.value()[2]){
            m_nBestMaxV = const_iter.value()[2];
        }
        qDebug() << "max_hsv id=" <<  const_iter.key() << ",maxH=" << const_iter.value()[0] << ",maxS=" << const_iter.value()[1] << ",maxV=" << const_iter.value()[2];
        ++const_iter;
    }
    QString title = tr("颜色区域的HSV信息:");
    title = title + "(minH=" + QString::number(m_nBestMinH) + ",minS=" + QString::number(m_nBestMinS) + ",minV=" + QString::number(m_nBestMinV) + ")";
    title = title + "(maxH=" + QString::number(m_nBestMaxH) + ",maxS=" + QString::number(m_nBestMaxS) + ",maxV=" + QString::number(m_nBestMaxV) + ")";
    ui->m_iColorAreaGBox->setTitle(title);
    ui->m_iAutoLabelParamWin->ShowColorThreshold(m_nBestMinH, m_nBestMinS, m_nBestMinV, m_nBestMaxH, m_nBestMaxS, m_nBestMaxV);
}

void CColorPickupWin::On_ImageFile_Choose()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片的路径："),
                                                     "./",
                                                     tr("image (*.png *.jpg)"));

    QFile imageFile(fileName);
    if(imageFile.exists()){
        ui->m_iImageFileNameEdit->setText(fileName);
        m_ImgFilename = fileName;
        ui->m_iImageView->Clear();
        ui->m_iImageView->Show_BackgroundImage(fileName);
        __Update_Label();
    }
}


void CColorPickupWin::On_Show_AutoLabel_Param(int state){
    if(state == Qt::Checked){
        ui->m_iAutoLabelParamWin->setVisible(true);
    } else {
        ui->m_iAutoLabelParamWin->setVisible(false);
    }
}

void CColorPickupWin::On_Auto_Label()
{
    if(Configs::GetInstance()->GetDetectAppPath() != ""){
        cv::Mat frame = cv::imread(ui->m_iImageFileNameEdit->text().toStdString());
        if(frame.empty()){
            LOG(ERROR) << "image " << ui->m_iImageFileNameEdit->text().toStdString() << " not exist";
            return;
        }
        QMap<QString, cv::Rect> map;
        int tmp_width = frame.cols, tmp_height = frame.rows;
        int gametype = 0;
        CAnnotation* pAnnotation = Configs::GetInstance()->GetAnnotation();
        if(Auto_Label_algo(frame, &map, gametype) == 0){
            LOG(INFO) << "Auto Label success, gametype=" << gametype;
            if(pAnnotation == nullptr){
                QString labelsXMLPath = Configs::GetInstance()->GetLabelsXMLPath();
                if(labelsXMLPath.size() == 0){
                    QString detectAppPath = Configs::GetInstance()->GetDetectAppPath();
                    if(detectAppPath.size() == 0){
                        labelsXMLPath = "./config/cap0_pos.xml";
                    } else {
                        labelsXMLPath = detectAppPath + "/models/cap0_pos.xml";
                    }
                }
                qDebug() << "labelsXMLPath = " << labelsXMLPath;
                pAnnotation = new CAnnotation(labelsXMLPath.toLocal8Bit().data(), map, tmp_width, tmp_height, gametype);
                Configs::GetInstance()->SetLabelsXMLPath(labelsXMLPath);
            } else {
                pAnnotation->m_LabelsObjectMap = map;
                pAnnotation->m_nGameType = gametype;
                pAnnotation->m_nWidth = static_cast<uint32_t>(tmp_width);
                pAnnotation->m_nHeight = static_cast<uint32_t>(tmp_height);
                pAnnotation->Update_To_Local();
            }
            LOG(INFO) << "pAnnotation->m_nGameType=" << pAnnotation->m_nGameType;
            __Update_Label();
        } else {
            QMessageBox::critical(this, tr("Auto标定错误"),
                                     tr("Auto标定错误，请详细检查参数!"),
                                     QMessageBox::Ok);
        }
    } else {
        QMessageBox::critical(this, tr("无法标定"),
                                 tr("识别程序路径为空，请先指定识别程序路径"),
                                 QMessageBox::Ok);
    }
}

/**
  * @}
  */

/**
  * @}
  */

/*************** (C) COPYRIGHT 2010-2018 intech *********END OF FILE***********/


