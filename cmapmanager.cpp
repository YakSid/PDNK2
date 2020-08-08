#include "cmapmanager.h"
#include <QDebug>

CMapManager::CMapManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);
}

CMapManager::~CMapManager() {}

void CMapManager::addFirstNode()
{
    auto node = new CNode(0, 0, 0);
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    m_scene->addItem(node);
    m_nodes.insert(node->getId(), node);
    slotNodeClicked(node->getId());
}

void CMapManager::addNode(qint32 parentId)
{
    //Рассчитать из данных родителя, где разместить нод
    auto node = new CNode();
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    m_scene->addItem(node);
}

void CMapManager::slotNodeClicked(qint32 id)
{
    if (id != m_selectedNodeId) {
        qDebug() << "Node" << id << "was clicked";
        setSelected(id);
        emit s_newNodeSelected(id);
    }
}

void CMapManager::setSelected(qint32 selectedId)
{
    if (selectedId == -1)
        return;

    auto it = m_nodes.find(selectedId);
    it.value()->setSelected(true);
    if (m_selectedNodeId != -1) {
        it = m_nodes.find(m_selectedNodeId);
        it.value()->setSelected(false);
    }
    m_selectedNodeId = selectedId;
}

// QPen penBlack(Qt::black);
// m_scene->addLine(20, 20, 50, 50, penBlack);
