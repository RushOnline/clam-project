#ifndef __SMSHARMONIZERCONFIGURATOR__
#define __SMSHARMONIZERCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SegmentTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor;
	
	class SMSHarmonizerConfigurator : public SMSConfigurator
	{
	public:
		SMSHarmonizerConfigurator();
		virtual ~SMSHarmonizerConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& cfg );
		virtual void SetConfig( const CLAM::ProcessingConfig& cfg );
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_BPF_Editor*            mEditorWidget;
		CLAM::SegmentTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSHarmonizerConfigurator.hxx
