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
    void setSelected(qint32 selectedId);

signals:
    void s_newNodeSelected(qint32 id, ENodeType type);

private slots:
    void slotNodeClicked(qint32 id);

private:
    QGraphicsScene *m_scene;
    QMap<qint32, CNode *> m_nodes;
    qint32 m_selectedNodeId { -1 };
};

#endif // CMAPMANAGER_H
