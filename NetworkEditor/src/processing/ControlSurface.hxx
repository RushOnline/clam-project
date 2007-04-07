
#ifndef __ControlSurface_hxx__
#define __ControlSurface_hxx__

#include <CLAM/OutControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Enum.hxx>
#include <QtGui/QWidget>

namespace CLAM
{

class ControlSurfaceConfig : public ProcessingConfig
{	
public:
	DYNAMIC_TYPE_USING_INTERFACE (ControlSurfaceConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TControlData, Min1 );
	DYN_ATTRIBUTE (1, public, TControlData, Default1 );
	DYN_ATTRIBUTE (2, public, TControlData, Max1 );
	DYN_ATTRIBUTE (3, public, TControlData, Min2 );
	DYN_ATTRIBUTE (4, public, TControlData, Default2 );
	DYN_ATTRIBUTE (5, public, TControlData, Max2 );


protected:
	void DefaultInit(void);
};

class ControlSurface : public Processing
{
	ControlSurfaceConfig mConfig;
	OutControl mOutput1;
	OutControl mOutput2;
	TControlData mLastValue1;
	TControlData mLastValue2;
	bool mFirstDoAfterStart;
public:
	ControlSurface();
	ControlSurface( const ControlSurfaceConfig & );

	bool Do();
	const char * GetClassName() const {return "ControlSurface";}

	const ProcessingConfig &GetConfig() const { return mConfig;}
	void SendControl(TControlData value1, TControlData value2);
protected:
	bool ConcreteConfigure(const ProcessingConfig& c);
	bool ConcreteStart();
};

} //namespace CLAM

#endif //__ControlSurface_hxx__


