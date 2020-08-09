#include "cnode.h"

const qreal SIZE = 15;

CNode::CNode(QObject *parent) : QObject(parent), QGraphicsItem() {}

CNode::CNode(qint32 id, ENodeType type, qreal xCoordinate, qreal yCoordinate)
{
    m_id = id;
    m_type = type;
    setX(xCoordinate);
    setY(yCoordinate);
}

CNode::~CNode() {}

void CNode::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void CNode::removeChild(qint32 childId)
{
    m_children.removeOne(childId);
}

void CNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit s_clicked(m_id);
    QGraphicsItem::mousePressEvent(event);
}

QRectF CNode::boundingRect() const
{
    if (m_type == eOutcome) {
        return QRectF(x(), y(), SIZE, SIZE);
    } else {
        return QRectF(x(), y(), SIZE + 10, SIZE);
    }
}

void CNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    auto rect = boundingRect();
    QBrush brush(Qt::white);
    if (m_selected) {
        brush.setColor(Qt::yellow);
    }
    QPen pen(Qt::black, 6);
    painter->setPen(pen);
    QPainterPath path;
    if (m_type == eOutcome) {
        painter->drawEllipse(rect);
        path.addEllipse(rect);
    } else {
        painter->drawRect(rect);
        path.addRect(rect);
    }
    painter->fillPath(path, brush);
}
