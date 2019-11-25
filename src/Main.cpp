#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){
	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;
		return 0;
	}
	Session s(argv[1]);
	s.start();
	Session s2 = std::move(s);
	Session s3 = s2;
	Session s4(argv[1]);
	s4 = s3;
	Session s5(argv[1]);
	s5 = std::move(s3);
	return 0;
}
