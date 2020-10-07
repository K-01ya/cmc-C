#include<stdio.h>
#define N 10

void remap(int a[N]){
	int i = 0, t = 0, i_neg = 0, i_zer = 0; 
	
	for (i; i < N; i++){
		if (a[i] < 0){
			t = a[i_zer];
			a[i_zer] = a[i_neg]; 
			a[i_neg] = a[i];
			a[i] = t;
			i_zer += 1;
			i_neg += 1;
		}
		else if (a[i] == 0){
			a[i] = a[i_zer];
			a[i_zer] = 0;
                        i_zer += 1;
		}
	}
}

int main(){
	int arr[N];
	for (int i = 0; i < N; i++)
		scanf("%d", &arr[i]);
	remap(arr);
	for (int i = 0; i < N; i++)
		printf("%d ", arr[i]);
	return 0;
}

