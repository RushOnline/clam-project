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

#include <cstdio>
#include <cstring>
#include <new>
#include "ErrOpenFile.hxx"
#include "OSDefines.hxx"
#include <errno.h>

namespace CLAM {

	ErrOpenFile::ErrOpenFile(const char* filename)
	{
		mMsg = new (std::nothrow) char[1024];
		if (!mMsg) return;
		snprintf(mMsg,1024,"Opening file \"%s\" failed: %s",
		         filename,strerror(errno));
	}

}
