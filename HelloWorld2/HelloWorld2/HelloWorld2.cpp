// HelloWorld2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
using namespace std;

int main()
{
	Kernel kernelInstance;
	Kernel *kernel = &kernelInstance;

	int response = 0;
	File* initialDrive = kernel->LoadFileSystem();
	cout << response << endl;
	cout << initialDrive->GetAbsolutePath() << endl;
	File* subfolder1 = kernel->CreateNewFile("sub1", FOLDER_ATT, initialDrive, response);
	cout << response << endl;
	cout << subfolder1->GetAbsolutePath() << endl;
	File* subfolder2 = kernel->CreateNewFile("sub2", FOLDER_ATT, subfolder1, response);
	cout << response << endl;
	cout << subfolder2->GetAbsolutePath() << endl;
	File* tmpDrive = kernel->GetFile("c:/", NULL, response);
	cout << response << "tmp drive" << endl;
	cout << tmpDrive->GetAbsolutePath() << endl;

	vector<File*> files = tmpDrive->GetChildren();
	for (vector<File*>::iterator iterator = files.begin(); iterator != files.end(); ++iterator)
	{
		cout << "*****************" << endl;
		cout << (*iterator)->GetName() << endl;
		cout << "*****************" << endl;
	}

	File* tmp = kernel->GetFile("c:/sub1", NULL, response);
	cout << response << endl;
	cout << tmp->GetAbsolutePath() << endl;

	File* tmp2 = kernel->GetFile("c:/sub1/sub2", NULL, response);
	cout << response << endl;
	cout << tmp2->GetAbsolutePath() << endl;

	response = kernel->RemoveFile(tmp2);
	cout << response << endl;
	if (tmp2 != NULL)
	{
		cout << "Weird" << endl;
	}
	else
	{
		cout << "OK" << endl;
	}
	
	AbstractInput* input = new StandardInput(kernel);
	AbstractOutput* output = new StandardOutput(kernel);
	AbstractOutput* errorOutput = new StandardOutput(kernel);
	
	shared_ptr<ofstream> fileOutput(new ofstream("D:/testOut.txt"));
	AbstractOutput* outputFile = new FileOutput(fileOutput, kernel);

	shared_ptr<ifstream> fileInput(new ifstream("D:/test.txt"));
	if (!fileInput)
	{
		// todo error 
		exit(1);
	}
	//AbstractInput* input = new FileInput(fileInput, kernel);
	
	int shellPid = kernel->Execute(0, initialDrive, "shell", "", STANDARD_TYPE, "", STANDARD_TYPE, "");
	vector<int> tmpVector;
	tmpVector.push_back(shellPid);
	kernel->WaitForChildren(tmpVector);

	/*AbstractProcess* cd = new Shell(1, 0, kernel);
	cd->Init(input, output, errorOutput, "");
	cd->Run();
	cd->Join();*/
	//

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

