/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __VMQT_PLOT_FUNDAMENTAL_H__
#define __VMQT_PLOT_FUNDAMENTAL_H__

#include <string>
#include "vmPlayableFundamentalPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		int PlotFundamental(const CLAM::Segment& s,
							int x=100,
							int y=100,
							int w=600,
							int h=250,
							const std::string& title="Fundamental Frequency Plot")
		{
			QApplication app;
			PlayableFundamentalPlot plot;
			plot.SetTitle(title.c_str());
			plot.SetGeometry(x,y,w,h);
			plot.SetData(s);
			plot.show();
			return app.exec();
		}
	}
}

#endif

