#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include "imageviewerwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenImage_triggered();

    void on_actionSharpen_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs;       // Вкладки

};

#endif // MAINWINDOW_H
