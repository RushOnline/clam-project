
#ifndef _DummyLadspa_hxx_
#define _DummyLadspa_hxx_

#ifdef USE_LADSPA

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Mapping.hxx>

#include <ladspa.h>

#include <string>

namespace CLAM
{
	class AudioInPort;
	class AudioOutPort;
	class InControl;
	class OutControl;

class DummyLadspaConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyLadspaConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, int, Index);
	DYN_ATTRIBUTE (2, public, int, SampleRate);
	DYN_ATTRIBUTE (3, public, int, Size);

protected:
	void DefaultInit(void);
};

class DummyLadspa : public Processing
{
private:
	DummyLadspaConfig mConfig;
	typedef void * SOPointer;
	LADSPA_Handle mInstance;
	const LADSPA_Descriptor * mDescriptor;
	SOPointer mSharedObject;

	std::vector< AudioInPort* > mInputPorts;
	std::vector< AudioOutPort* > mOutputPorts;

	std::vector< InControl* > mInputControls;
	std::vector< OutControl* > mOutputControls;
	std::vector< LADSPA_Data > mInputControlValues;
	std::vector< LADSPA_Data > mOutputControlValues;

	void ConfigurePortsAndControls();
	void RemovePortsAndControls();
	void UpdatePointers();

public:
	DummyLadspa();
	DummyLadspa( const DummyLadspaConfig & );

	bool Do();
	virtual ~DummyLadspa(){}

	const char * GetClassName() const {return "DummyLadspa";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig; }
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif // USE_LADSPA

#endif



