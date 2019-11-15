#ifndef CNODE_H
#define CNODE_H

#include <QObject>

/// Класс графического представления этапа приказа.
/// Отображает требуемую фигуру на карте приказа.
/// Хранит указатель на свой CStage или COutcome.

class CNode : public QObject
{
    Q_OBJECT
public:
    explicit CNode(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CNODE_H
