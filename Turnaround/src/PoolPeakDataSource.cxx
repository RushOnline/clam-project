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

#include <typeinfo>
#include <CLAM/Pool.hxx>
#include <CLAM/Array.hxx>
#include "PoolPeakDataSource.hxx"

#include <iostream> // debug

CLAM::VM::PoolPeakDataSource::PoolPeakDataSource()
	: _nFrames(0)
	, _frameDivision(0)
	, _samplingRate(44100)
	, _positionFrameData(0)
	, _magnitudeFrameData(0)
	, _nBins(0)
	, _currentFrame(0)
{
}

void CLAM::VM::PoolPeakDataSource::clearData()
{
	_positionData.resize(0);
	_magnitudeData.resize(0);
	_nFrames=0;
	_frameDivision=0;
	_positionFrameData=0;
	_magnitudeFrameData=0;
	_currentFrame=0;
	_nBins = 0;
}

void CLAM::VM::PoolPeakDataSource::setDataSource(unsigned nBins)
{
	_nBins = nBins;
}

void CLAM::VM::PoolPeakDataSource::updateData(std::vector<CLAM::TData> positionData, std::vector<CLAM::TData> magnitudeData, CLAM::TData samplingRate, CLAM_Annotator::FrameDivision *frameDivision, unsigned nFrames)
{
	_positionFrameData = 0;
	_magnitudeFrameData = 0;
	_samplingRate = samplingRate;
	_nFrames = nFrames;
	_frameDivision = frameDivision;

	_positionData.reserve(_nFrames*_nBins);
	_magnitudeData.reserve(_nFrames*_nBins);
	_positionData.insert(_positionData.end(), positionData.begin(), positionData.end());
	_magnitudeData.insert(_magnitudeData.end(), magnitudeData.begin(), magnitudeData.end());
	_positionFrameData = &_positionData[0];
	_magnitudeFrameData = &_magnitudeData[0];
	std::cout << "pool position size " << _positionData.size() << ", magnitude " << _magnitudeData.size() << std::endl;
}

bool CLAM::VM::PoolPeakDataSource::setCurrentTime(double timeMiliseconds)
{
	unsigned newFrame = _frameDivision ? _frameDivision->GetItem(timeMiliseconds*_samplingRate): 0;
	if (_nFrames==0) newFrame = 0;
	else if (newFrame>=_nFrames) newFrame=_nFrames-1;
	_positionFrameData = positionData()? positionData()+_nBins*newFrame : 0;
	_magnitudeFrameData = magnitudeData()? magnitudeData()+_nBins*newFrame : 0;
	if (newFrame == _currentFrame) return false;
	_currentFrame = newFrame;
	return true;
}


