#ifndef MultiLibloSource_hxx
#define MultiLibloSource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControlArray.hxx>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

#include "lo/lo.h"
#ifdef GetClassName // windows.h introduces that
#undef GetClassName
#endif

namespace CLAM
{

class MultiLibloSourceConfig : public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( MultiLibloSourceConfig, 3, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, OscPath);
	DYN_ATTRIBUTE( 1, public, std::string, ServerPort);
	DYN_ATTRIBUTE( 2, public, CLAM::TData, NumberOfArguments);
	//TODO number of arguments/ports
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNumberOfArguments(3);
		SetOscPath("/clam/target");
		SetServerPort("");
	};
};

class MultiLibloSource : public CLAM::Processing
{
	typedef MultiLibloSourceConfig Config;

public:
	MultiLibloSource(const Config& config = Config()) 
		: _serverThread(0)
		, _typespec("fff")
		, _oscPath(config.GetOscPath())
		, _port("")
		, _methodSetted(false)
		
	{
		Configure( config );
	}
	
	~MultiLibloSource()
	{
		DeleteMethod();
		RemoveOldControls();
	}

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		return true;
	}
	const char* GetClassName() const
	{
		return "MultiLibloSource";
	}
protected:
	void DeleteMethod()
	{
		// if exists a previous method handler
		if (_methodSetted)
		{
			lo_server_thread_del_method(_serverThread,_oscPath.c_str(),_typespec.c_str()); //delete it
			DecInstance(_port.c_str());
		}
		if (IsPortUsed(_port.c_str()) && GetInstances(_port.c_str())==0) // if was the last instance
		{
			std::cout <<"MultiLibloSource: Shutting down the server..."<<std::endl;
			lo_server_thread_free(_serverThread);
			RemoveServer(_port.c_str());
		}
	}

        void RemoveOldControls()
        {
		_outControls.Clear();
		GetOutControls().Clear();
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		RemoveOldControls();
		CopyAsConcreteConfig(_config, config);
//		_config.AddAll();
//		_config.UpdateData();
//set outputs:
		int nOutputs = int(_config.GetNumberOfArguments());
		if (nOutputs < 1)
		{
			_config.SetNumberOfArguments(1);
			nOutputs = 1;
		}
		// multi-port names share user-configured identifier
		std::string oscPath=_config.GetOscPath();
		std::replace(oscPath.begin(),oscPath.end(),'/','_');
		_outControls.Resize(nOutputs,oscPath, this);
		std::string port = _config.GetServerPort();

		/* not defined port */
		if (port=="")
		{
			std::cout << "MultiLibloSource::ConcreteConfigure server NOT started -- default config" << std::endl;
			return true;
		}
		// if there are no servers on port
		if (not IsPortUsed(port.c_str())) 
			_serverThread = ServerStart(port.c_str(),this); // start new server
		else 
		{ // if exists server on port
			_serverThread = (*ServersInstances().find(port)).second.thread;	//uses existing thread
			DeleteMethod();
		}
		// define processing callback catcher (floats, for now)
		std::string typespecMask="";
		for (int i=0;i<nOutputs;i++)
			typespecMask+="f"; //add "f"s as arguments
		_typespec=typespecMask;
		_oscPath=_config.GetOscPath();
		_port=port;
		lo_server_thread_add_method(_serverThread, _oscPath.c_str(), _typespec.c_str(), controls_handler, this);
		_methodSetted=true;
		// add instance on map
		IncInstance(_port.c_str()); //,path,typespec
		lo_server_thread_start(_serverThread);
		return true; // Configuration ok
	}

private:
	static void error(int num, const char *m, const char *path);

	static int generic_handler(const char *path, const char *types, lo_arg **argv,
			 int argc, void *data, void *user_data);

	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static int quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	// server management related structs, methods, and attributes
	typedef struct
	{
		lo_server_thread thread;
		unsigned int nProcessings;
	} ServerInstance;
	// map key: port
	typedef std::map<std::string,ServerInstance> OscServersMap;
	static OscServersMap & ServersInstances()
	{
		static OscServersMap sOscServersMap;
		return sOscServersMap;
	}
	static bool IsPortUsed(const char* port)
	{
		return ( ServersInstances().find(port) != ServersInstances().end() );
	}
	static lo_server_thread ServerStart(const char* port, void * parent);
	static bool RemoveServer(const char* port);
	static bool IncInstance(const char* port);
	static bool DecInstance(const char* port);
	static const unsigned int GetInstances (const char* port);
	lo_server_thread _serverThread;
	std::string _typespec;
	std::string _oscPath;
	std::string _port;
	bool _methodSetted;
	Config _config;
	OutControlArray _outControls;
};

} //namespace

#endif
