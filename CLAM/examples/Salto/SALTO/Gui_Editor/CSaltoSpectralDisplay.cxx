#include "CSaltoSpectralDisplay.hxx"
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "SDTooltip.hxx"

namespace CLAM
{

CSaltoSpectralDisplay::CSaltoSpectralDisplay(int X,int Y,int W,int H):Fl_Widget(X,Y,W,H)
{
//	ptr=0;
	size=0;

 	mSDToolTip = new SDToolTip(0,0,140,15);
 
	mSDToolTip->hide();
 	window()->add(mSDToolTip);
}

CSaltoSpectralDisplay::~CSaltoSpectralDisplay()
{
	delete mSDToolTip;
}

int CSaltoSpectralDisplay::SetPtr(DataArray& displayPeakArray)
{
  int max = displayPeakArray.Size();
  memcpy(ptr,displayPeakArray.GetPtr(),max*sizeof(float));

/*  for(int i=0;i<max;i++)
  {
    ptr[i] = displayPeakArray[i];
  }
*/
  return max;
}


void CSaltoSpectralDisplay::draw(void)
{
	fl_push_clip(x(),y(),w(),h());
		fl_color(FL_BLACK);
		fl_rectf(x(),y(),w(),h());
		if (size && ptr)
		{
//			fl_color(FL_GREEN);
			float dx = float(w())/float(size);
			float cx = x();
			int yVal;
			for (int i=0;i<size;i++)
			{
				yVal = -(int)(ptr[i]);
				if (yVal<99&&yVal>0)
				{
  				fl_color(FL_RED);
					fl_line((int)cx,y()+h(),(int)cx,y()+yVal);
					fl_color(FL_WHITE);
					fl_circle((int)cx,y()+yVal,1);
				}
				cx+=dx;
			}
		}
 	fl_pop_clip();
	mSDToolTip->redraw();
}

/**
	*	Handle function, draws the tooltip when the mouse is over the widget and changes its value depending on the position
	*/
int CSaltoSpectralDisplay::handle(int e)
{
	int ex = Fl::event_x();
	int ey = Fl::event_y();

	if (e==FL_LEAVE)
	{
  	mSDToolTip->hide();
		return 1;
	}

	if (e==FL_ENTER)
	{
		mSDToolTip->position(ex,ey+10);
		mSDToolTip->SetValue( ptr[mSDToolTip->x()-x()], (22050.0/512.0)*(mSDToolTip->x()-x())  );	
		mSDToolTip->show();
		return 1;
	}

	if (e==FL_MOVE)
		{
			if (mSDToolTip->visible())	//mSDToolTip->hide();
				((Fl_Double_Window*)window())->damage(FL_DAMAGE_EXPOSE,mSDToolTip->x(),mSDToolTip->y(),mSDToolTip->w(),mSDToolTip->h());

			mSDToolTip->position(ex,ey+10);
			mSDToolTip->SetValue( ptr[mSDToolTip->x()-x()], (22050.0/512.0)*(mSDToolTip->x()-x())  );	
			
			//mSDToolTip->show();
			((Fl_Double_Window*)window())->damage(FL_DAMAGE_EXPOSE,mSDToolTip->x(),mSDToolTip->y(),mSDToolTip->w(),mSDToolTip->h());
			mSDToolTip->redraw();
			return 1;
	}

/*
	if (e==FL_MOVE)
	{
		if (Fl::event_x()-x()<100 && prev>=100)
		{
     	if (w->visible()) {
				w->hide();
//				((Fl_Window*)window())->damage(FL_DAMAGE_EXPOSE,w->x(),w->y(),100,20);
			}
		}
		if (Fl::event_x()-x()>=100 && prev<100)
		{
    	w->position(Fl::event_x(),Fl::event_y());
			sprintf(str, "%d", mBufferDuration[100]);
			w->label(str);
			w->show();
		}
		prev = Fl::event_x()-x();
	}
*/
	
	return Fl_Widget::handle(e);
}

} // end of namespace CLAM
