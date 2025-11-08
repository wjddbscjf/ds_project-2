#include "Manager.h"

struct Command
{
	// Save the information contained in the line to the variables below
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
		// If the element does not enter
		c.name = "ERROR";
		return c;
	}
	else if (c.cmd == "ADD_ST")
	{
		if (ss >> c.option)
		{
			if (c.option == "dept_no")
			{
				if (!(ss >> c.dept)) // If the element does not enter
					c.name = "ERROR";
				return c;
			}
			else if (c.option == "name")
			{
				if (!(ss >> c.name)) // If the element does not enter
					c.name = "ERROR";
				return c;
			}
		}
		////If the element does not enter
		c.name = "ERROR";
		return c;
	}
	else if (c.cmd == "PRINT_ST")
	{
		if (!(ss >> c.dept)) // If the element does not enter
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
				ADD_BP(c.name, c.dept, c.id, c.income); // Execute a command
			}
		}
		else if (c.cmd == "SEARCH_BP")
		{
			if (c.name == "ERROR")
				printErrorCode(300); // ERROR
			else
			{
				if (c.option == "name")
				{
					SEARCH_BP_NAME(c.name); // Execute a command
				}
				else
				{
					SEARCH_BP_RANGE(c.name, c.option); // Execute a command
				}
			}
		}
		else if (c.cmd == "PRINT_BP")
		{
			if (c.name == "ERROR" || bptree->getRoot() == NULL)
				printErrorCode(400); // ERROR
			else
			{
				PRINT_BP(); // Execute a command
			}
		}
		else if (c.cmd == "ADD_ST")
		{
			if (c.name == "ERROR")
				printErrorCode(500); // ERROR
			else
			{
				if (c.option == "dept_no")
					ADD_ST_DEPTNO(c.dept); // Execute a command

				else if (c.option == "name")
					ADD_ST_NAME(c.name); // Execute a command
			}
		}
		else if (c.cmd == "PRINT_ST")
		{
			if (c.name == "ERROR")
				printErrorCode(600); // ERROR
			else
			{
				PRINT_ST(c.dept); // Execute a command
			}
		}
		else if (c.cmd == "DELETE")
		{
			DELETE(); // Execute a command
		}
		else if (c.cmd == "EXIT")
		{
			// EXIT command has no ERROR
			printSuccessCode("EXIT");
			return;
		}
	}
}

void Manager::LOAD()
{
	ifstream emp("employee.txt");
	if (!emp || bptree->getRoot() != nullptr)
		printErrorCode(100); // ERROR
	else
	{
		string line;
		while (getline(emp, line))
		{
			stringstream ss(line);
			string name;
			int dept, id, income;
			// Assigning information within a file as a variable
			ss >> name >> dept >> id >> income;
			if (ss.fail())
				continue;

			EmployeeData *newEmp = new EmployeeData(); // Declaration of data to be inserted
			newEmp->setData(name, dept, id, income);
			bptree->Insert(newEmp); // inserting
		}
		// Read all data within a file
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
	flog << "====================\n\n";
	bptree->Insert(data);
}

void Manager::SEARCH_BP_NAME(string name) // Search data by name
{
	auto node = bptree->searchDataNode(name);
	if (node == nullptr || bptree->getRoot() == nullptr)
	{
		// if can't find target node || BPtree has no data -> print ERROR
		printErrorCode(300);
	}
	else
	{
		auto iter = node->getDataMap()->find(name)->second; // Find information such as the target name within the node.
		flog << "=======SEARCH_BP=======" << endl;
		flog << name << "/" << iter->getDeptNo() << "/" << iter->getID() << "/" << iter->getIncome() << endl;
		flog << "=======================\n\n";
	}
}
void Manager::SEARCH_BP_RANGE(string start, string end)
{
	// Find emplooyee with the Range
	bptree->searchRange(start, end);
}

void Manager::ADD_ST_DEPTNO(int dept_no) // Find employee information with department code
{
	if (dept_no % 100 != 0 || dept_no < 100 || dept_no > 800)
	{
		// invalid dept_no
		printErrorCode(500);
		return;
	}
	if (!bptree || !bptree->getRoot())
	{

		printErrorCode(500);
		return;
	}
	int runIdx = dept_no / 100 - 1;
	EmployeeData **arr = bptree->searchDept_no(dept_no); // employees of having dept_no
	if (!arr)											 // no data
	{
		printErrorCode(500);
		return;
	}

	int i = 0;
	while (arr[i] != nullptr)
	{
		// Insert
		stree->Insert(arr[i]);
		++i;
	}
	delete[] arr;

	printSuccessCode("ADD_ST");
	return;
}

void Manager::ADD_ST_NAME(string name) // Insert employee information with target name
{
	auto tar = bptree->searchDataNode(name); // Find node information with target name
	if (tar == nullptr)						 // no node
	{
		printErrorCode(600);
		return;
	}
	auto data = tar->getDataMap();
	auto iter = data->find(name); // Find a map with a specific name within a node
	if (iter == data->end() || iter->second == nullptr)
	{
		// if no name in node
		printErrorCode(600);
		return;
	}

	bool ok = stree->Insert(iter->second); // insert

	if (!ok)
	{
		printErrorCode(600);
		return;
	}

	printSuccessCode("ADD_ST");
	return;
}

void Manager::PRINT_BP() // Output all data in the B+ Tree
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

void Manager::PRINT_ST(int dept) // Output target(dept_no) data in the Selection Tree
{
	if (dept % 100 != 0 || dept < 100 || dept > 800) // invalid dept_no
	{
		printErrorCode(600);
		return;
	}
	stree->printEmployeeData(dept); // Output
	return;
}

void Manager::DELETE() // deleting max value in Selection tree
{
	if (stree->getRoot() == nullptr) // if selectionTree is empty
	{
		printErrorCode(700);
		return;
	}
	else
	{
		if (stree->Delete())
			printSuccessCode("DELETE");
		else // delete fail
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