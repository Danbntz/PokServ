////////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////
#include "otpch.h"
#include "duel.h"

#include "player.h"
#include "game.h"
#include "configmanager.h"

extern Game g_game;
extern ConfigManager g_config;

Duel::Duel(Player* _leader)
{
	if(_leader)
	{
		leader = _leader;
		leader->setDuel(this);
		leader->sendPlayerDuelIcons(leader);
	}
}

void Duel::disband()
{
	leader->setDuel(NULL);
	leader->sendTextMessage(MSG_INFO_DESCR, "Your duel has been disbanded.");
    std::string valor = "-1";
    leader->setStorage(89142, valor);
    leader->setStorage(84910, "-1");
    leader->setStorage(84911, "-1");
	leader->setPDuel(0);
	leader->updateLosesBall(0);
    
	leader->sendPlayerDuelIcons(leader);
	for(PlayerVector::iterator it = inviteList.begin(); it != inviteList.end(); ++it)
	{
		(*it)->removeDuelInvitation(this);
		(*it)->sendPlayerDuelIcons(leader);
		(*it)->sendPlayerDuelIcons(*it);
		leader->sendPlayerDuelIcons(*it);
	}

	inviteList.clear();
	for(PlayerVector::iterator it = memberList.begin(); it != memberList.end(); ++it)
	{
		(*it)->setDuel(NULL);
		(*it)->sendTextMessage(MSG_INFO_DESCR, "Your duel has been disbanded.");

		(*it)->sendPlayerDuelIcons(*it);
		(*it)->sendPlayerDuelIcons(leader);
		leader->sendPlayerDuelIcons(*it);
		(*it)->setPDuel(0);
		(*it)->updateLosesBall(0);
		(*it)->setStorage(84910, "-1");
		(*it)->setStorage(84911, "-1");
	}

	memberList.clear();
	leader = NULL;
	delete this;
}

bool Duel::leave(Player* player)
{
	if(!isPlayerMember(player) && player != leader)
		return false;

	bool missingLeader = false;
	if(leader == player)
	{
		if(!memberList.empty())
		{
			if(memberList.size() == 1 && inviteList.empty())
				missingLeader = true;
		}
		else
			missingLeader = true;
	}

	//since we already passed the leadership, we remove the player from the list
	PlayerVector::iterator it = std::find(memberList.begin(), memberList.end(), player);
	if(it != memberList.end())
		memberList.erase(it);

	it = std::find(inviteList.begin(), inviteList.end(), player);
	if(it != inviteList.end())
		inviteList.erase(it);

	player->setDuel(NULL);

	player->sendTextMessage(MSG_INFO_DESCR, "You have left the duel.");
	player->sendPlayerDuelIcons(player);
	player->setPDuel(0);
	player->updateLosesBall(0);
	player->setStorage(84910, "-1");
	player->setStorage(84911, "-1");

	updateIcons(player);

	char buffer[105];
	sprintf(buffer, "%s has left the duel.", player->getName().c_str());
	
	
	if(player->getVocationId() > 1)
	player->setVocation(1);

	broadcastMessage(MSG_INFO_DESCR, buffer);
	if(missingLeader || canDisband())
		disband();

	return true;
}

bool Duel::join(Player* player)
{
	if(isPlayerMember(player) || !isPlayerInvited(player))
		return false;
		
		if(!player->getSummonCount() == 1 || !leader->getSummonCount() == 1){
            char buffer[200];
			sprintf(buffer, "Sorry not have pokemon.");
	        player->sendTextMessage(MSG_INFO_DESCR, buffer);
            return false;
            }
		
		if(player->getMana() < leader->getPDuel()){
            char buffer[200];
			sprintf(buffer, "Sorry not have %d pokemons.", leader->getPDuel());
	        player->sendTextMessage(MSG_INFO_DESCR, buffer);
            return false;
            }
		
	memberList.push_back(player);
	player->setDuel(this);

	player->removeDuelInvitation(this);
	PlayerVector::iterator it = std::find(inviteList.begin(), inviteList.end(), player);
	if(it != inviteList.end())
		inviteList.erase(it);

	char buffer[200];
	sprintf(buffer, "%s has joined the duel.", player->getName().c_str());
	leader->sendTextMessage(MSG_INFO_DESCR, buffer);

	sprintf(buffer, "You have joined %s'%s duel.", leader->getName().c_str(), (leader->getName()[leader->getName().length() - 1] == 's' ? "" : "s"));
	player->sendTextMessage(MSG_INFO_DESCR, buffer);
	player->setPDuel(leader->getPDuel());
	player->updateLosesBall(0);
	player->setStorage(84911, "1");
	leader->setStorage(84911, "1");
	player->setStorage(84910, leader->getName().c_str());
	leader->setStorage(84910, player->getName().c_str());

	updateIcons(player);
	
	std::string text = "START";
	g_game.addAnimatedText(player->getPosition(), 215, text);
	g_game.addAnimatedText(leader->getPosition(), 215, text);
	
	return true;
}

