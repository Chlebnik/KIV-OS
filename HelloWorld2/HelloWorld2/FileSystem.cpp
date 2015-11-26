#include "stdafx.h"

File* FileSystem::CreateNewFile(string name, FileAttribute fileAttribute, string callerPath, int& response)
{
	return NULL;
}

File* FileSystem::CreateNewFile(string name, FileAttribute fileAttribute, File* parent, int& response)
{
	return NULL;
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
			if(!f->IsRoot())
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
	return 0;
}

int FileSystem::RemoveFile(File* file)
{
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