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
bool t1 = false;
bool t2 = false;
bool t3 = false;
int counter = 0;




void readFile (std::string fileName, int id)
{
  std::string name = fileName;
  std::ifstream fileTemp;
	fileTemp.open(name);
  //std::string test;
	std::string msg = "wait ID is: "+ std::to_string(id) +"\n";

	
	//char ch;
	std::string ch;
	std::fstream file(name, std::fstream::in);

	//std::unique_lock<mutex> ul(m);

	while(!file.eof())
	{
		while(m.try_lock()==false)//knock until true
		{
			//std::cout << msg;
		}

		if(Global == ch && !file.eof())
		{
			file >> std::skipws >> ch;
			//std::cout << ch << "\n";
			Global = ch;
			ch = "\0";
			
			//m.unlock();
      


      //std::cout << counter << "\n";
      //if(counter > 10)
      //{
      //  std::terminate();
      //}
      //counter++;
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

}

int main ()
{

std::ofstream fileOut;
fileOut.open("hw5.out");

//readFile("hw5-3.in");

std::thread thread1(readFile, "hw5-1.in", 1);
std::thread thread2(readFile, "hw5-2.in", 2);
std::thread thread3(readFile, "hw5-3.in", 3);
while(true)
{
	while(m.try_lock()==false)
	{
	//std::cout <<"Main is waiting\n";
	}
	std::string ch = "\0";
	if(Global != ch)
	{
		fileOut << Global << "\n";
		Global = ch;
	}
	m.unlock();
	if(t1 && t2 && t3)
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
