// libTorrent - BitTorrent library
// Copyright (C) 2005, Jari Sundell
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// In addition, as a special exception, the copyright holders give
// permission to link the code of portions of this program with the
// OpenSSL library under certain conditions as described in each
// individual source file, and distribute linked combinations
// including the two.
//
// You must obey the GNU General Public License in all respects for
// all of the code used other than OpenSSL.  If you modify file(s)
// with this exception, you may extend this exception to your version
// of the file(s), but you are not obligated to do so.  If you do not
// wish to do so, delete this exception statement from your version.
// If you delete this exception statement from all source files in the
// program, then also delete it here.
//
// Contact:  Jari Sundell <jaris@ifi.uio.no>
//
//           Skomakerveien 33
//           3185 Skoppum, NORWAY

#ifndef LIBTORRENT_TRACKER_TRACKER_UDP_H
#define LIBTORRENT_TRACKER_TRACKER_UDP_H

#include "net/socket_address.h"
#include "net/socket_datagram.h"
#include "utils/task.h"
#include "tracker_base.h"

namespace torrent {

class TrackerUdp : public SocketDatagram, public TrackerBase {
public:
  TrackerUdp(TrackerInfo* info, const std::string& url);
  ~TrackerUdp();
  
  virtual bool        is_busy() const;

  virtual void        send_state(TrackerInfo::State state,
				 uint64_t down,
				 uint64_t up,
				 uint64_t left);

  virtual void        close();

  virtual void        read();
  virtual void        write();
  virtual void        except();

private:
  void                receive_failed(const std::string& msg);

  bool                parse_url();

  SocketAddress       m_connectAddress;
  SocketAddress       m_bindAddress;

  char*               m_readBuffer;
  int                 m_readLength;
  
  char*               m_writeBuffer;
  int                 m_writeLength;

  TaskItem            m_taskDelay;
};

}

#endif