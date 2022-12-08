#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "models/CFunctionGraph.h"

#include "widgets/CFunctionBlockDiagramWidget.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
    CFunctionBlockDiagramWidget *       m_fbd{};
    QVBoxLayout *                       m_vBox{};
};

#endif // MAINWINDOW_H
