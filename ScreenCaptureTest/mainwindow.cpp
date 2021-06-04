#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ScreenCapture/screencapture.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    auto img = ScreenCapture::capture();
    if (!img.isNull())
    {
        auto file = QFileDialog::getSaveFileName(nullptr,
                                                 QStringLiteral("Save captured image"),
                                                 QString(),
                                                 QStringLiteral("Images (*.bmp *.png *.jpg);"));
        img.save(file);
    }
}
