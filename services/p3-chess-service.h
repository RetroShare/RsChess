/*
 * libretroshare/src/services: p3-chess-service.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2011 by Bernd Stramm.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com",
 * and "Bernd.Stramm@gmail.com".
 */

#ifndef RS_QTCHESS_SERVICE_H
#define RS_QTCHESS_SERVICE_H

#include "p3service.h"
#include "serialiser/rs-chess-data.h"

#include <deque>
#include <map>
#include <string>

/// @brief Class Rs chess client
class RsChessClient 
{
public:

  RsChessClient () {}

  virtual void serviceReceive (const std::string & remoteId) = 0; 

};


/// @brief Class Chess Rs service
//
class Chess_RsService : public p3Service
{
public:

  Chess_RsService ();
  int tick ();

  void sendMove (const std::string & remoteId, const std::string & moveData);
  int receiveMove (const std::string & remoteId, std::string & moveData);

  void registerClient (const std::string & remoteId, RsChessClient * client);
  void unregisterClient (const std::string & remoteId);
  void setDefaultClient (RsChessClient * client);

private:

  typedef std::deque <RsChessItem> ItemList;
  typedef std::map <std::string, RsChessClient *>  ClientMap;

  ItemList  outQ;
  ItemList  inQ;

  ClientMap          clients;
  RsChessClient    * defaultClient;

};


extern Chess_RsService * rsChessService;

#endif

// EOF   
