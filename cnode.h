#ifndef CNODE_H
#define CNODE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

/** @class class CNode
 * Класс графического представления этапа приказа.
 *
 * Отображает требуемую фигуру на карте приказа.
 * Хранит указатель на свой CStage или COutcome.
 */

class CNode : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CNode(QObject *parent = nullptr);
    CNode(qint32 id, qreal xCoordinate, qreal yCoordinate);
    ~CNode();
    void setSelected(bool selected);
    bool isSelected() const { return m_selected; }
    qint32 getId() const { return id; }

signals:
    void s_clicked(qint32 id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    qint32 id;
    bool m_selected { false };
};

#endif // CNODE_H
