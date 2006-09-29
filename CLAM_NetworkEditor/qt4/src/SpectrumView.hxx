#ifndef SpectrumView_hxx
#define SpectrumView_hxx

#include <QtOpenGL/QGLWidget>
#include <QtGui/QLabel>
#include <CLAM/Processing.hxx>
#include <CLAM/PortMonitor.hxx>
#include <CLAM/DataTypes.hxx>


class SpectrumViewWidget : public QWidget
{
	enum Dimensions {
	};
public:
	SpectrumViewWidget(CLAM::Processing * processing, QWidget * parent=0)
		: QWidget(parent)
		, _monitor(dynamic_cast<CLAM::SpectrumPortMonitor*>(processing))
	{
		startTimer(50);
	}
	void paintEvent(QPaintEvent * event)
	{
		QPolygonF _line;
		QPainter painter(this);
		painter.setPen(QColor(0x77,0x77,0x77,0x77));
		painter.translate(0,height()/2);
		painter.scale(width(),height()/2);
		painter.drawLine(0,0,1,0);

		painter.setPen(Qt::black);
		const CLAM::Spectrum & audio = _monitor->FreezeAndGetData();
		const CLAM::Array<CLAM::TData> & data = audio.GetMagBuffer();
		int size = data.Size();
		for (int i=0; i<size; i++)
			_line << QPointF(double(i)/size, data[i]);
		_monitor->UnfreezeData();
		painter.drawPolyline(_line);
	}
	void timerEvent(QTimerEvent *event)
	{
		if (_monitor->GetExecState() != CLAM::Processing::Running)
			return;
		update();
	}
private:
	CLAM::SpectrumPortMonitor * _monitor;
	std::vector<CLAM::TData> _data;
};



#endif//SpectrumView_hxx
