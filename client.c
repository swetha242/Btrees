#include "header.h"
int main()
{
	int t,i,j,key,n1,n2,ch,k,ch1;
	char s1[4],s2[5];
	btree *bt,s;
	printf("Enter value of t: ");
	scanf("%d",&t);
	int rootidx=0;//stores index of root in array
	int no=1;//indicates no of nodes allocated(used for freeing memory in fn freemem)
	FILE *f=fopen("dataset.csv","r");
	bt=(btree*)malloc(500000*sizeof(btree));//create a huge array of nodes
	create(bt,t);//create btree(i.e root)
	for(i=0;i<1000000;i++)
	{
		fscanf(f,"%d,%[^,],%[^,],%d,%d",&key,s1,s2,&n1,&n2);
		insert(bt,t,key,s1,s2,n1,n2,&rootidx,&no);
		
	}
	do
	{
		printf("Enter 1 to search and 2 to delete\n");
		scanf("%d",&ch);
		if(ch==1 || ch==2)
		{
			printf("Enter key of element : ");
			scanf("%d",&k);
			if(ch==1)
				search(bt,k,rootidx);
			else
				rem(bt,k,rootidx,t,&rootidx);
		}
		else
			printf("Wrong Entry!!\n");
		printf("Enter 1 to continue\n");
		scanf("%d",&ch1);
	}while(ch1==1);	
	
	freemem(bt,&no);
	free(bt);
	fclose(f);
	return 0;
}
