#include "cmapmanager.h"
#include <QDebug>

const qint32 MILLION = 1000000;

CMapManager::CMapManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);
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
    slotNodeClicked(node->getId());
}

void CMapManager::addNode(qint32 id, ENodeType type)
{
    auto parentId = m_selectedNodeId;
    auto parentNode = m_nodes.find(m_selectedNodeId).value();
    parentNode->addChild(id);
    quint16 layer = parentNode->getLayer() + 1;
    qreal x, y;
    // TODO: позже алгоритм размещения нода на мапе
    x = 25 * layer;
    y = 20 * (parentNode->getChildrenCount() - 1);
    //! т.к. в СОрдер два разных мапа для этапов и исходов, а в мапМенеджере один, то
    //! здесь всем айдишникам нодов типа этап прибавляется миллион, чтобы избежать совпадающих айди с исходами
    if (type == eStage)
        id += MILLION;
    auto node = new CNode(id, type, x, y); // NOTE: коррдинаты норм потом сделать
    node->setParentId(parentId);
    node->setLayer(layer);
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    m_scene->addItem(node);
    m_nodes.insert(id, node);
}

void CMapManager::setSelected(qint32 selectedId, ENodeType type)
{
    if (selectedId == -1)
        return;

    if (type == eStage && selectedId < MILLION) {
        selectedId += MILLION;
    }

    auto it = m_nodes.find(selectedId);
    it.value()->setSelected(true);
    if (m_selectedNodeId != -1) {
        it = m_nodes.find(m_selectedNodeId);
        it.value()->setSelected(false);
    }
    m_selectedNodeId = selectedId;
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

// QPen penBlack(Qt::black);
// m_scene->addLine(20, 20, 50, 50, penBlack);
