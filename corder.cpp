#include "corder.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QtDebug>

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

    QJsonArray jOutcomes;
    for (auto outcome : m_outcomes) {
        auto jOutcome = new QJsonObject();
        jOutcome->insert("id", outcome->getId());
        jOutcome->insert("parentId", outcome->getParentId());
        auto checks = *outcome->getChecks();
        QJsonArray jChecks;
        for (auto check : checks) {
            auto jCheck = new QJsonObject();
            jCheck->insert("type", check->type);
            jCheck->insert("trait", check->trait);
            QJsonArray jSpinValues;
            for (auto spinValue : check->spinValues) {
                jSpinValues.append(spinValue);
            }
            jCheck->insert("spinValues", jSpinValues);
            // Записываю по порядку "номер кнопки", затем сразу "айди", типа: 0,15,1,16,2,46,3,134
            QJsonArray jStagesId;
            for (auto it = check->stagesId.begin(); it != check->stagesId.end(); it++) {
                jStagesId.append(qint32(it.key()));
                jStagesId.append(it.value());
            }
            jCheck->insert("stagesId", jStagesId);

            jChecks.append(*jCheck);
        }
        jOutcome->insert("checks", jChecks);
        jOutcomes.append(*jOutcome);
    }
    jObjInsideDoc["outcomes"] = jOutcomes;

    // TODO: СЕЙЧАС добавить инфу стейджа и награды
    QJsonArray jStages;
    for (auto stage : m_stages) {
        auto jStage = new QJsonObject();
        jStage->insert("id", stage->getId());
        jStage->insert("parentId", stage->getParentId());
        auto variants = *stage->getVariants();
        QJsonArray jVariants;
        for (auto variant : variants) {
            auto jVariant = new QJsonObject();
            jVariant->insert("text", variant->text);
            jVariant->insert("outcomeId", variant->outcomeId);
            jVariant->insert("resource", variant->resource);
            jVariant->insert("resourceCount", variant->resourceCount);

            jVariants.append(*jVariant);
        }
        jStage->insert("variants", jVariants);
        jStages.append(*jStage);
    }
    jObjInsideDoc["stages"] = jStages;

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

void COrder::updateOutcome(qint32 id, const QList<SCheck *> &checks)
{
    auto it = m_outcomes.find(id);
    if (it != m_outcomes.end())
        it.value()->update(checks);
}

const QList<SCheck *> *COrder::getOutcomeChecks(qint32 outcomeId)
{
    auto it = m_outcomes.find(outcomeId);
    if (it != m_outcomes.end())
        return it.value()->getChecks();
    return nullptr;
}

qint32 COrder::addStage(qint32 parentId)
{
    auto stage = new CStage(_makeMinId(eStage));
    stage->setParentId(parentId);
    m_stages.insert(stage->getId(), stage);
    return stage->getId();
}

void COrder::updateStage(qint32 id, const QList<SVariant *> &variants, qint32 time, QString text,
                         const QList<SReward *> rewards)
{
    auto it = m_stages.find(id);
    if (it != m_stages.end())
        it.value()->updateInfo(variants, time, text, rewards);
}

const SStageInfo COrder::getStageInfo(qint32 id)
{
    SStageInfo result;
    auto it = m_stages.find(id);
    if (it != m_stages.end())
        result = it.value()->getStageInfo();
    return result;
}

void COrder::setStageFinal(qint32 id, bool final)
{
    auto it = m_stages.find(id);
    if (it != m_stages.end())
        it.value()->setFinal(final);
}

const QList<SVariant *> *COrder::getStageVariants(qint32 stageId)
{
    auto it = m_stages.find(stageId);
    if (it != m_stages.end())
        return it.value()->getVariants();
    return nullptr;
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
        for (int i = 0; true; i++) {
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
