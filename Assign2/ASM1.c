#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define errinfile(s) 	printf(" Error in opening file %s \n",s);
#define cout(x) 		printf(" %d\n",x);
#define newline			printf("\n")
#define deb(x)			printf("\n"#x"\t %d\n",x);	

typedef struct MOT{
	char meno[30];
	int  opcode;
	int  type;
	int  size;
	int  chain;
}MOT;

typedef struct ST{
	char symbol[30];
	int address;
}ST;

typedef struct output{
	int location_counter;
	char mneno_opcode[30];
	char op1[30];
	char op2[30];
}output;

typedef struct LT{
	char literal[30];
	int address;
}LT;

MOT mnemonics_table[30];
int meno_table_len=0;

void createMOT(){
    char * line = NULL;
    char * words[5];
    size_t len = 0;
	FILE *fp = fopen("MOT.txt","r");
	if(fp==NULL)
		errinfile("MOT.txt");
	int j=0;
    while ((getline(&line, &len, fp)) != -1) {
    	char *token;
		   /* get the first token */
	   token = strtok(line," ");
	   /* walk through other tokens */
	   int i=0;
	   while( token != NULL ) {
	   	  words[i++]=token;
		  token = strtok(NULL," ");
	   }
		// printf(" %s",words[0]);
		strcpy(mnemonics_table[j].meno,words[0]);
		mnemonics_table[j].opcode=atoi(words[1]);
		mnemonics_table[j].type=atoi(words[2]);
		mnemonics_table[j].size=atoi(words[3]);
		mnemonics_table[j].chain=atoi(words[4]);
		j++;
		meno_table_len++;
	}

	// Print the MOT 
	// for(int j=0;j<13;j++){
	// 	printf("%d\t %s %d\n",j,mnemonics_table[j].meno,mnemonics_table[j].type);
	// }
}


int search_mnemo(char * str){
	for(int i=0;i<meno_table_len;i++)
		if(strcmp(str,mnemonics_table[i].meno)==0)
			return i;
	return -1;
}



int search_symbol(char * str,ST * symbol_table,int len){
	for(int i=0;i<len;i++)
		if(strcmp(str,symbol_table[i].symbol)==0)
			return i;
	return -1;
}


int is_reg(char * str){
	if(
		strcmp(str,"AREG")==0 ||
		strcmp(str,"BREG")==0 ||
		strcmp(str,"CREG")==0 ||
		strcmp(str,"DREG")==0
	)
	return 1;
	return 0;
}


int extract_literal(char *str){
	int ans=0;
	int i=1;
	while(1){
		ans+=(int)(str[i]-'0');
        if(str[i+1]=='\'') break;
		ans*=10;
        i++;
	}
	return ans;
}

int search_literal(char *word,LT *lt_table,int start,int end){
	for(int i=start;i<end;i++)
	if(strcmp(word,lt_table[i].literal)==0)
			return i;
	return -1;
}

void print_LT(LT *lt_table,int size){
	printf("Literal\taddress\n");
	for(int i=0;i<size;i++)
		printf("%s\t%d\n",lt_table[i].literal,lt_table[i].address);
	printf("\n");
}

void print_ST(ST *st_table,int size){
	printf("Symbol\taddress\n");
	for(int i=0;i<size;i++)
		printf("%s\t%d\n",st_table[i].symbol,st_table[i].address);
	printf("\n");
}

void print_output(output out){
	printf("%d\t%s\t%s\t%s\n",out.location_counter,out.mneno_opcode,out.op1,out.op2);
}

