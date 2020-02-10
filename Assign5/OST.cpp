#include <iostream>
#include <vector>
#define swap(a,b)			int temp;temp=a;a=b;b=temp
#define deb1(x)				cout<<#x<<" "<<x<<endl;
#define displayTapes(V)		for(int value:V)cout<<value<<" ";cout<<endl;

using namespace std;

//find the minimum MRT of given Array
double findMRT(vector <int> arr){
	int size=arr.size();
	int Mfactor=size,MRT=0;
	for(int i=0;i<size;i++)MRT+=(Mfactor--)*arr[i];
	return MRT/double(size);
}


//find MRT of all tapess
double findMRTs(int *arr,int size,int tapes){
	vector <int > *Array_of_tapes=new vector<int>[tapes];
	for(int i=0;i<size;i++){
		Array_of_tapes[i%tapes].push_back(arr[i]);
	}
	for(int i=0;i<tapes;i++){
		cout<<"Content of Tape "<<i<<endl;
		displayTapes(Array_of_tapes[i]);
		cout<<"MRT of Tape "<<i+1<<" :: "<<findMRT(Array_of_tapes[i])<<endl<<endl;
	}
}

//sort the array using binary sort
void sort(int *arr,int size){
	for(int i=0;i<size-1;i++)
	for(int j=0;j<size-i-1;j++)
	if(arr[j]>arr[j+1]){swap(arr[j],arr[j+1]);}
}


void OptimalStorage(int *arr,int size,int tapes){
	//Sort the Array for Finding MRT
	sort(arr,size);
	cout<<"Sorted ::"<<endl;
	for(int i=0;i<size;i++)cout<<arr[i]<<" ";cout<<endl<<endl;
	findMRTs(arr,size,tapes);
}

int main()
{
	int size,tapes;
	cout<<"Enter the number of tapes ::";cin>>tapes;
	cout<<"Enter the number of Programs :: ";cin>>size;
	int *arr=new int[size];
	cout<<"Enter the lengths of Programs ::\n";
	for(int i=0;i<size;i++)cin>>arr[i];
	OptimalStorage(arr,size,tapes);
}



/*
	OUTPUT
	Enter the number of tapes ::3
	Enter the number of Programs :: 7
	Enter the lengths of Programs ::
	1 2 3 4 5 6 7 
	Sorted ::
	1 2 3 4 5 6 7 

	Content of Tape 0
	1 4 7 
	MRT of Tape 1 :: 6

	Content of Tape 1
	2 5 
	MRT of Tape 2 :: 4.5

	Content of Tape 2
	3 6 
	MRT of Tape 3 :: 6

*/