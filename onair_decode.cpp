#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>

using namespace std;

int count = 0;
char flip = '0';
char buffer;

#define SYNC_FLG_LEN 250
#define SYNC_FLG_BITLEN 10
#define SYNC_FLG_POP_TIMES 3

void sync() {
    while( (buffer=getc(stdin)) != EOF) {
        if(buffer == '0') count++;
        if(count > SYNC_FLG_LEN){
            while( (buffer=getc(stdin)) != EOF && buffer != '1')
                count++;
            return;
        }
        if(buffer == '1') count=0;
    }
}

int main() {
    
    sync();
    cerr << "::synced::";
    int expected_len = (int)ceil((double)count/double(SYNC_FLG_BITLEN * SYNC_FLG_POP_TIMES));
    cerr << "@expected_len: " << expected_len <<endl <<"-------------------------------------------------"<< endl;

    count = 1;
    flip = '1';
    while( (buffer=getc(stdin))!=EOF ){
        if((flip == '1' && buffer == '1') || (flip == '0' && buffer == '0'))  {
            count++;
        } else{
            if(flip == '1') {
                // bits dash
                // cout << flip <<":"<< count << endl;
                if(count >= 0.3*expected_len and count <= 1.5*expected_len){
                    cout << '.';
                } else if(count > 1.5*expected_len and count <= 2.5*expected_len){
                    cout << '-';
                } else{
                    cerr << "\n-------------------------------------------------"<< endl;
                    cerr << "::desynced::";
                    if(count > expected_len * 2.8){
                        cerr << "@normally"<<endl;
                    }else{
                        cerr << "@abort" << endl;
                        cerr << "expected:" << expected_len << "\ncount:" << count << "\nflip:" << flip << endl;
                    }
                    break;
                }
            }
            else if(flip == '0'){
                // nothing or space
                // cout << flip <<":"<< count << endl;
                if(count >= 1.5*expected_len and count <= 2.5*expected_len){
                    cout<<endl;
                }
            }
            flip = buffer;
            count = 1;
        }
    }

    return 0;
}