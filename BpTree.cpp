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
    }
    cur->insertDataMap(newData->getName(), newData);

    if (excessDataNode(cur))
        splitDataNode(cur);

    return true;
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
    auto index = pIndexNode->getIndexMap();
    int mid_pos = index->size() / 2;
    auto iter = index->begin();
    for (int i = 0; i < mid_pos; i++)
        iter++;
    string mid_name = iter->first;
    auto eraseNode = index->begin();
    BpTreeIndexNode *node = new BpTreeIndexNode();
    auto node_index = node->getIndexMap();

    while (iter != index->end())
    {
        node->insertIndexMap(iter->first, iter->second);
        eraseNode = iter;
        iter++;
        index->erase(eraseNode);
    }
    if (!pIndexNode->getParent())
    {                                      // In case, pIndexNode has no parent
        auto Root = new BpTreeIndexNode(); // new root
        Root->setMostLeftChild(pIndexNode);
        Root->insertIndexMap(mid_name, node); // setting new root

        pIndexNode->setParent(Root); // setting parent as new root
        node->setParent(Root);
        root = Root; // root change
    }
    else
    { // In case, pIndexNode has parent
        node->setParent(pIndexNode->getParent());
        pIndexNode->getParent()->insertIndexMap(mid_name, node);
        if (excessIndexNode(pIndexNode->getParent()))
            splitIndexNode(pIndexNode->getParent());
    }
}

BpTreeNode *BpTree::searchDataNode(string name)
{
    BpTreeNode *cur = root; // setting cur for travelsal

    while (cur->getIndexMap() && !cur->getIndexMap()->empty()) // while(cur is indexNode)
    {
        auto index = cur->getIndexMap();
        auto iter = index->begin();
        bool moved = false;
        while (iter != index->end()) // map data travelsal
        {
            if (name == iter->first)
            {
                cur = iter->second;
                moved = true;
                break;
            }
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
    if (!root)
        return;

    BpTreeNode *cur = root;

    // start 이상인 첫 번째 데이터 노드로 이동
    while (cur->getIndexMap() && !cur->getIndexMap()->empty())
    {
        auto index = cur->getIndexMap();
        auto iter = index->begin();
        bool moved = false;

        while (iter != index->end())
        {
            if (start < iter->first)
            {
                // start보다 작으면 왼쪽으로,
                // 크거나 같으면 해당 자식으로 이동
                if (iter != index->begin())
                {
                    auto prev = iter;
                    --prev;
                    cur = prev->second;
                }
                else
                    cur = cur->getMostLeftChild();

                moved = true;
                break;
            }
            ++iter;
        }

        // 마지막까지 갔으면 맨 마지막 자식으로
        if (!moved)
        {
            --iter;
            cur = iter->second;
        }
    }

    // 현재 cur은 start 이상인 첫 리프 노드 근처일 것
    bool found = false;

    //  pNext 포인터 따라가며 end 이하 출력
    while (cur)
    {
        auto dataMap = cur->getDataMap();
        for (auto iter = dataMap->begin(); iter != dataMap->end(); ++iter)
        {
            string key = iter->first;
            if (key >= start && key <= end)
            {
                if (found == false)
                {
                    cout << "=======SEARCH_BP=======" << endl;
                    found = true;
                }
                auto data = iter->second;
                cout << data->getName() << "/" << data->getDeptNo() << "/" << data->getID() << "/" << data->getIncome() << endl;
            }
            else if (key > end)
            {
                cout << "=======================\n\n" ;
                return;
            }
        }
        cur = cur->getNext();
    }

    if (!found)
    {
        cout << "========ERROR========" << endl;
        cout << "300" << endl;
        cout << "=====================\n\n";
    }
}
EmployeeData **BpTree::searchDept_no(int dept_no)
{
    EmployeeData **sendData;
    auto cur = root; // travelsal tree
    while (cur->getIndexMap() && !cur->getIndexMap()->empty())
    {
        cur = cur->getMostLeftChild();
        if (!cur)
            return nullptr;
    }
    int cnt = 0;
    auto node = cur; // for pastring data
    while (cur != nullptr)
    {
        auto data = cur->getDataMap();
        if (data != nullptr && !data->empty())
        {
            auto iter = data->begin();
            while (iter != data->end())
            {
                if (iter->second != nullptr && iter->second->getDeptNo() == dept_no)
                    ++cnt;
                ++iter;
            }
        }
        cur = cur->getNext();
    }
    sendData = new EmployeeData *[cnt + 1];
    int idx = 0;

    while (node != nullptr)
    {
        auto datamap = node->getDataMap();
        if (datamap != nullptr && !datamap->empty())
        {
            auto iter = datamap->begin();
            while (iter != datamap->end())
            {
                EmployeeData *src = iter->second;
                if (src != nullptr && src->getDeptNo() == dept_no)
                {
                    EmployeeData *copy = new EmployeeData();
                    copy->setData(src->getName(), src->getDeptNo(), src->getID(), src->getIncome());
                    sendData[idx] = copy;
                    ++idx;
                }
                ++iter;
            }
        }
        node = node->getNext();
    }
    sendData[idx] = nullptr;
    return sendData;
}
void BpTree::print()
{
    auto cur = root;
    while (cur->getIndexMap() && !cur->getIndexMap()->empty())
    {
        cur = cur->getMostLeftChild();
        if (!cur)
            cout << "Bptree Print ERROR~";
    }
    while (cur != nullptr)
    {
        auto data = cur->getDataMap();
        if (data != nullptr && !data->empty())
        {
            auto iter = data->begin();
            while (iter != data->end())
            {
                auto info = iter->second;
                cout << info->getName() << "/" << info->getDeptNo() << "/" << info->getID() << info->getIncome() << endl;
                ++iter;
            }
        }
        cur = cur->getNext();
    }
}