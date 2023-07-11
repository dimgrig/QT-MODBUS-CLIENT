#include "ModbusClient.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>

#include "Sleeper.h"

void uint_to_usa(unsigned int value, unsigned short *data) {
    unsigned short *ptr = (unsigned short *) &value;
    for (unsigned char i = 0; i < 2; ++i) {
        *data = *ptr;
        data++;
        ptr++;
    }
}

void double_to_usa(double value, unsigned short *data) {
    unsigned short *ptr = (unsigned short *) &value;
    for (unsigned char i = 0; i < 4; ++i) {
        *data = *ptr;
        data++;
        ptr++;
    }
}

double usa_to_double(const unsigned short *data) {
    double result = 0.0;
    unsigned short *ptr = (unsigned short *) &result;
    for (unsigned char i = 0; i < 4; ++i) {
        *ptr = *data;
        data++;
        ptr++;
    }
    return result;
}

ModbusClient::ModbusClient(QWidget *parent)
    : QObject(parent)
{
    this->on_read_json();
}

ModbusClient::~ModbusClient()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void ModbusClient::set_connect_type(ModbusConnection connection_type)
{
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    if (connection_type == Serial) {
#if QT_CONFIG(modbus_serialport)
        modbusDevice = new QModbusRtuSerialMaster(this);
#endif
    } else if (connection_type == Tcp) {
        modbusDevice = new QModbusTcpClient(this);
    }

    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        //statusBar()->showMessage(modbusDevice->errorString(), 5000);
    });

    if (!modbusDevice) {
        //ui->connectButton->setDisabled(true);
        if (connection_type == Serial)
            qCDebug(logDebug) << (tr("Could not create Modbus master."), 5000);
        else
            qCDebug(logDebug) << (tr("Could not create Modbus client."), 5000);
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &ModbusClient::onModbusStateChanged);
    }
}

void ModbusClient::on_connect(ModbusConnection connection_type, QString address, int server_address) {
    _connection_type = connection_type;
    _address = address;
    _server_address = server_address;

    set_connect_type(connection_type);

    if (!modbusDevice)
        return;

    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (_connection_type == Serial) {
//            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
//                ui->portEdit->text());
//#if QT_CONFIG(modbus_serialport)
//            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
//                m_settingsDialog->settings().parity);
//            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
//                m_settingsDialog->settings().baud);
//            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
//                m_settingsDialog->settings().dataBits);
//            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
//                m_settingsDialog->settings().stopBits);
//#endif
        } else {
            const QUrl url = QUrl::fromUserInput(_address);
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        }
        modbusDevice->setTimeout(2000);
        modbusDevice->setNumberOfRetries(3);
        if (!modbusDevice->connectDevice()) {
            qCDebug(logDebug) << tr("Connect failed: ") + modbusDevice->errorString();
        } else {
            //Sleeper sl;
            //sl.sleep(5);
            qCDebug(logDebug) << tr("Connected: ") + _address << modbusDevice->state();
            //ui->actionConnect->setEnabled(false);
            //ui->actionDisconnect->setEnabled(true);
        }
    } else {
        modbusDevice->disconnectDevice();
        //ui->actionConnect->setEnabled(true);
        //ui->actionDisconnect->setEnabled(false);
    }
}

void ModbusClient::onModbusStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    //ui->actionConnect->setEnabled(!connected);
    //ui->actionDisconnect->setEnabled(connected);

    if (state == QModbusDevice::UnconnectedState)
        qCDebug(logDebug) << tr("Disconnect: ") + _address << modbusDevice->state();
        //ui->connectButton->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState) {
        qCDebug(logDebug) << tr("Connect: ") + _address << modbusDevice->state();
        //ui->connectButton->setText(tr("Disconnect"));

        //this->on_read(QModbusDataUnit::Coils, 0, 10);
        //this->on_read(QModbusDataUnit::HoldingRegisters, 0, 10);

        //this->on_write(QModbusDataUnit::Coils, 0, 10);
        //this->on_write(QModbusDataUnit::HoldingRegisters, 0, 10);
    }
}

