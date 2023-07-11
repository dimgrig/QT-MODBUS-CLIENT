#ifndef MODBUSCLIENT_H
#define MODBUSCLIENT_H

#include <QMainWindow>
#include <QModbusDataUnit>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFrame>
#include <QHostAddress>
#include <QObject>
#include <QToolButton>
#include <QVariant>

#include <Logger.h>

QT_BEGIN_NAMESPACE

class QModbusClient;
class QModbusReply;

QT_END_NAMESPACE

enum ModbusConnection {
    Serial,
    Tcp
};

class ModbusClient : public QObject
{
    Q_OBJECT
public:
    ModbusClient(QWidget *parent = nullptr);
    ~ModbusClient();

signals:
    void read_ready(QModbusDataUnit mdu);
public slots:
    //void initActions();
    //QModbusDataUnit read_request() const;
    //QModbusDataUnit write_request() const;
    void on_connect(ModbusConnection = Tcp, QString address = QHostAddress(QHostAddress::LocalHost).toString(),
                    int server_address = 1);

    void on_read(QModbusDataUnit mdu);
    void on_write(QModbusDataUnit mdu);
    void on_read(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries);
    void on_write(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries);

    void on_write(QString reg_name, unsigned int value);
    void on_write(QString reg_name, double value);
private slots:
    void set_connect_type(ModbusConnection connection_type);
    void onModbusStateChanged(int state);

    //QModbusDataUnit read_request(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries) const;
    //QModbusDataUnit write_request(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries) const;

    void on_read_ready();

    void onReadWriteButtonClicked();

    //void onWriteTableChanged(int);

    void on_read_json();

private:
    QModbusReply *lastRequest = nullptr;
    QModbusClient *modbusDevice = nullptr;
    //SettingsDialog *m_settingsDialog = nullptr;
    //WriteRegisterModel *writeModel = nullptr;

    ModbusConnection _connection_type = Tcp;
    int _server_address = 1;
    QString _address = "192.168.1.3:502";


    std::map<QString, QModbusDataUnit> _mdum;
};

#endif // MODBUSCLIENT_H

