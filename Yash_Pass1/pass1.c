#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct OPTAB
{
	char mnemonic_code[10];
	int machine_code;
	char class[2];
	int no_operands;
	int chain;
}OPTAB;
OPTAB optab[27]={	
			{},
			{"ADD",1,"IS",2,-1},
			{"BC",7,"IS",1,-1},
			{"COMP",6,"IS",2,-1},
			{"DIV",8,"IS",2,7},
			{"END",2,"AD",0,6},
			{"EQU",5,"AD",0,-1},
			{"DC",1,"DL",1,8},
			{"DS",2,"DL",1,-1},
			{"MOVER",4,"IS",2,10},
			{"MOVEM",5,"IS",2,-1},
			{},
			{"LTORG",3,"AD",0,-1},
			{"MULT",3,"IS",2,9},
			{},
			{"ORIGIN",4,"AD",0,-1},
			{"PRINT",10,"IS",1,-1},
			{},
			{"READ",9,"IS",1,-1},
			{"STOP",0,"IS",0,20},
			{"SUB",2,"IS",0,21},
			{"START",1,"AD",0,-1},
			{},{},{},{},{}
		};
typedef union length_or_value
{
	int length;
	char value[5];
}lorv;

typedef struct SYMTAB
{
	char sym_name[10];
	int address;
	lorv var;
	int flag;
}SYMTAB;
SYMTAB symtab[20];

typedef struct LITTAB
{
	char lit_name[10];
	int address;
}LITTAB;
LITTAB littab[20];

