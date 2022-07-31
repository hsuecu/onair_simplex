#include <iostream>

using namespace std;

// Q => 19khz
// Q_bar => 18.8khz

int main() {

	float Q;
	float Q_bar;
	while(1){
		cin >> Q;
		cin >> Q_bar;
		if(Q > Q_bar){
			cout << '1';
		} else{
			cout << '0';
		}
	}
	return 0;
}