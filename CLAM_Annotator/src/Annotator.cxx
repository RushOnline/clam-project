#define QTPORT

#include "Annotator.hxx"
#include "Project.hxx"
#include "FrameDivision.hxx"

#include <QtGui/QAction>
#include <QtCore/QThread>
#include <qmessagebox.h>
#include <qtabwidget.h>
#include <QtGui/QFileDialog>
#include <QtCore/QSettings>
#include <QtGui/QSplitter>
#include <QtGui/QTabBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QCloseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDockWidget>
#include <QtGui/QSplashScreen>

#include <algorithm>
#include <iostream>
#include <utility>
#include <fstream>
#include <QTimer>
#include "TaskRunner.hxx"

//xamat
#include <time.h>

#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>

#include <CLAM/AudioFile.hxx>
#include <CLAM/Array.hxx>
#include <CLAM/Text.hxx>
#include <CLAM/XMLStorage.hxx>

#include <vmBPFPlot.hxx>
#include <vmAudioPlot.hxx>

#include "AudioLoadThread.hxx"

#include <CLAM/ContiguousSegmentation.hxx>
#include <CLAM/DiscontinuousSegmentation.hxx>
#include "SchemaBrowser.hxx"
#include <vmBPFPlayer.hxx>
#include <QStatusBar>
#include "ui_About.qt4.hxx"
#define VERSION "2.1"

#ifndef RESOURCES_BASE
#define RESOURCES_BASE "../resources"
#endif

using CLAM::VM::AudioPlot;
using CLAM::VM::BPFPlot;

using CLAM::TData;
using CLAM::TIndex;

void Annotator::abortLoader()
{
	if (!mAudioLoaderThread) return;
	delete mAudioLoaderThread;
	mAudioLoaderThread=0;
}
bool Annotator::loaderCreate(CLAM::Audio & audio, const std::string & filename)
{
	abortLoader();
	mAudioLoaderThread = new AudioLoadThread(audio, filename);
	return mCurrentAudio.GetSize()!=0;
}

void Annotator::loaderLaunch()
{
	CLAM_ASSERT(mAudioLoaderThread, "Launching a loader when none created");
	mAudioLoaderThread->start();
}
bool Annotator::loaderFinished()
{
	if (!mAudioLoaderThread) return true;
	if (!mAudioLoaderThread->isFinished()) return false;
	abortLoader();
	return true;
}

void Annotator::computeSongDescriptors()
{
	if (!mProjectOverview->currentItem()) return;
	QString filename = mProjectOverview->currentItem()->text(0);
	filename  = projectToAbsolutePath(filename.toStdString()).c_str();
	if (!std::ifstream(filename.toUtf8()))
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Unable to open selected file '%1'</b></p>.").arg(filename));
		return;
	}
	if (!mProject.HasExtractor() || mProject.GetExtractor()=="")
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: No extractor defined for the project.</b></p>"
					"<p>Unable to extract descriptors from song. "
					"Define the extractor first, please.</p>"));
		return;
	}
	mStatusBar << "Launching Extractor..." << mStatusBar;
	TaskRunner * runner = new TaskRunner();
	addDockWidget( Qt::BottomDockWidgetArea, runner);
	// Wait the window to be redrawn after the reconfiguration
	// before loading the cpu with the extractor
	qApp->processEvents();
	QDir projectPath(mProjectFileName.c_str());
	projectPath.cdUp();
	bool ok = runner->run(QString(mProject.GetExtractor().c_str()),
			QStringList() << filename,
			projectPath.absolutePath());
	if (!ok)
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: Unable to launch the extractor.</b></p>"
					"<p>Check that the extractor is well configured and you have permissions to run it.</p>"
					"<p>The configured command was:</p><tt>%1</tt>")
				.arg(mProject.GetExtractor().c_str())
				);
		delete runner;
		return;
	}
	return;
	/*
	while (extractor.isRunning())
	{
	}
	if (!extractor.normalExit())
	{
		QMessageBox::critical(this, tr("Extracting descriptors"),
				tr("<p><b>Error: The extractor was terminated with an error.</b></p>"));
		return;
	}
	*/
	loadDescriptorPool(); // TODO: This should be done thru an slot from the runner
}

Annotator::Annotator(const std::string & nameProject = "")
	: AnnotatorBase( )
	, QMainWindow( 0, Qt::WDestructiveClose)
	, mCurrentIndex(0)
	, mpDescriptorPool(0)
	, mFrameDescriptorsNeedUpdate(false)
	, mDescriptorsNeedSave(false)
	, mMustUpdateMarkedAudio(false)
	, mpAudioPlot(0)
	, mAudioRefreshTimer(new QTimer(this))
	, mAudioLoaderThread(0)
	, mGlobalDescriptors(0)
	, mSegmentDescriptors(0)
	, mBPFEditor(0)
	, mCurrentBPFIndex(-1)
	, mSegmentation(0)
	, mPlayer(0)
	, mStatusBar(statusBar())
{

	QSplashScreen * splash = new QSplashScreen( QPixmap(":/logos/images/annotator-splash1.png") );
	splash->setCursor( QCursor(Qt::WaitCursor) );
	splash->show();
	splash->showMessage("Loading data ... ");

	setupUi(this);
	mGlobalDescriptors = new CLAM_Annotator::DescriptorTableController(mDescriptorsTable, mProject);
	mSegmentDescriptors = new CLAM_Annotator::DescriptorTableController(mSegmentDescriptorsTable, mProject);
	mAbout = new QDialog(this);
	Ui::About aboutUi;
	aboutUi.setupUi(mAbout);
	statusBar();
	initInterface();
	setMenuAudioItemsEnabled(false);
	loadSettings();
	mAudioRefreshTimer->setSingleShot(true);
	connect (mAudioRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshAudioData()) );
	if (nameProject!="") mProjectFileName = nameProject;
	if (mProjectFileName=="") return;
	try
	{
		CLAM::XMLStorage::Restore(mProject,mProjectFileName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Project File", 
			constructFileError(mProjectFileName,e));
		return;
	}
	initProject();
	updateAuralizationOptions();
	QTimer::singleShot(1000, splash, SLOT(close()));
}