int pooltab[10];
int lc,ptp,ltp,stp;
FILE * ic;
enum rc{areg=1,breg=2,creg=3,dreg=4};
void initialise()
{
	int i;
	lc=0;
	ptp=ltp=stp=1;
	pooltab[0]=1;
	for(i=1;i<10;i++)
	   pooltab[i]=-1;
	for(i=1;i<20;i++)
	  symtab[i].address=-1; 
}
int search(char * token )//search in mot
{
	int ind=(int)(token[0])-64;
	if(strcmp(token,optab[ind].mnemonic_code)==0)
		return ind;
	else
	{
		while(ind !=-1)
		{	
			ind=optab[ind].chain;
			if(strcmp(token,optab[ind].mnemonic_code)==0)
				return ind;
		}
		if(ind==-1)
		{
			printf("\n!Invalid mnemonic code %s",token);
				return -1;
		}
	}
}
void add_to_symtab(char symbol[],int lc,int length ,char value[],int flag)
{
	int i=1;
	while(strcmp(symtab[i].sym_name,"")!=0)
	  i++;
	strcpy(symtab[i].sym_name,symbol);
	if(lc!=-1)
		symtab[i].address=lc;
	else 
		symtab[i].address=-1;
	if(flag==1)
    {
		symtab[i].flag=1;
		strcpy(symtab[i].var.value,value);
	}
	else if(flag==0)
	{
		symtab[i].flag=0;
		symtab[i].var.length=length;
    }
    else
        symtab[i].flag=-1;
}
void add_to_littab(char literal[],int addr)
{
	int i=1;
	while(strcmp(littab[i].lit_name,"")!=0)
	  i++;
	strcpy(littab[i].lit_name,literal);
	if(addr!=-1)
		littab[i].address=addr;
	else
		littab[i].address=-1;
}
int search_in_symtab(char symbol[])
{
	int i=1;
	while(strcmp(symtab[i].sym_name,"")!=0)
	{
		if(strcmp(symtab[i].sym_name,symbol)==0)
		  return i;
		i++;
	}
	return -1;	
}
int search_in_littab(char literal[],int ptp)
{
	int i=ptp;
	while(strcmp(littab[i].lit_name,"")!=0)
	{
		if(strcmp(littab[i].lit_name,literal)==0)
		  return i;
		i++;
	}
	return -1;	
}
void process_tokens(char ** tokens,int no_of_tokens)
{
	int i,ind,ind1,inds,indl,lc_old,pool_pointer,last_token;
	char * arr[3];
	char * copy;
	char delimiter[]="+-";
	char delimiter_used;
	int lit_flag=0;
	if(strcmp(tokens[0],"-")!=0)
	{
		if(search_in_symtab(tokens[0])==-1)
			add_to_symtab(tokens[0],lc,0,"",-1);
	}
	if(strcmp(tokens[1],"START")==0||strcmp(tokens[1],"ORIGIN")==0)
	{
		if(strcmp(tokens[1],"START")==0)
		{
			ic=fopen("ic.text","w");
			lc_old=lc;
			lc=atoi(tokens[2]);
			fprintf(ic,"%d\tAD01 C%s\n",lc_old,tokens[2]);
			fclose(ic);
		}
		else
		{
			ic=fopen("ic.text","a");
			lc_old=lc;
			if(isalpha(*tokens[2]))
			{
				copy=strdup(tokens[2]);
				i=0;
				arr[i]=strtok(tokens[2],delimiter);
				while(arr[i]!=NULL)
				{
					i++;
					arr[i]=strtok(NULL,delimiter);
				}
				delimiter_used=copy[arr[0]-tokens[2]+strlen(arr[0])];
				last_token=i-1;
    			arr[last_token][strlen(arr[last_token])-2]='\0';
				ind=search_in_symtab(arr[0]);
				if(ind!=-1)
				  lc=symtab[ind].address;
				if(delimiter_used=='+')
				  lc+=atoi(arr[1]);
				else
				  lc-=atoi(arr[1]);
			}
			else
			{
				lc=atoi(tokens[2]);
			}
			fprintf(ic,"%d\tAD04 C%d\n",lc_old,lc);
			lc_old=lc;
			fclose(ic);
		}	
	}
	else if(strcmp(tokens[1],"LTORG")==0)
	{
		ic=fopen("ic.text","a");
		fprintf(ic,"%d\tAD03\n",lc);
		pool_pointer=0;
		while(pooltab[pool_pointer]!=-1)
		     pool_pointer++;
		pool_pointer--;
		i=pooltab[pool_pointer];
		while(strcmp(littab[i].lit_name,"")!=0)
		{
			
			littab[i].address=lc;
			lc++;
			i++;
		}
		pooltab[pool_pointer+1]=i;	
		fclose(ic);
	}
	else if(strcmp(tokens[1],"END")==0)
	{
		ic=fopen("ic.text","a");
		fprintf(ic,"%d\tAD02\n",lc);
		pool_pointer=0;
		while(pooltab[pool_pointer]!=-1)
		{
			pool_pointer++;
		}
		pool_pointer--;
		i=pooltab[pool_pointer];
		while(strcmp(littab[i].lit_name,"")!=0)
		{
			littab[i].address=lc;
			lc++;
			i++;
		}	
		fclose(ic);	
	}
	else if(strcmp(tokens[1],"EQU")==0)
	{
		
		ic=fopen("ic.text","a");
		fprintf(ic,"%d\tAD05\n",lc);
		ind=search_in_symtab(tokens[0]);
		if(ind!=-1)
		{
			ind1=search_in_symtab(tokens[2]);
			if(ind1!=-1)
			 symtab[ind].address=symtab[ind1].address;
		}
		fclose(ic);
	}
	else if(strcmp(tokens[1],"DS")==0||strcmp(tokens[1],"DC")==0)
	{
		ic=fopen("ic.text","a");
		if(strcmp(tokens[1],"DS")==0)
		{
			fprintf(ic,"%d\tDL02 C%s\n",lc,tokens[2]);
			ind=search_in_symtab(tokens[0]);
			if(ind!=-1)
			{
				symtab[ind].address=lc;
				symtab[ind].var.length=atoi(tokens[2]);
				symtab[ind].flag=0;
			}
			lc=lc+atoi(tokens[2]);
		}
		else
		{
			fprintf(ic,"%d\tDL01 C%s\n",lc,tokens[2]);
			ind=search_in_symtab(tokens[0]);
			if(ind!=-1)
			{
				symtab[ind].address=lc;
				strcpy(symtab[ind].var.value,tokens[2]);
				symtab[ind].flag=1;
			}
			lc=lc+1;
		}
		fclose(ic);
	}
	else if(strcmp(tokens[1],"STOP")!=0)
	{
		ic=fopen("ic.text","a");
		ind=search(tokens[1]);
		if(ind==-1)
			printf("\nerror! mnemonic %s not found ",tokens[1]);
		else
		{
			if(tokens[3]!=NULL)
			{
				if(isalpha(*tokens[3]))
				{
					inds=search_in_symtab(tokens[3]);
					if(inds==-1)
						add_to_symtab(tokens[3],-1,-1,"-1",-1);
					lit_flag=0;
				}
				else
				{
					lit_flag=1;
					i=0;
					while(pooltab[i]!=-1)
					{
						i++;
					}
					indl=search_in_littab(tokens[3],pooltab[i-1]);
					if(indl==-1)
						add_to_littab(tokens[3],-1);
				}
			}	
		}
		if(strcmp(tokens[2],"AREG")==0)
		{
			if(lit_flag)
			{
				indl=search_in_littab(tokens[3],pooltab[i-1]);
				fprintf(ic,"%d\tIS%d R01 L%d\n",lc,optab[ind].machine_code,indl);
			}
			else
			{
				inds=search_in_symtab(tokens[3]);
				fprintf(ic,"%d\tIS%d R01 S%d\n",lc,optab[ind].machine_code,inds);
			}
		}
		else if(strcmp(tokens[2],"BREG")==0)
		{
			if(lit_flag)
			{
				indl=search_in_littab(tokens[3],pooltab[i-1]);
				fprintf(ic,"%d\tIS%d R02 L%d\n",lc,optab[ind].machine_code,indl);
			}
			else
			{
				inds=search_in_symtab(tokens[3]);
				fprintf(ic,"%d\tIS%d R02 S%d\n",lc,optab[ind].machine_code,inds);
			}
		}
		else if(strcmp(tokens[2],"CREG")==0)
		{
			if(lit_flag)
			{
				indl=search_in_littab(tokens[3],pooltab[i-1]);
				fprintf(ic,"%d\tIS%d R03 L%d\n",lc,optab[ind].machine_code,indl);
			}
			else
			{
				inds=search_in_symtab(tokens[3]);
				fprintf(ic,"%d\tIS%d R03 S%d\n",lc,optab[ind].machine_code,inds);
			}
		}
		else
		{
			printf("\n in dreg");
			if(lit_flag)
			{
				indl=search_in_littab(tokens[3],pooltab[i-1]);
				fprintf(ic,"%d\tIS%d R04 L%d\n",lc,optab[ind].machine_code,indl);
			}
			else
			{
				inds=search_in_symtab(tokens[3]);
				fprintf(ic,"%d\tIS%d R04 S%d\n",lc,optab[ind].machine_code,inds);
			}
		}
		lc++;
		fclose(ic);
	}
	else
	{
		ic=fopen("ic.text","a");
		fprintf(ic,"%d\tIS00\n",lc);
		lc++;
		fclose(ic);
	}
} 
int main()
{
	initialise();
	FILE * fp;
	int line_count=0,no_of_tokens,i;
	char *line_buf=NULL,*token=NULL,ch;
	char **tokens=NULL;
	int last_token;
	ssize_t line_size;
	size_t line_buf_size=0;
	fp=fopen("input.txt","r");
	while(!feof(fp))
	{
		ch=getc(fp);
		if(ch=='\n')
			line_count++;
	}
	printf("no of lines:%d",line_count);
	rewind(fp);
	line_size = getline(&line_buf, &line_buf_size, fp);
	while (line_size > 0)
	{
		tokens=(char **) malloc(4*sizeof(char*));
		for(i=0;i<4;i++)
		{
			tokens[i]=(char*)malloc(10*sizeof(char));
		}
		no_of_tokens=0;
		i=0;
	    token = strtok(line_buf, " "); 
    	while (token != NULL) 
		{ 
			no_of_tokens++;
			strcpy(tokens[i++],token);
	       	token = strtok(NULL, " "); 
    	}
    	last_token=no_of_tokens-1;
    	tokens[last_token][strlen(tokens[last_token])-2]='\0';
   		process_tokens(tokens,no_of_tokens);	 
    	line_size = getline(&line_buf, &line_buf_size, fp);  
    	free(tokens); 
	}
	line_buf=NULL;
	printf("\n--------------SYMBOL TABLE----------------");
	i=1;
	while(strcmp(symtab[i].sym_name,"")!=0)
	{
		if(symtab[i].flag==1)
		   printf("\n%d %s\t %d %s",i,symtab[i].sym_name,symtab[i].address,symtab[i].var.value);
		else if(symtab[i].flag==0)
		   printf("\n%d %s\t %d %d",i,symtab[i].sym_name,symtab[i].address,symtab[i].var.length);
		else
		   printf("\n%d %s\t %d -",i,symtab[i].sym_name,symtab[i].address);
		i++;
	}
	printf("\n--------------LITERAL TABLE----------------");
	i=1;
	while(strcmp(littab[i].lit_name,"")!=0)
	{
		printf("\n%d %s\t %d",i,littab[i].lit_name,littab[i].address);
		i++;
	}
	printf("\n------------------------------\n");
	fclose(fp);
	fp=fopen("symtab.txt","w");
	fprintf(fp,"--------------SYMBOL TABLE----------------");
	i=1;
	while(strcmp(symtab[i].sym_name,"")!=0)
	{
		if(symtab[i].flag==1)
		   fprintf(fp,"\n%d %s\t %d %s",i,symtab[i].sym_name,symtab[i].address,symtab[i].var.value);
		else if(symtab[i].flag==0)
		   fprintf(fp,"\n%d %s\t %d %d",i,symtab[i].sym_name,symtab[i].address,symtab[i].var.length);
		else
		   fprintf(fp,"\n%d %s\t %d -",i,symtab[i].sym_name,symtab[i].address);
		i++;
	}
	fprintf(fp,"\n------------------------------\n");
	fclose(fp);
	fp=fopen("littab.txt","w");
	fprintf(fp,"--------------LITERAL TABLE----------------");
	i=1;
	while(strcmp(littab[i].lit_name,"")!=0)
	{
		fprintf(fp,"\n%d %s\t %d",i,littab[i].lit_name,littab[i].address);
		i++;
	}
	fprintf(fp,"\n------------------------------\n");
	fclose(fp);
	return 0;
}
