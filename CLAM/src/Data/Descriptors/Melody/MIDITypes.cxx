#include "MIDITypes.hxx"

namespace CLAM
{
	////////////// MIDIEvent /////////////////////

	void MIDIEvent::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	unsigned MIDIEvent::Ticks() const
	{
		return (unsigned)((GetTime() * 1000.0 * 96.0) / 480.0);
	}

	int MIDIEvent::Length() const
	{
		return GetEvent().Size();
	}


	////////////// MIDITrack ///////////////////

	void MIDITrack::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetChannel(-1);
		GetTrackMelody().AddNumberOfNotes();
		GetTrackMelody().UpdateData();
		GetTrackMelody().SetNumberOfNotes(0);
	}


	/////////////// MIDISong ////////////////////////////

	void MIDISong::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNumberOfTracks(0);
	}
}

// END

