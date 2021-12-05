#include "Header_HW3.h"

void main()
{
	PProduct head = NULL, tail = NULL;
	Manager m;
	FILE* instructions,*manot,*out;
	char name[MAX];
	int option,amount,i;
	m.occupid = 0; // reset occupid
	out = fopen("output.txt", "w"); // create output file
	if (!out)
		Error_Msg(out, "could not open file, kitchen was not created");
	fclose(out);
	instructions = fopen("instructions.txt", "r");
	if (!instructions)
		Error_Msg(out, "could not open file, kitchen was not created");
	fscanf(instructions, "%d", &(m.amount));//amount of tables that are available
	m.arr = (Table*)malloc(sizeof(struct Table)*(m.amount)); // allocate memory for the tables array
	if (!(m.arr))
		Error_Msg(out,"Memory allocation failed");
	for (i = 0; i < m.amount; i++)//reset all the data needed for each table
	{
		m.arr[i].head = NULL;
		m.arr[i].ProductAmount = 0;
		m.arr[i].Bill = 0;
	}
	while (fscanf(instructions, "%d", &option)==1) //read the next insturction from file
	{
		out = fopen("output.txt", "a");
		if (!out)
			Error_Msg(out, "could not open file, kitchen was not created");
		switch (option)
		{
		case 1:
			manot = fopen("Manot.txt", "r");
			if (!manot)
				Error_Msg(out, "could not open file, kitchen was not created");
			head = CreateProducts(head, tail, manot, out);
			fclose(manot);
			break;
		case 2:
			fscanf(instructions, "%s %d", name, &amount);
			AddItems(head, name, amount, out);
			break;
		case 3:
			fscanf(instructions, "%d %s %d", &i, name, &amount);
			m.occupid += OrderItem(m, name, i, amount, out, head); 
			break;
		case 4:
			fscanf(instructions, "%d %s %d", &i, name, &amount);
			RemoveItem(i, name, amount, m, out);
			break;
		case 5:
			fscanf(instructions, "%d", &i);
			RemoveTable(i, m, out, head);
			break;
		}
		fclose(out);
	}
	fclose(instructions);
	Delete_List(head);
	free(m.arr);
}