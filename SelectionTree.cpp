#include "SelectionTree.h"
#include <vector>
#include <algorithm>

void SelectionTree::setTree() // Configuring an Initial Tree in the constructor
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
        nodes[i] = new SelectionTreeNode(); // setting all nodes about selectionTree

    for (int i = 0; i < size; i++) // setting leaf node
        nodes[i + 8] = run[i];

    for (int i = 7; i >= 1; i--) // Establishing Parent-Child Relationships in Internal Node
    {
        nodes[i]->setLeftChild(nodes[i * 2]);
        nodes[i]->setRightChild(nodes[i * 2 + 1]);
        nodes[i * 2]->setParent(nodes[i]);
        nodes[i * 2 + 1]->setParent(nodes[i]);
    }

    root = nodes[1]; // root setting
}

bool SelectionTree::UpdateWinner_Insert(int runIdx) // Rearrange the tree after Insert is in progress
{
    int pos = runIdx + 8; // Position of leaf node with run
    EmployeeHeap *heap = run[runIdx]->getHeap();
    if (heap->IsEmpty())
        return false;

    EmployeeData *top = heap->Top();
    nodes[pos]->setEmployeeData(top);

    while (pos > 1)
    {
        // In travelsal reestablishing parent-child relationships
        int parent = pos / 2;
        EmployeeData *left = nodes[parent * 2]->getEmployeeData();
        EmployeeData *right = nodes[parent * 2 + 1]->getEmployeeData();
        EmployeeData *winner = nullptr;
        // If left and right children exist
        if (left && right)
        {
            if (left->getIncome() >= right->getIncome())
                winner = left;
            else
                winner = right;
        }
        // If left child exist
        else if (left)
            winner = left;
        ////If right child exist
        else if (right)
            winner = right;

        // No update required if parent data is already the same
        if (nodes[parent]->getEmployeeData() == winner)
            break;

        nodes[parent]->setEmployeeData(winner); // setting winner
        pos = parent;                           // travelsl
    }

    root = nodes[1];
    return true;
}

bool SelectionTree::UpdateWinner_Delete(int runIdx)
{
    int pos = runIdx + 8;
    EmployeeHeap *heap = run[runIdx]->getHeap();

    if (heap->IsEmpty())
        nodes[pos]->setEmployeeData(nullptr);
    else
        nodes[pos]->setEmployeeData(heap->Top());

    while (pos > 1)
    {
        int parent = pos / 2;
        // In travelsal reestablishing parent-child relationships
        EmployeeData *left = nodes[parent * 2]->getEmployeeData();
        EmployeeData *right = nodes[parent * 2 + 1]->getEmployeeData();
        EmployeeData *winner = nullptr;
        // If left and right children exist
        if (left && right)
        {
            if (left->getIncome() >= right->getIncome())
                winner = left;
            else
                winner = right;
        }
        else if (left) // If left child exist
        {
            winner = left;
        }
        else if (right) // If right child exist
        {
            winner = right;
        }
        else // if has no child
        {
            winner = nullptr;
        }
        // setting winner
        nodes[parent]->setEmployeeData(winner);
        pos = parent; // for travelsal
    }

    root = nodes[1];
    return true;
}

bool SelectionTree::Insert(EmployeeData *newData)
{
    int deptPos = newData->getDeptNo() / 100 - 1;    // Run position that matches the newData's employee dept_no
    int pos = deptPos + 7;                           // node position
    auto beforeTop = run[deptPos]->getHeap()->Top(); // Top before inserting
    run[deptPos]->getHeap()->Insert(newData);        // inserting
    auto afterTop = run[deptPos]->getHeap()->Top();  // Top after inserting

    if (beforeTop != afterTop) // if(top is changed)
    {
        if (UpdateWinner_Insert(deptPos)) // update winer
            return true;
        else
            return false;
    }
    // top isn't changed
    return true;
}

bool SelectionTree::Delete()
{
    if (!root || !root->getEmployeeData())
        return false;

    int runIdx = root->getEmployeeData()->getDeptNo() / 100 - 1; // Run position that matches the newData's employee dept_no
    run[runIdx]->getHeap()->Delete();                            // delete run's top element
    UpdateWinner_Delete(runIdx);                                 // update winner after deleting
    return true;
}

bool SelectionTree::printEmployeeData(int dept_no)
{
    int runIdx = dept_no / 100 - 1; // run value
    auto heap = run[runIdx]->getHeap();
    if (!heap || heap->IsEmpty())
        return false;

    vector<EmployeeData *> v; // Create for output
    for (int i = 1; i <= heap->getDatanum(); i++)
        v.push_back(heap->getHeapArr()[i]); // pasting

    sort(v.begin(), v.end(), [](EmployeeData *a, EmployeeData *b)
         { return a->getIncome() > b->getIncome(); }); // sorting descend with income

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
