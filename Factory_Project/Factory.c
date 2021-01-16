#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int no,target_amount;
	float process_time,total_time,busy_till;
	struct node * next;
	int targets[];
	
}node;

void process(node * ,float *,int);
void show(node *,float [],int,int,float);
int choose(node *,node *);
node * units_input(node*,int, FILE *);
float find_max(float *,int);

int main()
{
	node * root =NULL;
	node * iter = root;
	int i,j,N,M,k;
	float *products,factory_time;
	FILE *input_file =fopen("input.txt","r");
	if(!input_file)
	{
		printf("An error occurred while opening the file.\nPlease make sure there is a file named input.txt!");
		return 0;
	}
		
	fscanf(input_file,"%d",&N); 
	root=units_input(root,N,input_file); // units input
	
	fscanf(input_file,"%d",&M);
	products=(float*)malloc(sizeof(float)*M); // products array memory allocate
	
	for(i=0;i<M;i++)
	
		fscanf(input_file,"%f",&products[i]); // product time input	
	
	float factory_start_time=products[0];
	for(i=0;i<M;i++)// chosing optimal way
	{
		iter=root;
		while(iter->target_amount!=0)
		{		
			process(iter,products,i);// processing time at the start and middle
			int chosen=choose(iter,root);// which way
			iter=root;	
			for(k=0;k<chosen;k++)
				iter=iter->next;// go chosen way
			
			k=0;
		}
		process(iter,products,i);// processing time at the end			
	}
	
	factory_time=find_max(products,M)-factory_start_time; // time between factory start and when the last product left the factory
	show(root,products,M,N,factory_time); // showing results
	printf("Process is completed.\nPlease check the output.txt file.");
	return 0;
}
float find_max(float * pr,int M)//Finding the max product output time
{
	float max = pr[0];// set the first item in pr as max
	int i;
	for(i=1;i<M;i++)
	{
		if(pr[i]>max)// if pr item i is bigger than max then set max as pr[i]
			max=pr[i];
	}
	return max;
}
node *units_input(node*root,int N,FILE * input_file)// Memory allocating for nodes
{
	int i,no,amount,j;
	float time;
	node * iter = root;	// set iter as root of the node
	for(i=0;i<N;i++)
	{	
		fscanf(input_file,"%d",&no);
		fscanf(input_file,"%f",&time);
		fscanf(input_file,"%d",&amount);//taking inputs
		
		if(i==0)//for the first node 
		{
			iter=(node*)malloc(sizeof(node)+sizeof(int)*amount);// allocate the memory with size of targets array plus size of node
			iter->next=NULL;// set next node null
			root=iter;// set root as iter
		}
			
		else
		{
			iter->next=(node*)malloc(sizeof(node)+sizeof(int)*amount);	// after than first node the process is same but root stands as before
			iter=iter->next;
			iter->next=NULL;
			
		}
		iter->no=no;
		iter->process_time=time;
		iter->target_amount=amount;	
		iter->total_time=0;
		iter->busy_till=0;// putting inputs into nodes
		
		for(j=0;j<amount;j++)
			fscanf(input_file,"%d ",&iter->targets[j]);	// taking the target node's numbers
	
	
	}
	return root;
}

void show(node * root,float products[],int m, int n,float ft)//showing results
{	
	FILE *output_file = fopen("output.txt","w");
	if(!output_file)
	{
		printf("An error occurred while writing the file!");
		return 0;
	}
		
	int i,j;
	fprintf(output_file,"Factory time is >> %.4f\n"
	"----------------------------------------------------------\n",ft);// show the max product output as factory time
	for(i=0;i<n;i++)
	{	
		fprintf(output_file,"Unit %d) Total Time >> %.4f \n",
		root->no,
		root->total_time);
		root=root->next;//showing each node's number and total process time
			
	}
	fprintf(output_file,"---------------------------------------------------------\n");
	for(i=0;i<m;i++)
			fprintf(output_file,"Product %d completed at >> %.4f\n",i,products[i]);// showing each product's output time	
		
}
int choose(node *iter,node *root)//chosing the minimum delay for target node's
{
	float min;
	int i,j,chosen,flag=1;
	node *temp;
	for(i=0;i<iter->target_amount;i++)// looking all possible next nodes
	{		
		temp=root;// set temp as root
		for(j=0;j<iter->targets[i];j++)//set the temp as first target node
			temp=temp->next;
		if(i==0)// if it is the first element in the array then set it as min and continue
		{
			min = temp->busy_till+temp->process_time;
			chosen=j;
			continue;	
		}
		if(temp->busy_till +temp->process_time< min) // if temp is lower than min then set min as temp's...
														//	...busy_till and set chosen as node number
			{			
				min =temp->busy_till+temp->process_time;
				chosen=j;		
			}			
	}
	return chosen;// return chosen node's number
}

void process(node * iter,float products[],int i)// processing time calculation
{		
	if(products[i]<iter->busy_till)// if product has come too early it has to wait
		products[i]=iter->busy_till;// set products input time as node's busy_till
		
	iter->total_time+=iter->process_time;// set node's process time as current total time plus node's process time
	iter->busy_till=products[i]+iter->process_time;// set node's busy time as product's arrived time plus node's process time
	products[i]=iter->busy_till; // set product's output time as node's new busy time
}


