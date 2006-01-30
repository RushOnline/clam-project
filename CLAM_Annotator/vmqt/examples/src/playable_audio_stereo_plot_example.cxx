#include <stdlib.h>
#include <QApplication>
#include <QFileDialog>
#include "vm_playable_audio_stereo_plot.hxx"
#include "audio_file_utils.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	QString fn = QFileDialog::getOpenFileName(0,"Choose a stereo audio file",".","Audio (*.wav *.ogg *.mp3)");
	if(fn.isEmpty())
	{
		printf("No file was choosen.\n");
		exit(1);
	}
	std::vector<CLAM::Audio> channels;
	printf("loading audio...\n");
	qtvm_examples_utils::load_audio_st(fn.toAscii().data(),channels);
	printf("done\n");
	
	CLAM::VM::PlayableAudioStereoPlot plot;
	plot.set_title("Audio Stereo");
	plot.set_geometry(100,50,640,400);
	plot.set_data(channels[0],channels[1]);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



