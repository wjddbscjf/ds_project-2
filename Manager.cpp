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
	// need no parameter
	if (c.cmd == "LOAD" || c.cmd == "EXIT" || c.cmd == "PRINT_BP" || c.cmd == "DELETE")
	{
		// LOAD, EXIT, PRINT_BP, DELETE
		return c;
	}
	else if (c.cmd == "ADD_BP")
	{
		// ADD_BP
		if (ss >> c.name >> c.dept >> c.id >> c.income)
			return c;
		else
			c.name = "ERROR";
	}
	else if (c.cmd == "SEARCH_BP")
	{
		// SEARCH_BP
		if (ss >> c.name)
		{
			if (ss >> c.option)
			{
				return c;
			}
			return c;
		}
		else
			c.name = "ERROR";
	}
	else if (c.cmd == "ADD_ST")
	{
		// ADD_ST
		if (ss >> c.option)
		{
			if (c.option == "dept_no") // option is dept_no
			{
				if (ss >> c.income)
					return c;
				else
					c.name = "ERROR";
				return c;
			}
			else if (c.option == "name") // c.option is name
			{
				if (ss >> c.name)
					return c;
				else
					c.name = "ERROR";
				return c;
			}
			else
				c.name = "ERROR";
			return c;
		}
	}
	else if (c.cmd == "PRINT_ST")
	{
		if (ss >> c.dept)
			return c;
		else
			c.name = "ERROR";
		return c;
	}
}

void Manager::run(const char *command)
{
	fin.open(command);
	string line;
	while (getline(fin, line))
	{
		if (line.empty() && line.back() == '\r')
			line.pop_back();
		Command c;
		c = CommandParsing(line);
		if (c.cmd == "LOAD")
		{
			if (bptree->getRoot() == NULL || !fin)
			{
				printErrorCode(100);
			}
			else
				LOAD();
		}
		else if (c.cmd == "ADD_BP")
		{
			if (c.cmd == "ERROR")
				//// print ERROR code if parameter is less than '4'
				printErrorCode(200);
			else
			{
			}
		}
		else if (c.cmd == "SEARCH_BP")
		{
			if (c.cmd == "ERROR")
				printErrorCode(300);
			else
			{
				if (c.option.size() == 1)
				{
				}
				else if (c.option == "name")
				{
				}
			}
		}
		else if (c.cmd == "PRINT_BP")
		{
			if (c.cmd == "ERROR" || bptree->getRoot() == NULL)
				printErrorCode(400);
			else
			{
			}
		}
		else if (c.cmd == "ADD_ST")
		{
			if (c.cmd == "ERROR")
				printErrorCode(500);
			else
			{
				printErrorCode(100);
			}
		}
		else if (c.cmd == "PRINT_ST")
		{
			if (c.cmd == "ERROR")
				printErrorCode(600);
			else
			{
			}
		}
		else if (c.cmd == "DELETE")
		{
			if (c.cmd == "ERROR")
				printErrorCode(700);
			else
			{
			}
		}
		else
		{ // cmd is EXIT
			return;
		}
	}
}

void Manager::LOAD()
{
}

void Manager::ADD_BP()
{
}

void Manager::SEARCH_BP_NAME(string name)
{
}

void Manager::SEARCH_BP_RANGE(string start, string end)
{
}

void Manager::ADD_ST_DEPTNO(int dept_no)
{
}

void Manager::ADD_ST_NAME(string name)
{
}

void Manager::PRINT_BP()
{
}

void Manager::PRINT_ST()
{
}

void Manager::DELETE()
{
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