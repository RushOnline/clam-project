// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef SMSTransformPanel_hxx
#define SMSTransformPanel_hxx

#include "SMSTransformationChain.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"
#include "Slotv0.hxx"

namespace CLAMVM
{
	class SMSConfigurator;
}

// forward declarations
class Fl_Widget;
class Fl_Group;
class Fl_Window;
class Fl_Select_Browser;
class Fl_Button;
class Fl_Tabs;
class Fl_Box;

class SMSScoreEditor 
{
public:
	SMSScoreEditor();
	~SMSScoreEditor();
	
	SigSlot::Signalv1< const CLAM::SMSTransformationChainConfig& > TransformationChainChanged;
	
	SigSlot::Slotv1< const CLAM::SMSTransformationChainConfig& > SetTransformationScore;

	SigSlot::Slotv0 ScoreWasChanged;

	void Show();
	void Hide();

protected:
	bool                               mUserChangedSomething; 

	void OnSetTransformationScore( const CLAM::SMSTransformationChainConfig& cfg );
	void OnScoreChanged();
	void ResetChangedStatus();
	bool IsScoreChanged() const { return mUserChangedSomething; }

	void ShowScoreOnBrowser( const CLAM::SMSTransformationChainConfig& cfg );
	void ShowFactoryProductsOnBrowser();

	void InsertNewConfiguratorInScoreBox( const char* transName );
	void RemoveAllConfiguratorsFromScoreBox();
	void RemoveConfiguratorFromScoreBox( int index );
	void SwapConfigurators( int source, int destination );
	void ApplyChangesIntoChain();
	void InsertConfigurationIntoChain( const char* name, const CLAM::ProcessingConfig& cfg,
					   CLAM::SMSTransformationChainConfig& chain );

	void ClearTransformationHelpWidget();
	void ClearTransformationEditWidget();
	void ClearTransformationWidgets() {
		ClearTransformationHelpWidget();
		ClearTransformationEditWidget();
	}
	void SetTransformationHelpWidget( CLAMVM::SMSConfigurator& conf );
	void SetTransformationEditWidget( CLAMVM::SMSConfigurator& conf );

	int  mHighlightedConfig;

private: // widgets
	Fl_Window *mMainWindow;
	Fl_Group *mChainTopologyDock;
	Fl_Select_Browser *mRepositoryBox;
	Fl_Select_Browser *mScoreContentsBox;
	Fl_Button *mMoveTransUpInScoreButton;
	Fl_Button *mMoveTransDownInScoreButton;
	Fl_Button *mRemoveTransFromScoreButton;
	Fl_Button *mAddTransformToScoreButton;
	Fl_Group *mTransParmDock;
	Fl_Button *mApplyChangesToCurrentCfg;
	Fl_Button *mDiscardConfigButton;
	Fl_Tabs *mTransTabs;
	Fl_Button *mApplyChangesToScoreButton;
	Fl_Button *mDiscardChangesButton;
	Fl_Group*    mHelpWidgetContainer;
	Fl_Group*    mConfigWidgetContainer;
	Fl_Box*      mNoConfigWidgetAvailable;

private: // Crappy FLTK attempt at improving the callback mechanism
 	inline void cb_mMoveTransUpInScoreButton_i(Fl_Button*, void*);
	static void cb_mMoveTransUpInScoreButton(Fl_Button*, void*);
	inline void cb_mMoveTransDownInScoreButton_i(Fl_Button*, void*);
	static void cb_mMoveTransDownInScoreButton(Fl_Button*, void*);
	inline void cb_mRemoveTransFromScoreButton_i(Fl_Button*, void*);
	static void cb_mRemoveTransFromScoreButton(Fl_Button*, void*);
	inline void cb_mAddTransformToScoreButton_i(Fl_Button*, void*);
	static void cb_mAddTransformToScoreButton(Fl_Button*, void*);
	inline void cb_mApplyChangesToCurrentCfg_i(Fl_Button*, void* );
	static void cb_mApplyChangesToCurrentCfg( Fl_Button*, void* );

	inline void cb_mApplyChangesButton_i(Fl_Button*, void* );
	static void cb_mApplyChangesButton( Fl_Button*, void* );
	inline void cb_mDiscardChangesButton_i(Fl_Button*, void*);
	static void cb_mDiscardChangesButton(Fl_Button*, void*);

	inline void cb_mRepositoryBrowser_i( Fl_Select_Browser*, void* );
	static void cb_mRepositoryBrowser( Fl_Select_Browser*, void* );

	inline void cb_mScoreBrowser_i( Fl_Select_Browser*, void* );
	static void cb_mScoreBrowser( Fl_Select_Browser*, void* );

private: // "True" callback methods
	void DiscardChangesAndClose();
	void ApplyChangesAndClose();
	void AddHighlightedToScore();
	void RemoveHighlightedFromScore();
	void MoveHighlightedDown();
	void MoveHighlightedUp();

	

};

#endif