bool Duel::removeInvite(Player* player)
{
	if(!isPlayerInvited(player))
		return false;

	PlayerVector::iterator it = std::find(inviteList.begin(), inviteList.end(), player);
	if(it != inviteList.end())
		inviteList.erase(it);

	leader->sendPlayerDuelIcons(player);
	player->sendPlayerDuelIcons(leader);

	player->removeDuelInvitation(this);
	if(canDisband())
		disband();

	return true;
}

void Duel::revokeInvitation(Player* player)
{
	if(!player || player->isRemoved())
		return;

	char buffer[150];
	sprintf(buffer, "%s has revoked %s invitation.", leader->getName().c_str(), (leader->getSex(false) ? "his" : "her"));
	player->sendTextMessage(MSG_INFO_DESCR, buffer);

	sprintf(buffer, "Invitation for %s has been revoked.", player->getName().c_str());
	leader->sendTextMessage(MSG_INFO_DESCR, buffer);
	removeInvite(player);
}

bool Duel::invitePlayer(Player* player)
{      
	leader->setVocation(7);
    leader->sendChannelsDialog();
    std::string valor = player->getName().c_str();
    leader->setStorage(89142, valor);
	leader->updateLosesBall(0);
	return true;
}

bool Duel::okDuel(Player* player)
{
	if(isPlayerInvited(player, true))
		return false;

	inviteList.push_back(player);
	player->addDuelInvitation(this);

	char buffer[150];
	sprintf(buffer, "%s has been invited in duel.%s", player->getName().c_str(), (!memberList.size() ? "." : ""));
	leader->sendTextMessage(MSG_INFO_DESCR, buffer);
	
    sprintf(buffer, "%s has invited you to %s duel.", leader->getName().c_str(), (leader->getSex(false) ? "his" : "her"));
	player->sendTextMessage(MSG_INFO_DESCR, buffer);
	
    sprintf(buffer, "Info Battle: Duel 1x1 - %d pokes.", leader->getPDuel());
	player->sendTextMessage(MSG_INFO_DESCR, buffer);
    
    leader->setVocation(1);
    leader->sendTextMessage(MSG_INFO_DESCR, buffer);
	leader->sendPlayerDuelIcons(player);
	player->sendPlayerDuelIcons(leader);
	return true;
}

void Duel::updateIcons(Player* player)
{
	if(!player || player->isRemoved())
		return;

	PlayerVector::iterator it;
	for(it = memberList.begin(); it != memberList.end(); ++it)
	{
		(*it)->sendPlayerDuelIcons(player);
		player->sendPlayerDuelIcons((*it));
	}

	for(it = inviteList.begin(); it != inviteList.end(); ++it)
	{
		(*it)->sendPlayerDuelIcons(player);
		player->sendPlayerDuelIcons((*it));
	}

	leader->sendPlayerDuelIcons(player);
	player->sendPlayerDuelIcons(leader);
}

void Duel::updateAllIcons()
{
	PlayerVector::iterator it;
	for(it = memberList.begin(); it != memberList.end(); ++it)
	{
		for(PlayerVector::iterator iit = memberList.begin(); iit != memberList.end(); ++iit)
			(*it)->sendPlayerDuelIcons((*iit));

		(*it)->sendPlayerDuelIcons(leader);
		leader->sendPlayerDuelIcons((*it));
	}

	leader->sendPlayerDuelIcons(leader);
	for(it = inviteList.begin(); it != inviteList.end(); ++it)
		(*it)->sendPlayerDuelIcons(leader);
}

void Duel::broadcastMessage(MessageClasses messageClass, const std::string& text, bool sendToInvitations/* = false*/)
{
	PlayerVector::iterator it;
	if(!memberList.empty())
	{
		for(it = memberList.begin(); it != memberList.end(); ++it)
			(*it)->sendTextMessage(messageClass, text);
	}

	leader->sendTextMessage(messageClass, text);
	if(!sendToInvitations || inviteList.empty())
		return;

	for(it = inviteList.begin(); it != inviteList.end(); ++it)
		(*it)->sendTextMessage(messageClass, text);
}

bool Duel::isPlayerMember(const Player* player, bool result/* = false*/) const
{
	if(!player || player->isRemoved())
		return result;

	return std::find(memberList.begin(), memberList.end(), player) != memberList.end();
}

bool Duel::isPlayerInvited(const Player* player, bool result/* = false*/) const
{
	if(!player || player->isRemoved())
		return result;

	return std::find(inviteList.begin(), inviteList.end(), player) != inviteList.end();
}
