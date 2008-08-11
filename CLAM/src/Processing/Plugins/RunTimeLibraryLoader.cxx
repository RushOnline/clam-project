#include "RunTimeLibraryLoader.hxx"

#include <iostream>
#include <cstdlib>
#include <dirent.h>
#ifdef WIN32
#	include <windows.h>
#	undef GetClassName
#else
#	include <dlfcn.h>
#endif

#include <fstream>

std::string RunTimeLibraryLoader::FileOfSymbol (void * symbolAddress)
{
#ifndef WIN32
	Dl_info info;
	int ok=dladdr(symbolAddress,&info);
	if (ok)
		return info.dli_fname;
#endif
	return "";
}

void RunTimeLibraryLoader::Load() const
{
	std::string path = GetPaths();
	// for each path, load libraries
	std::vector <std::string> environmentPaths = SplitPathVariable(path);
	for (unsigned i=0; i<environmentPaths.size(); i++)
	{
		LoadLibrariesFromPath(environmentPaths[i]);
		//std::cout<<"[FAUST DEBUG] \tUsing environment path: "<<environmentPaths[i]<<std::endl;
	}
	//std::cout<<"[FAUST DEBUG] \tload of RunTimeLibraryLoader"<<std::endl;
}

void RunTimeLibraryLoader::LoadLibrariesFromPath(const std::string & path) const
{
	//std::cerr << "[" << libraryType() << " Plugins] Looking at path '" << path << "'" << std::endl;
	DIR* dir = opendir(path.c_str());
	if (!dir)
	{
	//	std::cerr << "[" << libraryType() << " Plugins]  Warning: could not open plugins dir: " << path << std::endl;
		return;
	}
	while ( struct dirent * dirEntry = readdir(dir) )
	{
		std::string pluginFilename(dirEntry->d_name);
		if(pluginFilename == "." || pluginFilename == "..")
			continue;
		std::string pluginFullFilename(path + std::string("/") + pluginFilename);
		void * handle = FullyLoadLibrary(pluginFullFilename);
		SetupLibrary( handle, pluginFullFilename );
//		std::cout<<"loaded plugin: "<<pluginFullFilename<<std::endl;
	}
	closedir(dir);
}


void * RunTimeLibraryLoader::FullyLoadLibrary(const std::string & libraryPath)
{
//	std::cout << "[" << libraryType() << " Plugins] FullyLoading " << libraryPath << std::endl;
#ifdef WIN32
//		SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
//			SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX );
	return LoadLibrary(libraryPath.c_str());
#else
	return dlopen( libraryPath.c_str(), RTLD_NOW);
#endif
}


void * RunTimeLibraryLoader::LazyLoadLibrary(const std::string & libraryPath)
{
#ifdef WIN32
//	TODO: if windows allow it, add here a lazy load
	return 0; 
#else
	return dlopen( libraryPath.c_str(), RTLD_LAZY);
#endif
}



void * RunTimeLibraryLoader::GetLibraryHandler(const std::string & libraryPath) const
{
#ifdef WIN32 //TODO: does windows have an equivalent method to have the handler?
	return 0;
#else
	return dlopen (libraryPath.c_str(), RTLD_NOLOAD);
#endif
}

//TODO: the name argument will be used to check on the plugins map 
// returns false on success, true on fail
bool RunTimeLibraryLoader::ReleaseLibraryHandler(void* handle, const std::string pluginFullFilename)
{
	std::cout<<"ReleaseLibraryHandler: "<<pluginFullFilename<<std::endl;
	if (!handle)
	{
		std::cout<<"Cannot release an empty handle!"<<std::endl;
		return true;
	}
#ifdef WIN32
	return (!FreeLibrary(handle));
#else 
	return (dlclose(handle));
#endif
}

std::string RunTimeLibraryLoader::LibraryLoadError()
{
#ifdef WIN32
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );
	std::string message((char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return message;
#else
	return dlerror();
#endif
}
std::vector<std::string> RunTimeLibraryLoader::SplitPathVariable(const std::string & pathVariable) const
{
	std::string content=pathVariable;
	std::vector<std::string> result;
	while (!content.empty())
	{
		size_t separatorPos = content.find(pathSeparator());
		if (separatorPos == std::string::npos)
		{
			result.push_back(content);
			break;
		} 
		result.push_back(content.substr(0, separatorPos));
		content = content.substr(separatorPos+1);
	}
	return result;
}

const std::string RunTimeLibraryLoader::GetPaths() const
{
	const char * envPath = getenv(pathEnvironmentVar());
	const char * envHome = getenv("HOME");
	std::string path = envPath ? envPath : "";
	if (envHome)
	{
		path += std::string(path.empty()? "":pathSeparator()) + envHome + homePath();
	}
	// add standardPath to the env path string (i.e. path1:path2)
	for (const char ** standardPath=standardPaths(); *standardPath; standardPath++)
		path += std::string(path.empty()? "":pathSeparator()) + *standardPath;
	return path;
}


const std::string RunTimeLibraryLoader::CompletePathFor(const std::string & subpathAndName) const 
{
	std::string paths=GetPaths();
	std::vector <std::string> environmentPaths = SplitPathVariable(paths);
	for (unsigned i=0; i<environmentPaths.size(); i++)
	{
		// get file name:
		std::string fileName = subpathAndName.substr( subpathAndName.rfind("/")+1); 
		// testDir= root_path + subpath:
		std::string testDir = environmentPaths[i] + "/" + subpathAndName.substr(0, subpathAndName.size()-fileName.size());
		// check if directory exists:
		DIR* dir = opendir(testDir.c_str());
		if (not dir) 
			continue; // directory doesn't match, skip
		closedir(dir);
		// check if file exists:
		std::fstream fin;
		std::string completeFileName=testDir+fileName;
		fin.open(completeFileName.c_str(),std::ios::in);
		if (not fin.is_open()) 
			continue; // file doesn't exist, skip
		fin.close();
		return completeFileName;
	}
	return "";
}

