#define _CRT_SECURE_NO_WARNINGS
//implementation of resturant managing system using linked list.
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 50

#ifndef BBB
#define BBB
//node of a product that is held in the kitchen
typedef struct Product {
	char* name;
	int quantity;
	int price;
	int AmountOrdered;
	struct Product* next;
}Product,*PProduct;
//node of a product that is held in a table
typedef struct TProduct {
	char* name;
	int quantity;
	int price;
	struct TProduct* next;
	struct TProduct* prev;
}TProduct,*PTProduct;
//node of a single table 
typedef struct Table {
	struct TProduct *p;
	struct TProduct *head;
	int Bill;
	int ProductAmount;
}Table,PTable;
// manager for all tables
typedef struct Manager {
	int amount;
	int occupid;
	struct Table* arr;
}Manager;

PProduct CreateProducts(PProduct head, PProduct tail,FILE *manot,FILE*out);
void AddItems(PProduct head,char* c, int n,FILE *out);
int OrderItem(Manager m, char* c, int index, int amount, FILE* out,PProduct head);
void RemoveItem(int TableNumber, char* ProductName, int Quantity, Manager m, FILE* out);
void RemoveTable(int TableNumber, Manager m, FILE* out, PProduct head);
void Error_Msg(FILE* f, char* s);
void Delete_List(PProduct head);

#endif