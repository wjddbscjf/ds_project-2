#pragma once
#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"
#include "SelectionTree.h"

#include <queue>

class BpTree
{
private:
	BpTreeNode *root;
	int order; // m children

public:
	BpTree(ofstream *fout, int order)
	{
		root = nullptr;
	}

	~BpTree()
	{
		if (root)
		{
			// travelsal all nodes and delete
			std::queue<BpTreeNode *> q;
			q.push(root);
			while (!q.empty())
			{
				BpTreeNode *node = q.front();
				q.pop();

				if (node->getIndexMap())
				{
					for (auto &pair : *node->getIndexMap())
						q.push(pair.second);
				}
				else if (node->getDataMap())
				{
					for (auto &pair : *node->getDataMap())
						delete pair.second;
				}
				delete node;
			}
			root = nullptr;
		}
	}

	void print();
	EmployeeData **searchDept_no(int dept_no);
	/* essential */
	bool Insert(EmployeeData *newData);
	bool excessDataNode(BpTreeNode *pDataNode);
	bool excessIndexNode(BpTreeNode *pIndexNode);
	void splitDataNode(BpTreeNode *pDataNode);
	void splitIndexNode(BpTreeNode *pIndexNode);
	BpTreeNode *getRoot() { return root; }
	BpTreeNode *searchDataNode(string name);
	void searchRange(string start, string end);
};

#endif
