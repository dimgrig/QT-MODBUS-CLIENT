#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTableView>


#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTableView>
#include <QTimer>

#include <iostream>

#include "aqp.h"
#include "icons.h"
#include "Logger.h"
#include "macroses.h"
#include "templates.h"
#include "Sleeper.h"
#include "TestRunner.h"

#include <QTranslator>
#include <QLibraryInfo>

#include "ModbusClient.h"

//using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init_auth_users();
signals:
    void return_pressed();
protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;
public slots:

private slots:
    void setDirty() { setWindowModified(true); }

    void auth_ok();
    void on_timer();
    void modbus_read(QModbusDataUnit mdu);

private:
    void connect_signal_slot();
    void menu_bar();
    void tool_bar();
    void mvc();
    void docks();
    void auth_users();
    void _saveGeometry();
    void restore_geometry();

    void test();

    bool okToClearData();

    Ui::MainWindow *ui;

    QTimer *timer = new QTimer(this);

    ModbusClient *modbus_client = new ModbusClient(this);

    QTranslator qtLanguageTranslator;
};
#endif // MAINWINDOW_H
