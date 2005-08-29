#include <qfiledialog.h>
#include <qaction.h>
#include "Message.hxx"
#include "Engine.hxx"
#include "ViewManager.hxx"
#include "QtSMSTools.hxx"

namespace QtSMS
{
	QtSMSTools::QtSMSTools()
		: SMSToolsBase(0,"SMSTools",WDestructiveClose)
	{
		InitSMSTools();
	}

	QtSMSTools::~QtSMSTools()
	{
		delete mEngine;
	}

	void QtSMSTools::InitSMSTools()
	{
		mEngine = Engine::Instance();
		setCentralWidget(Engine::DisplayManager()->GetView(this));
		InitialState();
	}

	void QtSMSTools::loadConfiguration()
	{
		QString filename = QFileDialog::getOpenFileName(QString::null,"*.xml",this);
		if(filename.isEmpty()) return;
		if(mEngine->LoadConfiguration(filename))
		{
			Engine::DisplayManager()->Flush(); // we have a new config
			Engine::DisplayManager()->SetAudio(ORIGINAL_AUDIO);
			ShowIncomingAudio();
			UpdateState();
		}
		else
		{
			CLAM::VM::Message(QMessageBox::Critical,"SMS Tools 2","Load configuration failed.");
		}
	}

	void QtSMSTools::newConfiguration()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::storeExtractedMelody()
	{

		QString filename = QFileDialog::getSaveFileName("extracted_melody_out.xml","*.xml",this);
		if(filename.isEmpty()) return;
		mEngine->StoreMelody((filename));

	}

	void QtSMSTools::loadAnalysisData()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::storeAnalysisData()
	{
		QString filename = QFileDialog::getSaveFileName("analysis_data_out.xml","*.xml",this);
		if(filename.isEmpty()) return;
		mEngine->StoreAnalysis((filename));
	}

