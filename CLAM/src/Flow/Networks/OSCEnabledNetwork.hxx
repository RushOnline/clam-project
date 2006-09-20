/*
 * Copyright (c) 2001-2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _OSC_ENABLED_NETWORK_
#define _OSC_ENABLED_NETWORK_


//CLAM dependencies
#include "Network.hxx"
#include "BasicFlowControl.hxx"
#include "Thread.hxx"

//OSC dependencies
#include <oscpack/ip/NetworkingUtils.h>
#include <oscpack/ip/UdpSocket.h>
#include <oscpack/ip/TimerListener.h>
#include <oscpack/osc/OscPacketListener.h>
#include <oscpack/osc/OscReceivedElements.h>

//Other dependencies
#include <queue>
#include <list>
#include <string>

namespace CLAM
{
	class OSCEnabledNetwork : public CLAM::Network
	{
		public:
		//Listener that will process every incoming packet
		class OscReceivePacketListener : public osc::OscPacketListener
		{
			virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );

			Network* mParentNetwork;
			int mPort;
			CLAM::Thread mThread;
			UdpListeningReceiveSocket *mReceiveSocket;
			bool mIsListening;
			std::queue<std::string> mMessageLog;
			
		public:
			OscReceivePacketListener(Network * network);
			~OscReceivePacketListener();
			void Start()
			{
				if ( IsListening() )
					return;
				mThread.Start();
				mIsListening = true;
			}
			void Stop()
			{
				if (not IsListening() )
					return;
				mReceiveSocket->AsynchronousBreak();
				mThread.Stop();
				mIsListening = false;
			}
			bool IsListening() const { return mIsListening; }
			const std::string GetLogMessage();
		private:
			void AddLogMessage(const std::string& message);
		};

	private:
		OscReceivePacketListener mListener;

	public:
		OSCEnabledNetwork();
		virtual ~OSCEnabledNetwork() { }

		virtual void StartListeningOSC();
		virtual void StopListeningOSC();
		virtual bool IsListeningOSC() const { return mListener.IsListening(); }
		//Gets the first message in the log queue
		const std::string GetLogMessage(void);
};

typedef OSCEnabledNetwork::OscReceivePacketListener OscControlDispatcher;

} // namespace CLAM


#endif // _OSC_ENABLED_NETWORK_
