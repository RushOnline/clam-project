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

#ifndef __QTPLOT__
#define __QTPLOT__

#include <string>
#include <vector>
#include <qwidget.h>
#include "VMColor.hxx"

namespace CLAM
{
    namespace VM
    {
		class QtPlot : public QWidget
		{	
		public:
			QtPlot(QWidget* parent = 0, const char * name = 0, WFlags f = 0);
			virtual ~QtPlot();
			
			virtual void Label(const std::string& label)=0;
			virtual void Geometry(int x,int y,int w,int h)=0;
			virtual void Show()=0;
			virtual void Hide()=0;

			virtual void SetMarks(std::vector<unsigned>& marks)=0;
			virtual std::vector<unsigned>& GetMarks()=0; 
			virtual void SetMarksColor(Color c)=0;

			virtual std::vector<QString> GetSegmentationTags()=0;

		};
    }
}

#endif
