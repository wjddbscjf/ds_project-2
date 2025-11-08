#pragma once
#include "EmployeeHeap.h"

void EmployeeHeap::Insert(EmployeeData *data)
{
    if (datanum + 1 >= maxCapacity)
        ResizeArray();

    heapArr[++datanum] = data;
    UpHeap(datanum);
    return;
}

EmployeeData *EmployeeHeap::Top()
{
    if (IsEmpty())
        return nullptr;
    else
        return heapArr[1];
}

void EmployeeHeap::Delete()
{
    if (IsEmpty())
        return;
    heapArr[1] = heapArr[datanum--];
    DownHeap(1);
}

bool EmployeeHeap::IsEmpty()
{
    return datanum == 0;
}

void EmployeeHeap::UpHeap(int index)
{
    while (index > 1) // travelsal until index == 1
    {
        int parent = index / 2; // parent setting
        if (heapArr[index]->getIncome() > heapArr[parent]->getIncome())
        // if(index's income > parent's income)
        {
            // swap data
            auto temp = heapArr[index];
            heapArr[index] = heapArr[parent];
            heapArr[parent] = temp;
            // resize index value
            index = parent;
        }
        else
            break;
    }
    return;
}

void EmployeeHeap::DownHeap(int index)
{
    while (index * 2 <= datanum) // travelsal until nodes has no child
    {
        int child = index * 2; // child = index's leftchild
        if (child + 1 <= datanum && heapArr[child]->getIncome() < heapArr[child + 1]->getIncome())
            // if(index's right child exists && rightchild.income > leftchild.income)
            child++; // child is rightchild

        if (heapArr[index]->getIncome() < heapArr[child]->getIncome())
        // if(index's income < child's income)
        {
            // swap data
            auto temp = heapArr[index];
            heapArr[index] = heapArr[child];
            heapArr[child] = temp;
            // resize index value
            index = child;
        }
        else
            break;
    }
    return;
}

void EmployeeHeap::ResizeArray()
{
    maxCapacity *= 2;
    EmployeeData **newArr = new EmployeeData *[maxCapacity + 1];
    for (int i = 1; i <= datanum; i++)
        newArr[i] = heapArr[i];

    delete[] heapArr;
    heapArr = newArr;
}