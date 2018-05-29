#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <urho3dwidget.h>
#include <Urho3D/Core/Context.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    context = new Urho3D::Context();
    w=new Urho3dWidget(context.Get (),this) ;
    w->Setup ();
    w->Start ();
    
    setCentralWidget (w);
}

MainWindow::~MainWindow()
{
    delete w;
    delete ui; 
}
