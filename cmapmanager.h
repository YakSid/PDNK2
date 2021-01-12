#ifndef CMAPMANAGER_H
#define CMAPMANAGER_H

/** @class class CMapManager
 * Класс обработчика карты.
 *
 * Хранит объекты класса CNode на графической сцене.
 * Перерисовывает карту, обновляет, рассчитывает размещение объектов, добавляет и удаляет объекты.
 * Реагирует на выбор объекта CNode на графической сцене и отображает в главном меню данные этапа или исхода,
 * то есть данные CStage или COutcome.
 */

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "cnode.h"

// NOTE: localId - используются внутри cnode и mapManager - это для единения между стейджами и ауткомами

class CMapManager : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CMapManager(QWidget *parent = nullptr);
    ~CMapManager();
    void addFirstNode();
    void addNode(qint32 id, ENodeType type);
    void deleteNode(qint32 id, ENodeType type, qint32 parentId, ENodeType parentType);
    void setSelected(qint32 selectedId, ENodeType type);
    //! Сделать финальным. Возможно только для стейджей!
    void setFinal(qint32 id, ENodeType type, bool final = true);
    //! Обработка ответа на запрос о копировании
    void canCopy(bool st);
    //! Добавить ноду дополнительного родителя и нарисовать линию
    void addAdditionalParentToNode(qint32 nodeId, ENodeType nodeType, qint32 parentId, ENodeType parentType);
    //! Окрасить линию от этапа к исходу
    void markLineFromStageToOutcome(qint32 stageId, qint32 outcomeId);
    //! Окрасить линии от исхода к этапам
    void markLineFromOutcomeToStages(qint32 outcomeId, QList<qint32> stagesId);

signals:
    void s_newNodeSelected(qint32 id, ENodeType type);
    void s_askToCopy(qint32 copiedId, ENodeType copiedType, qint32 selectedId, ENodeType selectedType);
    void s_nodeDoubleClicked();

private slots:
    void slotNodeClicked(qint32 id);
    void slotNodeDoubleClicked(qint32 id);

private:
    //! Перевод id из mw системы outcome и stage в map с едиными id nodes (+million если стейдж)
    qint32 _idFromMW(qint32 id, ENodeType type);
    //! Узнаём type исходя из нашего локального id
    ENodeType _typeToMW(qint32 id);
    //! Перевод id в вид MW (Использовать только рядом с верхней функцией, а не то возможны просчёты)
    qint32 _idToMW(qint32 id);
    //! Удалить нод с локальным id. Copied - удаляем ли мы нод при копировании или просто
    void _deleteNode(qint32 localId, qint32 localParentId, bool copied = false);
    //! Нарисовать линию от parent к destinationNode
    void _addLine(const CNode *parent, CNode *destinationNode);
    //! Нарисовать временную линию выбора удалив существующие
    void _makeSelectionLines(const CNode *parent, QList<CNode *> destinationNodes);
    //! Удалить все существующие линии выбора
    void _removeAllSelectionLines();

private:
    QGraphicsScene *m_scene;
    QMap<qint32, CNode *> m_nodes;
    qint32 m_selectedNodeId { -1 };
    //! Номер копируемого нода, если сейчас происходит копирование
    qint32 m_copiedNodeId { -1 };
    //! Указатели на окрашенные сейчас линии
    QList<QGraphicsItem *> m_markedLines;
};

#endif // CMAPMANAGER_H
