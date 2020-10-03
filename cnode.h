#ifndef CNODE_H
#define CNODE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "cconstants.h"

/** @class class CNode
 * Класс графического представления этапа приказа.
 *
 * Отображает требуемую фигуру на карте приказа.
 * Хранит указатель на свой CStage или COutcome.
 */

const qreal NODE_SIZE = 15;
//! Ширина контура outlineWidth
const qreal OWID = 2;

class CNode : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CNode(QObject *parent = nullptr);
    CNode(qint32 id, ENodeType type, qreal xCoordinate, qreal yCoordinate);
    ~CNode();
    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }
    qint32 getId() const { return m_id; }
    void setParentId(qint32 parentId) { m_parentId = parentId; }
    qint32 getParentId() const { return m_parentId; }
    void addChild(qint32 childId) { m_children.append(childId); }
    void removeChild(qint32 childId);
    qint32 getChildrenCount() { return m_children.count(); }
    void setLayer(quint16 layer) { m_layer = layer; }
    quint16 getLayer() const { return m_layer; }

signals:
    void s_clicked(qint32 id);
    void s_doubleClicked(qint32 id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    qint32 m_id;
    ENodeType m_type;
    qint32 m_parentId;
    QList<qint32> m_children;
    //! Глубина/Уровень на котором он находится на карте/Количество поколений перед ним
    quint16 m_layer;
    bool m_selected { false };
    //! Копируется прямо сейчас
    bool m_copiyed { false };
};

#endif // CNODE_H