void Annotator::loadSettings()
{
	QSettings settings;
	int posX = settings.value(VERSION "/LastSession/PosX", pos().x()).toInt();
	int posY = settings.value(VERSION "/LastSession/PosY", pos().y()).toInt();
	int sizeX = settings.value(VERSION "/LastSession/SizeX", size().width()).toInt();
	int sizeY = settings.value(VERSION "/LastSession/SizeY", size().height()).toInt();
	resize(QSize(sizeX,sizeY));
	move(QPoint(posX,posY));
	mProjectFileName = settings.value(VERSION "/LastSession/ProjectFile", "").toString().toStdString();

	mRecentOpenedProjects.clear();
	QStringList recents = settings.value(VERSION "/LastSession/RecentOpenedProjects").toStringList();
	for(QStringList::Iterator it = recents.begin(); it!=recents.end(); it++ )
		appendRecentOpenedProject(it->toStdString());
}

void Annotator::saveSettings()
{
	QSettings settings;
	settings.setValue(VERSION "/LastSession/ProjectFile", mProjectFileName.c_str()  );
	settings.setValue(VERSION "/LastSession/PositionX", pos().x());
	settings.setValue(VERSION "/LastSession/PositionY", pos().y());
	settings.setValue(VERSION "/LastSession/SizeX", size().width());
	settings.setValue(VERSION "/LastSession/SizeY", size().height());

	QStringList recents;
	for (unsigned i=0; i<mRecentOpenedProjects.size(); i++)
		recents <<  mRecentOpenedProjects[i].c_str();
	settings.setValue(VERSION "/LastSession/RecentOpenedProjects", recents);
}

void Annotator::appendRecentOpenedProject(const std::string & projectFilename)
{
	std::deque<std::string>::iterator found = 
		std::find(mRecentOpenedProjects.begin(), mRecentOpenedProjects.end(), projectFilename);
	if (found != mRecentOpenedProjects.end()) mRecentOpenedProjects.erase(found);
	mRecentOpenedProjects.push_front(projectFilename);

	while (mRecentOpenedProjects.size()>MaxRecentFiles)
		mRecentOpenedProjects.pop_back();
	updateRecentFilesMenu();

}

void Annotator::updateRecentFilesMenu()
{
	mRecentFilesMenuSeparator->setVisible(mRecentOpenedProjects.size() > 0);
	for (int i = 0; i < mRecentOpenedProjects.size(); ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(mRecentOpenedProjects[i].c_str());
		mRecentFilesActions[i]->setText(text);
		mRecentFilesActions[i]->setData(mRecentOpenedProjects[i].c_str());
		mRecentFilesActions[i]->setVisible(true);
	}
	for (int i = mRecentOpenedProjects.size(); i < MaxRecentFiles; ++i)
		mRecentFilesActions[i]->setVisible(false);
}

Annotator::~Annotator()
{
	saveSettings();
	abortLoader();
	if (mSegmentation) delete mSegmentation;
}

void Annotator::initInterface()
{
	mProjectOverview->setSortingEnabled(false); // Unordered

	mProjectDocumentation = new QTextBrowser( mMainTabWidget);
	mMainTabWidget->insertTab(0, mProjectDocumentation, "Project Documentation");
	mMainTabWidget->setCurrentIndex(0);

	QVBoxLayout * frameLevelContainerLayout = new QVBoxLayout(mFrameLevelContainer);
	mFrameLevelTabBar = new QTabBar(mFrameLevelContainer);
	frameLevelContainerLayout->setMargin(2);
	frameLevelContainerLayout->addWidget(mFrameLevelTabBar);
	mBPFEditor = new BPFPlot(
			mFrameLevelContainer);
	mBPFEditor->SetFlags(CLAM::VM::eAllowVerEdition);//|CLAM::VM::eHasVerticalScroll); // QTPORT: What about this flag
	mBPFEditor->setAutoFillBackground(true);
	frameLevelContainerLayout->addWidget(mBPFEditor);
	mBPFEditor->SetZoomSteps(5,5);

	mpAudioPlot = new AudioPlot(mAudioPlotContainer); // ,0,0,false);
	QVBoxLayout * audioPlotContainerLayout = new QVBoxLayout(mAudioPlotContainer);
	mpAudioPlot->setAutoFillBackground(true);
	audioPlotContainerLayout->setMargin(2);
	audioPlotContainerLayout->addWidget(mpAudioPlot);
#ifndef QTPORT
	mpAudioPlot->Label("Audio");
	mCurrentAudio.SetSize(20000);
//	mpAudioPlot->SetData(mCurrentAudio);
#endif//QTPORT

	mSchemaBrowser = new SchemaBrowser;
	mMainTabWidget->addTab(mSchemaBrowser, "Description Schema");

	mPlayer = new CLAM::VM::BPFPlayer(this);

	resetTabOrder();
	makeConnections();
}

void Annotator::resetTabOrder()
{
#ifndef QTPORT
	setTabOrder(mFrameLevelTabBar, mBPFEditor);
	setTabOrder(mBPFEditor,mpAudioPlot);
	setTabOrder(mpAudioPlot,mSegmentationSelection);
	setTabOrder(mSegmentationSelection,mSegmentDescriptorsTable);
#endif//QTPORT
}

