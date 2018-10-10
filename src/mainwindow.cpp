#include <mainwindow.h>
#include <source/forms/ui_mainwindow.h>

void MainWindow::StartButtonClicked(){
  timer.start();
  currentFeed->start(QThread::HighestPriority);
  currentFeed->running = true;

}

void MainWindow::onImageReceived(QImage img)
{
  ui->label1->setPixmap(QPixmap::fromImage(img));
  std::cout << (1000 / (float)timer.elapsed()) << std::endl;
  timer.restart();
}

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow){
	ui->setupUi(this);

  currentFeed = new UVCThread(this);
  connect(
    currentFeed, 
    SIGNAL(captureImage(QImage)), 
    this, 
    SLOT(onImageReceived(QImage)));

  connect(
    ui->MainWindow_Button_Start,
    &QPushButton::clicked,
    this,
    &MainWindow::StartButtonClicked);
}

MainWindow::~MainWindow() {
	delete ui;
}