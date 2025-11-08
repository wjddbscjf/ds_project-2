#pragma once
#include "SelectionTreeNode.h"

class SelectionTree
{
private:
    SelectionTreeNode *root;
    ofstream *fout;
    SelectionTreeNode *run[8];
    SelectionTreeNode *nodes[16];

public:
    SelectionTree(ofstream *fout)
    {
        this->fout = fout;
        for (int i = 0; i < 8; i++)
            run[i] = nullptr;
        for (int i = 0; i < 16; i++)
            nodes[i] = nullptr;
    }

    ~SelectionTree()
    {

        for (int i = 0; i < 8; i++)
        {
            delete run[i];
            run[i] = nullptr;
        }
        for (int i = 1; i < 16; i++)
        {
            delete nodes[i];
            nodes[i] = nullptr;
        }
        root = nullptr;
        fout = nullptr;
    }

    void setRoot(SelectionTreeNode *pN) { this->root = pN; }
    SelectionTreeNode *getRoot() { return root; }

    void setTree();

    bool Insert(EmployeeData *newData);
    bool Delete();
    bool printEmployeeData(int dept_no);
    bool UpdateWinner_Insert(int runIdx);
    bool UpdateWinner_Delete(int runIdx);
};