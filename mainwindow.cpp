#include "mainwindow.h"

// Generating header
#include "ui_mainwindow.h"
#include "widgets/FunctionBlockDiagramWidget.h"

#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QAction>

using view::FunctionBlockDiagramWidget;

MainWindow::MainWindow( QWidget * parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    QVector< SFunctionInfo > functionInfoList =
    {
        { "Temperature", 3, 2 },
        { "Motor", 2, 1 },
        { "Pressure", 1, 1 },
        { "Kek", 2, 2 },
    };

    QSize size( 1000, 700 );
    this->resize( size );

    m_fbd = new FunctionBlockDiagramWidget( 5, 5, functionInfoList, this );

    this->setCentralWidget( m_fbd );

    connect( ui->exportToFileAction, &QAction::triggered,
             this,
             [ this ] () -> void
    {
        QString fileName = QFileDialog::getSaveFileName(
                    this,
                    tr( "Экспорт в ..." ),
                    "~/graph.txt",
                    tr("Text file (*.txt)"));
        m_fbd->exportGraphToFile( fileName );
    } );

    connect( ui->addLogicAndAction, &QAction::triggered,
             this,
             [ this ] () -> void
    {
        m_fbd->createLogicAndBlock();
    } );

}

MainWindow::~MainWindow()
{
    delete ui;
}