void Annotator::markProjectChanged(bool changed)
{
	mProjectNeedsSave = changed;
	fileSave_projectAction->setEnabled(changed);
}

void Annotator::initProject()
{
	updateSongListWidget();

	QString projectDescription = "<h1>Project Documentation</h1>\n";
	if (mProject.HasDescription())
		projectDescription += mProject.GetDescription().c_str();
	else
		projectDescription += "<p>No project documentation available</p>";
	mProjectDocumentation->setHtml(projectDescription);

	try
	{
		mProject.LoadScheme(projectToAbsolutePath(mProject.GetSchema()));
	}
	catch (CLAM::XmlStorageErr & e)
	{
		QMessageBox::warning(this,"Error Loading Schema File",
			constructFileError(mProject.GetSchema(),e));
		return;
	}
	adaptInterfaceToCurrentSchema();
	
	markProjectChanged(false);
	mDescriptorsNeedSave = false;
	appendRecentOpenedProject(mProjectFileName);
}

void Annotator::adaptInterfaceToCurrentSchema()
{
	mStatusBar << "Adapting Interface to Song level descriptors..." << mStatusBar;
	mGlobalDescriptors->refreshSchema("Song");
	mStatusBar << "Adapting Interface to Frame level descriptors..." << mStatusBar;
	adaptEnvelopesToCurrentSchema();
	mStatusBar << "Adapting Interface to Segmentations..." << mStatusBar;
	adaptSegmentationsToCurrentSchema();
	mStatusBar << "Updating schema browser..." << mStatusBar;
	mSchemaBrowser->setSchema(mProject.GetAnnotatorSchema());
	mStatusBar << "User interface adapted to the new schema." << mStatusBar;
}

void Annotator::segmentDescriptorsTableChanged(int row, int column)
{
	mSegmentDescriptors->updateData(row, mpDescriptorPool);
	markCurrentSongChanged();
}

void Annotator::globalDescriptorsTableChanged(int row, int column)
{
	mGlobalDescriptors->updateData(row, mpDescriptorPool);
	markCurrentSongChanged();
}

void Annotator::adaptSegmentationsToCurrentSchema()
{
	mSegmentationSelection->clear();
	const std::list<std::string> & segmentationNames =
		mProject.GetNamesByScopeAndType("Song", "Segmentation");
	for (std::list<std::string>::const_iterator it =  segmentationNames.begin();
		it != segmentationNames.end();
		it++)
	{
		mStatusBar << "Adding Segmentation: " << it->c_str() << mStatusBar;
		mSegmentationSelection->addItem(it->c_str());
	}
}

void Annotator::refreshSegmentation()
{
	if (!mpDescriptorPool) return;
	if (mSegmentationSelection->currentText()==QString::null) return; // No segmentation
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	const CLAM::DataArray & descriptorsMarks = 
		mpDescriptorPool->GetReadPool<CLAM::DataArray>("Song",currentSegmentation)[0];
	int nMarks = descriptorsMarks.Size();
	CLAM_Annotator::SegmentationPolicy policy = 
		mProject.GetAttributeScheme("Song",currentSegmentation).GetSegmentationPolicy();
	CLAM::Segmentation * theSegmentation=0;
	switch (policy)
	{
		case CLAM_Annotator::SegmentationPolicy::eUnsized:
			// Not yet implemented, using Continuous by now
		case CLAM_Annotator::SegmentationPolicy::eContinuous:
		{
			theSegmentation = 
				new CLAM::ContiguousSegmentation(
					mCurrentAudio.GetSize(),
					&descriptorsMarks[0],
					&descriptorsMarks[0]+nMarks);
		} break;
		case CLAM_Annotator::SegmentationPolicy::eOverlapping:
			// Not yet implemented, using Discontinuous by now
		case CLAM_Annotator::SegmentationPolicy::eDiscontinuous:
		{
			theSegmentation = 
				new CLAM::DiscontinuousSegmentation(
					mCurrentAudio.GetSize(),
					&descriptorsMarks[0],
					&descriptorsMarks[0]+nMarks);
		} break;
	}
	if (mSegmentation) delete mSegmentation;
	mSegmentation = theSegmentation;
	mpAudioPlot->SetSegmentation(mSegmentation);
	auralizeMarks();

	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	mSegmentDescriptors->refreshSchema(childScope);
}

void Annotator::updateSegmentations()
{
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	CLAM::DataArray & descriptorMarks = 
		mpDescriptorPool->GetWritePool<CLAM::DataArray>("Song",currentSegmentation)[0];
	const std::vector<double> & marks = mSegmentation->onsets();
	int nMarks = marks.size();
	descriptorMarks.Resize(nMarks);
	descriptorMarks.SetSize(nMarks);
	for (int i=0; i<nMarks; i++)
	{
		descriptorMarks[i] = marks[i];
	} 
	mDescriptorsNeedSave = true;
}

void Annotator::removeSegment(unsigned index)
{
	mStatusBar << "Removing segment at " << index << mStatusBar;
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to shrink
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid segment to be removed");
	mpDescriptorPool->Remove(childScope, index);
}

void Annotator::insertSegment(unsigned index)
{
	mStatusBar << "Inserting segment at " << index << mStatusBar;
	std::string currentSegmentation = mSegmentationSelection->currentText().toStdString();
	std::string childScope = mProject.GetAttributeScheme("Song",currentSegmentation).GetChildScope();
	if (childScope=="") return; // No child scope to grow up
	CLAM_ASSERT(index<mpDescriptorPool->GetNumberOfContexts(childScope),
		"Invalid position to insert a segment");
	mpDescriptorPool->Insert(childScope, index);
}

