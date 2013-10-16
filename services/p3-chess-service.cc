/*
 * libretroshare/src/services: p3-chess-service.cc
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

#include "p3-chess-service.h"
#include "serialiser/rsserviceids.h"
#include "serialiser/rs-chess-data.h"

Chess_RsService * rsChessService (0);

/// @brief Constructor
/// @details Create a new serialiser
//
Chess_RsService::Chess_RsService ()
  :p3Service (RS_SERVICE_TYPE_GAME_QTCHESS),
   defaultClient (0)
{
  addSerialType (new RsChessSerialiser);
}

/// @brief Chess tick service
///
/// @return 
int Chess_RsService::tick ()
{
  /** first get rid of everything in the send queue **/
  while (!outQ.empty()) 
  {
    RsChessItem * chItem = new RsChessItem;
    *chItem = outQ.front();
    outQ.pop_front();
    sendItem (chItem);
  }

  /** now read everthing from the wires and put in the receive queue */
  
  while (receivedItems()) 
  {
    RsItem * item = recvItem ();
    RsChessItem * chItem = dynamic_cast <RsChessItem*> (item);
    if (chItem) 
    {
      inQ.push_back (*chItem);
      std::string remoteId = chItem->PeerId();
      ClientMap::iterator cit = clients.find (remoteId);
      if (cit != clients.end()) 
      {
        RsChessClient * client = cit->second;
        if (client) 
        {
          client->serviceReceive (remoteId);
        }
      } 
      else if (defaultClient) {
          defaultClient->serviceReceive (remoteId);
      }
    }
  }
  return 0;
}

/// @brief Chess service send move
///
/// @param remoteId
/// @param moveData
void Chess_RsService::sendMove (const std::string & remoteId, const std::string & moveData)
{
  int32_t serial (0);

  RsChessItem item;
  item.PeerId (remoteId);
  item.setPriorityLevel (1);
  serial++;
  item.setSerial (serial);

  item.setData (moveData);
  outQ.push_back (item);
}

/// @brief Chess service receive move
///
/// @param remoteId
/// @param moveData
///
/// @return 
int Chess_RsService::receiveMove (const std::string & remoteId, std::string & moveData)
{
  moveData.clear ();
  ItemList::iterator  it;
  for (it = inQ.begin(); it != inQ.end(); it ++) 
  {
    if (it->PeerId() == remoteId) 
    {
      moveData = it->stringData();
      inQ.erase (it);
      break;
    }
  }
  return moveData.size();
}

/// @brief Chess Rs service register client
///
/// @param remoteId
/// @param client
void Chess_RsService::registerClient (const std::string & remoteId, RsChessClient * client)
{
  clients[remoteId] = client;
}

/// @brief Chess Rs service unregister client
///
/// @param remoteId
void Chess_RsService::unregisterClient (const std::string & remoteId)
{
  clients.erase (remoteId);
}

/// @brief Chess Rs service set default client
///
/// @param client
void Chess_RsService::setDefaultClient (RsChessClient * client)
{
  defaultClient = client;
}

// EOF   
