#include <vector>
#include <qlayout.h>
#include <qframe.h>
#include "AudioPlotController.hxx"
#include "QtAudioPlayer.hxx"
#include "SingleLabel.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtAudioPlot::QtAudioPlot(QWidget* parent) : QtPresentation(parent)
		{
			_playBounds.SetBegin(-1.0);
			_playBounds.SetEnd(-1.0);
			showRightAmp=true;
			SetPlotController();
			InitAudioPlot();	
			Connect();
		}

		QtAudioPlot::~QtAudioPlot()
		{
		}

		void QtAudioPlot::InitAudioPlot()
		{
			_panel = new QHBoxLayout;
			
			lefthole = new QFrame(this);
			lefthole->setFixedSize(50,30);
			_panel->addWidget(lefthole);

			_player = new QtAudioPlayer(this);
			_player->setFixedSize(75,30);
			_panel->addWidget(_player);

			_panel->addStretch(1);

			_leftAmpLab = new SingleLabel(this,"","Amplitude");
			_rightAmpLab = new SingleLabel(this,"","Amplitude Right");

			_panel->addWidget(_leftAmpLab);
			_panel->addWidget(_rightAmpLab);

			_panel->addStretch(1);

			_labelsGroup = new TimeSegmentLabelsGroup(this);
			_labelsGroup->setFixedSize(186,25);
			_panel->addWidget(_labelsGroup);

			righthole = new QFrame(this);
			righthole->setFixedSize(20,30);
			_panel->addWidget(righthole);

			AddToMainLayout(_panel);	
		}

		void QtAudioPlot::SetData(const Audio& audio)
		{
			((AudioPlotController*)_controller)->SetData(audio);
			SetPData(audio);
		}

		void QtAudioPlot::SetForegroundColor(Color c)
		{
			((AudioPlotController*)_controller)->SetDataColor(c);
		}

		void QtAudioPlot::SetDialColor(Color c)
		{
			((AudioPlotController*)_controller)->SetDialColor(c);
		}

		void QtAudioPlot::SetRegionColor(Color c)
		{
			((AudioPlotController*)_controller)->SetRegionColor(c);
		}

		void QtAudioPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
						((AudioPlotController*)_controller)->SetKeyShiftPressed(true);
						break;
				default:
						break;
			}
		}

		void QtAudioPlot::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
						((AudioPlotController*)_controller)->SetKeyShiftPressed(false);			
						break;
				default:
						break;
			}
		}

		void QtAudioPlot::updateRegion(MediaTime time)
		{
			if(	time.GetBegin()==_playBounds.GetBegin() &&
				time.GetEnd()==_playBounds.GetEnd()) return;
	
			_playBounds=time;
			_player->stop();
			_player->SetPlaySegment(time);
			_labelsGroup->UpdateLabels(time);

			UpdateAmpLabels(time);
			emit regionTime(time);
		}

		void QtAudioPlot::UpdateAmpLabels(MediaTime time)
		{
			if(time.HasDuration())
			{
			    _leftAmpLab->Update(((AudioPlotController*)_controller)->GetAmp(time.GetBegin()));
				_leftAmpLab->SetToolTip("Amplitude Left");
				_rightAmpLab->Update(((AudioPlotController*)_controller)->GetAmp(time.GetEnd()));
				if(showRightAmp)
				{
					_rightAmpLab->show();
				}
			}
			else
			{
				_leftAmpLab->Update(((AudioPlotController*)_controller)->GetAmp(time.GetBegin()));
				_leftAmpLab->SetToolTip("Amplitude");
				_rightAmpLab->clear();
				_rightAmpLab->hide();
			}
		}

		void QtAudioPlot::SetPlotController()
		{
			SetController(new AudioPlotController());
		}

		void QtAudioPlot::Connect()
		{
			// Connections
			connect(((AudioPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((AudioPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((AudioPlotController*)_controller),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
		}

		void QtAudioPlot::SetPData(const Audio& audio)
		{
			std::vector<Audio> data;
			data.resize(1);
			data[0]=audio;
			((QtAudioPlayer*)_player)->SetData(data);
		}

		void QtAudioPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
		}

		void QtAudioPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
		}

		void QtAudioPlot::closeEvent(QCloseEvent *e)
		{
			RemoveFromPlayList();
			QtPlot::closeEvent(e);
		}

		void QtAudioPlot::RemovePlayPanel()
		{
			RemoveFromMainLayout(_panel);
			lefthole->hide();
			_player->hide();
			_leftAmpLab->hide();
			_rightAmpLab->hide();
			_labelsGroup->hide();
			showRightAmp=false;
		}

		void QtAudioPlot::SetKeyPressed(QKeyEvent* e)
		{
			keyPressEvent(e);
		}

		void QtAudioPlot::SetKeyReleased(QKeyEvent* e)
		{
			keyReleaseEvent(e);
		}

		void QtAudioPlot::SetMarks(std::list<unsigned>& marks)
		{
			((AudioPlotController*)_controller)->SetMarks(marks);
		}

		void QtAudioPlot::SetMarksColor(Color c)
		{
			((AudioPlotController*)_controller)->SetMarksColor(c);
		}
	}	
}

// END