void Annotator::adaptEnvelopesToCurrentSchema()
{
	while (mFrameLevelTabBar->count())
		mFrameLevelTabBar->removeTab(0);

	const std::list<std::string>& names = mProject.GetNamesByScopeAndType("Frame", "Float");
	const unsigned nTabs = names.size();
	std::list<std::string>::const_iterator name = names.begin();
	for (unsigned i = 0; i<nTabs; name++, i++)
	{
		mFrameLevelTabBar->addTab(name->c_str());
	}
}

void Annotator::makeConnections()
{
	// Action Signals
	connect(fileExitAction, SIGNAL(activated()), this, SLOT(close()));
	connect(fileNew_projectAction, SIGNAL(activated()), this, SLOT(fileNew()));
	connect(fileOpen_projectAction, SIGNAL(activated()), this, SLOT(fileOpen()));
	connect(fileAdd_to_projectAction, SIGNAL(activated()), this, SLOT(addSongsToProject()));
	connect(editDelete_from_projectAction, SIGNAL(activated()), this, SLOT(deleteSongsFromProject()));
	connect(fileSave_project_asAction, SIGNAL(activated()), this, SLOT(fileSaveAs()));
	connect(fileSave_projectAction, SIGNAL(activated()), this, SLOT(fileSave()));
	connect(projectLoadSchemaAction, SIGNAL(activated()), this, SLOT(loadSchema()));
	connect(songSaveDescriptorsAction, SIGNAL(activated()), this, SLOT(saveDescriptors()));
	connect(playbackAuralizeSegmentOnsetsAction, SIGNAL(toggled(bool)), this, SLOT(updateAuralizationOptions()));
	connect(playbackAuralizeFrameLevelDescriptorsAction, SIGNAL(toggled(bool)), this, SLOT(updateAuralizationOptions()));
	connect(playbackLinkCurrentSegmentToPlaybackAction, SIGNAL(toggled(bool)), this, SLOT(linkCurrentSegmentToPlayback(bool)));
	connect(songComputeDescriptorsAction, SIGNAL(activated()), this, SLOT(computeSongDescriptors()));
	connect(helpWhats_thisAction, SIGNAL(activated()), this, SLOT(whatsThis()));
	connect(mPlayAction, SIGNAL(activated()), this, SLOT(startPlaying()));
	connect(mPauseAction, SIGNAL(activated()), this, SLOT(pausePlaying()));
	connect(mStopAction, SIGNAL(activated()), this, SLOT(stopPlaying()));
	connect(helpAboutAction,SIGNAL(activated()), mAbout,SLOT(show()));

	mRecentFilesMenuSeparator = mFileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i) {
		mRecentFilesActions.push_back(new QAction(this));
		mRecentFilesActions[i]->setVisible(false);
		connect(mRecentFilesActions[i], SIGNAL(triggered()), this, SLOT(fileOpenRecent()));
		mFileMenu->addAction(mRecentFilesActions[i]);
	}

	// Changing the current song
	connect(mProjectOverview, SIGNAL(itemSelectionChanged()),
			this, SLOT(currentSongChanged()));
	// Changing the current frame level descriptor
	connect(mFrameLevelTabBar, SIGNAL(selected(int)),
			this, SLOT(changeFrameLevelDescriptor(int)));
	// Changing the current segmentation descriptor
	connect(mSegmentationSelection, SIGNAL(activated(const QString&)),
			this, SLOT(refreshSegmentation()));
	// Apply global descriptors changes
	connect(mDescriptorsTable, SIGNAL(valueChanged( int, int) ),
			this, SLOT(globalDescriptorsTableChanged(int, int) ) );
	// Apply segment descriptors changes
	connect(mSegmentDescriptorsTable, SIGNAL(valueChanged( int, int) ),
			this, SLOT(segmentDescriptorsTableChanged(int, int) ) );
	// Apply frame descriptor changes
	connect( mBPFEditor, SIGNAL(yValueChanged(int, float)),
		 this, SLOT(frameDescriptorsChanged(int, float)));

	// Segment editing
	connect(mpAudioPlot, SIGNAL(segmentOnsetChanged(unsigned,double)),
		this, SLOT(segmentationMarksChanged(unsigned, double)));
	connect(mpAudioPlot, SIGNAL(segmentOffsetChanged(unsigned,double)),
		this, SLOT(segmentationMarksChanged(unsigned, double)));
	connect(mpAudioPlot, SIGNAL(currentSegmentChanged()),
		this, SLOT(changeCurrentSegment()));
	connect(mpAudioPlot, SIGNAL(segmentDeleted(unsigned)),
		this, SLOT(removeSegment(unsigned)));
	connect(mpAudioPlot, SIGNAL(segmentInserted(unsigned)),
		this, SLOT(insertSegment(unsigned)));

	// BPF editing
/*
	connect( mBPFEditor, SIGNAL(yValueChanged(unsigned, double)),
		 mPlayer, SLOT(updateYValue(int, double)));
*/

	// Interplot viewport syncronization
