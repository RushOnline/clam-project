#include "LibloSingleton.hxx"

/*CLAM::LibloSingleton& CLAM::LibloSingleton::GetInstance()
{
	static CLAM::LibloSingleton theInstance;
	return theInstance;
}*/


const bool CLAM::LibloSingleton::IsPathRegistered(const unsigned int & port, const std::string & path, const std::string & typespec) const
	{
		std::list<OSCInstance>::const_iterator it;
		for (it=_OSCInstances.begin();it!=_OSCInstances.end();it++)
		{
			OSCInstance instance=(*it);
			if (instance.port==port and instance.path==path and instance.typespec==typespec)
				return true;
		}
		return false;
	}

bool CLAM::LibloSingleton::RemoveRegisteredPath(const unsigned int & port, const std::string & path, const std::string & typespec)
	{
		std::list<OSCInstance>::iterator it;
		for (it=_OSCInstances.begin();it!=_OSCInstances.end();it++)
		{
			OSCInstance instance=(*it);
			if (instance.port==port and instance.path==path and instance.typespec==typespec)
			{
				lo_server_thread_del_method(GetServerThread(port),path.c_str(),typespec.c_str()); //delete it
				_OSCInstances.erase(it);
				std::cout<<"LibloSingleton: removed path "<<path << " on port "<<port<<std::endl;
				return true;
			}
		}
		return false;
	}

int CLAM::LibloSingleton::quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	printf("quiting\n\n");
	fflush(stdout);
	return 0;
}

void CLAM::LibloSingleton::error(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}


/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int CLAM::LibloSingleton::generic_handler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
#if 0
	int i;
	printf("path: <%s>\n", path);
	for (i=0; i<argc; i++) 
	{
		printf("arg %d '%c' ", i, types[i]);
		lo_arg_pp((lo_type)types[i], argv[i]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
#endif
	return 1;
}

