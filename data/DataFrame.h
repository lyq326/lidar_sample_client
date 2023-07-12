#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <QtCore>
#include <QByteArray>
#include <QMap>
#include <QString>
#include <QVector>
#include <QTime>
#include <QObject>
#include <QDebug>
#include <stdint.h>
#include <QJsonObject> // { }
#include <QJsonArray> // [ ]
#include <QJsonDocument> // 解析Json
#include <QJsonValue> // int float double bool null { } [ ]
#include <QJsonParseError>

class DataFrame: public QObject {

Q_OBJECT

private:
    // QVector<QByteArray> bytesDataStorage;
    QByteArray bytesFrameRaw;

#if FIX_DATA_FRAME
    struct DataFrameStuc
    {
        quint8 SOF[2];
        quint16 frameSequence;

        qreal volRef;
        qreal volChna;
        qreal volChnb;
        qreal volDiff;

        QTime time_info;
        // uint8_t __unused[2];
        qreal temperature;

        qreal pressure;
        // uint32_t humidity;

        qreal cn2;

        quint32 len;
        quint32 check_sum;

    };
    DataFrameStuc frameStuc;
    DataFrameStuc toFrameStuc();


    DataFrame* fromFrameStuc(DataFrameStuc stuc);
#endif


    union DataSectionData
    {
        uint8_t _b[4];
        int32_t _d;
        float _f;
        char _h[4];
        
        // quint64;
    };

    QVector<bool> _dataSecUsed;
public:
    class DataSectionValue{
    public:
        // DataSectionValue(QString name, QMetaType::Type t, quint32 offset);
        DataSectionValue(QString name = "Section", QMetaType::Type t = QMetaType::UnknownType, quint32 offset = 0);

    public:
        quint32 _index;
        quint32 _length;
        quint32 _offset = 0;
        QString _name = "";
        QMetaType::Type _type;
        DataSectionData _data;
        QChar _data_char;
        QString _data_str;
        QByteArray _data_hex;

        const QString toString() const;
        const QJsonObject toJsonObj() const;
        void fromRaw(const QByteArray& );
        void fromStr(const QString& str);
        friend inline QDebug operator<<(QDebug out, const DataSectionValue& val)
        {
            out <<" DataSectionInfo("<<val._name<<QMetaType::typeName(val._type)<<val.toString()<<val._offset<<"---"<<val._offset+QMetaType::sizeOf(val._type)<<")";
            return out;
        }
    };

    using DataSectionsType = QMap<qint32, DataFrame::DataSectionValue>;

    DataSectionsType _dataSections;

    void addDataSection(QString name, QMetaType::Type t, quint32 offset);
    DataFrame* fromFrameRaw(const QByteArray& raw);
    QByteArray toFrameRaw();
    DataFrame* fromFrameStr(const QString& str);
    const QJsonObject toJsonObj() const;

    DataSectionsType& dataSections();
    void setDataSections();

signals:
    void newSectionsVaild(const DataSectionsType &s);
};



#endif