#pragma once

#include <QMainWindow>
#include <QDebug>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;

};