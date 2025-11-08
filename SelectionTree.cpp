#include "SelectionTree.h"
#include <vector>
#include <algorithm>

void SelectionTree::setTree()
{
    const int size = 8;
    for (int i = 0; i < size; i++)
    {
        run[i] = new SelectionTreeNode();
        run[i]->HeapInit();
        if (!run[i]->getHeap()->IsEmpty())
            run[i]->setEmployeeData(run[i]->getHeap()->Top());
    }
    for (int i = 1; i <= 15; i++)
        nodes[i] = new SelectionTreeNode();

    for (int i = 0; i < size; i++)
        nodes[i + 8] = run[i];

    for (int i = 7; i >= 1; i--)
    {
        nodes[i]->setLeftChild(nodes[i * 2]);
        nodes[i]->setRightChild(nodes[i * 2 + 1]);
        nodes[i * 2]->setParent(nodes[i]);
        nodes[i * 2 + 1]->setParent(nodes[i]);
    }

    root = nodes[1];
}

bool SelectionTree::UpdateWinner_Insert(int runIdx)
{
    int pos = runIdx + 8;
    EmployeeHeap *heap = run[runIdx]->getHeap();
    if (heap->IsEmpty())
        return false;

    EmployeeData *top = heap->Top();
    nodes[pos]->setEmployeeData(top);

    while (pos > 1)
    {
        int parent = pos / 2;
        EmployeeData *left = nodes[parent * 2]->getEmployeeData();
        EmployeeData *right = nodes[parent * 2 + 1]->getEmployeeData();
        EmployeeData *winner = nullptr;

        if (left && right)
        {
            if (left->getIncome() >= right->getIncome())
                winner = left;
            else
                winner = right;
        }
        else if (left)
            winner = left;
        else if (right)
            winner = right;

        // 부모 데이터가 이미 동일한 경우엔 갱신 불필요
        if (nodes[parent]->getEmployeeData() == winner)
            break;

        nodes[parent]->setEmployeeData(winner);
        pos = parent;
    }

    root = nodes[1];
    return true;
}

bool SelectionTree::UpdateWinner_Delete(int runIdx)
{
    int pos = runIdx + 8;
    EmployeeHeap *heap = run[runIdx]->getHeap();
    nodes[pos]->setEmployeeData(heap->IsEmpty() ? nullptr : heap->Top());

    while (pos > 1)
    {
        int parent = pos / 2;
        EmployeeData *L = nodes[parent * 2]->getEmployeeData();
        EmployeeData *R = nodes[parent * 2 + 1]->getEmployeeData();
        EmployeeData *winner = nullptr;

        if (L && R)
            winner = (L->getIncome() >= R->getIncome()) ? L : R;
        else
            winner = L ? L : R;

        nodes[parent]->setEmployeeData(winner);
        pos = parent;
    }

    root = nodes[1];
    return true;
}

bool SelectionTree::Insert(EmployeeData *newData)
{
    int deptPos = newData->getDeptNo() / 100 - 1;
    int pos = deptPos + 7;
    auto beforeTop = run[deptPos]->getHeap()->Top();
    run[deptPos]->getHeap()->Insert(newData);
    auto afterTop = run[deptPos]->getHeap()->Top();

    if (beforeTop != afterTop)
    {
        if (UpdateWinner_Insert(deptPos))
            return true;
        else
            return false;
    }
    return true;
}

bool SelectionTree::Delete()
{
    if (!root || !root->getEmployeeData())
        return false;

    int runIdx = root->getEmployeeData()->getDeptNo() / 100 - 1;
    run[runIdx]->getHeap()->Delete();
    UpdateWinner_Delete(runIdx);
    return true;
}

bool SelectionTree::printEmployeeData(int dept_no)
{
    int runIdx = dept_no / 100 - 1;
    auto heap = run[runIdx]->getHeap();
    if (!heap || heap->IsEmpty())
        return false;

    vector<EmployeeData *> v;
    for (int i = 1; i <= heap->getDatanum(); i++)
        v.push_back(heap->getHeapArr()[i]);

    sort(v.begin(), v.end(), [](EmployeeData *a, EmployeeData *b)
         { return a->getIncome() > b->getIncome(); });

    cout << "=======PRINT_ST=======" << endl;
    for (int i = 0; i < (int)v.size(); i++)
    {
        EmployeeData *d = v[i];
        cout << d->getName() << "/" << d->getDeptNo() << "/"
             << d->getID() << "/" << d->getIncome() << endl;
    }
    cout << "======================\n\n";
    return true;
}
