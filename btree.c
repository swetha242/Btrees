#include "header.h"
//create btree(i.e only root initially at index 0)
void create(btree *bt,int t)
{
	bt[0].n=0;
	bt[0].leaf=1;
	bt[0].keys=(struct node*)malloc((2*t-1)*sizeof(struct node));
	bt[0].child=(int*)malloc(2*t*sizeof(int));
	
}
//allocate memory for keys and child of a new node
void allocnode(btree *s,int t)
{
	(*s).keys=(struct node*)malloc((2*t-1)*sizeof(struct node));
	(*s).child=(int*)malloc(2*t*sizeof(int));	
	
}
//insert a record in btree bt 
void insert(btree *bt,int t,int k,char s1[4],char s2[5],int n1,int n2,int *rootidx,int *no)
{
	//if root is full split it
	if(bt[*rootidx].n==2*t-1)
	{
		btree s;//s is the new root
		s.leaf=0;
		s.n=0;
		//allocate memory for s(key and child array)
		allocnode(&s,t);
		s.child[0]=*rootidx;//child of new node now points to old rootnode
		*rootidx=*no;//update rootidx to next free index
		bt[(*no)++]=s;//make s the root
		split(bt,0,t,*rootidx,no);//split root
		insert_notfull(bt,t,k,s1,s2,n1,n2,*rootidx,no);//insert k into btree		
	}
	else
		insert_notfull(bt,t,k,s1,s2,n1,n2,*rootidx,no);	
}
void split(btree *bt,int i,int t,int idx,int *no)
{
	btree x,z,y;//z and y are children of x 
	x=bt[idx];
	allocnode(&z,t);
	y=bt[x.child[i]];//y is child of x that is full
	z.leaf=y.leaf;
	z.n=t-1;
	int j;
	//copy t-1 keys from y to z
	for(j=0;j<t-1;j++)
		z.keys[j]=y.keys[j+t];
	//if y is not leaf copy children
	if(y.leaf==0)
	{
		for(j=0;j<t;j++)
			z.child[j]=y.child[j+t];
	}
	//update no. of keys of y to t-1
	bt[x.child[i]].n=t-1;
	//shift children nodes of x by 1 to the right
	for(j=x.n;j>i;j--)
		x.child[j+1]=x.child[j];
	//insert z at the next available free node
	bt[(*no)++]=z;
	//make z x's child
	x.child[i+1]=(*no)-1;
	//shift keys by one to the right
	for(j=x.n-1;j>=i;j--)
		x.keys[j+1]=x.keys[j];
	x.keys[i]=y.keys[t-1];//add t-1'th key to x
	x.n=x.n+1;
	bt[idx]=x;
}	
//insert element into a btree whose root is not full		
void insert_notfull(btree *bt,int t,int k,char s1[4],char s2[5],int n1,int n2,int idx,int *no)
{
	int i=bt[idx].n-1;
	if(bt[idx].leaf==1)//if leaf node
	{
		while(i>=0 && k<bt[idx].keys[i].key)
		{
			bt[idx].keys[i+1]=bt[idx].keys[i];
			i=i-1;
		}
		//insert element at right position(i.e keys are sorted)
		bt[idx].keys[i+1].key=k;
		strcpy(bt[idx].keys[i+1].s1,s1);
		strcpy(bt[idx].keys[i+1].s2,s2);
		bt[idx].keys[i+1].n1=n1;
		bt[idx].keys[i+1].n2=n2;
		bt[idx].n+=1;
	}
	else//not a leaf
	{
		int j;
		while(i>=0 && k<bt[idx].keys[i].key)
			i=i-1;
		i=i+1;
		j=bt[idx].child[i];//index of child
		if(bt[j].n==2*t-1)//if child is full call split
		{
			split(bt,i,t,idx,no);
			if(k>bt[idx].keys[i].key)
				i=i+1;
		}
		insert_notfull(bt,t,k,s1,s2,n1,n2,bt[idx].child[i],no);			
	}
}
//search for element whose key is k
void search(btree *bt,int k,int idx)
{
	int i=0;
	btree x=bt[idx];
	while(i<x.n && k>x.keys[i].key)
		i+=1;
	if(i<x.n && k==x.keys[i].key)//element found
		{
		printf("\n%d found at index %d in node at location %d\n",k,i,idx);
		printf("Record: %d %s %s %d %d\n",k,x.keys[i].s1,x.keys[i].s2,x.keys[i].n1,x.keys[i].n2);
		return;
		}
	else if(x.leaf)
	{
		printf("Element not found\n");
		return;
	}
	else//search the children nodes
		search(bt,k,x.child[i]);
}
//remove an element whose key is k
void rem(btree *bt,int k,int idx,int t,int *rootidx)
{
	btree x=bt[idx];
	int i=0;
	while(i<x.n && k>x.keys[i].key)
		i+=1;
	if(i<x.n && k==x.keys[i].key)
	{
		if(x.leaf)//remove from leaf
		{
			int j;	
			//shift keys to the left		
			for(j=i;j<x.n-1;j++)
				bt[idx].keys[j]=bt[idx].keys[j+1];
			bt[idx].n-=1;//update no of keys
		}
		else//remove from non-leaf
		{
			delnonleaf(bt,idx,i,t,rootidx);
		}
	}
	else
	{
		if(x.leaf)
			{
				printf("Key not present\n");
			}
		else
		{
			int flag,c1,c2;
			//flag set to 1 if element might be in the last child of x
			if(i==x.n)
				flag=1;
			else
				flag=0;
			c1=bt[idx].child[i];
			c2=bt[idx].child[i-1];
			//if the child has less than t keys call adjust
			if(bt[c1].n<t)
				adjust(bt,idx,i,t,rootidx);
				
			if(flag && i>bt[idx].n)
				rem(bt,k,c2,t,rootidx);
			else
				rem(bt,k,c1,t,rootidx);
		}
	}
		
}
/*On traversing a tree during deletion,if node has less than t keys,adjust is called so that node has at least keys*/
void adjust(btree *bt,int idx,int i,int t,int *rootidx)
{
	//if node not the first child and previous sibling has atleast t keys
	if(i!=0 && bt[bt[idx].child[i-1]].n>=t)
		borrowprev(bt,idx,i,t);
	//if node not last child and next sibling has atleast t keys
	else if(i!=bt[idx].n && bt[bt[idx].child[i+1]].n>=t)
		borrownext(bt,idx,i,t);
	else//both siblings have t-1 nodes
	{
		if(i!=bt[idx].n)//merge with next sibling
			merge(bt,idx,i,t,rootidx);
		else//merge with previous sibling
			merge(bt,idx,i-1,t,rootidx);
	}
}
//fn to borrow last key from previous sibling(i-1th child) and insert it into ith child
void borrowprev(btree *bt,int idx,int i,int t)
{
	int n1,n2,c1,c2,j;
	c1=bt[idx].child[i];
	c2=bt[idx].child[i-1];	
	n1=bt[c1].n;
	n2=bt[c2].n;
	//shift keys by 1
	for(j=n1-1;j>=0;j--)
		bt[c1].keys[j+1]=bt[c1].keys[j];
	//if not a leaf shift children by 1
	if(bt[c1].leaf==0)
	{
		for(j=n1;j>=0;j--)
			bt[c1].child[j+1]=bt[c1].child[j];
	}
	//set child's first key to i-1th key of parent
	bt[c1].keys[0]=bt[idx].keys[i-1];
	if(bt[c1].leaf==0)//if child not leaf set child's first child to sibling's last child
		bt[c1].child[0]=bt[c2].child[n2];
	bt[idx].keys[i-1]=bt[c2].keys[n2-1];
	bt[c1].n+=1;
	bt[c2].n-=1;
}
//fn to borrow first key from next sibling(i+1th child) and insert it into ith child
void borrownext(btree *bt,int idx,int i,int t)
{
	int n1,n2,c1,c2,j;
	c1=bt[idx].child[i];//current child
	c2=bt[idx].child[i+1];//next child	
	n1=bt[c1].n;
	n2=bt[c2].n;
	//keys[i] is inserted as last child of c1
	bt[c1].keys[n1]=bt[idx].keys[i];
	if(bt[c1].leaf==0)//1st child of c2 is set as last child of c1
		bt[c1].child[n1+1]=bt[c2].child[0];
	bt[idx].keys[i]=bt[c2].keys[0];//1st key of c2 inserted as i'th key of parent
	
	//shift keys and children nodes of c2 by 1
	for(j=1;j<n2;j++)
		bt[c2].keys[j-1]=bt[c2].keys[j];
		
	if(bt[c2].leaf==0)
	{
		for(j=1;j<=n2;j++)
			bt[c2].child[j-1]=bt[c2].child[j];
	}
	bt[c1].n+=1;//update no of keys
	bt[c2].n-=1;
}
//delete an element from a non leaf node
void delnonleaf(btree *bt,int idx,int i,int t,int *rootidx)
{
	int n1,n2,c1,c2,k;
	struct node pred,succ;
	c1=bt[idx].child[i];
	c2=bt[idx].child[i+1];
	k=bt[idx].keys[i].key;
	n1=bt[c1].n;//no of keys in both it's children
	n2=bt[c2].n;
	if(n1>=t)//left sibling has at least t keys
	{
		pred=getPred(bt,c1,t);//get predecessor
		bt[idx].keys[i]=pred;//pred replaces element to be deleted
		rem(bt,pred.key,c1,t,rootidx);//remove pred
	}
	else if(n2>=t)//right sibling has atleast t keys
	{
		succ=getSucc(bt,c2,t);//get succesor
		bt[idx].keys[i]=succ;//succ replaces element to be deleted 			
		rem(bt,succ.key,c2,t,rootidx);//remove succ
	}
	else//both siblings don't have t keys
	{
		merge(bt,idx,i,t,rootidx);//merge the siblings
		rem(bt,k,c1,t,rootidx);//remove key k from merged node
		
	}
}
//fn to get predecessor
struct node getPred(btree *bt,int idx,int t)
{
	btree x=bt[idx];
	//keep moving to rightmost node until you reach leaf
	while(x.leaf==0)
	{
		x=bt[x.child[x.n]];
	}
	return(x.keys[x.n-1]);//return last key
}
//fn to get successor
struct node getSucc(btree *bt,int idx,int t)
{
	btree x=bt[idx];
	//keep moving to leftmost node until you reach leaf
	while(x.leaf==0)
		x=bt[x.child[0]];
	return(x.keys[0]);//return first key
}
//merge child[i] and child[i+1] of parent at index idx
void merge(btree *bt,int idx,int i,int t,int *rootidx)
{
	int n1,n2,c1,c2,j;
	c1=bt[idx].child[i];
	c2=bt[idx].child[i+1];
	n1=bt[c1].n;
	n2=bt[c2].n;
	// Pulling a key from the current node and inserting it into (t-1)th position of child[i]
	bt[c1].keys[t-1]=bt[idx].keys[i];
	// Copying the keys from child[i+1] to child[i] at the end
	for(j=0;j<n2;j++)
		bt[c1].keys[j+t]=bt[c2].keys[j];
	// Copying the children from child[i+1] to child[i] at the end
	if(bt[c1].leaf==0)
	{
		for(j=0;j<=n2;j++)
			bt[c1].child[j+t]=bt[c2].child[j];
	}
	// Moving all keys and children after i in the current node one step before
    
	for(j=i+1;j<bt[idx].n;j++)
		bt[idx].keys[j-1]=bt[idx].keys[j];
	for(j=i+2;j<=bt[idx].n;j++)
		bt[idx].child[j-1]=bt[idx].child[j];
	bt[c1].n+=bt[c2].n+1;
	bt[idx].n-=1;
	//update rootidx if entire root deleted
	if(idx==*rootidx && bt[idx].n==0)
		*rootidx=c1;		
}
//function to free memory
void freemem(btree *bt,int *no)
{
	int i;
	for(i=0;i<(*no);i++)
	{	
			
		free(bt[i].keys);
		free(bt[i].child);
		
		
	}
}
		
	
		
							
	
	
