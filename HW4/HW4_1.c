#include <stdio.h>

int binom(int n, int k){
    double res = 1;
    for (int i=1; i<=k; ++i)
        res = res * (n-k+i) / i;
    return (int) (res + 0.01);
}

int main(){
    int n, m;
    printf("Enter N, M: ");
    scanf ("%d %d",&n,&m);
    printf ("%d", binom(n,m));
    return 0;
}