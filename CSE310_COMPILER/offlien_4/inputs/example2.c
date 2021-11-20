int sqrtfloor(int x){
    int lo, hi;
    lo = 0;
    hi = x;
    while(lo <= hi){
        int mid;
        mid = (lo+hi)/2;
        if(mid*mid <= x){
            lo = mid+1;
        }
        else{
            hi = mid-1;
        }
    }

    return hi;
}

int main(){

    int a;
    a = sqrtfloor(100);
    println(a);

    a = sqrtfloor(55);
    println(a);
}