/*
	connect(mpAudioPlot, SIGNAL(xRulerRange(double,double)),
		mBPFEditor, SLOT(setHBounds(double,double)));
	connect( mBPFEditor, SIGNAL(selectedXPos(double)),
		 mpAudioPlot, SLOT(setSelectedXPos(double)));
	connect(mpAudioPlot, SIGNAL(selectedXPos(double)),
		mBPFEditor, SLOT(selectPointFromXCoord(double)));
*/
	// Interplot locator syncronization
	connect(mpAudioPlot, SIGNAL(selectedRegion(double,double)), // Was xRulerRange
		mBPFEditor, SLOT(updateLocator(double)));
	connect(mBPFEditor, SIGNAL(selectedRegion(double,double)), // Was xRulerRange
		mpAudioPlot, SLOT(updateLocator(double)));
	// Playhead update
	connect(mPlayer, SIGNAL(playingTime(double)),
		mpAudioPlot, SLOT(updateLocator(double)), Qt::DirectConnection);
	connect(mPlayer, SIGNAL(playingTime(double)),
		mBPFEditor, SLOT(updateLocator(double)), Qt::DirectConnection);
	connect(mPlayer, SIGNAL(stopTime(double,bool)),
		mBPFEditor, SLOT(updateLocator(double,bool)));
	connect( mPlayer, SIGNAL(stopTime(double,bool)),
		 mpAudioPlot, SLOT(updateLocator(double,bool)));
	connect(mpAudioPlot, SIGNAL(selectedRegion(double,double)),
		mPlayer, SLOT(timeBounds(double,double)));
	connect(mBPFEditor, SIGNAL(selectedRegion(double,double)),
		mPlayer, SLOT(timeBounds(double,double)));

	connect(mBPFEditor, SIGNAL(selectedRegion(double,double)),
		mpAudioPlot, SLOT(updateLocator(double,double)));
/*
	connect(mpAudioPlot, SIGNAL(stopPlayingTime(double)),
		this, SLOT(onStopPlaying(double)));
*/
}
void Annotator::fileOpenRecent()
{
	// This hack is from the qt example, don't ask me...
	QAction *action = qobject_cast<QAction *>(sender());
	if (!action) return;
	mProjectFileName = std::string(action->data().toString().toAscii());
	try
	{
		CLAM::XMLStorage::Restore(mProject,mProjectFileName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Project File", 
			constructFileError(mProjectFileName,e));
		return;
	}
	initProject();
}

void Annotator::linkCurrentSegmentToPlayback(bool enabled)
{
	if (enabled)
	{
		int answer = QMessageBox::warning(this, 
				tr("Linking Current Segment to Playback"),
				tr("<p>This feature is still experimental and it may hang the application.</p>"
					"<p>Are you sure you want to activate it?</p>"),
				QMessageBox::Yes | QMessageBox::Default,
				QMessageBox::Cancel | QMessageBox::Escape);
		if (answer==QMessageBox::Cancel)
		{
			playbackLinkCurrentSegmentToPlaybackAction->setChecked(false);
			return;
		}
	}
	mpAudioPlot->setCurrentSegmentFollowsPlay(enabled);
}

void Annotator::markCurrentSongChanged()
{
	mDescriptorsNeedSave = true;
	if (!mProjectOverview->currentItem()) return;
	mProjectOverview->currentItem()->setText(2, "Yes");	
}


void Annotator::changeCurrentSegment()
{
	mStatusBar << "Segment changed to " << mSegmentation->current() << mStatusBar;
	// TODO: Some widgets may have half edited information. Need update.
	// TODO: Some times is not worth to update the information (deleted segment)
	mSegmentDescriptors->refreshData(mSegmentation->current(),mpDescriptorPool);
}

void Annotator::frameDescriptorsChanged(int pointIndex,float newValue)
{
	/*TODO: right now, no matter how many points have been edited all descriptors are updated. This
	  is not too smart/efficient but doing it otherwise would mean having a dynamic list of slots 
	  in the class.*/
	int index = mFrameLevelTabBar->currentIndex();
	mBPFs[index].second.SetValue(pointIndex,TData(newValue));
	mFrameDescriptorsNeedUpdate = true;
}

void Annotator::segmentationMarksChanged(unsigned, double)
{
	updateSegmentations();
	if(isPlaying())
		mMustUpdateMarkedAudio = true;
	else
		auralizeMarks();
}

void Annotator::updateSongListWidget()
{
	mProjectOverview->clear();
	std::vector< CLAM_Annotator::Song> songs = mProject.GetSongs();
	for ( std::vector<CLAM_Annotator::Song>::const_iterator it = songs.begin() ; it != songs.end() ; it++)
	{
		QTreeWidgetItem * item = new QTreeWidgetItem(
			mProjectOverview,
			QStringList()
				<< it->GetSoundFile().c_str()
				<< tr("Yes")
				<< tr("No") );
	}
	mProjectOverview->show();
	mProjectOverview->resizeColumnToContents(0);
}

void Annotator::closeEvent ( QCloseEvent * e ) 
{
	saveDescriptors();

	if ( mProjectNeedsSave )
	{
		if(QMessageBox::question(this, "Close project", 
			"Do you want to save changes to the project?", 
			QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes)
		{
			fileSave();
		}
	}
	e->accept();
}

void Annotator::markAllSongsUnchanged()
{
	QTreeWidgetItemIterator it (mProjectOverview);
	for ( ; *it ; ++it )
	{
		(*it)->setText(2, "No");
	}
}

void Annotator::deleteSongsFromProject()
{
	QList< QTreeWidgetItem * > toBeDeleted = mProjectOverview->selectedItems();
	for ( QList< QTreeWidgetItem* >::iterator it = toBeDeleted.begin();
			it!= toBeDeleted.end(); it++ )
		delete *it;
	markProjectChanged(true);
}

void Annotator::addSongsToProject()
{
	QStringList files = QFileDialog::getOpenFileNames(this,
		"Add files to the project",
		projectToAbsolutePath(".").c_str(),
		"Songs (*.wav *.mp3 *.ogg)");
	QStringList::Iterator it = files.begin();
	for (; it != files.end(); it++ )
	{
		mProject.AppendSong(absoluteToProjectPath(it->toStdString()));
	}
	updateSongListWidget();
	markProjectChanged(true);
}

void Annotator::fileOpen()
{
	QString qFileName = QFileDialog::getOpenFileName(this, "Choose a project to work with", QString::null, "*.pro");
	if(qFileName == QString::null) return;

	mProjectFileName = std::string(qFileName.toAscii());
	try
	{
		CLAM::XMLStorage::Restore(mProject,mProjectFileName);
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,"Error Loading Project File", 
			constructFileError(mProjectFileName,e));
		return;
	}
	initProject();
}

