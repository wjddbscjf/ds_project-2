#include "BpTree.h"

bool BpTree::Insert(EmployeeData *newData)
{
    if (root == nullptr)
    // newData need to be root node
    {
        BpTreeDataNode *data = new BpTreeDataNode();
        data->insertDataMap(newData->getName(), newData);
        root = data;
        return true;
    }
    BpTreeNode *cur = root;
    while (cur->getIndexMap() && !cur->getIndexMap()->empty()) // searching adequete inserting position
    {
        auto index = cur->getIndexMap();
        auto iter = index->begin();
        bool moved = false;
        while (iter != index->end())
        {
            if (newData->getName() < iter->first)
            {
                if (iter != index->begin())
                {
                    auto prev = iter;
                    --prev;
                    cur = prev->second;
                }
                else
                {
                    cur = cur->getMostLeftChild();
                }
                moved = true;
                break;
            }
            ++iter;
        }
        if (!moved)
        {
            --iter;
            cur = iter->second;
        }
        cur->insertDataMap(newData->getName(), newData);

        if (excessDataNode(cur))
            splitDataNode(cur);

        return true;
    }
}

bool BpTree::excessDataNode(BpTreeNode *pDataNode)
{
    int count = 0;
    auto datamap = pDataNode->getDataMap();

    for (auto iter = datamap->begin(); iter != datamap->end(); ++iter)
        ++count;

    return count > order - 1; // dataNode excess key count return 'true'
}

bool BpTree::excessIndexNode(BpTreeNode *pIndexNode)
{
    int count = 0;
    auto indexmap = pIndexNode->getIndexMap();

    for (auto iter = indexmap->begin(); iter != indexmap->end(); ++iter)
        ++count;

    return count > order - 1; // indexNode excess key count return 'true'
}

void BpTree::splitDataNode(BpTreeNode *pDataNode)
{
    auto data = pDataNode->getDataMap();
    int mid_pos = data->size() / 2;
    auto iter = data->begin();

    for (int i = 0; i < mid_pos; i++)
        ++iter;

    string mid_name = iter->first; // key value in mid_pos

    BpTreeDataNode *node = new BpTreeDataNode();
    auto eraseData = data->begin();
    while (iter != data->end())
    {
        node->insertDataMap(iter->first, iter->second);
        eraseData = iter;
        ++iter;
        data->erase(eraseData);
    }
    if (!pDataNode->getNext())
    { // In case, pDataNode has no next node
        pDataNode->setNext(node);
        node->setPrev(pDataNode);
    }
    else
    { // pDataNode has next node
        // re-connecting double-checked list
        node->setNext(pDataNode->getNext());
        node->setPrev(pDataNode);
        pDataNode->getNext()->setPrev(node);
        pDataNode->setNext(node);
    }
    // setting Parent
    if (!pDataNode->getParent())
    {                                      // In case, pDataNode has no parent
        auto Root = new BpTreeIndexNode(); // new root
        Root->setMostLeftChild(pDataNode);
        Root->insertIndexMap(mid_name, node); // setting new root

        pDataNode->setParent(Root); // setting parent as new root
        node->setParent(Root);
        root = Root; // root change
    }
    else
    { // In case, pDataNode has parent
        node->setParent(pDataNode->getParent());
        pDataNode->getParent()->insertIndexMap(mid_name, node);
        if (excessIndexNode(pDataNode->getParent()))
            splitIndexNode(pDataNode->getParent());
    }
}

void BpTree::splitIndexNode(BpTreeNode *pIndexNode)
{
}

BpTreeNode *BpTree::searchDataNode(string name)
{
    // 연결리스트로 다시 구성할것.
    // - 근데 오히려 연결리스트로 하면 안될듯 ?
    BpTreeNode *cur = root; // setting cur for travelsal

    while (cur->getIndexMap() && !cur->getIndexMap()->empty()) // while(cur is indexNode)
    {
        auto index = cur->getIndexMap();
        auto iter = index->begin();
        bool moved = false;
        while (iter != index->end()) // map data travelsal
        {
            if (name == iter->first) // target name is existing
                return cur;          // 이렇게 하면 dataNode가 아니라 indexNode가 반환중임. 이럼 어캄 ?
            else if (name < iter->first)
            {
                if (iter != index->begin())
                {
                    auto prev = iter;
                    --prev;
                    cur = prev->second;
                }
                else
                {
                    cur = cur->getMostLeftChild();
                }
                moved = true;
                break;
            }
            ++iter; // move next map data
        }
        if (!moved)
        {
            --iter;
            cur = iter->second;
        }
    }

    auto data = cur->getDataMap();
    if (data)
    {
        auto it = data->find(name);
        if (it != data->end())
            return cur; // 해당 데이터가 있는 노드 반환
    }
    return nullptr;
}

void BpTree::searchRange(string start, string end)
{
    BpTreeNode *cur = root;
    while (cur->getIndexMap() && !cur->getIndexMap()->empty())
    {
    }
}