/*
 * Copyright (c) 2001-2007 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "NetworkPlayer.hxx"

namespace CLAM
{

std::string NetworkPlayer::SourcesAndSinksToString()
{
	std::string sourceNames;
	std::string sinkNames;
	const Network & net = GetNetwork();

	const Network::Processings & sources = GetSources();
	for (Network::Processings::const_iterator it=sources.begin(); it!=sources.end(); ++it)
		sourceNames += " * source:\t"+net.GetNetworkId( *it )+"\n";

	const Network::Processings & sinks = GetSinks();
	for (Network::Processings::const_iterator it=sinks.begin(); it!=sinks.end(); ++it)
		sinkNames += " * sink:\t"+net.GetNetworkId( *it )+"\n";

	return (sourceNames+sinkNames);
}

} //namespace