void Annotator::fileNew()
{
	mProjectFileName = "";
	mProject = CLAM_Annotator::Project();
	loadSchema();
	initProject();
	markProjectChanged(true);
}

void Annotator::fileSaveAs()
{
	QString qFileName = QFileDialog::getSaveFileName(this, "Saving the project", QString::null,"*.pro");
	if(qFileName == QString::null) return;

	mProjectFileName = qFileName.toStdString();
	fileSave();
}

void Annotator::fileSave()
{
	if(mProjectFileName=="")
	{
		fileSaveAs();
		return;
	}
	CLAM::XMLStorage::Dump(mProject,"Project",mProjectFileName);
	markAllSongsUnchanged();
	markProjectChanged(false);
	appendRecentOpenedProject(mProjectFileName);
}

void  Annotator::loadSchema()
{
	QString qFileName = QFileDialog::getOpenFileName(
			this,
			"Choose an Schema",
			projectToAbsolutePath(".").c_str(),
			"Description Schemes (*.sc)");
	if(qFileName == QString::null) return;

	std::string schemaFile = absoluteToProjectPath(qFileName.toStdString());
	mProject.SetSchema(schemaFile);
	initProject();
}

void  Annotator::saveDescriptors()
{
	if (mFrameDescriptorsNeedUpdate)
	{
		updateEnvelopesData();
		mFrameDescriptorsNeedUpdate=false;
		mDescriptorsNeedSave=true;
	}
	if (!mDescriptorsNeedSave) return;

	if (QMessageBox::question(this,QString("Save Descriptors"),
		QString("Do you want to save current song's descriptors?"),
		QString("Save Changes"),QString("Discard Them")) != 0) return;

	CLAM::XMLStorage::Dump(*mpDescriptorPool,"Pool",projectToAbsolutePath(mCurrentDescriptorsPoolFileName));

	mDescriptorsNeedSave = false;
}

std::string Annotator::projectToAbsolutePath(const std::string & file)
{
	QString projectPath = QDir::cleanPath((mProjectFileName+"/../").c_str());
	mProject.SetBasePath(projectPath.toStdString());
	QDir qdir = QString(file.c_str());
	if (qdir.isRelative())
		return QDir::cleanPath( QDir(projectPath).filePath(file.c_str()) ).toStdString();
	return file;
}

std::string Annotator::absoluteToProjectPath(const std::string & file)
{
	QDir qdir = QString(file.c_str());
	if (qdir.isRelative()) return file;
	const std::string & basePath = mProject.GetBasePath();
	if (file.substr(0,basePath.length()+1)==(basePath+"/"))
		return file.substr(mProject.GetBasePath().length()+1);
	return file;
}

void Annotator::currentSongChanged()
{
	stopPlaying();
	mStatusBar << "Saving Previous Song Descriptors..." << mStatusBar;
	saveDescriptors();
	QTreeWidgetItem * item = mProjectOverview->currentItem();

	if (item == 0) return;

	setCursor(Qt::waitCursor);

	const std::string & filename = item->text(0).toStdString();
	mCurrentIndex = songIndexInTable(filename);
	if (mCurrentIndex <0) return;
	CLAM_Annotator::Song & currentSong = mProject.GetSongs()[mCurrentIndex];
	mCurrentSoundFileName = currentSong.GetSoundFile();
	if (currentSong.HasPoolFile())
		mCurrentDescriptorsPoolFileName = currentSong.GetPoolFile();
	else 
		mCurrentDescriptorsPoolFileName = mCurrentSoundFileName + ".pool";
	mStatusBar << "Loading descriptors..." << mStatusBar;
	loadDescriptorPool();
	mStatusBar << "Filling Global Descriptors..." << mStatusBar;
	refreshGlobalDescriptorsTable();
	mStatusBar << "Drawing Audio..." << mStatusBar;
	mAudioRefreshTimer->stop();
	
	mpAudioPlot->hide();
	mBPFEditor->hide();
	setMenuAudioItemsEnabled(false);
	const std::string absolutePath = projectToAbsolutePath(filename).c_str();
	if (!loaderCreate(mCurrentAudio, absolutePath))
	{
		QMessageBox::critical(this, tr("Error opening audio file"), absolutePath.c_str());
		return;
	}
	setMenuAudioItemsEnabled(true);

	refreshSegmentation();
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->show();
	mBPFEditor->show();
	drawAudio(projectToAbsolutePath(filename).c_str());
	mStatusBar << "Drawing LLD..." << mStatusBar;
	refreshEnvelopes();
	mStatusBar << "Done" << mStatusBar;
	loaderLaunch();
	setCursor(Qt::arrowCursor);
	mAudioRefreshTimer->start(4000);
}

