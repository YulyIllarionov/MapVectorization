#ifndef LAYERSVIEWER_H
#define LAYERSVIEWER_H

#include <QWidget>

namespace Ui {
class LayersViewer;
}

class LayersViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LayersViewer(QWidget *parent = 0);
    ~LayersViewer();

private:
    Ui::LayersViewer *ui;
};

#endif // LAYERSVIEWER_H
