#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class WorkThread;


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	~MainWindow();
	explicit MainWindow(QWidget* parent = 0);

private:
	QScopedPointer<Ui::MainWindow> ui;
	QScopedPointer<WorkThread> m_worker;

	int m_updateTimer;
	QStringList m_cachedList;
	void timerEvent(QTimerEvent* event);

signals:
	void OnUpdateList();

private slots:
	void DoBrowse();
	void DoUpload();
	void DoStart(int numFiles);
	void DoTransfer(QString file, QString result);
	void DoFinish();
	void DoUpdateList();
};


#endif // MAINWINDOW_H
