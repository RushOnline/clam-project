#include <algorithm>
#include "CLAMGL.hxx"
#include "vmDataArrayRenderer.hxx"

namespace CLAM
{
	namespace VM
	{

		DataArrayRenderer::DataArrayRenderer()
			: mHugeMode(false)
			, mHugeModeEnabled(true)
			, mMustProcessData(false)
			, mDataColor(0,0,255)
		{
		}

		DataArrayRenderer::~DataArrayRenderer()
		{
		}
	
		void DataArrayRenderer::SetData(const DataArray& data)
		{
			mCachedData = data;
			mMustProcessData = true;
			emit requestUpdate();
		}

		void DataArrayRenderer::SetDataColor(const Color& c)
		{
			mDataColor = c;
			emit requestRefresh();
		}

		void DataArrayRenderer::SetHugeModeEnabled(bool en)
		{
			mHugeModeEnabled = en;
		}

		void DataArrayRenderer::Render()
		{
			if(!mEnabled || !mCachedData.Size()) return;
			if(mMustProcessData) {ProcessData(); mMustProcessData=false;}
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(mLocalView.left,mLocalView.right,mLocalView.bottom,mLocalView.top,-1.0,1.0);
			glMatrixMode(GL_MODELVIEW);
			glLineWidth(1);
			glColor3ub(mDataColor.r,mDataColor.g,mDataColor.b);
			glBegin(GL_LINE_STRIP);
			(mHugeMode) ? DrawHugeMode() : DrawNormalMode();
			glEnd();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		void DataArrayRenderer::SetHBounds(double left, double right)
		{
			if(!mCachedData.Size()) return;
		  
			mView.left = (left-mXRange.min)*double(mCachedData.Size())/mXRange.Span();
			mView.right = (right-mXRange.min)*double(mCachedData.Size())/mXRange.Span();
			mMustProcessData = true;
		}

		void DataArrayRenderer::SetVBounds(double bottom, double top)
		{
			Renderer2D::SetVBounds(bottom,top);
			mLocalView.bottom = mView.bottom;
			mLocalView.top = mView.top;
		}

		void DataArrayRenderer::SetViewport(const GLViewport& v)
		{
			Renderer2D::SetViewport(v);
			mMustProcessData = true;
		}

		void DataArrayRenderer::ProcessData()
		{
			mLocalView.left = 0.0;
		   
			unsigned offset = (mView.left > 0) ? unsigned(mView.left) : 0;
			unsigned length = unsigned(mView.right-mView.left);

			if(mHugeModeEnabled)
			{
				if(length/5 >= (unsigned)mViewport.w)
				{
					mHugeMode = true;
					unsigned start_search, end_search, search_interval_len, search_rem_interval_len;
					unsigned max_size = (unsigned)mViewport.w;
					mMaxArray.Resize(max_size);
					mMinArray.Resize(max_size);
					mMaxArray.SetSize(max_size);
					mMinArray.SetSize(max_size);

					search_interval_len = length / max_size;
					search_rem_interval_len = length % max_size;
			
					unsigned first_pass_iterations = (search_rem_interval_len) ? max_size-1 : max_size;
					start_search = offset;
					end_search = start_search + search_interval_len;				
			
					for(unsigned i=0; i < first_pass_iterations; i++)
					{
						mMaxArray[i] = *std::max_element(mCachedData.GetPtr()+start_search, 
														 mCachedData.GetPtr()+end_search);
						mMinArray[i] = *std::min_element(mCachedData.GetPtr()+start_search, 
														 mCachedData.GetPtr()+end_search);
						start_search = end_search;
						end_search += search_interval_len;	
					}
					if(!search_rem_interval_len) return;
					mMaxArray[max_size-1] = *std::max_element(mCachedData.GetPtr()+start_search, 
															  mCachedData.GetPtr()+start_search+search_rem_interval_len);
					mMinArray[max_size-1] = *std::min_element(mCachedData.GetPtr()+start_search, 
															  mCachedData.GetPtr()+start_search+search_rem_interval_len);
					mLocalView.right = double(mMaxArray.Size())-1;
					return;
				}
			}
	
			mHugeMode = false;

			mProcessedData.Resize(length+1);
			mProcessedData.SetSize(length+1);
			std::copy(mCachedData.GetPtr()+offset, 
					  mCachedData.GetPtr()+offset+length+1, 
					  mProcessedData.GetPtr());
			mLocalView.right = double(mProcessedData.Size())-1;
		}

		void DataArrayRenderer::DrawHugeMode()
		{
			for(TSize i=0; i < mMaxArray.Size(); i++)
			{
				glVertex2d(double(i),mMaxArray[i]);
				glVertex2d(double(i),mMinArray[i]);
			}
		}

		void DataArrayRenderer::DrawNormalMode()
		{
			for(TSize i=0; i < mProcessedData.Size(); i++)
			{
				glVertex2d(double(i),mProcessedData[i]);
			}
		}
	}
}

// END