void Annotator::refreshEnvelopes()
{
	if (!mpDescriptorPool) return;

	mStatusBar << "Loading LLD Data..." << mStatusBar;

	// TODO: Not all the things should be done here
	mBPFs.clear();
//	mBPFEditor->SetAudioPtr(&mCurrentAudio);
	mBPFEditor->SetXRange(0.0,double(mCurrentAudio.GetDuration())/1000.0);

	mPlayer->SetDuration(double(mCurrentAudio.GetDuration())/1000.0);
	mPlayer->SetAudioPtr(&mCurrentAudio);

	mCurrentBPFIndex = -1;

	mFrameLevelTabBar->setCurrentIndex(-1);
	const std::list<std::string>& divisionNames = mProject.GetNamesByScopeAndType("Song", "FrameDivision");

	std::list<std::string>::const_iterator divisionName;
	for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
	{
		const CLAM_Annotator::FrameDivision & division = mpDescriptorPool->GetReadPool<CLAM_Annotator::FrameDivision>("Song",*divisionName)[0];
		
		const std::string & frameDivisionChildScope = mProject.GetAttributeScheme("Song", *divisionName).GetChildScope();
		const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType(frameDivisionChildScope, "Float");
		std::list<std::string>::const_iterator it;
		for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++/*, i++*/)
		{
			CLAM::BPF transcribed;
			refreshEnvelope(transcribed, frameDivisionChildScope, *it, division);
			std::pair<TData, TData> minmaxy = GetMinMaxY(transcribed);
			BPFInfo bpf_info;
			bpf_info.first=minmaxy;
			bpf_info.second=transcribed;
			mBPFs.push_back(bpf_info);
		}
		mPlayer->SetData(mBPFs[0].second);

	}
}

void Annotator::refreshAudioData()
{
	mStatusBar << "Refresing audio..." << mStatusBar;
	bool finished= loaderFinished();
	if (finished)
	{
		mStatusBar << "Last refresh, updating segment auralization..." << mStatusBar;
		mAudioRefreshTimer->stop();
		auralizeMarks();
	}
	mpAudioPlot->SetData(mCurrentAudio,true);

	if (!finished)
		mAudioRefreshTimer->start(2000);
}

void Annotator::drawAudio(const char * filename)
{
	mpAudioPlot->hide();
	mBPFEditor->hide();
	setMenuAudioItemsEnabled(false);
	loaderCreate(mCurrentAudio, filename);
	setMenuAudioItemsEnabled(true);

	refreshSegmentation();
	mpAudioPlot->SetData(mCurrentAudio);
	mpAudioPlot->show();
	mBPFEditor->show();
}

void Annotator::refreshEnvelope(CLAM::BPF & bpf, const std::string& scope, const std::string& descriptorName, const CLAM_Annotator::FrameDivision & division)
{
	CLAM::TData firstCenter = division.GetFirstCenter();
	CLAM::TData interCenterGap = division.GetInterCenterGap();
	const CLAM::TData* values = mpDescriptorPool->GetReadPool<CLAM::TData>(scope,descriptorName);

	int audioSize=mCurrentAudio.GetSize();
	TData sr = mCurrentAudio.GetSampleRate();

	int nFrames = mpDescriptorPool->GetNumberOfContexts(scope);
	int frameSize = audioSize/nFrames;

	bpf.Resize(nFrames);
	bpf.SetSize(nFrames);
	for(int i=0; i<nFrames ;i++)
	{
		bpf.SetXValue(i,(firstCenter+i*interCenterGap)/sr);
		bpf.SetValue(i,TData(values[i]));
	}
}

void Annotator::updateEnvelopesData()
{
	// TODO: Any child scope of any FrameDivision in Song not just Frame, which may not even exist
	mFrameDescriptorsNeedUpdate = false;
	unsigned i=0, editors_size = mBPFs.size();
	std::list<std::string>::const_iterator it;
	const std::list<std::string>& descriptorsNames = mProject.GetNamesByScopeAndType("Frame", "Float");

	for(it = descriptorsNames.begin() ;i < editors_size; i++, it++)
	{
		updateEnvelopeData(i, mpDescriptorPool->GetWritePool<CLAM::TData>("Frame",*it));
	}
}

void Annotator::updateEnvelopeData(int bpfIndex, CLAM::TData* descriptor)
{
	int nPoints = mBPFs[bpfIndex].second.Size();
	for (int i=0; i<nPoints; i++)
	{
		descriptor[i] = mBPFs[bpfIndex].second.GetValueFromIndex(i);
	}
}


void Annotator::loadDescriptorPool()
{
	mFrameDescriptorsNeedUpdate = false;
	mDescriptorsNeedSave = false;

	CLAM::DescriptionDataPool * tempPool = new CLAM::DescriptionDataPool(mProject.GetDescriptionScheme());

	//Load Descriptors Pool
	CLAM_ASSERT(mCurrentDescriptorsPoolFileName!="", "Empty file name");
	std::string poolFile = projectToAbsolutePath(mCurrentDescriptorsPoolFileName);
	try
	{
		CLAM::XMLStorage::Restore(*tempPool,poolFile);
		std::ostringstream os;
		os <<"Read data did not validate with schema."<<std::endl;
		if (!mProject.ValidateDataPool(*tempPool, os))
		{
			std::cerr << os.str() << std::endl;
			QMessageBox::warning(this, tr("Error Loading Descriptors Pool File"),
					os.str().c_str());
			delete tempPool;
			return;
		}
	}
	catch (CLAM::XmlStorageErr e)
	{
		QMessageBox::warning(this,tr("Error Loading Descriptors Pool File"), 
			constructFileError(poolFile,e));
		delete tempPool;
		return;
	}

	//Create Descriptors Pool
	if (mpDescriptorPool) delete mpDescriptorPool;
	mpDescriptorPool = tempPool;
}

void Annotator::refreshGlobalDescriptorsTable()
{
	std::cout << "Refressing song data..." << std::endl;
	if (!mpDescriptorPool) return;
	mGlobalDescriptors->refreshData(0,mpDescriptorPool);
	mDescriptorsTable->show();
}

