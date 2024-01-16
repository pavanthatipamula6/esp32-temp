#include <stdio.h>

void longestSubarray(int arr[],int n, int k){
        printf("in function");
        int start =0,stop=0;
        int maxLength = 0;
        int sum=arr[0];
        
        for(int i=0;i<n;i++){
            sum+=arr[i];
            while(sum/n>=k && start<=i){
                if(i-start +1>maxLength){
                    maxLength=i-start+1;
                    stop=i;
                }
                sum-=arr[start];
                start++;
            }
        }
        printf("subaarray in range %d to %d",start,stop);
        printf("sub array is ");
        for(int i=start;i<=stop;i++){
            printf("%d ",arr[i]);
        }
}


int main()
{
    int k;
    int arr[]={1,2,3,4,5,6,7,8};
    int n= sizeof(arr)/sizeof(arr[0]);
    
    printf("enter a positive integer");
    scanf("%d",&k);
    // //for size of array
    // printf("Enter the size of array");
    // scanf("%d",&n);

    // int arr[n];
    // printf("enter array elements");
    // for(int i=0;i<n;i++){
    //     scanf("%d,",&arr[i]);
    // }
    
    
    printf("ok");
    longestSubarray(arr, n, k);

    return 0;
}

