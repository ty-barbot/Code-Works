#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iterator>
#include <string.h>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <sstream>
#include <condition_variable>
// Global varibles //

//char Global;
std::string Global;
std::condition_variable cv;
std::mutex m;
bool ready = true;
bool t1,t2,t3,t4,t5,t6,t7,t8,t9,t10 = false;
int threadID = 0;

void readFile (std::string fileName, int id)
{
  std::string name = fileName;
  std::ifstream fileTemp;
	fileTemp.open(name);

	//char ch;
	std::string ch;
	std::fstream file(name, std::fstream::in);

	while(!file.eof())
	{
		while(m.try_lock()==false)//knock until true
		{
			//std::cout << msg;
		}

		if(Global == ch && !file.eof())
		{
			file >> std::skipws >> ch;
			Global = ch;
			ch = "\0";
		}
		m.unlock();
	}
	std::cout << "thread finish\n";
	if(id == 1)
	{
		t1 = true;
	}
	if(id == 2)
	{
		t2 = true;
	}
	if(id == 3)
	{
		t3 =true;
	}
	if(id == 4)
	{
		t4 =true;
	}
	if(id == 5)
	{
		t5 =true;
	}

	if(id == 6)
	{
		t6 =true;
	}

	if(id == 7)
	{
		t7 =true;
	}

	if(id == 8)
	{
		t8 =true;
	}

	if(id == 9)
	{
		t9 =true;
	}
	if(id == 10)
	{
		t10 =true;
	}


}

int main ()
{

std::ofstream fileOut;
fileOut.open("hw5.out");

//readFile("hw5-3.in");

std::thread thread1(readFile, "hw9.data", 1);
std::thread thread2(readFile, "hw9.data", 2);
std::thread thread3(readFile, "hw9.data", 3);
std::thread thread4(readFile, "hw9.data", 4);
std::thread thread5(readFile, "hw9.data", 5);
std::thread thread6(readFile, "hw9.data", 6);
std::thread thread7(readFile, "hw9.data", 7);
std::thread thread8(readFile, "hw9.data", 8);
std::thread thread9(readFile, "hw9.data", 9);
std::thread thread10(readFile, "hw9.data", 10);

while(true)
{
	while(m.try_lock()==false)
	{
	//std::cout <<"Main is waiting\n";
	}
	std::string ch = "\0";
	if(Global != ch)
	{
		fileOut << Global <<"thread ID is: "<< threadID << "\n";
		Global = ch;
	}
	m.unlock();
	if(t1 && t2 && t3  && t4  && t5  && t6  && t7  && t8  && t9  && t10)
	{
		break;
	}
}

thread1.join();
thread2.join();
thread3.join();
fileOut.close();

//std::cout <<"\xE2\x99\xA0"<< "\n";
//fileOut << Global << "\n";
/*
all (10) threads must pull from the file 
and depsits all words into one sigle 
out file without a race condiotion of losing
data and possbily

*/

}
