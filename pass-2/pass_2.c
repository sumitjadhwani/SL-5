#include<stdio.h>
#include<stdlib.h>
struct icode{
	
	char lc[20];
	char ins[20];
	char op1[20];
	char op2[20];
}entry;
int ltorg_calls=0;
struct symtbl
{
	char ind[10];
	char sym[20];
	char add[20];

};
int search_st(int val)
{
	struct symtbl obj;
	FILE *fp1;
	fp1=fopen("symtbl.txt","r+");
	fscanf(fp1,"%s %s %s",obj.ind,obj.sym,obj.add);
	int i=1;
	while(i!=val)
	{
		i++;
		fscanf(fp1,"%s %s %s",obj.ind,obj.sym,obj.add);
	}
	return atoi(obj.add);
}
int search_lt(int val)
{
	struct symtbl obj;
	FILE *fp1;
	fp1=fopen("littbl.txt","r+");
	fscanf(fp1,"%s %s %s",obj.ind,obj.sym,obj.add);
	int i=1;
	while(i!=val)
	{
		i++;
		fscanf(fp1,"%s %s %s",obj.ind,obj.sym,obj.add);
	}
	i=1;
	char temp[10];	
	while(obj.add[i]>='0' && obj.add[i]<='9')
	{
		temp[i-1]=obj.add[i-1];
		i++;
	}	
			temp[i-1]=obj.add[i-1];
	return atoi(temp);
	
}

char* substring(char str[100]) {
   
    int i=0,c = 0,j=0;
	char *sub;
	sub=(char*)malloc(13);

   for(j=0;j<10;j++)
   sub[j]='\0';
   while (isdigit(str[c])!=1) 
      c++;
   while((str[c]-'0')>=0 && (str[c]-'0'<=9))
   {
		sub[i++]=str[c++];
   }
   return (char*)sub;
	
}
void ltorg(int n)
{
	FILE *fp=fopen("litpool.txt","r+");
	FILE *fp1=fopen("littbl.txt","r+");
	int i=0;
	int arr[100];
	char t1[40],t2[40],t3[40];
	while(i!=n)
	{			
		fscanf(fp,"\n%s %s\n",t1,t2);
		i++;	
	}	
	fscanf(fp,"\n%s %s\n",t1,t2);

	fscanf(fp,"\n%s %s\n",t1,t3);
	char s1[40],s2[40],s3[40];
	i=0;
	int k=atoi(t2);

	while(i!=k-1)
	{
		i++;
		fscanf(fp1,"\n%s %s %s\n",s1,s2,s3);
	}
	i=0;
	int end=atoi(t3);
	if(isdigit(t3[0])==0)
	{
	end=1+k;
	}
	while(k<end )
	{
		fscanf(fp1,"%s %s %s",s1,s2,s3);
		//printf("%s\n",s2);
		char t=s2[1];
		///printf("%c\n",t);
		printf("%d\t00\t00\t%c\n",atoi(s3),t);
		k++;
	}	
	
}
void handle_ad()
{
		
	if(strcmp(substring(entry.ins),"03")==0)
		ltorg(ltorg_calls++);
	if(strcmp(substring(entry.ins),"02")==0)
	{

		ltorg(ltorg_calls++);
		exit(0);
	}
}
void handle_is()
{
	char* op=substring(entry.ins);
	if(strcmp(op,"00")==0)
		{
			printf("%s\t00\t00\t00\n",entry.lc);
			return;
		}
	char* ind=substring(entry.op1);
	char* val=substring(entry.op2);
	char lit[10];
	char sym[10];
	if(entry.op2[1]=='S')
		{
			int temp=search_st(atoi(val));
			printf("%s\t+%s\t%s\t%d\n",entry.lc,op,ind,temp);
		}
	else
		{
 			int temp2=search_lt(atoi(val));
			printf("%s\t+%s\t%s\t%d\n",entry.lc,op,ind,temp2);	
		
		}
}
void handle_dl()
{
	char *op=substring(entry.ins);
	
	if(strcmp(op,"1")==0)   //DC
	printf("%s\t00\t00\t%s\n",entry.lc,substring(entry.op1));
	
	else if(strcmp(op,"2")==0)//DS
	printf("%s\t00\t00\t00\n",entry.lc);

	
}


int main()
{
	FILE *fp1,*fp2,*fp3,*fp4;
	fp1=fopen("icode","r+");
	fp2=fopen("output","w+");
	char ins[100];
	fscanf(fp1,"%s %s %s %s",entry.lc,entry.ins,entry.op1,entry.op2);
	while(entry.lc[0]!='\0')
	{
		//printf("\n\n%s %s %s %s\n",entry.lc,entry.ins,entry.op1,entry.op2);
		if(entry.ins[1]=='A')
		{
			handle_ad();
		}
		else if(entry.ins[1]=='I')
		{
			handle_is();
		}
		else if(entry.ins[1]=='D')
		{
			handle_dl();
		}
		int i=0;
		for(i=0;i<20;i++)
		{
			entry.lc[0]='\0';
			entry.op1[i]='\0';
			entry.op2[i]='\0';
			entry.ins[i]='\0';
		}
		fscanf(fp1,"%s %s %s %s",entry.lc,entry.ins,entry.op1,entry.op2);
	}
}
