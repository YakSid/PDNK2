#include "cmapmanager.h"
#include <QDebug>

//! т.к. в СОрдер два разных мапа для этапов и исходов, а в мапМенеджере один, то
//! всем айдишникам нодов типа этап прибавляется миллион, чтобы избежать совпадающих айди с исходами
const qint32 MILLION = 1000000;

CMapManager::CMapManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);
    // setRenderHint(QPainter::Antialiasing);
    /*// NOTE: Координатная разметка
    QPen penGreen(Qt::green);
    const qint32 LIMIT = 20;
    for (int i = -LIMIT; i < LIMIT; i++)
        for (int j = -LIMIT; j < LIMIT; j++) {
            QLineF line(i * 10, j * 10, i * 10, j * 10 - 1);
            m_scene->addLine(line, penGreen);
        }
    QLineF axisX(0, 0, 100, 0);
    m_scene->addLine(axisX, QPen(Qt::blue));
    QLineF axisY(0, -100, 0, 100);
    m_scene->addLine(axisY, QPen(Qt::blue));
    QLineF axisRed(60, -100, 60, 100);
    m_scene->addLine(axisRed, QPen(Qt::red));
    //*/
}

CMapManager::~CMapManager() {}

void CMapManager::addFirstNode()
{
    auto node = new CNode(0, eOutcome, 0, 0);
    node->setParentId(-1);
    node->setLayer(0);
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    m_scene->addItem(node);
    m_nodes.insert(node->getId(), node);
}

// TODO: чуть позже: расширить отступы, чтобы не у краёв были ноды. И можно добавить перемещение мышью?
// TODO: чуть позже улучшить алгоритм размещения нодов на мапе
void CMapManager::addNode(qint32 id, ENodeType type)
{
    auto parentId = m_selectedNodeId;
    auto parentNode = m_nodes.find(m_selectedNodeId).value();
    quint16 layer = parentNode->getLayer() + 1;
    quint16 nodesOnLayer = 0;
    for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
        if (it.value()->getLayer() == layer)
            nodesOnLayer++;
    }
    if (type == eStage) {
        id += MILLION;
    }
    parentNode->addChild(id);
    qreal x, y;
    x = (25 + 2 * OWID + (2 * NODE_SIZE + 10) / 2) * layer;
    y = (20 + NODE_SIZE + 2 * OWID) * nodesOnLayer;

    //Создание нода
    auto node = new CNode(id, type, x, y);
    node->setParentId(parentId);
    node->setLayer(layer);
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    m_scene->addItem(node);
    m_nodes.insert(id, node);

    //Добавление линии
    QPen penBlack(Qt::black);
    QLineF line;
    if (type == eStage) {
        //родитель - аутком
        line.setP1(QPointF(parentNode->x() + NODE_SIZE / 2 + OWID, parentNode->y()));
        line.setP2(QPointF(x - (NODE_SIZE + 10) / 2 - OWID, y));
    } else {
        //родитель - стейдж
        line.setP1(QPointF(parentNode->x() + (NODE_SIZE + 10) / 2 + OWID, parentNode->y()));
        line.setP2(QPointF(x - NODE_SIZE / 2 - OWID, y));
    }
    penBlack.setWidth(2);
    m_scene->addLine(line, penBlack);
}

void CMapManager::setSelected(qint32 selectedId, ENodeType type)
{
    if (selectedId == -1)
        return;

    if (type == eStage && selectedId < MILLION) {
        selectedId += MILLION;
    }

    auto it = m_nodes.find(selectedId);
    if (!it.value()->isSelected()) {
        it.value()->setSelected(true);
        if (m_selectedNodeId != -1) {
            it = m_nodes.find(m_selectedNodeId);
            it.value()->setSelected(false);
        }
        m_selectedNodeId = selectedId;
    }
}

void CMapManager::slotNodeClicked(qint32 id)
{
    if (id != m_selectedNodeId) {
        qDebug() << "Node" << id << "was clicked";
        if (id > MILLION) {
            setSelected(id, eStage);
            emit s_newNodeSelected(id - MILLION, eStage);
        } else {
            setSelected(id, eOutcome);
            emit s_newNodeSelected(id, eOutcome);
        }
    }
}
