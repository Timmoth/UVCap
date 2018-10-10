#include <mainwindow.h>
#include <source/forms/ui_mainwindow.h>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow){
	
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}