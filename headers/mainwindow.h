#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QMetaType>
#include <QStandardItemModel>
#include <QProcess>

#include <uvc_thread.h>

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
	QElapsedTimer timer;
	UVCThread *currentFeed;

public slots:
    void onImageReceived(QImage);

private slots:
	void StartButtonClicked();

};