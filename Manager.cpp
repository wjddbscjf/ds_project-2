#include "Manager.h"

struct Command
{
	string cmd;
	string name;
	string option;
	int id;
	int dept;
	int income;
};

Command CommandParsing(const string &line)
{
	Command c;
	stringstream ss(line);
	ss >> c.cmd;

	if (c.cmd == "LOAD" || c.cmd == "EXIT" || c.cmd == "PRINT_BP" || c.cmd == "DELETE")
	{
		return c;
	}
	else if (c.cmd == "ADD_BP")
	{
		// ADD_BP name dept id income
		if (!(ss >> c.name >> c.dept >> c.id >> c.income))
			c.name = "ERROR";
		return c;
	}
	else if (c.cmd == "SEARCH_BP")
	{
		if (ss >> c.name)
		{
			if (ss >> c.option) // optional range
				return c;
			c.option = "name";
			return c;
		}
		c.name = "ERROR";
		return c;
	}
	else if (c.cmd == "ADD_ST")
	{
		if (ss >> c.option)
		{
			if (c.option == "dept_no")
			{
				if (!(ss >> c.dept))
					c.name = "ERROR";
				return c;
			}
			else if (c.option == "name")
			{
				if (!(ss >> c.name))
					c.name = "ERROR";
				return c;
			}
		}
		c.name = "ERROR";
		return c;
	}
	else if (c.cmd == "PRINT_ST")
	{
		if (!(ss >> c.dept))
			c.name = "ERROR";
		return c;
	}

	return c;
}

void Manager::run(const char *command)
{
	fin.open(command);
	string line;
	while (getline(fin, line))
	{
		cout << line << endl;
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		Command c;
		c = CommandParsing(line);
		if (c.cmd == "LOAD")
			LOAD();
		else if (c.cmd == "ADD_BP")
		{
			if (c.name == "ERROR")
				//// print ERROR code if parameter is less than '4'
				printErrorCode(200);
			else
			{
				ADD_BP(c.name, c.dept, c.id, c.income);
			}
		}
		else if (c.cmd == "SEARCH_BP")
		{
			if (c.name == "ERROR")
				printErrorCode(300);
			else
			{
				if (c.option == "name")
				{
					SEARCH_BP_NAME(c.name);
				}
				else
				{
					SEARCH_BP_RANGE(c.name, c.option);
				}
			}
		}
		else if (c.cmd == "PRINT_BP")
		{
			if (c.name == "ERROR" || bptree->getRoot() == NULL)
				printErrorCode(400);
			else
			{
				PRINT_BP();
			}
		}
		else if (c.cmd == "ADD_ST")
		{
			if (c.name == "ERROR")
				printErrorCode(500);
			else
			{
				if (c.option == "dept_no")
					ADD_ST_DEPTNO(c.dept);

				else if (c.option == "name")
					ADD_ST_NAME(c.name);
			}
		}
		else if (c.cmd == "PRINT_ST")
		{
			if (c.name == "ERROR")
				printErrorCode(600);
			else
			{
				PRINT_ST(c.dept);
			}
		}
		else if (c.cmd == "DELETE")
		{
			DELETE();
		}
		else if(c.cmd=="EXIT")
		{ 
			printSuccessCode("EXIT");
			return;
		}
	}
}

void Manager::LOAD()
{
	ifstream emp("employee.txt");
	if (!emp || bptree->getRoot() != nullptr)
		printErrorCode(100);
	else
	{
		string line;
		while (getline(emp, line))
		{
			stringstream ss(line);
			string name;
			int dept, id, income;

			ss >> name >> dept >> id >> income;
			if (ss.fail())
				continue;

			EmployeeData *newEmp = new EmployeeData();
			newEmp->setData(name, dept, id, income);
			bptree->Insert(newEmp);
		}
		emp.close();
		printSuccessCode("LOAD");
	}
	return;
}

void Manager::ADD_BP(string name, int dept, int id, int income)
{
	// add data directly, Error is handled in "run"
	EmployeeData *data = new EmployeeData();
	data->setData(name, dept, id, income);

	// Success
	flog << "=======ADD_BP=======" << endl;
	flog << name << "/" << dept << "/" << id << "/" << income << endl;
	flog << "====================" << endl
		 << endl;
	bptree->Insert(data);
}

void Manager::SEARCH_BP_NAME(string name)
{
	auto node = bptree->searchDataNode(name);
	if (node == nullptr || bptree->getRoot() == nullptr)
	{
		// if can't find target node || BPtree has no data -> print ERROR
		printErrorCode(300);
	}
	else
	{
		auto iter = node->getDataMap()->find(name)->second;
		flog << "=======SEARCH_BP=======" << endl;
		flog << name << "/" << iter->getDeptNo() << "/" << iter->getID() << "/" << iter->getIncome() << endl;
		flog << "=======================\n\n";
	}
}
void Manager::SEARCH_BP_RANGE(string start, string end)
{
	bptree->searchRange(start, end);
}

void Manager::ADD_ST_DEPTNO(int dept_no)
{
	if (dept_no % 100 != 0 || dept_no < 100 || dept_no > 800)
	{

		printErrorCode(500);
		return;
	}
	if (!bptree || !bptree->getRoot())
	{

		printErrorCode(500);
		return;
	}
	int runIdx = dept_no / 100 - 1;
	EmployeeData **arr = bptree->searchDept_no(dept_no);
	if (!arr)
	{
		printErrorCode(500);
		return;
	}

	int i = 0;
	while (arr[i] != nullptr)
	{
		stree->Insert(arr[i]);
		++i;
	}
	delete[] arr;

	printSuccessCode("ADD_ST");
	return;
}

void Manager::ADD_ST_NAME(string name)
{
	auto tar = bptree->searchDataNode(name);
	if (tar == nullptr)
	{
		printErrorCode(600);
		return;
	}
	auto data = tar->getDataMap();
	auto iter = data->find(name);
	if (iter == data->end() || iter->second == nullptr)
	{
		printErrorCode(600);
		return;
	}

	bool ok = stree->Insert(iter->second);

	if (!ok)
	{
		printErrorCode(600);
		return;
	}

	printSuccessCode("ADD_ST");
	return;
}

void Manager::PRINT_BP()
{
	if (!bptree->getRoot())
	{
		printErrorCode(400);
		return;
	}
	flog << "=======PRINT_BP=======" << endl;
	bptree->print();
	flog << "======================\n\n";
	return;
}

void Manager::PRINT_ST(int dept)
{
	if (dept % 100 != 0 || dept < 100 || dept > 800)
	{
		printErrorCode(600);
		return;
	}
	stree->printEmployeeData(dept);
	return;
}

void Manager::DELETE()
{
	if (stree->getRoot() == nullptr)
	{
		printErrorCode(700);
		return;
	}
	else
	{
		if (stree->Delete())
			printSuccessCode("DELETE");
		else
			printErrorCode(700);
	}
}

void Manager::printErrorCode(int n)
{
	flog << "========ERROR========\n";
	flog << n << "\n";
	flog << "=====================\n\n";
}

void Manager::printSuccessCode(string success)
{
	flog << "========" << success << "========\n";
	flog << "Success" << "\n";
	flog << "====================\n\n";
}