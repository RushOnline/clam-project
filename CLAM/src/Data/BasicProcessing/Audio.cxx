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

#include"Audio.hxx"

using namespace CLAM;


void Audio::DefaultInit(void)
{
	// Attribute instantiation.
	AddSampleRate();
	AddBeginTime();
	AddBuffer();
	UpdateData();

	// Attribute initialization (Default values).
	SetSampleRate(44100);
	SetBeginTime(0);//By default, audio starts at 0
}

void Audio::SetEndTime(TTime time)
{
	SetSize(int((time-GetBeginTime())/1000*GetSampleRate()));
}

void Audio::SetSize(int s)
{
	int oldSize;
	oldSize=GetSize();
	if (s==oldSize)
		return;

	CLAM_ASSERT(s>=0,"Audio::SetSize(): Negative size specified");
	if (HasBuffer()) {
		GetBuffer().Resize(s);
		GetBuffer().SetSize(s);
	}
	if(s>oldSize)
		memset(GetBuffer().GetPtr()+oldSize,0,(s-oldSize)*sizeof(TData));
}

void Audio::SetDuration(TTime duration)
{
	SetSize(GetIndexFromTime(duration));
}

TTime Audio::GetTimeFromIndex(TIndex index) const
{
	return (TTime)(index*1000/GetSampleRate());
}

TIndex Audio::GetIndexFromTime(TTime time) const
{
	return (TIndex) (time/1000*GetSampleRate());
}

void Audio::GetAudioChunk(TTime beginTime, TTime endTime,Audio& chunk, bool configureChunk) const
{
	GetAudioChunk(GetIndexFromTime(beginTime),GetIndexFromTime(endTime),chunk, configureChunk);
}

void Audio::GetAudioSlice(TTime beginTime, TTime endTime,Audio& slice, bool configureSlice) const
{
	GetAudioSlice(GetIndexFromTime(beginTime),GetIndexFromTime(endTime),slice, configureSlice);
}



void Audio::GetAudioSlice( TIndex beginIndex, TIndex endIndex, Audio& slice, bool configureChunk ) const
{
	CLAM_ASSERT( beginIndex >=0, "Negative indexes are not allowed for audio slices" );
	CLAM_ASSERT( endIndex <= GetSize(), "Slices are not allowed to surpass audio size" );


	TIndex size=endIndex-beginIndex;

	DataArray tmpArray;
	tmpArray.SetPtr( GetBuffer().GetPtr() + beginIndex );
	tmpArray.SetSize( size );
	slice.SetBuffer( tmpArray );

	if(configureChunk)
	{
		slice.SetBeginTime(GetTimeFromIndex(beginIndex));
		slice.SetSampleRate( GetSampleRate() );
		slice.GetBuffer().SetSize(size);
	}
	CLAM_ASSERT(HasBuffer(),"Audio::GetAudioChunk: Buffer not initialized");
	
}

void Audio::GetAudioChunk(TIndex beginIndex,TIndex endIndex,Audio& chunk, bool configureChunk) const
{
	
	/*Note that begin index is allowed to be less than zero and the end index to be beyond the end*/
	CLAM_ASSERT(endIndex>beginIndex,

	           "Audio::GetAudioChunk: Incorrect index boundaries for audio chunk");
	TSize nBytesToCopy,offset=0;
	
	if(beginIndex>=GetSize()){
		TIndex size=endIndex-beginIndex;
		if(configureChunk) chunk.SetSize(size);
		//make sure that 0's are set in non written part of audio
		memset(chunk.GetBuffer().GetPtr(),0,size*sizeof(TData));
		return;
	}
	
	chunk.SetBeginTime(GetTimeFromIndex(beginIndex));

	if(configureChunk)
	{
		chunk.SetSampleRate( GetSampleRate() );
		TIndex size=endIndex-beginIndex;
		chunk.SetSize(size);
		chunk.SetSampleRate( GetSampleRate() );
		chunk.SetBeginTime( GetTimeFromIndex(beginIndex) );
	}
	
		
	CLAM_ASSERT(HasBuffer(),"Audio::GetAudioChunk: Buffer not initialized");
	
	/*Whenever trying to copy samples before the beginning or after end of 
	  actual audio, zeros will be added at the beginning or end of chunk*/

	if(beginIndex<0)
	{
		offset=-beginIndex;
		beginIndex=0;
		//make sure that 0's are set in non written part of audio
		memset(chunk.GetBuffer().GetPtr(),0,offset*sizeof(TData));
	}
	if(endIndex>=GetSize())
	{ 
		TSize ending=endIndex-GetSize();
		memset(chunk.GetBuffer().GetPtr()+GetSize()-beginIndex ,0,ending*sizeof(TData));
		endIndex=GetSize();
	}


	nBytesToCopy=(endIndex-beginIndex)*sizeof(TData);
	
	CLAM_ASSERT(
		nBytesToCopy>=0
		&& beginIndex>=0
		&& int(nBytesToCopy+beginIndex*sizeof(TData))<=GetBuffer().SizeInBytes(),
		"Error");

	memcpy(chunk.GetBuffer().GetPtr()+offset,GetBuffer().GetPtr()+beginIndex,nBytesToCopy);
}

void Audio::SetAudioChunk(TTime beginTime,const Audio& chunk)
{
	SetAudioChunk(GetIndexFromTime(beginTime),chunk);
}

void Audio::SetAudioChunk(TIndex beginIndex,const Audio& chunk)
{
	CLAM_ASSERT(beginIndex<GetSize(),"Audio::SetAudioChunk: Incorrect begin index");
	TSize nBytesToCopy,offset=0;
	TIndex endIndex=beginIndex+chunk.GetSize();
	if(endIndex>GetSize()) endIndex=GetSize();
	if(beginIndex<0){ 
		offset=-beginIndex;
		beginIndex=0;}
	
	CLAM_ASSERT(chunk.HasBuffer()&&HasBuffer(),"Audio::SetAudioChunk: one of the buffers is not initialized") ;
	nBytesToCopy=(endIndex-beginIndex)*sizeof(TData);
	memcpy(GetBuffer().GetPtr()+beginIndex,chunk.GetBuffer().GetPtr()+offset,nBytesToCopy);
}
