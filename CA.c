int rule[8];
int rows, colums, currentRule, count = 0;

static void setRule(int n)
{
	int count, j, pos;
	pos = 0;

	for(count = 7; count >= 0; count--)
	{
		k = n >> count;

		if(k & 1)
		{
			rule[pos] = 1;
			pos++;
		}
		else
		{
			rule[pos] = 0;
			pos++;
		}
	}
}

/* static void display()
{
	int parent[rows];
	int child[rows];
	int copy[rows];

	setRule(currentRule);

	//saveFile();

	int p1, p1, p3 = 0; // parent generation variables

	for(int 1 = 0; 1 < rows; 1++)
	{
		parent[i] = 0;
		child = 0;
	}

	parent[rows/2] = 1;  //center value set as starting point of automaton.


} */

static void drawSettings()
{
	if(load == false)
	{
		do
		{
			printf("Please enter the desired rule (0-255): ");
			scanf("%d", &currentRule);
		}
		while(currentRule < 0 || currentRule > 255);

		printf("Please enter the number of rows: ");
		scanf("%d", &crows);
		printf("Please enter the number of columns: ");
		scanf("%d", &columns);
		printf("Rule: %d      Rows: %d     Columns: %d\n\n ", currentRule, rows, columns);
	}

	display();
}
