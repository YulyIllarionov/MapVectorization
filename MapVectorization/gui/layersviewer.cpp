#include "layersviewer.h"
#include "ui_layersviewer.h"

LayersViewer::LayersViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayersViewer)
{
    ui->setupUi(this);
}

LayersViewer::~LayersViewer()
{
    delete ui;
}
