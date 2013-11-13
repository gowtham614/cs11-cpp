#include "Heap.hh"

#include <stdlib.h> 
#include <iostream>
using namespace std;

#define HEAP_SIZE 100

int main()
{
	int curr,prev = -1;
	bool ok = true;

	Heap<int,HEAP_SIZE> h;

	for (int i = 0; i < HEAP_SIZE; i++)
	{
		h.add_value(rand() % 1000);
	}

	for (int i = 0; i < HEAP_SIZE; i++)
	{
		curr = h.get_first_value();

		if(curr < prev){
			cout << "out of order " << i << endl;

			ok = false;
			break;
		}
	}

	string s1 = "red";
    	string s2 = "green";

	cout << endl;
	cout << "s1 = \" " << s1 << " \"" << endl;
	cout << "s2 = \" " << s2 << " \"\n" << endl;
	
    	cout << "s1 < s2 = " << (s1 < s2 ? "true" : "false") << "\n" << endl;
    	
    	Heap<string,4> hs;

	hs.add_value(s1);
	hs.add_value(s2);
	
	string hs1 = hs.get_first_value();
    	string hs2 = hs.get_first_value();

	ok = ok && (hs1 < hs2) ;

	if(ok)
		cout << "heap works fine" << endl;
	else
		cout << "heap needs some work /:" << endl;

	return 0;
}
