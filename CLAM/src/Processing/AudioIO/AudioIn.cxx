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

#include "AudioIn.hxx"

using namespace CLAM;

bool AudioIn::ConcreteConfigure(const ProcessingConfig& cfg)
	throw(ErrProcessingObj)
{
	try {
		mConfig = dynamic_cast<const AudioIOConfig&>(cfg);
	}
	catch (std::bad_cast) {
		throw(ErrProcessingObj("AudioIn::ConcreteConfigure(): Wrong argument type.",this));
	}

	if (mpDevice)
		mpDevice->Unregister(*this);

	AudioManager *m;

	try {
		m = &AudioManager::Singleton();
	}
	catch (Err &e) {
		ErrProcessingObj ne("AudioIn::ConcreteConfigure(): No AudioManager found.",this);
		ne.Embed(e);
		throw(ne);
	}

	bool res;
	try {
		res = m->Register(*this);
	}
	catch (Err &e) {
		mStatus += e.what();
		res = false;
	}

	if (res == false)
		mStatus += "AudioIn::ConcreteConfigure(): "
			       "Failed to register in AudioManager.";

	Output.SetParams(mConfig.GetFrameSize());

	return res;
}

bool AudioIn::ConcreteStart(void) 
{
	if (!mpDevice)
		throw(Err("AudioIn::ConcreteStart(): No Device found!"));
	mpDevice->Start();
	return true;
}

void AudioIn::GetDeviceInfo(AudioDevice::TInfo &info) const
{
	if (mpDevice)
		mpDevice->GetInfo(info);
	else
		info.Reset();
}

bool AudioIn::Do()
{
	bool res = Do(Output.GetData());
	Output.LeaveData();
	return res;
}
