#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "widgets/FunctionBlockDiagramWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget * parent = nullptr );
    ~MainWindow();

private:
    FunctionGraph *                     m_functionGraph{};
    FunctionBlockDiagramWidget *        m_fbd{};
    QVBoxLayout *                       m_vBox{};

    Ui::MainWindow * ui;
};

#endif // MAINWINDOW_H
