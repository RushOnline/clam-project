// generated by Fast Light User Interface Designer (fluid) version 1.0011

#ifndef UserInterface_hxx
#define UserInterface_hxx
#include <FL/Fl.H>
#include "FL/Fl_Tile.H"
#include "Fl_Smart_Tile.H"
class Fl_Smart_Tile;
#include "ForwardDeclarations.hxx"
#include "CLAMPresentations.hxx"
#include "CLAMViews.hxx"
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Menu_Bar.H>

class UserInterface {
public:
  public: AnalysisSynthesisExampleGUI* mAnalysisSynthesisExample;
  UserInterface();
  Fl_Window *mWindow;
  Fl_Box *mResizable;
private:
  Fl_Output *mConfigurationText;
  Fl_Menu_Bar *mMenuBar;
  static Fl_Menu_Item menu_mMenuBar[];
  inline void cb_Load_i(Fl_Menu_*, void*);
  static void cb_Load(Fl_Menu_*, void*);
  inline void cb_Edit_i(Fl_Menu_*, void*);
  static void cb_Edit(Fl_Menu_*, void*);
  inline void cb_Store_i(Fl_Menu_*, void*);
  static void cb_Store(Fl_Menu_*, void*);
  static Fl_Menu_Item *mDisplayInSM;
  static Fl_Menu_Item *mDisplayInSound;
  inline void cb_mDisplayInSound_i(Fl_Menu_*, void*);
  static void cb_mDisplayInSound(Fl_Menu_*, void*);
  static Fl_Menu_Item *mDisplayInSpec;
  inline void cb_mDisplayInSpec_i(Fl_Menu_*, void*);
  static void cb_mDisplayInSpec(Fl_Menu_*, void*);
  static Fl_Menu_Item *mPlayInputSound;
  inline void cb_mPlayInputSound_i(Fl_Menu_*, void*);
  static void cb_mPlayInputSound(Fl_Menu_*, void*);
  static Fl_Menu_Item *mAnalyze;
  inline void cb_mAnalyze_i(Fl_Menu_*, void*);
  static void cb_mAnalyze(Fl_Menu_*, void*);
  inline void cb_Load1_i(Fl_Menu_*, void*);
  static void cb_Load1(Fl_Menu_*, void*);
  static Fl_Menu_Item *mStoreAnalysisData;
  inline void cb_mStoreAnalysisData_i(Fl_Menu_*, void*);
  static void cb_mStoreAnalysisData(Fl_Menu_*, void*);
  inline void cb_Load2_i(Fl_Menu_*, void*);
  static void cb_Load2(Fl_Menu_*, void*);
  static Fl_Menu_Item *mDoTransformation;
  inline void cb_mDoTransformation_i(Fl_Menu_*, void*);
  static void cb_mDoTransformation(Fl_Menu_*, void*);
  static Fl_Menu_Item *mSynthesize;
  inline void cb_Synthesize_i(Fl_Menu_*, void*);
  static void cb_Synthesize(Fl_Menu_*, void*);
  static Fl_Menu_Item *mOutputSM;
  static Fl_Menu_Item *mVisualizeOutputs;
  inline void cb_Sound_i(Fl_Menu_*, void*);
  static void cb_Sound(Fl_Menu_*, void*);
  inline void cb_Sinusoidal_i(Fl_Menu_*, void*);
  static void cb_Sinusoidal(Fl_Menu_*, void*);
  inline void cb_Residual_i(Fl_Menu_*, void*);
  static void cb_Residual(Fl_Menu_*, void*);
  static Fl_Menu_Item *mPlayOutputs;
  inline void cb_Sound1_i(Fl_Menu_*, void*);
  static void cb_Sound1(Fl_Menu_*, void*);
  inline void cb_Sinusoidal1_i(Fl_Menu_*, void*);
  static void cb_Sinusoidal1(Fl_Menu_*, void*);
  inline void cb_Residual1_i(Fl_Menu_*, void*);
  static void cb_Residual1(Fl_Menu_*, void*);
  static Fl_Menu_Item *mMelodySM;
  static Fl_Menu_Item *mMelodyAnalyze;
  inline void cb_mMelodyAnalyze_i(Fl_Menu_*, void*);
  static void cb_mMelodyAnalyze(Fl_Menu_*, void*);
  static Fl_Menu_Item *mMelodyStore;
  inline void cb_mMelodyStore_i(Fl_Menu_*, void*);
  static void cb_mMelodyStore(Fl_Menu_*, void*);
  static Fl_Menu_Item *mStoreOutputs;
  inline void cb_Sound2_i(Fl_Menu_*, void*);
  static void cb_Sound2(Fl_Menu_*, void*);
  inline void cb_Sinusoidal2_i(Fl_Menu_*, void*);
  static void cb_Sinusoidal2(Fl_Menu_*, void*);
  inline void cb_Residual2_i(Fl_Menu_*, void*);
  static void cb_Residual2(Fl_Menu_*, void*);
  inline void cb_Exit_i(Fl_Menu_*, void*);
  static void cb_Exit(Fl_Menu_*, void*);
  inline void cb_About_i(Fl_Menu_*, void*);
  static void cb_About(Fl_Menu_*, void*);
  Fl_Smart_Tile *mSmartTile;
  void AboutWindow();
  Fl_Window *mWindow2;
  inline void cb_mWindow2_i(Fl_Window*, void*);
  static void cb_mWindow2(Fl_Window*, void*);
  void LoadConfiguration(void);
  void EditConfiguration(void);
  void StoreConfiguration(void);
  void DisplayInputSound(void);
  void DisplayInputSpectrum(void);
  void Analyze(void);
  void LoadAnalysisData(void);
  void StoreAnalysisData(void);
  void Synthesize(void);
  void AnalyzeMelody(void);
  void StoreMelody(void);
  void StoreOutputSound(void);
  void DisplayOutputSound(void);
  void StoreOutputSoundResidual(void);
  void DisplayOutputSoundResidual(void);
  void StoreOutputSoundSinusoidal(void);
  void DisplayOutputSoundSinusoidal(void);
  void LoadTransformation(void);
  void Transform(void);
  void Exit(void);
  Fl_Window* Attach( const char* title, CLAM::Audio* data );
  static void _Detach(Fl_Window* buffer,UserInterface* ui);
  void Detach(Fl_Window* buffer);
  void PlayInputSound();
  void PlayOutputSound();
  void PlaySinusoidal();
  void PlayResidual();
  void LoadSound();
  void Init();
  Fl_Window* mAudioInputDisplay;
  Fl_Window* mAudioOutputDisplay;
  Fl_Window* mAudioOutputResidualDisplay;
  Fl_Window* mAudioOutputSinusoidalDisplay;
public:
  void Update();
};
#endif
