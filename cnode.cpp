#include "cnode.h"

const qreal SIZE = 15;

CNode::CNode(QObject *parent) : QObject(parent), QGraphicsItem() {}

CNode::CNode(qint32 id, qreal xCoordinate, qreal yCoordinate)
{
    this->id = id;
    setX(xCoordinate);
    setY(yCoordinate);
}

CNode::~CNode() {}

void CNode::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void CNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit s_clicked(id);
    QGraphicsItem::mousePressEvent(event);
}

QRectF CNode::boundingRect() const
{
    return QRectF(x(), y(), SIZE, SIZE);
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
    painter->drawEllipse(rect);
    QPainterPath path;
    path.addEllipse(rect);
    painter->fillPath(path, brush);
}
