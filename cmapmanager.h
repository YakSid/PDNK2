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

class CMapManager : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CMapManager(QWidget *parent = nullptr);
    ~CMapManager();
    void addFirstNode();
    void addNode(qint32 id, ENodeType type);
    void deleteNode(qint32 id, ENodeType type);
    void setSelected(qint32 selectedId, ENodeType type);
    //! Обработка ответа на запрос о копировании
    void canCopy(bool st);

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
    //! Удалить нод с локальным id
    void _deleteNode(qint32 localId);
    void _addLine(QPointF parentPoint, ENodeType type, QPointF point);

private:
    QGraphicsScene *m_scene;
    QMap<qint32, CNode *> m_nodes;
    qint32 m_selectedNodeId { -1 };
    //! Номер копируемого нода, если сейчас происходит копирование
    qint32 m_copiedNodeId { -1 };
};

#endif // CMAPMANAGER_H
