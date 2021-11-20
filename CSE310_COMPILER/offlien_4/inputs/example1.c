int a[10];
int b[10];

int mergesort(int l, int r){
    if(l == r){
        return 0;
    }

    int mid;
    mid = (l+r)/2;

    mergesort(l, mid);
    mergesort(mid+1, r);

    int i, j, k;
    i = l;
    j = mid+1;
    for(k = 0; i <= mid || j <= r; k++){
        if(j > r || (i <= mid && a[i] <  a[j]))
            b[k] = a[i++];
        else
            b[k] = a[j++];
    }

    i = l;
    for(k = 0; i<=r; k++){
        a[i++] = b[k];
    }

    return 0;
}

int main(){
    a[0] = 50;
    a[1] = 80;
    a[2] = 10;
    a[3] = 0;
    a[4] = 90;
    a[5] = 70;
    a[6] = 20;
    a[7] = 30;
    a[8] = 60;
    a[9] = 40;

    mergesort(0, 9);

    int i;
    for(i = 0; i<10; i++){
        int x;
        x = a[i];
        println(x);
    }
}