int main()
{
	createMOT();
	int symbol_table_size=0;ST symbol_table[30];
	int literal_table_size=0,pull_index_of_literal=0;LT literal_table[30];
    FILE * fp;
    char * line = NULL;
    char * words[5];
    size_t len = 0;
	int k=0,stopflag=0;
	output out;
	out.location_counter=0;
	int mnemo_type=0;
    fp = fopen("code.asm", "r");
    if (fp == NULL)
        errinfile("code.asm");

    while ((getline(&line, &len, fp)) != -1) {
    	char *token;
		   /* get the first token */
	   token = strtok(line," ");
	   /* walk through other tokens */
	   int i=0;
	   while( token != NULL ) {
	   	  words[i++]=token;
		  token = strtok(NULL," ");
	   }
	   k=0;
	   while(1){
		   if(words[i-1][k]=='\n'){
		   		words[i-1][k]='\0';
				break;
		   }
		   k++;
	   }
		int len_words=i;
        int magic_index=0;

	//    for(int j=0;j<len_words;j++)
	//    	printf("%s ",words[j]);printf("\n");

		//Pass 1 Start
		int mnemo_index=search_mnemo(words[0]);
		// deb(mnemo_index);
		if(mnemo_index==-1){
			//means it is Label
			//Store it in Symbol if present already then change the address
			k=0;
			for(k=0;k<symbol_table_size;k++)
			if(strcmp(words[0],symbol_table[k].symbol)==0)
				break;
			if(k<symbol_table_size){
				//stopflag
				symbol_table[k].address=out.location_counter;
			}
			else{
				strcpy(symbol_table[symbol_table_size].symbol,words[0]);
				symbol_table[symbol_table_size].address=out.location_counter;
				symbol_table_size++;
			}
            magic_index=1;
		}
		mnemo_index=search_mnemo(words[0+magic_index]);
		mnemo_type=mnemonics_table[mnemo_index].type;

			if(mnemo_type==1){
				sprintf(out.mneno_opcode,"AD,%d",mnemonics_table[mnemo_index].opcode);
				strcpy(out.op1,"--");
				strcpy(out.op2,"--");
				if(strcmp(words[0+magic_index],"START")==0){
					if(len_words==2)
					sprintf(out.op2,"C,%d",atoi(words[1+magic_index]));
					else sprintf(out.op2,"C,%d",0);
					print_output(out);
					if(len_words>(1+magic_index)) out.location_counter=atoi(words[1+magic_index]);
					else out.location_counter=0;
				}
				else if(strcmp(words[0+magic_index],"END")==0){
					//Nothing for Now
					// deb(pull_index_of_literal);
					// deb(literal_table_size);
					print_output(out);
					for(int i=pull_index_of_literal;i<literal_table_size;i++){
						literal_table[i].address=out.location_counter;
						sprintf(out.mneno_opcode,"----");
						sprintf(out.op1,"--");
						sprintf(out.op2,"--");
						print_output(out);
						out.location_counter+=1; //Size of Literal
					}
					pull_index_of_literal=literal_table_size;
				}
				else if(strcmp(words[1],"EQU")==0){
					print_output(out);
					int A=search_symbol(words[0],symbol_table,symbol_table_size);
					int B=search_symbol(words[2],symbol_table,symbol_table_size);
					if(A==-1||B==1)
					printf("Error in EQU \n");
					symbol_table[A].address=symbol_table[B].address;

				}
				else if(strcmp(words[0+magic_index],"ORG")==0){
					print_output(out);
					out.location_counter=atoi(words[1+magic_index]);
				}
				else if(strcmp(words[0+magic_index],"LTORG")==0){
					//Do the Literal storing thing
					print_output(out);
					for(int i=pull_index_of_literal;i<literal_table_size;i++){
						literal_table[i].address=out.location_counter;
						sprintf(out.mneno_opcode,"----");
						sprintf(out.op1,"--");
						sprintf(out.op2,"--");
						print_output(out);
						out.location_counter+=1; //Size of Literal
					}
					pull_index_of_literal=literal_table_size;
				}
			}

			else if(mnemo_type==2){
				sprintf(out.mneno_opcode,"IS,%d",mnemonics_table[mnemo_index].opcode);
				strcpy(out.op1,"00");
				strcpy(out.op2,"00");
                //For handling two Operand
				if(strcmp(mnemonics_table[mnemo_index].meno,"STOP")==0){
						stopflag=1;
				}
				else if(len_words==(3+magic_index))
				{
					sprintf(out.op1,"R%d",(int)words[1+magic_index][0]-64);
					//May be Symbol or literal
					if(words[2+magic_index][0]=='\'')//means it is a literal
					{
						int index=search_literal(words[2+magic_index],literal_table,pull_index_of_literal,literal_table_size);
						if(index==-1)
						{
							strcpy(literal_table[literal_table_size].literal,words[2+magic_index]);
							literal_table[literal_table_size].address=-1;
							sprintf(out.op2,"L,%d",literal_table_size);
							literal_table_size++;
						}
						if(index!=-1)
						sprintf(out.op2,"L,%d",index);
					}
						
					else if(!is_reg(words[2+magic_index]))//check it ,is it symbol or reg
					{
						int index=search_symbol(words[2+magic_index],symbol_table,symbol_table_size);
						if(index==-1)
						{
							strcpy(symbol_table[symbol_table_size].symbol,words[2+magic_index]);
							symbol_table[symbol_table_size].address=-1;
							sprintf(out.op2,"S,%d",symbol_table_size);
							symbol_table_size++;
						}
						if(index!=-1)
						sprintf(out.op2,"S,%d",index);
					}	
					else
						sprintf(out.op2,"R%d",(int)words[2+magic_index][0]-64);
				}
				else //One one Operand
				{
					if(!is_reg(words[1+magic_index]))//check it ,is it symbol or reg
					{
						int index=search_symbol(words[1+magic_index],symbol_table,symbol_table_size);
						if(index==-1)
						{
							strcpy(symbol_table[symbol_table_size].symbol,words[2+magic_index]);
							symbol_table[symbol_table_size].address=out.location_counter;
							symbol_table_size++;
						}
						sprintf(out.op2,"S,%d",index);
					}	
					else
						sprintf(out.op2,"R%d",(int)words[1+magic_index][0]-64);					
				}
				print_output(out);
				//output
                out.location_counter+=mnemonics_table[mnemo_index].size;
			}

			else if(mnemo_type==3){
				//DS or DC Instruction
				//A  DC '10';
                sprintf(out.mneno_opcode,"DL,%d",mnemonics_table[mnemo_index].opcode);
                strcpy(out.op1,"00");
				strcpy(out.op2,"00");
                int increment_in_LC=0;
                if(strcmp(words[1],"DC")==0){
                    increment_in_LC=mnemonics_table[mnemo_index].size;
                    int x=extract_literal(words[2]);
                    sprintf(out.op2,"%d",x);
                }
                else //DS
                {
                    increment_in_LC=(atoi(words[2]))*mnemonics_table[mnemo_index].size;
                }
                //output
				print_output(out);
                out.location_counter+=increment_in_LC;
			}
    }
    fclose(fp);
    if (line)free(line);
	newline;
	print_LT(literal_table,literal_table_size);
	print_ST(symbol_table,symbol_table_size);
	return 0;    
}
