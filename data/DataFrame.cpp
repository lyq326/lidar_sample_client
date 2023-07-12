// #include "data/DataFrame.h"
#include "DataFrame.h"


QByteArray DataFrame::toFrameRaw()
{
    return bytesFrameRaw;
}

void DataFrame::addDataSection(QString name, QMetaType::Type t, quint32 offset){
    DataSectionValue* v = new DataSectionValue(name, t, offset);
    this->_dataSections.insert(v->_index, std::move(*v));
    // qDebug()<<offset<<"&"<<length<<"->"<<index;
}


DataFrame* DataFrame::fromFrameRaw(const QByteArray& raw){
    bool ok;
    QByteArray b;
    // QString str;
    this->bytesFrameRaw = raw;

    if(this->_dataSections.size() == 0){
        return this;
    }
    // for(auto k : this->_dataSections.keys())
    for(auto& val : this->_dataSections)
    {
        // auto& val = _dataSections[k];
        b.clear();
        // str.clear();

        b = raw.mid(val._offset, val._length);
        // qDebug()<<b;
        // qDebug()<<b.toUInt(&ok, 16);
        std::reverse(b.begin(), b.end());
        // qDebug()<<b.toUInt(&ok, 16);
        val.fromRaw(b);
        // qDebug()<<val;

    }
    emit newSectionsVaild(dataSections());
    return this;
}

DataFrame* DataFrame::fromFrameStr(const QString& str)
{
    return nullptr;
}

const QJsonObject DataFrame::toJsonObj() const
{
    QJsonObject secs;
    QJsonObject raw;
    QJsonArray secs_val;

    for(auto& val : this->_dataSections)
    {
        // secs_val.append(val.toJsonObj());
        // secs.insert(val._name, val.toJsonObj());
        secs.insert(val._name, QJsonValue(val.toString()));
    }
    // secs.insert("name", )
    // secs.insert("secion", secs_val);
    // qDebug()<<secs;
    return secs;
}

DataFrame::DataSectionsType& DataFrame::dataSections(){
    return this->_dataSections;
}

DataFrame::DataSectionValue::DataSectionValue(QString name, QMetaType::Type t, quint32 offset)
    : _name(name) ,_type(t), _offset(offset) {
    if(_name == ""){
        _name = QMetaType::typeName(_type);
    }
    if(_type == QMetaType::QString){
        _length = 4;
    }
    else{
        _length = QMetaType::sizeOf(_type);
    }
    _index = (offset<<8)+_length;
};

void DataFrame::DataSectionValue::fromStr(const QString& str){
    bool ok;

    if((this->_type == QMetaType::SChar)||(this->_type == QMetaType::UChar)){
        // this->_data._d = (qint32)(raw[this->_offset]);
        (this->_data._b)[0] = (str.toLatin1().data())[0];;
    }
    else if(this->_type == QMetaType::QString){
        this->_data_str = str;
    }
    else if(this->_type == QMetaType::UShort){
        this->_data._d = (qint32)str.toUShort(&ok, 16);
    }
    else if(this->_type == QMetaType::Short){
        this->_data._d = (qint32)str.toShort(&ok, 16);
    }
    else if(this->_type == QMetaType::UInt){
        this->_data._d = (qint32)str.toUInt(&ok, 16);
    }
    else if(this->_type == QMetaType::Int){
        this->_data._d = (qint32)str.toInt(&ok, 16);
        // qDebug()<<this->_data._d;
    }
    else if(this->_type == QMetaType::Float){
        this->_data._d = str.toUInt(&ok, 16);
    }
    // qDebug()<<this;
}

void DataFrame::DataSectionValue::fromRaw(const QByteArray& raw){
    fromStr(raw.toHex());
}

const QString DataFrame::DataSectionValue::toString() const {
    switch (this->_type)
    {
        case QMetaType::QChar:
            return QString(_data_char);
        case QMetaType::QString:
            return QString(_data_str);
        case QMetaType::QByteArray:
            return _data_hex.toHex();
        case QMetaType::SChar:
        case QMetaType::UChar:
        case QMetaType::Short:
        case QMetaType::UShort:
        case QMetaType::Int:
        case QMetaType::UInt:
            return QString::number(this->_data._d);
        case QMetaType::Float:
            return QString::number(this->_data._f);
        // case hex_8b:
        //     // return  (this->data._h);
        //     return;
                
        // default:
        //     return ;
    }
}

const QJsonObject DataFrame::DataSectionValue::toJsonObj() const
{
    QJsonObject sec_val;
    sec_val.insert("o", QJsonValue((qint64)_offset));
    sec_val.insert("l", QJsonValue((qint64)_length));
    sec_val.insert("t", QJsonValue(QMetaType::typeName(_type)));
    sec_val.insert("v", QJsonValue(this->toString()));

    return sec_val;
}

std::ostream& operator<<(std::ostream& out, const DataFrame::DataSectionValue& m)
{
    // out << "name: " <<  m.name << ", age: " << m.age;
}