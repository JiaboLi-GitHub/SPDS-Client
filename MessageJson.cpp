#include "MessageJson.h"
#include<qstring.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>

QByteArray MessageJson::verificationDataToQByteArrey(QString mailAddress)
{
    QJsonObject verificationData_json;
    verificationData_json.insert("RequestType", TcpData::RequestType::VerificationCode);
    QJsonObject data_json;
    data_json.insert("mailAddress", mailAddress);
    verificationData_json.insert("data", data_json);
    QJsonDocument document = QJsonDocument::QJsonDocument(verificationData_json);
    QByteArray requestCode_byteArray = document.toBinaryData();
    return requestCode_byteArray;
}