	void QtSMSTools::saveSynthesizedAudio()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_audio_out.wav","Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSound((filename));
	}

	void QtSMSTools::saveSynthesizedSinusoidal()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_sinusoidal_out.wav","Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSoundSinusoidal((filename));
	}

	void QtSMSTools::saveSynthesizedResidua()
	{
		QString filename = QFileDialog::getSaveFileName("synthesized_residual_out.wav","Audio (*.wav *.ogg)",this);
		if(filename.isEmpty()) return;
		mEngine->StoreOutputSoundResidual((filename));
	}

	void QtSMSTools::loadTransformationScore()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::newTransformationScore()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::analyze()
	{
		mEngine->Analyze();
		Engine::DisplayManager()->SetAnalyzedData();
		UpdateState();
	}

	void QtSMSTools::melodyExtraction()
	{
		NotImplemented();
		return;

		// there are something wrong
		mEngine->ExtractMelody();
		UpdateState();
	}

	void QtSMSTools::doTransformation()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::undoTransformation()
	{
		// TODO
		NotImplemented();
	}
   
	void QtSMSTools::synthesize()
	{
		mEngine->Synthesize();
		Engine::DisplayManager()->SetSynthesizedData();
		UpdateState();
	}

	void QtSMSTools::displayBWSonogram(bool on)
	{
		if(mMenuViewColorSonogram->isOn()) mMenuViewColorSonogram->setOn(false);
 
		// TODO
		NotImplemented();
	}

	void QtSMSTools::displayColorSonogram(bool on)
	{
		if(mMenuViewBWSonogram->isOn()) mMenuViewBWSonogram->setOn(false);

		// TODO
		NotImplemented();
	}

	void QtSMSTools::displayFundamentalFrequency(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,FUND_FREQ);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,FUND_FREQ);
		}
	}

	void QtSMSTools::displayOriginalAudio(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,ORIGINAL_AUDIO);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,ORIGINAL_AUDIO);
		}
	}

	void QtSMSTools::displayResidualSpectrum(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(SPECTRUM_GROUP_VIEW,RES_SPEC);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(SPECTRUM_GROUP_VIEW,RES_SPEC);
		}
	}

	void QtSMSTools::displaySinusoidalSpectrum(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(SPECTRUM_GROUP_VIEW,SIN_SPEC);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(SPECTRUM_GROUP_VIEW,SIN_SPEC);
		}
	}

	void QtSMSTools::displaySinusoidalTracks(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SIN_TRACKS);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SIN_TRACKS);
		}
	}

	void QtSMSTools::displaySpectrumAndPeaks(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(SPECTRUM_GROUP_VIEW,SPEC_AND_PEAKS);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(SPECTRUM_GROUP_VIEW,SPEC_AND_PEAKS);
		}
	}

	void QtSMSTools::displaySynthesizedSound(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SYNTH_AUDIO);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SYNTH_AUDIO);
		}
	}

	void QtSMSTools::displaySynthesizedResidual(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SYNTH_RESIDUAL);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SYNTH_RESIDUAL);
		}
	}

	void QtSMSTools::displaySynthesizedSinusoidal(bool on)
	{
		if(on)
		{
			Engine::DisplayManager()->ShowConcreteView(TIME_GROUP_VIEW,SYNTH_SINUSOIDAL);
		}
		else
		{
			Engine::DisplayManager()->HideConcreteView(TIME_GROUP_VIEW,SYNTH_SINUSOIDAL);
		}
	}

	void QtSMSTools::showOnlineHelp()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::showLicense()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::showAboutBox()
	{
		// TODO
		NotImplemented();
	}

	void QtSMSTools::InitialState()
	{
		// menu File
		mMenuFileNewCfg->setEnabled(true);
		mMenuFileLoadCfg->setEnabled(true);
		mMenuFileLoadAnalysis->setEnabled(true);
		mMenuFileStoreAnalysis->setEnabled(false);
		mMenuFileSaveMelody->setEnabled(false);
		mMenuFileNewScore->setEnabled(true);
		mMenuFileLoadScore->setEnabled(true);
		mMenuFileSaveSynSound->setEnabled(false);
		mMenuFileSaveSynSinusoidal->setEnabled(false);
		mMenuFileSaveSynResidual->setEnabled(false);
		// menu SMS Analysis
		mMenuAnalysisAnalyze->setEnabled(false);
		mMenuAnalysisExtractMelody->setEnabled(false);
		// menu SMS Transformation
		mMenuTransformApply->setEnabled(false);
		mMenuTransformUndo->setEnabled(false);
		// menu SMS Synthesis
		mMenuSynthSinthesize->setEnabled(false);
		// menu View
		mMenuViewOriginalAudio->setEnabled(false);
		mMenuViewFundFreq->setEnabled(false);
		mMenuViewSinTracks->setEnabled(false);
		mMenuViewColorSonogram->setEnabled(false);
		mMenuViewBWSonogram->setEnabled(false);
		mMenuViewSpecPeaks->setEnabled(false);
		mMenuViewSinSpec->setEnabled(false);
		mMenuViewResSpec->setEnabled(false);
		mMenuViewSynAudio->setEnabled(false);
		mMenuViewSynSinusoidal->setEnabled(false);
		mMenuViewSynResidual->setEnabled(false);
		// menu Help
		mMenuOnlineHelp->setEnabled(true);
		mMenuLicense->setEnabled(true);
		mMenuAbout->setEnabled(true); 
	}

	void QtSMSTools::UpdateState()
	{
		InitialState();

		if(mEngine->State().GetHasConfig())
		{
			if(mEngine->State().GetHasAudioIn())
			{
				mMenuAnalysisAnalyze->setEnabled(true);
				mMenuViewOriginalAudio->setEnabled(true);
			}
			if(mEngine->State().GetHasAnalysis())
			{
				mMenuSynthSinthesize->setEnabled(true);
			}
		}

		if(mEngine->State().GetHasAnalysis())
		{
			mMenuFileStoreAnalysis->setEnabled(true);
			mMenuAnalysisExtractMelody->setEnabled(true);
			mMenuViewFundFreq->setEnabled(true);
			mMenuViewSinTracks->setEnabled(true);
			mMenuViewColorSonogram->setEnabled(true);
			mMenuViewBWSonogram->setEnabled(true);
			mMenuViewSpecPeaks->setEnabled(true);
			mMenuViewSinSpec->setEnabled(true);
			mMenuViewResSpec->setEnabled(true);
			if(mEngine->State().GetHasTransformationScore())
			{
				mMenuTransformApply->setEnabled(true);
			}
		}

		if(mEngine->State().GetHasMelody())
		{
			mMenuFileSaveMelody->setEnabled(true);
		}

		if(mEngine->State().GetHasAudioOut())
		{
			mMenuFileSaveSynSound->setEnabled(true);
			mMenuFileSaveSynSinusoidal->setEnabled(true);
			mMenuFileSaveSynResidual->setEnabled(true);
			mMenuViewSynAudio->setEnabled(true);
			mMenuViewSynSinusoidal->setEnabled(true);
			mMenuViewSynResidual->setEnabled(true);
		}

		if(mEngine->State().GetHasTransformation())
		{
			mMenuTransformUndo->setEnabled(true);
		}
	}

	void QtSMSTools::ShowIncomingAudio()
	{		
		mMenuViewOriginalAudio->setOn(true);
		displayOriginalAudio(true);
	}

	void QtSMSTools::NotImplemented()
	{
		CLAM::VM::Message(QMessageBox::Information,"SMS Tools 2","Sorry, not implemented yet.");
	}
}

// END

