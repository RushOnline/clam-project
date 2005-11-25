#include "Segmentation.hxx"
#include "SegmentEditor.hxx"
#include <iostream>
namespace CLAM
{
	namespace VM
	{
		SegmentEditor::SegmentEditor()
			: mStrategy(0)
			, mMinX(0.0)
			, mMaxX(100.0)
			, mMinY(0.0)
			, mMaxY(100.0)
			, mLeftBound(0.0)
			, mRightBound(100.0)
			, mBottomBound(0.0)
			, mTopBound(100.0)
			, mScreenWidth(0)
			, mScreenHeight(100)
			, mMustProcessData(false)
			, mMousePressed(false)
			, mKeyInsertPressed(false)
			, mKeyDeletePressed(false)
			, mKeyShiftPressed(false)
			, mKeyCtrlPressed(false)
			, mEditionMode(Idle)
			, mDraggedSegment(0)
		{
		}

		SegmentEditor::~SegmentEditor()
		{
			if(mStrategy) delete mStrategy;
		}

		void SegmentEditor::SetSegmentation(Segmentation* s)
		{
			mStrategy = s;
			mMustProcessData = true;
		}

		void SegmentEditor::SetHBounds(double left, double right)
		{
			mLeftBound = left;
			mRightBound = right;
			mRenderer.SetHBounds(mLeftBound,mRightBound);
		}

		void SegmentEditor::SetVBounds(double bottom, double top)
		{
			mBottomBound = bottom;
			mTopBound = top;
			mRenderer.SetVBounds(mBottomBound,mTopBound);
			mRenderer.SetMargin(double(PIXMARGIN)*(mTopBound-mBottomBound)/double(mScreenHeight));
		}

		void SegmentEditor::Draw()
		{
			if(!mStrategy) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
		}

		void SegmentEditor::ScreenArea(int w, int h)
		{
			mScreenWidth = w;
			mScreenHeight = h;
			mRenderer.SetMargin(double(PIXMARGIN)*(mTopBound-mBottomBound)/double(mScreenHeight));
		}

		void SegmentEditor::SetXRange(double xmin, double xmax)
		{
			mMinX = xmin;
			mMaxX = xmax;
		}

		void SegmentEditor::SetYRange(double ymin, double ymax)
		{
			mMinY = ymin;
			mMaxY = ymax;
		}

		void SegmentEditor::MousePos(double x, double y)
		{
			if(!mStrategy) return;

			switch (mEditionMode)
			{
				case DraggingOnset:
					mStrategy->dragOnset(mDraggedSegment,x);
					mMustProcessData = true;
					emit requestRefresh();	
					return;
				case DraggingOffset:
					mStrategy->dragOffset(mDraggedSegment,x);
					mMustProcessData = true;
					emit requestRefresh();	
					return;
				case DraggingBody:
					return;
				default:
					// Just continue below
//					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}

			unsigned size = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);

			unsigned index;
			index = mStrategy->pickOnset(x,tolerance);
			if (index != size) 
			{
				emit working(true);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			index = mStrategy->pickOffset(x,tolerance);
			if (index != size) 
			{
				emit working(true);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			if(mKeyInsertPressed)
			{
				emit working(true);				
			}
		}

		void SegmentEditor::MousePressed(double x, double y)
		{
			if(!mStrategy) return;
			if(mKeyInsertPressed)
			{
				mStrategy->insert(x);
				mMustProcessData = true;
				emit requestRefresh();
			}
			unsigned nSegments = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);
			unsigned index;
			index = mStrategy->pickOnset(x,tolerance);
			if (index!=nSegments)
			{
				mEditionMode=DraggingOnset;
				mDraggedSegment=index;
				std::cout << "Dragging onset " << index << std::endl;
				return;
			}
			index = mStrategy->pickOffset(x,tolerance);
			if (index!=nSegments)
			{
				mEditionMode=DraggingOffset;
				mDraggedSegment=index;
				std::cout << "Dragging offset " << index << std::endl;
				return;
			}
			index = mStrategy->pickSegmentBody(x);
			{
				mEditionMode=DraggingBody;
				mDraggedSegment=index;
				// This two lines maybe only when Alt is pressed
				mStrategy->current(index);
				emit requestRefresh();	
				std::cout << "Current segment is " << index << std::endl;
				return;
			}
		}

		void SegmentEditor::MouseReleased(double x, double y)
		{
			mEditionMode=Idle;
			mMousePressed = false;
			emit working(false);
			emit cursorChanged(QCursor(Qt::ArrowCursor));
		}

		void SegmentEditor::KeyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					mKeyShiftPressed = true;
					break;

				case Qt::Key_Insert:
					mKeyInsertPressed = true; 
					break;
						
				case Qt::Key_Delete:
					mKeyDeletePressed = true; 
					mStrategy->remove(mStrategy->current());
					mMustProcessData = true;
					emit requestRefresh();
					break;

				default:
					break;
			}
		}

		void SegmentEditor::KeyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					mKeyShiftPressed = false;
					break;

				case Qt::Key_Insert:
					mKeyInsertPressed = false; 
					break;
						
				case Qt::Key_Delete:
					mKeyDeletePressed = false; 
					break;

				default:
					break;
			}
		}

		void SegmentEditor::ProcessData()
		{
			mRenderer.SetData(*mStrategy);
			mMustProcessData = false;
		}
	}
}

// END
