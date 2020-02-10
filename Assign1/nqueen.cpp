#include <iostream>
#include <unistd.h>
#include <cmath>
#define box_line 		cout<<"+";for(int w=0;w<size;w++)cout<<"---+";cout<<endl
using namespace std;


int count=0;


//Print the Board using Array Board
void printBoard(int arr[],int size,int no)
{	
	cout<<"Board "<<no+1<<" :: \n"<<endl;
	box_line;
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
			arr[i]==j+1?cout<<"| \u2655 ":cout<<"| - ";cout<<"|"<<endl;
		box_line;
	}
	cout<<endl;
}


bool isSafe(int board[],int row,int col)
{
	for(int i=0;i<row;i++)
	{
		//check for the Queen's place ,is it at same columns as other or not 
		//check for the diagonal Queen's too
		//If it is there than return false 
		if((board[i]-1)==col || abs(board[i]-col-1)==abs(i-row))
			return false;
	}
	//Else we can place the queen
	return true;
}

void solveNQproblem(int board[],int size,int row)
{
	//check for all column where queen should be palced


	for(int i=0;i<size;i++)
	{
		//check for the place ,is it safe or not
		if(isSafe(board,row,i))
		{
			board[row]=i+1;
			
			//If placed at all rows than print the solution
			if(row==size-1)
				printBoard(board,size,count),count++;
			//Else go for next row
			else 
				solveNQproblem(board,size,row+1);
		}	
	}
}

bool solveNQ()
{
	int n;cout<<"Enter the number of Queen"<<endl;
	cin>>n;
	// Board size n*n
	
	int board[n];

	//initialize the board to zero  
	for(int i=0;i<n;i++)board[i]=0;
	solveNQproblem(board,n,0);
	
	// Check for solution is exist or not
	if(count==0)
	return false;
	cout<<"Total solution are "<<count<<endl;
	return true;
}

int main()
{
	if(!solveNQ())
	cout<<"Solution does not exist "<<endl;
	return 0;
}


/*
		 0  1  2  3
A[0]=1  -1  Q -1 -1
A[1]=3  -1 -1 -1  Q
A[2]=0   Q -1 -1 -1 
A[3]=2  -1 -1  Q -1

*/

