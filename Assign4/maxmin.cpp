#include <iostream>
#define deb(x)          cout<<#x<<" "<<x.max<<" "<<x.min<<endl;
#define max(a,b)        (a>b)?a:b;
#define min(a,b)        (a<b)?a:b;
using namespace std;


struct maxmin
{
    int max;
    int min;
};

maxmin getmaxmin(int * a,int low,int high)
{
    maxmin ans,la_ans,ra_ans;
    // Base Cases 
    // if only one elemenet is there
    if(low==high){
        ans.max=a[low];
        ans.min=a[low];
        // deb(ans);
        return ans;
    }
    // if only two element is there
    if(high==low+1){
        ans.max=max(a[low],a[high]);
        ans.min=min(a[low],a[high]);
        // deb(ans);
        return ans;
    }
    int mid=(low+high)/2;
    la_ans=getmaxmin(a,low,mid);
    ra_ans=getmaxmin(a,mid+1,high);
    ans.max=max(la_ans.max,ra_ans.max);
    ans.min=min(la_ans.min,ra_ans.min);
    // deb(ans);
    return ans;
}

void maxminProblem(){
    int size;cout<<"Enter the Size of Array :: ";cin>>size;
    int *a = new int [size];cout<<"Enter the "<<size<<" element of array"<<endl;
    for(int i=0;i<size;i++)cin>>a[i];
    maxmin ans = getmaxmin(a,0,size-1);
    cout<<"Max element from given Array :: "<<ans.max<<endl;
    cout<<"Min element from given Array :: "<<ans.min<<endl;
}

int main(){
    string ch;
    do{
        maxminProblem();
        cout<<"Do you want to input again (YES/NO) ? :: ";
        cin>>ch;
    }while(ch=="YES"||ch=="yes");
    return 0;
}