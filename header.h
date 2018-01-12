#include <stdio.h>
#include <stdlib.h>
#include<string.h>
struct node//stores all the records
{
	int key;
	char s1[4];
	char s2[5];
	int n1;
	int n2;
};	
struct Btree//structure used to implement btree
{
	int n;//no of keys
	int leaf;//indicates whether leaf node or not
	struct node *keys;//array of keys
	int *child;//array of child indices
};
typedef struct Btree btree; 

void create(btree *bt,int t);
void allocnode(btree *s,int t);
void insert(btree *bt,int t,int k,char s1[4],char s2[5],int n1,int n2,int *rootidx,int *no);
void split(btree *bt,int i,int t,int idx,int *no);
void insert_notfull(btree *bt,int t,int k,char s1[4],char s2[5],int n1,int n2,int idx,int *no);
void search(btree *bt,int k,int idx);
void rem(btree *bt,int k,int idx,int t,int *rootidx);
void adjust(btree *bt,int idx,int i,int t,int *rootidx);
void borrowprev(btree *bt,int idx,int i,int t);
void borrownext(btree *bt,int idx,int i,int t);
void merge(btree *bt,int idx,int i,int t,int *rootidx);
struct node getSucc(btree *bt,int idx,int t);
struct node getPred(btree *bt,int idx,int t);
void delnonleaf(btree *bt,int idx,int i,int t,int *rootidx);
void freemem(btree *bt,int *no);
