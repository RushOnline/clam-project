/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "SegmentPresentation.hxx"
#include "ViewConfiguration.hxx"
#include "CBL.hxx"
using namespace CLAMGUI;

void ProcDataPresentation<Segment>::PublishCallbacks()
{
	Aspect a;

	HookCb hook = makeMemberFunctor1( View*, mTimeFreqPres, SegmentTFPresentation, LinkWithView );

	a.AddRequest( ProcDataView<Segment>::Services("GetTFView"), &hook );

	mLinkedView->Configure( a );
}

void ProcDataPresentation<Segment>::Show()
{
	mTimeFreqPres.Show();
}

void ProcDataPresentation<Segment>::Do()
{
	mTimeFreqPres.Do();
}

void ProcDataPresentation<Segment>::Init( const Geometry& g, const char* label )
{
}
