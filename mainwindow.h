#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

class FunctionGraph;
class FunctionBlockDiagramWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    FunctionGraph *                     m_functionGraph{};
    FunctionBlockDiagramWidget *        m_fbd{};
    QVBoxLayout *                       m_vBox{};
};

#endif // MAINWINDOW_H
