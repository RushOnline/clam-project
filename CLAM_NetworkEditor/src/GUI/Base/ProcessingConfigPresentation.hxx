/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __PROCESSINGCONFIGPRESENTATION_HXX__
#define __PROCESSINGCONFIGPRESENTATION_HXX__

#include "Presentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace NetworkGUI
{

class ProcessingConfigPresentation : public CLAMVM::Presentation
{
protected:
//	CLAM::ProcessingConfig * mConfig;

	virtual void OnNewConfig( CLAM::ProcessingConfig* ) = 0;
public:
	ProcessingConfigPresentation();       
	virtual ~ProcessingConfigPresentation(){}
	

public: // signals
	SigSlot::Signalv1< CLAM::ProcessingConfig* > ApplyConfig;
	SigSlot::Slotv1< CLAM::ProcessingConfig* > SetConfig;
	virtual void Show() = 0;
	virtual void Hide() = 0;

};

} // namespace NetworkGUI

#endif // __PROCESSINGCONFIGPRESENTATION_HXX__
