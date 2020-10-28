#include "cmapmanager.h"
#include <QDebug>

//! т.к. в СОрдер два разных мапа для этапов и исходов, а в мапМенеджере один, то
//! всем айдишникам нодов типа этап прибавляется миллион, чтобы избежать совпадающих айди с исходами
const qint32 MILLION = 1000000;

CMapManager::CMapManager(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene();
    setScene(m_scene);
    // TODO: позже настроить ширину границ, чтобы антиалайзинг норм выглядел
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

CMapManager::~CMapManager()
{
    for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
        delete it.value();
    }
}

void CMapManager::addFirstNode()
{
    auto node = new CNode(0, eOutcome, 0, 0);
    node->setToolTip(QString::number(0));
    node->setMainParentId(-1);
    node->setLayer(0);
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    m_scene->addItem(node);
    m_nodes.insert(node->getId(), node);
}

// TODO: чуть позже: расширить отступы, чтобы не у краёв были ноды. И можно добавить перемещение мышью?
// TODO: позже улучшить алгоритм размещения нодов на мапе
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
    node->setToolTip(QString::number(id));
    node->setMainParentId(parentId);
    node->setLayer(layer);
    connect(node, &CNode::s_clicked, this, &CMapManager::slotNodeClicked);
    connect(node, &CNode::s_doubleClicked, this, &CMapManager::slotNodeDoubleClicked);
    m_scene->addItem(node);
    m_nodes.insert(id, node);

    //Добавление линии
    _addLine(parentNode, node);
}

void CMapManager::deleteNode(qint32 id, ENodeType type)
{
    _deleteNode(_idFromMW(id, type));
}

void CMapManager::setSelected(qint32 selectedId, ENodeType type)
{
    if (selectedId == -1)
        return;

    selectedId = _idFromMW(selectedId, type);

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

void CMapManager::setFinal(qint32 id, ENodeType type, bool final)
{
    if (id == -1)
        return;

    id = _idFromMW(id, type);

    auto it = m_nodes.find(id);
    it.value()->setFinal(final);
}

void CMapManager::canCopy(bool st)
{
    auto copiedNode = m_nodes.find(m_copiedNodeId).value();
    if (st) {
        //Можно копировать, рисуем новую линию и удаляем старую вместе с нодом
        //Находим родителя копируемого и выбранный нод, чтобы найти их координаты для отрисовки линии
        auto selectedNode = m_nodes.find(m_selectedNodeId).value();
        //Замена id для доп.родителей и перерисовка их линий
        auto additionalParents = copiedNode->getAdditionalParentsId();
        for (auto additionalParent : additionalParents) {
            auto additionalParentNode = m_nodes.find(additionalParent).value();
            additionalParentNode->removeChild(copiedNode->getId());
            additionalParentNode->addChild(selectedNode->getId());
            selectedNode->addAdditionalParent(additionalParentNode->getId());
            _addLine(additionalParentNode, selectedNode);
        }
        //Копирование основного нода
        auto parentId = copiedNode->getMainParentId();
        auto parentNode = m_nodes.find(parentId).value();
        parentNode->removeChild(copiedNode->getId());
        parentNode->addChild(selectedNode->getId());
        selectedNode->addAdditionalParent(parentId);
        _addLine(parentNode, selectedNode);
        _deleteNode(m_copiedNodeId);
        qDebug() << "Произошло копирование";
    } else {
        //Нельзя копировать
        copiedNode->setCopied(false);
        qDebug() << "Копирование отменено";
    }
    m_copiedNodeId = -1;
}

void CMapManager::addAdditionalParentToNode(qint32 nodeId, ENodeType nodeType, qint32 parentId, ENodeType parentType)
{
    nodeId = _idFromMW(nodeId, nodeType);
    parentId = _idFromMW(parentId, parentType);

    auto it = m_nodes.find(nodeId);
    if (it != m_nodes.end()) {
        it.value()->addAdditionalParent(parentId);
    }
    //Рисуем линию
    auto node = m_nodes.find(nodeId).value();
    auto parentNode = m_nodes.find(parentId).value();
    _addLine(parentNode, node);
}

void CMapManager::slotNodeClicked(qint32 id)
{
    if (id != m_selectedNodeId) {
        // qDebug() << "Node" << id << "was selected";
        if (id >= MILLION) {
            setSelected(id, eStage);
            emit s_newNodeSelected(id - MILLION, eStage);
        } else {
            setSelected(id, eOutcome);
            emit s_newNodeSelected(id, eOutcome);
        }
        //Если есть копируемый
        if (m_copiedNodeId != -1) {
            //Спросить можно ли копировать
            emit s_askToCopy(_idToMW(m_copiedNodeId), _typeToMW(m_copiedNodeId), _idToMW(id), _typeToMW(id));
        }
    } else {
        //если кликнули выбранный
    }
}

void CMapManager::slotNodeDoubleClicked(qint32 id)
{
    if (m_copiedNodeId == id) {
        //!Специальный сигнал о самостоятельной отмене копирования
        emit s_askToCopy(-1, eStage, 0, eStage);
    } else {
        qDebug() << "Копирование началось copiedNodeId" << id;
        m_copiedNodeId = id;
        emit s_nodeDoubleClicked();
    }
}

qint32 CMapManager::_idFromMW(qint32 id, ENodeType type)
{
    if (type == eStage && id < MILLION) {
        return id + MILLION;
    } else {
        return id;
    }
}

ENodeType CMapManager::_typeToMW(qint32 id)
{
    if (id >= MILLION) {
        return eStage;
    } else {
        return eOutcome;
    }
}

qint32 CMapManager::_idToMW(qint32 id)
{
    if (id >= MILLION) {
        return id - MILLION;
    } else {
        return id;
    }
}

void CMapManager::_deleteNode(qint32 localId)
{
    auto it = m_nodes.find(localId);

    //Удаление входящих линий
    auto lines = it.value()->getAllLines();
    for (auto line : lines) {
        m_scene->removeItem(line);
    }

    m_scene->removeItem(it.value());
    delete it.value();
    m_nodes.remove(localId);
    m_scene->update();
}

void CMapManager::_addLine(const CNode *parent, CNode *destinationNode)
{
    QPen penBlack(Qt::black);
    QLineF line;
    if (destinationNode->getType() == eStage) {
        //родитель - аутком
        line.setP1(QPointF(parent->x() + NODE_SIZE / 2 + OWID, parent->y()));
        line.setP2(QPointF(destinationNode->x() - (NODE_SIZE + 10) / 2 - OWID, destinationNode->y()));
    } else {
        //родитель - стейдж
        line.setP1(QPointF(parent->x() + (NODE_SIZE + 10) / 2 + OWID, parent->y()));
        line.setP2(QPointF(destinationNode->x() - NODE_SIZE / 2 - OWID, destinationNode->y()));
    }
    penBlack.setWidth(2);
    auto linePointer = m_scene->addLine(line, penBlack);

    destinationNode->addLine(parent->getId(), linePointer);
}
