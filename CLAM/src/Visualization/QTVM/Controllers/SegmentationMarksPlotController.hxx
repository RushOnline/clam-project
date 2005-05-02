/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __SEGMENTATIONMARKSPLOTCONTROLLER__
#define __SEGMENTATIONMARKSPLOTCONTROLLER__

#include <vector>
#include "VLineArrayRenderer.hxx"
#include "PlotController.hxx"

namespace CLAM
{
    namespace VM
    {
	class SegmentationMarksPlotController : public PlotController
	{
 
	public:
	    SegmentationMarksPlotController();
	    virtual ~SegmentationMarksPlotController();

	    void SetMarks(std::vector<unsigned>& marks);
	    std::vector<unsigned>& GetMarks();

	    void SetMarksColor(Color c);

	    virtual void Draw();
	    virtual void SurfaceDimensions(int w,int h);

	    virtual void SetMousePos(TData x, TData y);
	    virtual void LeaveMouse();

	    virtual void SetLeftButtonPressed(bool pressed);

	    void SetKeyInsertPressed(bool pressed);
	    void SetKeyDeletePressed(bool pressed);

	    void InsertMark(unsigned elem);
	    void RemoveMark(int index, unsigned elem);
	    void UpdateMark(int index, unsigned elem);

	protected:
	    virtual void SetHBounds(const TData& left, const TData& right);
	    virtual void SetVBounds(const TData& bottom, const TData& top);

	    virtual bool CanDrawSelectedPos();

	private:
	    std::vector<unsigned> _marks;
	    VLineArrayRenderer _marksRenderer;
	    bool _mustProcessMarks;
	    bool _hit;
	    unsigned _currentElem;
	    int _currentIndex;
	    bool _keyInsertPressed;
	    bool _keyDeletePressed;

	    void ProcessMarksData();

	    bool HaveElem(unsigned elem);
	    void InsertElem(unsigned elem);
	    void RemoveElem(int index);
	    void Update(int index, unsigned elem);

	    unsigned GetPixel(const TData& x) const;
	    int Hit(const TData& x);
	};
    }
}

#endif