void ModbusClient::on_read(QModbusDataUnit mdu) {
    if (!modbusDevice)
        return;

    if (auto *reply = modbusDevice->sendReadRequest(mdu, _server_address)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusClient::on_read_ready);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qCDebug(logDebug) << (tr("Read error: ") + modbusDevice->errorString(), 5000);
    }
}

void ModbusClient::on_read(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries) {
    this->on_read(QModbusDataUnit(type, startAddress, numberOfEntries));
}

void ModbusClient::on_read_ready()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
//        for (int i = 0, total = int(unit.valueCount()); i < total; ++i) {
//            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
//                                     .arg(QString::number(unit.value(i),
//                                          unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
//            //ui->readValue->addItem(entry);
//            qCDebug(logDebug) << entry;
//        }
        emit read_ready(unit);
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        qCDebug(logDebug) << (tr("Read response error: %1 (Mobus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
    } else {
        qCDebug(logDebug) << (tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16), 5000);
    }

    reply->deleteLater();
}

void ModbusClient::on_write(QModbusDataUnit mdu) {
    if (!modbusDevice)
        return;
    //statusBar()->clearMessage();

    if (auto *reply = modbusDevice->sendWriteRequest(mdu, _server_address)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    qCDebug(logDebug) << (QString("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                        5000);
                } else if (reply->error() != QModbusDevice::NoError) {
                    qCDebug(logDebug) << (QString("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                reply->deleteLater();
            });
        } else {
            // broadcast replies return immediately
            reply->deleteLater();
        }
    } else {
        qCDebug(logDebug) << (tr("Write error: ") + modbusDevice->errorString(), 5000);
    }
}

void ModbusClient::on_write(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries) {
    this->on_write(QModbusDataUnit(type, startAddress, numberOfEntries));
}

void ModbusClient::on_write(QString reg_name, unsigned int value) {
    if (_mdum.count(reg_name) > 0) {
        QVector<quint16> values(2, 0);
        uint_to_usa(value, &values[0]);
        _mdum[reg_name].setValues(values);
        this->on_write(_mdum.at(reg_name));
    }
}

void ModbusClient::on_write(QString reg_name, double value) {
    if (_mdum.count(reg_name) > 0) {
        QVector<quint16> values(4, 0);
        double_to_usa(value, &values[0]);
        _mdum[reg_name].setValues(values);
        this->on_write(_mdum.at(reg_name));
    }
}

void ModbusClient::onReadWriteButtonClicked()
{
//    if (!modbusDevice)
//        return;
//    //ui->readValue->clear();
//    //statusBar()->clearMessage();

//    QModbusDataUnit writeUnit = write_request(type, startAddress, numberOfEntries);
//    QModbusDataUnit::RegisterType table = writeUnit.registerType();
////    for (int i = 0, total = int(writeUnit.valueCount()); i < total; ++i) {
////        if (table == QModbusDataUnit::Coils)
////            writeUnit.setValue(i, writeModel->m_coils[i + writeUnit.startAddress()]);
////        else
////            writeUnit.setValue(i, writeModel->m_holdingRegisters[i + writeUnit.startAddress()]);
////    }

//    if (auto *reply = modbusDevice->sendReadWriteRequest(read_request(), writeUnit, _server_address)) {
//        if (!reply->isFinished())
//            connect(reply, &QModbusReply::finished, this, &ModbusClient::on_read_ready);
//        else
//            delete reply; // broadcast replies return immediately
//    } else {
//        qCDebug(logDebug) << (tr("Read error: ") + modbusDevice->errorString(), 5000);
//    }
}

//QModbusDataUnit ModbusClient::read_request(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries) const
//{
////    const auto table =
////        static_cast<QModbusDataUnit::RegisterType>(ui->writeTable->currentData().toInt());

//    //int startAddress = ui->readAddress->value();
//    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

//    // do not go beyond 10 entries
//    //quint16 numberOfEntries = qMin(ui->readSize->currentText().toUShort(), quint16(startAddress));
//    return QModbusDataUnit(type, startAddress, numberOfEntries);
//}

//QModbusDataUnit ModbusClient::write_request(QModbusDataUnit::RegisterType type, int startAddress, quint16 numberOfEntries) const
//{
////    const auto table =
////        static_cast<QModbusDataUnit::RegisterType>(ui->writeTable->currentData().toInt());

//    //int startAddress = ui->writeAddress->value();
//    //Q_ASSERT(startAddress >= 0 && startAddress < 10);

//    // do not go beyond 10 entries
//    //quint16 numberOfEntries = qMin(ui->writeSize->currentText().toUShort(), quint16(10 - startAddress));
//    return QModbusDataUnit(type, startAddress, numberOfEntries);
//}

void ModbusClient::on_read_json() {
    std::map<std::string, std::string> map;

    QScopedPointer<QFile> mapFile(new QFile(":json/json/modbus.json"));
    //QScopedPointer<QFile> mapFile(new QFile("C:/QtCreatorProjects/NC3-MMB/modbus.json"));
    mapFile.data()->open(QIODevice::ReadOnly);
    QString val = mapFile.data()->readAll();
    mapFile.data()->close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    //QJsonObject jObject = doc.object();
    QJsonArray json_array = doc.array();
    //qCDebug(logDebug()) << doc.isArray() << json_array.size();

    //QVariantMap dataMap = (((mainMap[(index + "_map").c_str()].toMap())["mappings"].toMap())[index.c_str()].toMap())["properties"].toMap();
    //QVariantMap dataMap = mainMap;

//    for(QVariantMap::const_iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
//        qCDebug(logDebug()) << it.key() << it.value();
//        //map[it.key().toStdString()] = ((it.value().toMap())["type"]).toString().toStdString();
//    }

//    foreach (const QJsonValue & value, json_array) {
//        QJsonObject obj = value.toObject();
//        qCDebug(logDebug()) << obj["RegisterName"].toString() << obj["ObjName"].toString() << obj["Address"].toInt() << obj["DataLength"].toInt();
//    }

    QString name;

    foreach (const QJsonValue & value, json_array) {
        QJsonObject obj = value.toObject();

        QString reg_name = obj["RegisterName"].toString();
        QString obj_name = obj["ObjName"].toString();
        int address = obj["Address"].toInt();
        quint16 size = obj["DataLength"].toInt();

        QModbusDataUnit::RegisterType register_type = QModbusDataUnit::RegisterType::Invalid;
        if (obj_name == "COIL") {
            register_type = QModbusDataUnit::RegisterType::Coils;
        } else if (obj_name == "INPUT") {
            register_type = QModbusDataUnit::RegisterType::InputRegisters;
        } else if (obj_name == "HOLDING") {
            register_type = QModbusDataUnit::RegisterType::HoldingRegisters;
        } else if (obj_name == "DISCRETEINPUT") {
            register_type = QModbusDataUnit::RegisterType::DiscreteInputs;
        }

        QModbusDataUnit mdu(register_type, address - 1, size);

        _mdum.emplace(reg_name, mdu);

    }

//    qCDebug(logDebug()) << _mdum.size();
//    for (const auto &value : _mdum) {
//        qCDebug(logDebug()) << value.first << value.second.startAddress() << value.second.valueCount();
//    }

    auto _mdum_copy = _mdum;
    _mdum.clear();

    //filter complex regs
    for (auto &value : _mdum_copy) {
        if ((value.first.end() - 1)->isDigit()) {
            QStringList sl = value.first.split("_");
            sl.pop_back();
            QString reg_name = sl.join("_");
            if (_mdum.count(reg_name) > 0) {
                QModbusDataUnit mdu = _mdum.at(reg_name);
                mdu.setValueCount(mdu.valueCount() + value.second.valueCount());
                _mdum[reg_name] = mdu;
            } else {
                _mdum[reg_name] = value.second;
            }
        } else {
            _mdum[value.first] = value.second;
        }
    }

    qCDebug(logDebug()) << _mdum.size();
    for (const auto &value : _mdum) {
        qCDebug(logDebug()) << value.first << value.second.startAddress() << value.second.valueCount();
    }

    //return map;
}
