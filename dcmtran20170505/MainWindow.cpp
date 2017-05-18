#include "MainWindow.h"
#include <QtWidgets>
#include "ui_MainWindow.h"
#include "WorkThread.h"
#include "Settings.h"


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), m_worker(nullptr)
{
	ui->setupUi(this);

	auto s = Settings();
	ui->lineEdit_Host->setText(s.host());
	ui->lineEdit_AETitle->setText(s.title());
	ui->lineEdit_Port->setText(s.portStr());
	ui->lineEdit_Directory->setText(s.directory());

	ui->progressBar->setRange(0, 100);
	ui->progressBar->setValue(0);

	connect(ui->btnBrowse, SIGNAL(clicked()), SLOT(DoBrowse()));
	connect(ui->btnUpload, SIGNAL(clicked()), SLOT(DoUpload()));
	connect(this, SIGNAL(OnUpdateList()), this, SLOT(DoUpdateList()));
}


MainWindow::~MainWindow()
{
	if (m_worker) {
		m_worker->terminate();
	}
}


void
MainWindow::DoBrowse()
{
	QFileDialog dlg{this, tr("Browse Directory")};
	dlg.setFileMode(QFileDialog::Directory);
	dlg.setOption(QFileDialog::ShowDirsOnly);

	if (dlg.exec() == QFileDialog::Accepted) {
		auto list = dlg.selectedFiles();
		ui->lineEdit_Directory->setText(list.first());
	}
}


void
MainWindow::DoUpload()
{
	auto host = ui->lineEdit_Host->text();
	if (host.isEmpty()) return;

	auto title = ui->lineEdit_AETitle->text();
	if (title.isEmpty()) return;

	auto port_str = ui->lineEdit_Port->text();
	if (port_str.isEmpty()) return;
	bool ok = false;
	auto port = port_str.toInt(&ok);
	if (!ok || port <= 0 || port >= 65535) return;

	auto directory = ui->lineEdit_Directory->text();
	if (directory.isEmpty()) return;

	if (m_worker) {
		m_worker->terminate();
	}

	m_worker.reset(new WorkThread(host, title, port, directory));
	connect(m_worker.data(), SIGNAL(OnStart(int)), SLOT(DoStart(int)));
	connect(m_worker.data(), SIGNAL(OnTransfer(QString, QString)),
			SLOT(DoTransfer(QString, QString)));
	connect(m_worker.data(), SIGNAL(OnFinish()), SLOT(DoFinish()));
	m_worker->start();

	auto s = Settings();
	s.setHost(host);
	s.setTitle(title);
	s.setPort(port);
	s.setDirectory(directory);
}


void
MainWindow::DoStart(int numFiles)
{
	m_updateTimer = startTimer(500);
	m_cachedList.clear();
	ui->listWidget->clear();
	ui->listWidget->addItem(tr("START TRANSFER..."));
	ui->progressBar->reset();
	ui->progressBar->setMaximum(numFiles);
	ui->btnUpload->setEnabled(false);
}


void
MainWindow::DoTransfer(QString file, QString result)
{
	m_cachedList.append(tr("%1   -->   %2").arg(file, result));
	ui->progressBar->setValue(ui->progressBar->value() + 1);
}


void
MainWindow::DoFinish()
{
	killTimer(m_updateTimer);
	DoUpdateList();
	ui->listWidget->addItem(tr("TRANSFER FINISHED."));
	ui->listWidget->scrollToBottom();
	ui->progressBar->setValue(ui->progressBar->maximum());
	ui->btnUpload->setEnabled(true);

	if (m_worker) {
		m_worker->wait();
		m_worker.reset();
	}
}


void
MainWindow::timerEvent(QTimerEvent*)
{
	if (m_cachedList.empty()) return;
	emit OnUpdateList();
}


void
MainWindow::DoUpdateList()
{
	for (auto item : m_cachedList) {
		ui->listWidget->addItem(item);
	}
	m_cachedList.clear();
	ui->listWidget->scrollToBottom();
}
