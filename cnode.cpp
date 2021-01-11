#include "cnode.h"
#include <QDebug>

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

void CNode::removeAdditionalParent(qint32 parentId)
{
    for (int i = 0; i < m_additionalParentsId.count(); i++) {
        if (m_additionalParentsId.at(i) == parentId) {
            m_additionalParentsId.removeAt(i);
            break;
        }
    }
}

void CNode::removeChild(qint32 childId)
{
    m_children.removeOne(childId);
}

void CNode::setCopied(bool st)
{
    m_copied = st;
    update();
}

const QList<QGraphicsItem *> CNode::getAllLines()
{
    QList<QGraphicsItem *> result;
    for (auto line : m_lines) {
        result.append(line);
    }
    return result;
}

void CNode::removeLineFromParent(qint32 parentId)
{
    auto line = m_lines.value(parentId);
    delete line;
    m_lines.remove(parentId);
}

void CNode::removeAllLines()
{
    qDeleteAll(m_lines);
    m_lines.clear();
}

void CNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_selected) {
        if (m_id != 0) {
            setCopied(true);
            emit s_doubleClicked(m_id);
        }
    } else {
        emit s_clicked(m_id);
    }
    QGraphicsItem::mousePressEvent(event);
}

QRectF CNode::boundingRect() const
{
    if (m_type == eOutcome) {
        return QRectF(-NODE_SIZE / 2, -NODE_SIZE / 2, NODE_SIZE, NODE_SIZE);
    } else {
        return QRectF(-(NODE_SIZE + 10) / 2, -NODE_SIZE / 2, NODE_SIZE + 10, NODE_SIZE);
    }
}

void CNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    auto rect = boundingRect();
    QBrush brush(Qt::white);
    if (m_final) {
        brush.setColor(Qt::blue);
    }
    if (m_selected) {
        brush.setColor(Qt::yellow);
    }
    QPen pen(Qt::black, 6);
    if (m_copied) {
        pen.setColor(Qt::darkGray);
    }
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
