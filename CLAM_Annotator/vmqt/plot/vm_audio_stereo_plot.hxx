#ifndef __VMQT_AUDIO_STEREO_PLOT_H__
#define __VMQT_AUDIO_STEREO_PLOT_H__

#include "Audio.hxx"
#include "vm_multidisplay_plot.hxx"

class QGridLayout;

namespace CLAM
{
	namespace VM
	{
		class AudioStereoPlot : public CLAM::VM::MultiDisplayPlot
		{
			Q_OBJECT
		public:
			AudioStereoPlot(QWidget* parent=0);
			virtual ~AudioStereoPlot();

			virtual void set_data(const CLAM::Audio& chn0, const CLAM::Audio& chn1, bool update=false);

		signals:
			void selectedRegion(double,double);

		public slots:
			virtual void backgroundWhite();
			virtual void backgroundBlack();

			void updateLocator(double);
			void updateLocator(double,bool);

		protected:
			QGridLayout* wp_layout;
			
			enum { MASTER=0, SLAVE=1 };

		private slots:
			void setMaxVScroll(int);

		private:
			CLAM::VM::Ruler*       wp_yruler0;
			CLAM::VM::Ruler*       wp_yruler1;
			CLAM::VM::ScrollGroup* wp_vscroll;

			void create_display();
			void init_audio_stereo_plot();
			std::pair<int,int> get_zoom_steps(CLAM::TSize size);
		};
	}
}

#endif

