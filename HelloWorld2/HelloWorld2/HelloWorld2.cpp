// HelloWorld2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
using namespace std;

int main()
{
	Kernel kernelInstance;
	Kernel *kernel = &kernelInstance;
	
	AbstractInput* input = new StandardInput(kernel);
	AbstractOutput* output = new StandardOutput(kernel);
	AbstractOutput* errorOutput = new StandardOutput(kernel);
	
	//AbstractProcess* cd = new Type(1, 0, kernel);
	//cd->Init(input, output, errorOutput, "D:/t*.txt");
	//cd->Run();

	//ifstream fileInput("C:/testData/test.txt");
	//if (!fileInput)
	//{
		// todo error 
	//	exit(1);
	//}
	//AbstractInput* input2 = new FileInput(fileInput, kernel);

	//fstream fileOutput("C:/testData/testOut.txt");
	//bstractOutput* outputFile = new FileOutput(fileOutput, kernel);

	/*AbstractProcess* shell = new Shell(0, kernel);
	shell->Init(input2, output, errorOutput, {});
	shell->Run();*/
	

	/*AbstractProcess* sortProcess = new Sort(0, kernel);
	sortProcess->Init(input, output, errorOutput, {});
	sortProcess->Run();*/

	//AbstractInput* inputPipe = new PipeInput(1, kernel);
	//AbstractOutput* outputPipe = new PipeOutput(1, kernel);
	//Pipe* pipe = new Pipe()
	//pipeMap[1] = pipe;
	//AbstractProcess* randProcess = new Rand(0, kernel);
	//AbstractProcess* sortProcess = new Sort(1, kernel);
	//randProcess->Init(input, outputPipe, errorOutput, {});
	//sortProcess->Init(inputPipe, outputFile, errorOutput, {});
	//thread t1(&AbstractProcess::Run,randProcess);
	//thread t2(&AbstractProcess::Run, sortProcess);
	//t1.join();
	//t2.join();
	return 0;
}

