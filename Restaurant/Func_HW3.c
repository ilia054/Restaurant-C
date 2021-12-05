#include "Header_HW3.h"
/**function name:CreateProducts
input: pointers to the Product structer head and tail, pointers to file manot and out.
output:Creates one-way linked list in the kitchen of the products found in the manot file and their data. returns pointer to the head of the list 
algorithem: while loop that reads from file to temporary structer , if the read was succesfull allocates memory to new node
and copys the data to that node, then link the node to the end of the list.
*/
PProduct CreateProducts(PProduct head, PProduct tail, FILE* manot, FILE* out)
{
    Product temp;
    PProduct newp;
    char buffer[MAX];
    int cnt=0;
    while ((fscanf(manot, "%s %d %d", buffer, &temp.quantity, &temp.price))==3)
    {
        newp = (Product*)malloc(sizeof(struct Product)); //allocate memory to node
        if (newp == NULL)
        {
            Delete_List(head);
            Error_Msg(out, "Failed memory allocation, kitchen was not created.");
        }
        newp->next = NULL; // NULL to end the list
        newp->name = (char*)malloc(strlen(buffer)+1); //allocate memory for name
        if (!(newp->name))
        {
            free(newp); // free the memory allocated to newp before exiting
            Delete_List(head); // free all priveous memmory allocated 
            Error_Msg(out, "Failed memory allocation, kitchen was not created.");
        }
        strcpy(newp->name, buffer);
        newp->price = temp.price;            //copy data to structer
        newp->quantity = temp.quantity;
        newp->AmountOrdered = 0;
        if (!cnt) //if the list is empty appoint node as the head
            head = newp;
        else if (cnt == 1) // if the list has only 1 product , link the current head to the  new node
            head->next = newp;
        else tail->next = newp; // if the list has 2 or more products link the tail to the new node
        tail = newp; // assign new node as the new tail
        cnt++;//count holds current amount of products on the list
    }
    fprintf(out,"the kitchen was created");
    return head;
}
/**function name:AddItems
input:pointer to Product structer head ,char pointer ProductName ,integer quantity and FILE pointer out
output:Add supply to an existing dish.
algorithem:check if the dish exists in the kitchen product list and if the quantity is intact,find the dish and add to its quantity, print to file.
*/
void AddItems(PProduct head,char* ProductName ,int Quantity,FILE *out)
{
    PProduct temp=head;
    if (Quantity <= 0) //check if quantity is intact
    {
        fprintf(out, "\nquantity is lower or equal to zero");
        return;
    }
    while (temp != NULL)
    {
        if (!(strcmp(temp->name, ProductName))) //find the dish in the list
        {
            temp->quantity += Quantity; // add to quantity
            fprintf(out, "\n%d %s were added to the kitchen",Quantity,ProductName);
            return; 
        }
        temp = temp->next;
    }
    fprintf(out, "\nProduct does not exist in the stock"); //print to file if product was not found
    return;
}
/**function name:OrderItem
input:variable m for Manager structer, char pointer Productname, integers TableNumber and quantity, FILE pointer out and Product pointer head
output:add the order to the head of the two-way list of products in the table, the supply of the product in the kitchen will be reduced and the bill of the table will be updated.
function returns 1 if the order was succesfull, if not returns 0
algorithem:check if input is currect and dish exists in the kitchen, allocate memory for new node and product name, update
the list on the table, the product node ordered from the kitchen and bill of the table.
*/
int OrderItem(Manager m, char* ProductName, int TableNumber, int Quantity, FILE* out, PProduct head)
{
    PProduct temp = head;
    TableNumber -= 1;
    if (TableNumber > m.amount || TableNumber < 0) //check if the table number is ok
    {
        fprintf(out, "\nThe table does not exist");
        return 0;
    }
    while (strcmp(temp->name, ProductName)) //check if product exists in the kitchen and if so temp points to it
    {

        temp = temp->next;
        if (temp == NULL)
        {
            fprintf(out, "\nWe dont have %s, sorry!", ProductName);
            return 0;
        }
    }
    if (Quantity > temp->quantity || Quantity <= 0) // check if the supply is sufficent for the order
    {
        fprintf(out, "\nWe dont have %s, sorry!", temp->name);
        return 0;
    }
    {
        m.arr[TableNumber].p = (TProduct*)malloc(sizeof(struct TProduct)); // allocate memory to new node
        if (!(m.arr[TableNumber].p)) //check if allocation was succesfull
        {
            Delete_List(head); 
            Delete_List(m.arr[TableNumber].head);
            Error_Msg(out, "Memory allocation failed!");
        }
        m.arr[TableNumber].p->name = (char*)malloc(strlen(ProductName) + 1);// allocate memory for name of the product
        if (!(m.arr[TableNumber].p->name))//check if allocation was succesfull
        {
            Delete_List(head);
            Delete_List(m.arr[TableNumber].head);
            Error_Msg(out, "\nMemory allocation failed!");
        }
        strcpy(m.arr[TableNumber].p->name, ProductName);
        m.arr[TableNumber].p->price = temp->price;
        m.arr[TableNumber].p->quantity = Quantity; //copy all data to the product details
        m.arr[TableNumber].p->next = m.arr[TableNumber].head;// add product to the head of the list of products in the table
        m.arr[TableNumber].p->prev = NULL; 
        m.arr[TableNumber].head = m.arr[TableNumber].p; //update the head
        if ((m.arr[TableNumber].head->next) != NULL)
            m.arr[TableNumber].head->next->prev = head; //connect former head with new head
        temp->quantity -= Quantity;//update quantity of product in the kitchen
        temp->AmountOrdered += Quantity; // update the total amount ordered of the product
        m.arr[TableNumber].Bill += ((temp->price) * Quantity); // update the bill of the table
        out = fopen("output.txt", "a");
        if (!out)
        {
            Delete_List(head);
            Delete_List(m.arr[TableNumber].head);
            Error_Msg(out, "\ncould not open file, kitchen was not created");
        }
        fprintf(out, "\n%d %s were added to table number %d", Quantity, temp->name, TableNumber+1);
        fclose(out);
        return 1; 
    }
}
/**function name:RemoveItem
input:integers TableNumber and Quantity,char pointer ProductName, variable m for Manager structer and FILE pointer out
output:update a current order if the order is canceled/partly canceled
algorithem: check if the order exists in the table and the quantity ordered is higher or equal to the quantity wished to cancel.
update the quantity ordered and the bill.
*/
void RemoveItem(int TableNumber, char* ProductName, int Quantity, Manager m, FILE* out)
{
    PProduct temp;
    TableNumber -= 1;
    temp = m.arr[TableNumber].head;
    if (!temp)//check if the table has active orderes
    {
        fprintf(out, "\nThe table doesnt have an existing order");
        return;
    }
    while (strcmp(temp->name,ProductName)) // check if the the table ordered the product
    {
        temp = temp->next;
        if (temp == NULL)
        { 
            fprintf(out, "\nThe product was not ordered by this table");
            return;
        }
    }
    if (Quantity > temp->quantity)//chech if the quantity to cancel is higher then quantity ordered
    {
        fprintf(out, "\nthe Quantity to cancle is bigger than the quantity ordered");
        return;
    }
    temp->quantity -= Quantity;//update quantity ordered
    m.arr[TableNumber].Bill -= ((temp->price) * Quantity);//update bill
    if (temp->quantity == 0) //update ProductAmount if quantity is 0 
        m.arr[TableNumber].ProductAmount--;
    fprintf(out,"\n%d %s was returned to the kitchen from table number %d", Quantity, ProductName, TableNumber+1);
    return;
}
/**function name:RemoveTable
input:integer TableNumber, variable m for Manager structer, pointer to structer Product head, and FILE pointer out
output:print to file all the dishes that were ordered and the bill, free all memory allocated to the product list on the table
and if the table was the last table, prints the most ordered dish of the day and the amount it was ordered.
algorithem:check if table has active orders, print all dished ordered and the amount to file, free all memory allocated for the list 
of products in the table,check if the table was the last table and if so print to file the most ordered dish of the day and the amount it was ordered.
*/
void RemoveTable(int TableNumber, Manager m, FILE* out, PProduct head)
{
    TableNumber -= 1;
    PTProduct temp = m.arr[TableNumber].head;
    PProduct helper = head;
    int max=0;
    char dish[MAX];
    

    if (m.arr[TableNumber].head == NULL)//check if table has active oreders
    {
        fprintf(out,"\nThe table number %d is not ordered yet",TableNumber+1);
        return;
    }
    while (temp != NULL)//write to file all dished that were ordered and the amount
    {
        fprintf(out, "\n%d %s. ", temp->quantity,temp->name);
        temp = temp->next;
    }
    temp = m.arr[TableNumber].head;
    fprintf(out, "%d nis, please!", m.arr[TableNumber].Bill);//write to file the bill
    Delete_List(m.arr[TableNumber].head);//delete list of products in the table
    m.occupid--; //table is now free
    if (!(m.occupid)) //check if it is the last table
    {
        while (helper != NULL)
        {
            if (helper->AmountOrdered > max)//find the most ordered dish of the day
            {
                
                max = helper->AmountOrdered;
                strcpy(dish,helper->name);
            }
            helper = helper->next;
        }
        fprintf(out, "\nThe most popular dish today is %s! (was ordered %d times)", dish, max);
    }
    return;
}
/**function name:Delete_List
input:pointer to Product structer head
output:deletes all memory allocated on the list 
*/
void Delete_List(PProduct head)
{
    PProduct temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->name);
        free(temp);
    }
    return;
}
/**function name:Error_Msg
input:FILE pointer f and char pointer s
output:writes to file s string and exits program;
*/
void Error_Msg(FILE* f, char* s)
{
    f = fopen("output.txt", "a");
    if (!f)
        printf("could not open to file");
    else 
    {
        fprintf(f, "\n%s", s);
        fclose(f);
    }
    exit(1);
}