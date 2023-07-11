#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <locale.h>

const int StatusTimeout = AQP::MSecPerSecond * 10;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //qCDebug(logDebug()) << this->ui->dw_batch_parameters->height();

    TestRunner tr;
    RUN_TEST(tr, FunctionsTest);
    RUN_TEST(tr, MacrosesTest);

    this->setFont(QFont("Noto Sans", 10)); //10
    //qCDebug(logDebug()) << this->font();

//    QLocale locale = QLocale::system(); // current locale
//    QLocale russian(QLocale::Russian);
//    locale.setNumberOptions(russian.numberOptions()); // borrow number options from the Russian locale
//    QLocale::setDefault(locale); // set as default
    QLocale::setDefault(QLocale::Russian);
    QLocale::setDefault(QLocale::Russian);
    QLocale::setDefault(QLocale::Russian);
    QLocale locale = QLocale::system(); // current locale
    //if (locale.name() != "ru_RU") {
//        qCDebug(logDebug()) << std::locale().name() << locale.name() << QLocale().name();
//        AQP::warning(this, "Ошибка", "Ошибка установки локали. Выполнить sudo dpkg-reconfigure locales и выбрать локаль ru_RU");
//        exit(1);
//    }

//    qCDebug(logDebug()) << std::locale().name() << locale.name();
//    setlocale(LC_ALL, "ru_RU");
//    //std::locale::global(std::locale(std::locale("ru_RU"), "C", std::locale::numeric));
    //qCDebug(logDebug()) << std::locale().name() << locale.name();

    this->setAttribute(Qt::WA_AlwaysShowToolTips);

    if (QFile(QSS).exists()) {
        QScopedPointer<QFile> styleFile(new QFile(QSS));
        styleFile.data()->open(QFile::ReadOnly);
        QString qssStr = styleFile.data()->readAll();
        styleFile.data()->close();
        this->setStyleSheet(qssStr);
    } else {
        qCDebug(logWarning()) << "style file does not exist";
    }

    //this->ui->centralwidget->hide();

    //this->ui->pb_test->setVisible(false);
    //this->ui->pb_test_oscillogram->setVisible(false);
    //this->ui->pb_quit->setVisible(false);

    //setWindowTitle(QString("%1[*]").arg(QApplication::applicationDisplayName()));
    setWindowTitle(QString("[*]"));

    //this->about_panel->hide();

    //this->installEventFilter(this);

    this->docks();
    this->mvc();

    this->menu_bar();
    this->tool_bar();

    this->connect_signal_slot();




//    this->ui->cb_language->addItems(QStringList() << "ru_RU" << "en_US");

//    connect(this->ui->cb_language, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
//           [=](const QString &str){
//       qtLanguageTranslator.load("QtLanguage_" + str, QCoreApplication::applicationDirPath());   // Загружаем перевод
//       qApp->installTranslator(&qtLanguageTranslator);        // Устанавливаем перевод в приложение
//    });

    connect(this->ui->tb_connect, &QToolButton::click, [=](){
        ModbusConnection connection_type = static_cast<ModbusConnection>(this->ui->cb_connection_type->currentIndex());
        QString address = this->ui->le_address->text();
        int server_address = this->ui->sb_server_address->value();
        modbus_client->on_connect(connection_type, address, server_address);
    });

    qtLanguageTranslator.load(QString("QtLanguage_") + QString("en_US"), QCoreApplication::applicationDirPath());
    qApp->installTranslator(&qtLanguageTranslator);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (okToClearData())
        event->accept();
    else
        event->ignore();

    this->_saveGeometry();

    QMainWindow::closeEvent(event);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    Q_UNUSED(target);
    Q_UNUSED(event);
//    if (event->type() == QEvent::Shortcut) {
//        // If I care about this shortcut, then return true to intercept
//        // Else, return false to let the application process it
//        if (!cfg.ENABLE_SHORTCUTS) {
//            qCDebug(logDebug()) << "SHORTCUT IGNORES";
//            return true;
//        }
//        qCDebug(logDebug()) << "SHORTCUT RUNS";
//    }

    return false;
}

void MainWindow::connect_signal_slot() {
    connect(this->timer, &QTimer::timeout, this, &MainWindow::on_timer);
    this->timer->start(1000);

    connect(this->modbus_client, &ModbusClient::read_ready, this, &MainWindow::modbus_read);

}

void MainWindow::menu_bar() {

}

void MainWindow::tool_bar() {

}

void MainWindow::auth_users() {

}

void MainWindow::init_auth_users() {

}

void MainWindow::mvc() {

}

void MainWindow::docks() {

}

void MainWindow::_saveGeometry() {

}

void MainWindow::test() {

}

bool MainWindow::okToClearData()
{
//    if (isWindowModified())
//        return AQP::okToClearData(&MainWindow::save_as, this, tr("Несохраненные изменения"), tr("Тестовая карта содержит несохраненные изменения. Сохранить изменения?"));
    return true;
}

void MainWindow::auth_ok() {

}

void MainWindow::on_timer() {
    //this->modbus_client->on_read(QModbusDataUnit::RegisterType::HoldingRegisters, MODBUS_UPDATE_START_INDEX, MODBUS_UPDATE_CNT);
}

void MainWindow::modbus_read(QModbusDataUnit mdu) {

}


