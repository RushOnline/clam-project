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

#include "GLWaveRenderer.hxx"
#include "DataTypes.hxx"
using CLAM::TData;
using namespace CLAMGUI;

void GLWaveRenderer::InitArray( unsigned int n_elems )
{
	GLfloat color[3] = { 0.0f, 0.0f, 0.0f };
	color[0] = GLfloat(r)/255.0f; color[1] = GLfloat(g)/255.0f; color[2] = GLfloat(b)/255.0f;

	ArrangeXScale();

	for (unsigned int k = 0; k < n_elems; k++ )
	{
		mIntertwined[k].r = color[0]; mIntertwined[k].g = color[1]; mIntertwined[k].b = color[2];

		mIntertwined[k].z = 0.0f;             // We make zero the z-values
	}
}

void GLWaveRenderer::ArrangeXScale()
{
	// we update here the x-axis coordinates

	CLAM_ASSERT( mSampRate!=0, "Sampling Rate was not set" );

	float invNFs = GLfloat( 1.0f / mSampRate );

	for ( unsigned int k = 0; k < mIntertwined.size(); k++ )
		mIntertwined[k].x = GLfloat( k * invNFs );
	
	mXConversionFactor = mSampRate;
}

void GLWaveRenderer::SetSamplingRate( TData value )
{
	mSampRate = value;

	ArrangeXScale();
}

void GLWaveRenderer::XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
{
	transleft = left;

	transright = ( mSampRate == 0 ? right : right*(1.0f/mSampRate) );
	integer = false;
}

void GLWaveRenderer::YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
{

	transtop = top + 1.0f;
	transbottom = bottom - 1.0f;
	integer = false;
}
