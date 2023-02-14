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
        { "Temperature", 0, 3, 2 },
        { "Motor", 0, 2, 1 },
        { "Pressure", 0, 1, 1 },
//        { "Kek", 0, 2, 2 },
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

    /** Toolbar */
    {
        QVector< SFunctionInfo > funcVec;
        {
            auto funcList = m_fbd->getGraph()->getFunctionInfos();
            std::move( funcList.begin(), funcList.end(), std::back_inserter( funcVec ) );
            std::sort( funcVec.begin(), funcVec.end(),
                       [] ( SFunctionInfo & first, SFunctionInfo & second ) -> bool
            {
                return first.funcName < second.funcName;
            });
        }

        for ( auto & info : funcVec )
        {
            QAction * funcAction = new QAction( this );
            funcAction->setObjectName( info.funcName + "_Action" );
            funcAction->setText( info.funcName );
            connect( funcAction, &QAction::triggered,
                     this,
                     [ this, info ] () -> void
            {
                m_fbd->addFunction( info.funcName );
            } );

            ui->toolBar->addAction( funcAction );
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
