#include "stdafx.h"

File* FileSystem::CreateNewFile(string name, FileAttribute fileAttribute, string callerPath, int& response)
{
	File* parent;
	if (callerPath.length() == 0)
	{
		// This will create new drive in combination of folder file attribute
		parent = NULL;
	}
	else
	{
		parent = GetFile(callerPath, NULL, response);
		if (response != 0)
		{
			// TODO error
			return NULL;
		}
	}
	
	return CreateNewFile(name, fileAttribute, parent, response);
}

File* FileSystem::CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response)
{
	File* f = NULL;

	if (parent == NULL)
	{
		if (fileAttribute == FOLDER_ATT)
		{
			// create drive
			bool exists = false;
			for (vector<File*>::iterator iterator = drives.begin(); iterator != drives.end(); ++iterator)
			{
				if (name == (*iterator)->GetName())
				{
					// already exists
					exists = true;
					break;
				}
			}
			if (!exists)
			{
				f = new File(name, fileAttribute, NULL);
				drives.push_back(f);
				response = 0;
			}
			else
			{
				// TODO error code
				response = 5;
			}
		}
		else
		{
			// todo error - drive must be a folder
		}
	}
	else
	{
		f = new File(name, fileAttribute, parent);
		response = parent->AddChild(f);
		
		// todo more error handling
		if (response != 0)
		{
			delete f;
			f = NULL;
		}		
	}
	return f;
}

File* FileSystem::GetFile(string path, File* sourceFile, int& response)
{
	vector<string> pathElements = Utils::Split(path, FILE_SEPARATOR);
	
	File* f = NULL;
	for (int i = 0; i < pathElements.size(); i++)
	{
		string element = pathElements[i];

		// Analyze first element
		if (i == 0)
		{
			if (element.back() == DRIVE_SUFFIX)
			{
				string driveName = element.substr(0, element.length() - 1);
				if (driveName.length() != 0)
				{
					for (vector<File*>::iterator iterator = drives.begin(); iterator != drives.end(); ++iterator)
					{
						if (driveName == (*iterator)->GetName())
						{
							f = *iterator;
							break;
						}
					}

					// drive not found
					if (f == NULL)
					{
						// TODO error code
						response = 8;
						goto endOfElementIteration;
					}
				}
				else
				{
					// TODO error (empty drive name)
					response = 1;
					goto endOfElementIteration;
				}
				// skip to next element
				continue;
			}
			else
			{
				f = sourceFile;
			}

		}

		// "."
		if (element == CURRENT_FOLDER)
		{
			continue;
		}

		// ".."
		if (element == PARENT_FOLDER)
		{
			if(f->IsRoot())
			{
				// TODO error - no parent folder
				response = 2;
				goto endOfElementIteration;
			}
			else
			{
				f = f->GetParent();
			}
		}
		else
		{
			// TODO error code - file not found
			response = 27;
			
			vector<File*> children = f->GetChildren();
			for (vector<File*>::iterator iterator = children.begin(); iterator != children.end(); ++iterator)
			{
				if (element == (*iterator)->GetName())
				{
					f = *iterator;
					if (!f->IsFolder() && i != pathElements.size() - 1)
					{
						// TODO error - file is not folder
						response = 3;
						goto endOfElementIteration;
					}
					response = 0;
					break;
				}
			}
		}


	}

	// I really do not like this, but as long as there is no naming for loops, it has to suffice
	endOfElementIteration:

	return f;
}

int FileSystem::RemoveFile(string path)
{
	int response;
	File* f = GetFile(path, NULL, response);
	if (response == 0)
	{
		return RemoveFile(f);
	}
	else
	{
		return response;
	}
}

int FileSystem::RemoveFile(File* file)
{
	if (!file->IsDeletable())
	{
		return 21; // TODO error - cannot be deleted
	}
	File* parent = file->GetParent();
	if (parent == NULL)
	{
		// TODO Deleting drive
	}
	else
	{
		int response = parent->RemoveChild(file);
		if (response == 0)
		{
			delete file;
			file = NULL;
		}
		else {
			return response;
		}
	}
	return 0;
}

bool FileSystem::ValidatePath(string path)
{
	return true;
}

bool FileSystem::ValidateFileName(string path)
{
	return true;
}