std::pair<double,double> Annotator::GetMinMaxY(const CLAM::BPF& bpf)
{
	double min_value=0;
	double max_value=0;
	for(TIndex i=0; i < bpf.Size(); i++)
	{
		double current = double(bpf.GetValueFromIndex(i));
		if(current > max_value)
		{
			max_value = current;
		}
		else if(current < min_value)
		{
			min_value = current;
		}
	}
	double span = max_value-min_value;
	min_value -= span*0.1;
	max_value += span*0.1;
	return std::make_pair(min_value, max_value);
}


int Annotator::songIndexInTable(const std::string& fileName)
{
	//TODO: have to optimize these tasks maybe by using a map or at least std::find
	std::vector<CLAM_Annotator::Song> fileNames = mProject.GetSongs();
	std::vector<CLAM_Annotator::Song>::iterator it = fileNames.begin();
	for (int i=0 ; it != fileNames.end(); it++, i++)
	{
		if (it->GetSoundFile() == fileName) return i;
	}
	return -1;
}

void Annotator::auralizeMarks()
{
	if (!mSegmentation) return;
	if(mClick.size()==0)
	{
		CLAM::AudioFile file;
		file.OpenExisting(RESOURCES_BASE"/sounds/click.wav");
		CLAM_ASSERT(file.IsReadable(), "The application requires the file '"
				RESOURCES_BASE"/sounds/click.wav' which couldn't be open.");
		int nChannels = file.GetHeader().GetChannels();
		mClick.resize(nChannels);
		for (int i=0; i<nChannels; i++)
			mClick[i].SetSize(5000);

		CLAM::MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile( file );
		CLAM::MultiChannelAudioFileReader reader(cfg);
		reader.Start();
		// Unused variable: int beginSample=0;
		reader.Do(mClick);
		reader.Stop();
	}
	const std::vector<double> & marks = mSegmentation->onsets();
	int nMarks = marks.size();
	mCurrentMarkedAudio.SetSize(0);
	mCurrentMarkedAudio.SetSize(mCurrentAudio.GetSize());
	mCurrentMarkedAudio.SetSampleRate(mCurrentAudio.GetSampleRate());
	int size = mCurrentMarkedAudio.GetSize();
	for (int i=0; i<nMarks; i++)
	{
		int samplePosition = marks[i]*mCurrentAudio.GetSampleRate();
		if(samplePosition<size)
			mCurrentMarkedAudio.SetAudioChunk(samplePosition,mClick[0]);
	} 
}

void Annotator::updateAuralizationOptions()
{
	bool playOnsets = playbackAuralizeSegmentOnsetsAction->isChecked();
	bool playLLDs = playbackAuralizeFrameLevelDescriptorsAction->isChecked();

	unsigned int LEFT_CHANNEL = 1;
	unsigned int RIGHT_CHANNEL = 2;
	mPlayer->SetAudioPtr(&mCurrentAudio);
//	if (playLLDs)
//		mPlayer->SetAudioPtr(0, LEFT_CHANNEL);
	if (playOnsets)
		mPlayer->SetAudioPtr(&mCurrentMarkedAudio, RIGHT_CHANNEL);
	mPlayer->SetPlayingFlags( CLAM::VM::eAudio | (playLLDs?CLAM::VM::eUseOscillator:0));
}

void Annotator::setMenuAudioItemsEnabled(bool enabled)
{
	playbackAuralizeSegmentOnsetsAction->setChecked(false); 
	playbackAuralizeFrameLevelDescriptorsAction->setChecked(false);
	playbackAuralizeSegmentOnsetsAction->setEnabled(enabled);
	playbackAuralizeFrameLevelDescriptorsAction->setEnabled(enabled);
}

QString Annotator::constructFileError(const std::string& fileName,const CLAM::XmlStorageErr& e)
{
	return tr(
		"<p><b>XML loading Error: <pre>%1</pre></b></p>"
		"<p>Check that your file '%2'\n"
		"is well formed and folllows the specifications"
		"</p>"
		).arg(e.what()).arg(fileName.c_str());
}

void Annotator::onStopPlaying(float time)
{
	stopPlaying();
	if(!mMustUpdateMarkedAudio) return;
	mMustUpdateMarkedAudio = false;
	auralizeMarks();
}

bool Annotator::isPlaying()
{

	return (mPlayer->IsPlaying());
}

void Annotator::changeFrameLevelDescriptor(int current)
{
	unsigned index = mFrameLevelTabBar->currentIndex();
	if (index >= (int)mBPFs.size()) return; // No valid descriptor
//	if (index == mCurrentBPFIndex) return; // No change
	mCurrentBPFIndex = index;
	double min_y = mBPFs[index].first.first;
	double max_y = mBPFs[index].first.second;
	mBPFEditor->SetData(&mBPFs[index].second);
	bool scale_log = (fabs(min_y) > 9999.99 || fabs(max_y) > 9999.99 || max_y-min_y < TData(5E-2));
	mBPFEditor->SetYRange(min_y,max_y, scale_log ? CLAM::VM::eLogScale : CLAM::VM::eLinearScale);

	mPlayer->SetData(mBPFs[index].second);
	mPlayer->SetPitchBounds(min_y, max_y);
	mBPFEditor->show();
}

void Annotator::startPlaying()
{

	if(!mPlayer) return;
	mPlayer->play();

}

void Annotator::pausePlaying()
{

	if(!mPlayer) return;
	mPlayer->pause();

}

void Annotator::stopPlaying()
{

	if(!mPlayer) return;
	mPlayer->stop();

}



