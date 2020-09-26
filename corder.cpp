#include "corder.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QtDebug>

COrder::COrder()
{
    // TODO: Заполнить стартовые параметры?
}

COrder::~COrder() {}

void COrder::saveToJSON(QString filename, const SMainSettings &settings)
{
    auto jDoc = new QJsonDocument();
    auto jObjInsideDoc = jDoc->object();
    //Основные настройки
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
    //Исходы
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
    //Этапы
    QJsonArray jStages;
    for (auto stage : m_stages) {
        auto jStage = new QJsonObject();
        jStage->insert("id", stage->getId());
        jStage->insert("parentId", stage->getParentId());
        jStage->insert("time", stage->getTime());
        jStage->insert("text", stage->getText());
        jStage->insert("final", stage->isFinal());
        auto rewards = *stage->getRewards();
        QJsonArray jRewards;
        for (auto reward : rewards) {
            auto jReward = new QJsonObject();
            jReward->insert("type", reward->type);
            jReward->insert("object", reward->object);
            jReward->insert("count", reward->count);
            jReward->insert("psyState", reward->psyState);
            jRewards.append(*jReward);
        }
        jStage->insert("rewards", jRewards);

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

SMainSettings COrder::loadFromJSON(QString filename)
{
    QFile jFile(filename);
    jFile.open(QFile::ReadOnly);
    auto jDoc = QJsonDocument().fromJson(jFile.readAll());
    jFile.close();
    //Основные настройки
    SMainSettings settings;
    QJsonObject jObjInsideDoc = jDoc.object();
    QJsonObject jMainSettings = jObjInsideDoc["mainSettings"].toObject();
    settings.locationType = jMainSettings["locationType"].toInt();
    settings.department = jMainSettings["department"].toInt();
    settings.threatLevel = jMainSettings["threatLevel"].toInt();
    settings.departmentPO = jMainSettings["departmentPO"].toInt();
    settings.areVampires = jMainSettings["areVampires"].toInt();
    settings.innerOrderType = jMainSettings["innerOrderType"].toInt();
    QJsonArray jHexTypeArray = jMainSettings["hexagonType"].toArray();
    settings.hexagonType.clear();
    for (auto val : jHexTypeArray) {
        settings.hexagonType.append(val.toBool());
    }
    QJsonArray jHexWelfare = jMainSettings["hexagonWelfare"].toArray();
    settings.hexagonWelfare.clear();
    for (auto val : jHexWelfare) {
        settings.hexagonWelfare.append(val.toBool());
    }
    QJsonArray jStaff = jMainSettings["staff"].toArray();
    settings.staff.clear();
    for (auto val : jStaff) {
        settings.staff.append(val.toInt());
    }
    QJsonArray jRes = jMainSettings["resources"].toArray();
    settings.resources.clear();
    for (auto val : jRes) {
        settings.resources.append(val.toInt());
    }
    QJsonArray jStaffReq = jMainSettings["staffReq"].toArray();
    settings.staffReq.clear();
    for (auto val : jStaffReq) {
        auto obj = val.toObject();
        auto req = new SReqToStaff;
        req->count = obj["count"].toInt();
        req->req = obj["req"].toInt();
        settings.staffReq.append(*req);
    }
    settings.text = jMainSettings["text"].toString();
    settings.needToDiscuss = jMainSettings["needToDiscuss"].toString();

    //Исходы
    QJsonArray jOutcomes = jObjInsideDoc["outcomes"].toArray();
    for (auto jRefOutcome : jOutcomes) {
        auto jOutcome = jRefOutcome.toObject();
        auto outcome = new COutcome(jOutcome["id"].toInt());
        outcome->setParentId(jOutcome["parentId"].toInt());
        QList<SCheck *> checks;
        auto jChecks = jOutcome["checks"].toArray();
        for (auto jRefCheck : jChecks) {
            auto jCheck = jRefCheck.toObject();
            auto check = new SCheck();
            check->type = jCheck["type"].toInt();
            check->trait = jCheck["trait"].toInt();
            QJsonArray jSpinValues = jCheck["spinValues"].toArray();
            check->spinValues.clear();
            for (auto val : jSpinValues) {
                check->spinValues.append(val.toInt());
            }
            QJsonArray jStagesId = jCheck["stagesId"].toArray();
            check->stagesId.clear();
            while (!jStagesId.isEmpty()) {
                qint32 firstVal = jStagesId.first().toInt();
                jStagesId.removeFirst();
                qint32 secondVal = jStagesId.first().toInt();
                jStagesId.removeFirst();
                check->stagesId.insert(COutcomeWidget::EOutcomeButton(firstVal), secondVal);
            }
            checks.append(check);
        }
        outcome->update(checks);
        m_outcomes.insert(outcome->getId(), outcome);
    }

    //Этапы
    QJsonArray jStages = jObjInsideDoc["stages"].toArray();
    for (auto jRefStage : jStages) {
        auto jStage = jRefStage.toObject();
        auto stage = new CStage(jStage["id"].toInt());
        stage->setParentId(jStage["parentId"].toInt());
        stage->setTime(jStage["time"].toInt());
        stage->setText(jStage["text"].toString());
        stage->setFinal(jStage["final"].toBool());
        QList<SReward *> rewards;
        auto jRewards = jStage["rewards"].toArray();
        for (auto jRefReward : jRewards) {
            auto jReward = jRefReward.toObject();
            auto reward = new SReward();
            reward->type = jReward["type"].toInt();
            reward->object = jReward["object"].toInt();
            reward->count = jReward["count"].toInt();
            reward->psyState = jReward["psyState"].toInt();
            rewards.append(reward);
        }
        stage->setRewards(rewards);
        QList<SVariant *> variants;
        auto jVariants = jStage["variants"].toArray();
        for (auto jRefVariant : jVariants) {
            auto jVariant = jRefVariant.toObject();
            auto variant = new SVariant();
            variant->text = jVariant["text"].toString();
            variant->outcomeId = jVariant["outcomeId"].toInt();
            variant->resource = jVariant["resource"].toInt();
            variant->resourceCount = jVariant["resourceCount"].toInt();
            variants.append(variant);
        }
        stage->setVariants(variants);
        m_stages.insert(stage->getId(), stage);
    }

    return settings;
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

const QList<SReward *> *COrder::getStageRewards(qint32 id)
{
    const QList<SReward *> *result = nullptr;
    auto it = m_stages.find(id);
    if (it != m_stages.end())
        result = it.value()->getRewards();
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

QList<qint32> COrder::getChildrenId(qint32 id, ENodeType type)
{
    QList<qint32> result;
    if (type == eOutcome) {
        for (auto outcome : m_outcomes) {
            if (outcome->getParentId() == id)
                result.append(outcome->getId());
        }
    } else {
        for (auto stage : m_outcomes) {
            if (stage->getParentId() == id)
                result.append(stage->getId());
        }
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
