﻿#include "syncmanager.h"


DocumentValue toBson(QJsonObject&obj)
{
    /*

    将json转换成string，再将string转换成bson
    */
    if(obj.empty())
        return DocumentValue({});
    QJsonDocument doc(obj);
    QByteArray array=doc.toJson(QJsonDocument::Indented);
    std::string json=array.data();
    DocumentValue value=bsoncxx::from_json(json);
    return value;
}
QJsonObject toJson(DocumentValue&value)
{
    if(value.view().empty())
        return QJsonObject();
    std::string json=bsoncxx::to_json(value);

    qDebug()<<QString::fromStdString(json);
    QByteArray array=json.data();
    QJsonDocument doc=QJsonDocument::fromJson(array);
    QJsonObject obj=doc.object();
    return obj;
}
void syncManager::syncHistoryFromLocal(int uid)
{
    //打开用户的历史记录文件
    QString file_path=QString("./history/%1.json").arg(uid);
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);

    //获取到用户的历史记录
    QJsonParseError error;
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll(),&error);
    QJsonObject obj=doc.object();

    //将json格式的记录，转换为bson格式
    DocumentValue value=toBson(obj);

    file.close();

    qDebug()<<QString::fromStdString(bsoncxx::to_json(value));

    //和数据库建立连接
    Instance inst{};
    Uri uri("mongodb://175.178.155.66:27017");
    Client client(uri);

    Database db=client["browser"];

    Collection coll=db["history"];



    //判断用户是否存在

    /*
        1.构造过滤器
        2.查询结果
        3.如果不存在该用户，则直接insert
        4.如果存在该用户，则直接修改
    */

    DocumentStreamBuilder doc_builder;
    auto result=coll.find_one(DocumentStreamBuilder{}<<"uid"<<uid<<finalize);

    if(!result)
        coll.insert_one(value.view());
    else
    {
        coll.find_one_and_update(DocumentStreamBuilder{}<<"uid"<<uid<<finalize,value.view());
    }

}
void syncManager::syncHistoryToLocal(int uid)
{
    //建立数据库的连接
    Instance inst={};
    Uri uri("mongodb://175.178.155.66:27017");
    Client client(uri);


    Database db=client["browser"];
    Collection coll=db["history"];
    //获取到指定用户的数据

    auto result=coll.find_one(DocumentStreamBuilder{}<<"_id"<<uid<<finalize);


    if(!result) return;

    DocumentValue value=result.value();

    qDebug()<<QString::fromStdString(bsoncxx::to_json(value));

    //数据转换成json格式
    QJsonObject obj=toJson(value);

    //打开json文件，并写回
    QString file_path=QString("./history/%1.json").arg(uid);
    QFile file(file_path);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);

    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray data=doc.toJson();
    file.write(data);
    file.close();






}

