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

#ifndef __PROCOBJPRESENTATION__
#define __PROCOBJPRESENTATION__

#include "ProcObjView.hxx"
#include "Presentation.hxx"
#include "GeometryKit.hxx"


namespace CLAMGUI
{


	template < typename ProcObjType >
	class ProcObjPresentation
		: public Presentation
	{
	public:

		ProcObjPresentation()
			{
				Geometry geo( 100, 100, 300, 200 );
				
				Init( geo, "Generic ProcObj Presentation" );
			}
		
		ProcObjPresentation( const Geometry& geo )
			{
				Init( geo, "Generic ProcObj Presentation" );
			}

		virtual ~ProcObjPresentation()
			{
			}
		
		void PublishCallbacks()
			{
				// :TODO: To implement this
			}
		
		void Show( )
			{
				// :TODO: To implement this
			}
		
	protected:

		virtual void Init( const Geometry& geo, const char* label  )
			{
				// :TODO: To implement this
			}
		
	private:
		
	};
	
}

#endif // ProcObjPresentation.hxx
