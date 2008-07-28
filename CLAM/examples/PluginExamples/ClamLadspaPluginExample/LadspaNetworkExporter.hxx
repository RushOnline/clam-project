#ifndef LadspaNetworkExporter_hxx
#define LadspaNetworkExporter_hxx

#include <ladspa.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"
#include "ControlSource.hxx"
#include "ControlSink.hxx"
#include "LadspaLibrary.hxx"

namespace CLAM
{

template<class T>
class LADSPAInfo
{
public:
	std::string name;
	T* processing;
	LADSPA_Data *dataBuffer;
};

class NetworkLADSPAPlugin
{
private:
	typedef std::vector< LADSPAInfo<AudioSource> > LADSPAInPortList;
	typedef std::vector< LADSPAInfo<AudioSink> > LADSPAOutPortList;
	typedef std::vector< LADSPAInfo<ControlSource> > LADSPAInControlList;
	typedef std::vector< LADSPAInfo<ControlSink> > LADSPAOutControlList;

	Network _network;
	LADSPAInPortList mReceiverList;
	LADSPAOutPortList mSenderList;
	LADSPAInControlList mInControlList;
	LADSPAOutControlList mOutControlList;
	unsigned long mClamBufferSize, mExternBufferSize;
	
public:
	NetworkLADSPAPlugin();
	~NetworkLADSPAPlugin();
	
	void Activate();
	void Deactivate();
	
	void ProcessInputPorts();
	void ProcessOutputPorts();
	void ProcessInputControls();
	void ProcessOutputControls();

	void UpdatePortFrameAndHopSize();
	void FillPortInfo( LADSPA_PortDescriptor* descriptors, char** names, LADSPA_PortRangeHint* rangehints );
	void ConnectTo(unsigned long port, LADSPA_Data * data);
	
	void Run( unsigned long nsamples );
	void CopyLadspaBuffersToGenerators(const unsigned long nframes);
	void CopySinksToLadspaBuffers(const unsigned long nframes);
	void ProcessInControlValues();
	void ProcessOutControlValues();
	static LADSPA_Descriptor * CreateLADSPADescriptor();
};

class LadspaNetworkExporter
{
public:
	LadspaNetworkExporter(LadspaLibrary & library)
	{
		LADSPA_Descriptor * descriptor = NetworkLADSPAPlugin::CreateLADSPADescriptor();
		if (not descriptor) return;
		library.AddPluginType(descriptor);
	}
};

} //namespace CLAM


#endif//LadspaNetworkExporter_hxx

