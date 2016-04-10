#ifndef IMAGEVIEWERWIDGET_H
#define IMAGEVIEWERWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QScrollBar>
#include <QLabel>
#include "opencv/cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

enum State
{
    def,zoom,pipette
};

namespace Ui {
class ImageViewerWidget;
}

class ImageViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewerWidget(QWidget *parent = 0);
    ImageViewerWidget(QString filename,QWidget *parent = 0);
    ~ImageViewerWidget();
    void SetState(State state);
    QLabel *GetLabel();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
signals:
    void sendPointColor(QRgb);
public slots:
    void zoomIn();
    void zoomOut();
    void setNormalSizeOfImage();
private:
    Ui::ImageViewerWidget *ui;
    cv::Mat cv_image;
    QImage image;
    QLabel *label;
    State _state;
    void scaleImage(double factor);
    void setup();
};

#endif // IMAGEVIEWERWIDGET_H
