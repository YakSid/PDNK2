#include "corder.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

COrder::COrder()
{
    //Заполнить стартовые параметры
}

COrder::~COrder() {}

void COrder::saveToJSON(QString filename, const SMainSettings &settings)
{
    auto jDoc = new QJsonDocument();
    auto jObjInsideDoc = jDoc->object();
    //Заполнение данных
    QJsonObject mainSettings;
    mainSettings.insert("locationType", settings.locationType);
    mainSettings.insert("department", settings.department);
    mainSettings.insert("threatLevel", settings.threatLevel);
    mainSettings.insert("departmentPO", settings.departmentPO);
    mainSettings.insert("areVampires", settings.areVampires);
    mainSettings.insert("innerOrderType", settings.innerOrderType);
    QJsonArray jHexTypeArray;
    for (auto hexType : settings.hexagonType) {
        jHexTypeArray.append(hexType);
    }
    mainSettings.insert("hexagonType", jHexTypeArray);
    QJsonArray jHexWelfareArray;
    for (auto hexWelfare : settings.hexagonWelfare) {
        jHexWelfareArray.append(hexWelfare);
    }
    mainSettings.insert("hexagonWelfare", jHexWelfareArray);
    QJsonArray jStaff;
    for (auto staff : settings.staff) {
        jStaff.append(staff);
    }
    mainSettings.insert("staff", jStaff);
    QJsonArray jRes;
    for (auto res : settings.resources) {
        jRes.append(res);
    }
    mainSettings.insert("resources", jRes);
    QJsonArray jStaffReq;
    for (auto req : settings.staffReq) {
        auto jObj = new QJsonObject();
        jObj->insert("count", req.count);
        jObj->insert("req", req.req);
        jStaffReq.append(*jObj);
    }
    mainSettings.insert("staffReq", jStaffReq);
    mainSettings.insert("text", settings.text);
    mainSettings.insert("needToDiscuss", settings.needToDiscuss);

    jObjInsideDoc["mainSettings"] = mainSettings;

    // TODO: СЕЙЧАС загрузка и сохранение
    /*
    QJsonArray jOutcomes;
    for (auto outcome : m_outcomes) {
        auto jOutcome = new QJsonObject();
        jFrag->insert("text", frag->getText());
        jFrag->insert("Ut", frag->isUt());
        jOutcomes.append(*jFrag);
    }
    jObjInsideDoc["outcomes"] = jOutcomes;

    QJsonArray jStages;
    for (auto stage : m_stages) {
        auto jStage = new QJsonObject();
        jFrag->insert("text", frag->getText());
        jFrag->insert("Ut", frag->isUt());
        jStages.append(*jFrag);
    }
    jObjInsideDoc["stages"] = jStages;
    */

    jDoc->setObject(jObjInsideDoc);
    QFile jFile(filename);
    jFile.open(QFile::WriteOnly);
    jFile.write(jDoc->toJson());
    jFile.close();
}

void COrder::loadFromJSON(QString filename)
{
    QFile jFile(filename);
    jFile.open(QFile::ReadOnly);
    auto jDoc = QJsonDocument().fromJson(jFile.readAll());
    jFile.close();
    //Распаковка данных
    QJsonObject jObjInsideDoc = jDoc.object();
    /*QJsonObject mainSettings = jObjInsideDoc["mainSettings"].toObject();
    auto res1 = mainSettings["id"].toString();
    QDate date;
    date.fromString(mainSettings["dateStr"].toString(), "dd.MM.yyyy");
    auto res2 = date;

    QJsonArray jArray = jObjInsideDoc["fragments"].toArray();
    for (QJsonValueRef jValueRef : jArray) {
        auto jFrag = jValueRef.toObject();
        auto frag = new fragment();
        frag->setText(jFrag["text"].toString());
        frag->setUt(jFrag["Ut"].toBool());
        currentKolDog->fragments.append(frag);
    }*/
}

qint32 COrder::addOutcome(qint32 parentId)
{
    auto outcome = new COutcome(_makeMinId(eOutcome));
    outcome->setParentId(parentId);
    m_outcomes.insert(outcome->getId(), outcome);
    return outcome->getId();
}

void COrder::updateOutcome(qint32 id)
{
    // TODO: чуть позже обновить уже при сохранении видимо?
}

qint32 COrder::addStage(qint32 parentId)
{
    auto stage = new CStage(_makeMinId(eStage));
    stage->setParentId(parentId);
    m_stages.insert(stage->getId(), stage);
    return stage->getId();
}

void COrder::updateStage(qint32 id)
{
    //
}

qint32 COrder::getParentId(qint32 id, ENodeType type)
{
    qint32 result;
    if (type == eOutcome) {
        auto it = m_outcomes.find(id);
        result = it.value()->getParentId();
    } else {
        auto it = m_stages.find(id);
        result = it.value()->getParentId();
    }
    return result;
}

qint32 COrder::_makeMinId(ENodeType type)
{
    qint32 minId = 0;
    bool exist;
    if (type == eOutcome) {
        for (int i = 1; true; i++) {
            exist = false;
            for (auto outcome : m_outcomes) {
                if (outcome->getId() == i)
                    exist = true;
            }
            if (!exist) {
                minId = i;
                break;
            }
        }
    } else {
        for (int i = 1; true; i++) {
            exist = false;
            for (auto stage : m_stages) {
                if (stage->getId() == i)
                    exist = true;
            }
            if (!exist) {
                minId = i;
                break;
            }
        }
    }
    return minId;
}
