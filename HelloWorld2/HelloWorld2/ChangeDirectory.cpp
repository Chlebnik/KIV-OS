#include "stdafx.h"


string ChangeDirectory::GetHelpContent()
{
	string help = "Displays the name of or changes the current directory.\n\n"
					"CD[/ D][drive:][path]\nCD[..]\n\n"
					"..Specifies that you want to change to the parent directory.\n\n"
					"Type CD drive : to display the current directory in the specified drive.\n"
					"Type CD without parameters to display the current drive and directory.";
	return help;
}

bool ChangeDirectory::HasValidParameters()
{
	bool valid = false;
	int i = 0;

	if (parameters.size() == 0) {
		valid = true;
		showHelp = true;	
	}else{
		for (std::vector<string>::iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (i == 0 && *it == "--help" && parameters.size() == 1) {
				showHelp = true;
				valid = true;
			}
			else if (i == 0 && *it == "/D" && parameters.size() < 3) {
				changeDrive = true;
				valid = true;
			}
			else if (i == 0 && parameters.size() == 1) {
				pathIndex = i;
				changeDrive = false;
				showHelp = false;
				valid = true;
			}
			else if (i == 1 && parameters.size() == 2){
				pathIndex = i;
				showHelp = false;
				valid = true;
			}
			i++;
		}
	}

	return valid;
}

bool ChangeDirectory::DirExists(const string& dirName_in)
{
	DWORD ftyp = kernel->OurGetFileAttributesA(dirName_in);

	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

int ChangeDirectory::RunProcess()
{
	int returnValue = 0;
	if (showHelp) {
		output->WriteLine(GetHelpContent());
	}
	else {
		returnValue = ChangePath(parameters[pathIndex]);
	}

	return returnValue;

}



int ChangeDirectory::ChangePath(string new_path) {
	int success = 0;

	if (DirExists(new_path)) {
		wchar_t* wstr = Utils::StringToWchar(new_path);

		if (!SetCurrentDirectory(wstr))
		{
			//"SetCurrentDirectory failed " + GetLastError()
			success = ERROR_INVALID_FILE_ATTRIBUTES;
		}
		system("Dir");
		delete[] wstr;
	}
	else {
		//"Given path doesn't exist!"
		success = ERROR_INVALID_FILE_ATTRIBUTES;
	}

	return success;
}
