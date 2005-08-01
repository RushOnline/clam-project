// generated by Fast Light User Interface Designer (fluid) version 1.0011

#ifndef Fl_Progress_hxx
#define Fl_Progress_hxx
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Box.H>

class Fl_Progress {
public:
  Fl_Progress();
  Fl_Window *mWindow;
  Fl_Slider *mSlider;
  Fl_Box *mLabel;
  ~Fl_Progress();
  static void s_timeout(void* ptr);
  void timeout(void);
  float mValue;
};


#endif
