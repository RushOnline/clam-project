/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ProcessingTree.hxx" 
#include <QtGui/QMouseEvent>
#include <QtGui/QHeaderView>
#include <iostream> 


#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx> 
#endif

#include <CLAM/ProcessingFactory.hxx> 


namespace NetworkGUI
{
/*const static char * processingClasses[] = {
	"Audio Input/Output",
	"AudioSource",
	"AudioSink",
	0,
	"Audio File I/O",
	"MonoAudioFileReader",
	"MonoAudioFileWriter",
	"MultiChannelAudioFileReader",
	"MultiChannelAudioFileWriter",
	0,
	"Generators",
	"SimpleOscillator",
	"Oscillator",
	"ADSR",
	"SquareWave",
	"WindowGenerator",
	"HarmonicPeaksGenerator",
//	"WaveGenerator",	TODO register
	0,
	"Arithmetic operations",
	"AudioMultiplier",
	"AudioAdder",
	"AudioMixer",
//	"SpectralPeakArrayAdder", TODO register
//	"SpectrumAdder", TODO register
//	"SpectrumAdder2", TODO register
	"SpectrumSubstracter2",
	"SpectrumProduct",
	0,
	"Controls",
	"OutControlSender",
	"ControlSurface",
	"ControlSource",
	"ControlSink",
	"ControlPrinter",
	"ControlTraceReader",
	"ControlTraceWriter",
	"ControlScaler", 
	"AutoPanner",
	"FlagControl",
	"Random",
	"OneOverF",
#if USE_OSCPACK
	"OSCSender",
#endif	
	0,
	"MIDI",
//	"MIDIKeyboard",
//	"MIDIInControl",
	"MIDIDispatcher",
	0,
	"Analysis",
	"FFT",
	"FFT_rfftw",
	"FFT_fftw3",
	"FFT_ooura",
	"FFT_numrec",
	"AudioWindowing",
	"SpectralAnalysis",
	"SMSAnalysisCore",
	"LPC_AutoCorrelation",
	"TonalAnalysis",
	"MelFilterBank",
	"CepstralTransform",
	0,
	"Synthesis",
	"IFFT",
	"IFFT_rfftw",
	"IFFT_fftw3",
	"IFFT_ooura",
	"OverlapAdd",
	"SpectralSynthesis",
	"SMSSynthesis",
	0,
	"SMS Transformations",
	"SMSFreqShift",
	"SMSPitchShift",
	"SMSResidualGain",
	"SMSSinusoidalGain",
	"SMSSineFilter",
	"SMSOddEvenHarmonicRatio",
	"SMSSpectralShapeShift",
	"SMSPitchDiscretization",
	"SMSGenderChange",
	"SMSHarmonizer",
	"SMSMorph",
	"SMSTimeStretch",
	"SMSTranspose",
	"VowelResonator",
	0,
	"Spectral Transformations",
	"OscillatingSpectralNotch",
	"SpectralDelay",
	"Vocoder",
	"ThreeBandFilter",
	"Robotization",
	"SampleAndHold",
	"ThreeBandCompressor",
	"Peakalizer",
	"ThreeBandAM",
//	"Deeser",
	0,	
	"Utils",
	"Fundamental2Control",
//	"ControlMapper" //	TODO register
	0,
	"Graphical Monitors",
	"Vumeter",
	"Oscilloscope",
	"BufferOscilloscope",
	"SpectrumView",
	"PeakView",
	"Tonnetz",
	"KeySpace",
	"Spectrogram",
	"PolarChromaPeaks",
	"ChordRanking",
	"LPModelView",
	"MelSpectrumView",
	"MelCepstrumView",
	"VectorView",
	"Tunner",
	0,
	"Usage Examples",
	"FilenameUsageExample",
	0,
	"Sample by Sample (Experimental)",
	"Data2Audio",
	"AudioStreamer",
	"DataAdder",
	"DataMultiplier",
	"DataGain",
	"DataSink",
	"DataDelay",
	"SilentSource",
	0,
	"Ebow Synthesizer (Experimental - GSoC)",
	"AudioDatabaseReader",
	"SDIFDatabaseProcessing",
	"OSCSourceProcessing",
	0,
	0
};*/

ProcessingTree::ProcessingTree( QWidget * parent)
	: QTreeWidget(parent)
{
	setColumnCount(1);
	setHeaderLabels(QStringList() << tr("Processings toolbox"));
	setRootIsDecorated( true );
	header()->hide();

/*	for (unsigned i=0; processingClasses[i]; i++)
	{
		QTreeWidgetItem * group = new QTreeWidgetItem( this, QStringList() << processingClasses[i] );
		i++;
		for (; processingClasses[i]; i++)
		{
			QTreeWidgetItem * item = new QTreeWidgetItem( group, QStringList() << processingClasses[i]);
			item->setText(1, processingClasses[i]); // processing factory key
			item->setIcon(0, QIcon(":/icons/images/processing.png"));
		}
	}*/

#ifdef USE_LADSPA
	RunTimeLadspaLibraryLoader ladspaLoader;
	ladspaLoader.Load();
#endif
	CLAM::ProcessingFactory::Values categories = CLAM::ProcessingFactory::GetInstance().GetSetOfValues("category");
	CLAM::ProcessingFactory::Values::const_iterator itCategory;

	for(itCategory = categories.begin(); itCategory != categories.end(); itCategory++)
	{
		CLAM::ProcessingFactory::Keys keys = CLAM::ProcessingFactory::GetInstance().GetKeys("category", *itCategory);
		//std::cout << "Category: " << *itCategory << std::endl;
		if( keys.size() > 0 )
		{
			CLAM::ProcessingFactory::Keys::const_iterator itKey;
			std::string category = *itCategory;
			QTreeWidgetItem * categoryTree = new QTreeWidgetItem( this, QStringList() << category.c_str());
			for(itKey = keys.begin(); itKey != keys.end(); itKey++)
			{
				std::string description;
				if(CLAM::ProcessingFactory::GetInstance().GetValuesFromAttribute(*itKey, "description").empty())
				{
					description = *itKey;
				}
				else
				{
					description = CLAM::ProcessingFactory::GetInstance().GetValuesFromAttribute(*itKey, "description").front();
				}
				std::string key = *itKey;
				QTreeWidgetItem * item = new QTreeWidgetItem( categoryTree, QStringList() << description.c_str());
				item->setIcon(0, QIcon(":/icons/images/processing.png"));
				item->setText(1, key.c_str());
			}
		}
	}

/*
	CLAM::ProcessingFactory::Keys::const_iterator itK;
	CLAM::ProcessingFactory::Keys keys = CLAM::ProcessingFactory::GetInstance().GetKeys("category", "LADSPA");
	if( keys.size() > 0)
	{
		QTreeWidgetItem * ladspaTree = new QTreeWidgetItem( this, QStringList() << "LADSPA" );
		for (	itK = keys.begin(); itK != keys.end(); itK++)
		{
			std::string description = CLAM::ProcessingFactory::GetInstance().GetValuesFromAttribute(*itK, "description").front();
			std::string factoryID = (*itK);
//			std::cout << "[LADSPA] key: " << factoryID << std::endl;
//			std::cout << "[LADSPA] description: " << description << std::endl; 
			QTreeWidgetItem * item = new QTreeWidgetItem( ladspaTree, QStringList() << description.c_str() );
			item->setIcon(0, QIcon(":/icons/images/processing.png"));
			item->setText(1, factoryID.c_str());
		}
	}
	
	keys = CLAM::ProcessingFactory::GetInstance().GetKeys("category", "CLAM");
//	std::cout << "keys size: "<< keys.size() << std::endl;
	if( keys.size() > 0)
	{
		QTreeWidgetItem * ladspaTree = new QTreeWidgetItem( this, QStringList() << "CLAM Plugins" );
		for (	itK = keys.begin(); itK != keys.end(); itK++)
		{
			std::string description = CLAM::ProcessingFactory::GetInstance().GetValuesFromAttribute(*itK, "description").front();
			std::string factoryID = (*itK);
			std::cout << "[CLAM Plugin] key: " << factoryID << std::endl;
//			std::cout << "[CLAM Plugin] description: " << description << std::endl; 
			QTreeWidgetItem * item = new QTreeWidgetItem( ladspaTree, QStringList() << description.c_str() );
			item->setIcon(0, QIcon(":/icons/images/processing.png"));
			item->setText(1, factoryID.c_str());
		}
	}
*/
	
	connect( this, SIGNAL( itemPressed(QTreeWidgetItem *,int) ),
		 this, SLOT( PressProcessing(QTreeWidgetItem *,int) ));
}

ProcessingTree::~ProcessingTree()
{
}

void ProcessingTree::PressProcessing(QTreeWidgetItem * item, int column)
{	
	if (!item) return;
	if (!item->parent()) return;

	// always use column 1
	QString className = item->text(1);
	QDrag *drag = new QDrag( this);
	QMimeData * data = new QMimeData;
	data->setText(className);
	drag->setMimeData(data);
	drag->setPixmap(QPixmap(":/icons/images/processing.png"));
	Qt::DropAction dropAction = drag->start();
}


} // namespace NetworkGUI

