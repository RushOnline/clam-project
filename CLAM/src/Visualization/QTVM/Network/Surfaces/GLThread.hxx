// DEPRECATED: See NetDisplaySurface !!!
#if 0                                                         
#ifndef __GLTHREAD__
#define __GLTHREAD__

#include "Thread.hxx"

namespace CLAM
{
    namespace VM
    {
		class NetDisplaySurface;
		class NetPlotController;

		class GLThread 
		{
			public:
				GLThread(NetDisplaySurface* surf);
		                virtual ~GLThread();
	
				void resizeViewport(int w, int h);
				void Start();
				void Stop();
	    
				void setController(NetPlotController* controller);
				void setView(double left, double right, double bottom, double top);
				void setBackgColor(double r, double g, double b);

		                void SetControllerToNULL();

			private:
				volatile bool _doRendering;
				volatile bool _doResize;
		                volatile bool _terminated;
				int _w, _h;
				double _left, _right, _bottom, _top;
				double _r, _g, _b;

				NetDisplaySurface* _surf;
				NetPlotController* _controller;

		                Thread _thread;

				void thread_code();
		};
    }
}

#endif
#endif


