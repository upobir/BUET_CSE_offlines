#include<iostream>
#include<string>
using namespace std;

int main(){
    while(1){
        cout<<"Enter two strings: "<<endl;
        string s1, s2;
        cin>>s1>>s2;
        int n = s1.size();
        int m = s2.size();

        int **memo = new int*[n+1];
        for(int i = 0; i<=n; i++) memo[i] = new int[m+1];

        for(int i = n; i>=0; i--){
            for(int j = m; j>=0; j--){
                if(i == n || j == m)
                    memo[i][j] = 0;
                else{
                    memo[i][j] = (memo[i+1][j] > memo[i][j+1])? memo[i+1][j]: memo[i][j+1];
                    if(s1[i] == s2[j]){
                        memo[i][j] = (memo[i][j] > memo[i+1][j+1] + 1)? memo[i][j] : memo[i+1][j+1] + 1;
                    }
                }
            }
        }

        cout<<"Longest LCS length: "<<memo[0][0]<<endl;
        cout<<"One LCS string: ";

        for(int i = 0, j = 0; i<n && j < m;){
            if(s1[i] == s2[j] && memo[i+1][j+1] + 1 >= memo[i+1][j] && memo[i+1][j+1] + 1 >= memo[i][j+1]){
                cout<<s1[i];
                i++;j++;
            }
            else{
                if(memo[i+1][j] > memo[i][j+1]){
                    i++;
                }
                else{
                    j++;
                }
            }
        }
        cout<<endl;



        for(int i = 0; i<=n; i++) delete[] memo[i];
        delete[] memo;
        cout<<endl;
    }
    return 0;
}
