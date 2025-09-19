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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "pokeaddon.h"
#include "tools.h"

#include "player.h"
#include "condition.h"

#include "game.h"
extern Game g_game;

bool PokeAddons::parseOutfitNode(xmlNodePtr p)
{
	if(xmlStrcmp(p->name, (const xmlChar*)"pokeoutfit"))
		return false;

	int32_t intValue;
	if(!readXMLInteger(p, "id", intValue))
	{
		std::cout << "[Error - PokeAddons::parseOutfitNode] Missing pokeoutfit id, skipping" << std::endl;
		return false;
	}

	PokeAddon newOutfit;
	newOutfit.outfitId = intValue;

	std::string name, strValue;
	if(readXMLString(p, "default", strValue))
		newOutfit.isDefault = booleanString(strValue);

	if(!readXMLString(p, "name", strValue))
	{
		std::stringstream ss;
		ss << "PokeAddon #" << newOutfit.outfitId;
		ss >> name;
	}
	else
		name = strValue;

	bool override = false;
	if(readXMLString(p, "override", strValue) && booleanString(strValue))
		override = true;

	/*if(readXMLInteger(p, "quest", intValue))
	{
		newOutfit.storageId = intValue;
		newOutfit.storageValue = "1";
	}
	else
	{
		if(readXMLInteger(p, "storageId", intValue))
			newOutfit.storageId = intValue;

		if(readXMLString(p, "storageValue", strValue))
			newOutfit.storageValue = strValue;
	}*/ //viktor 29/03/2019

	for(xmlNodePtr listNode = p->children; listNode != NULL; listNode = listNode->next)
	{
		if(xmlStrcmp(listNode->name, (const xmlChar*)"list"))
			continue;

		PokeAddon outfit = newOutfit;
		if(!readXMLInteger(listNode, "looktype", intValue) && !readXMLInteger(listNode, "lookType", intValue))
		{
			std::cout << "[Error - PokeAddons::parseOutfitNode] Missing looktype for an outfit with id " << outfit.outfitId << std::endl;
			continue;
		}

		outfit.lookType = intValue;
		/*if(!readXMLString(listNode, "gender", strValue))
		{
			std::cout << "[Error - PokeAddons::parseOutfitNode] Missing gender(s) for an outfit with id " << outfit.outfitId
				<< " and looktype " << outfit.lookType << std::endl;
			continue;
		}*/ //viktor 29/03/2019
        strValue = "1";
		IntegerVec intVector;
		if(!parseIntegerVec(strValue, intVector))
		{
			std::cout << "[Error - PokeAddons::parseOutfitNode] Invalid gender(s) for an outfit with id " << outfit.outfitId
				<< " and looktype " << outfit.lookType << std::endl;
			continue;
		}

		if(readXMLInteger(listNode, "addons", intValue))
			outfit.addons = intValue;

		if(readXMLString(listNode, "name", strValue))
			outfit.name = strValue;
		else
			outfit.name = name;
		
		if(readXMLInteger(listNode, "surf", intValue))
			outfit.surf = intValue;
		
		if(readXMLInteger(listNode, "ride", intValue))
			outfit.ride = intValue;
		
		if(readXMLInteger(listNode, "fly", intValue))
			outfit.fly = intValue;


		bool add = false;
		PokeAddonMap::iterator fit;
		for(IntegerVec::iterator it = intVector.begin(); it != intVector.end(); ++it)
		{
			fit = pokeAddonsMap[(*it)].find(outfit.outfitId);
			if(fit != pokeAddonsMap[(*it)].end())
			{
				if(override)
				{
					fit->second = outfit;
					if(!add)
						add = true;
				}
				else
					std::cout << "[Warning - PokeAddons::parseOutfitNode] Duplicated outfit for gender " << (*it) << " with lookType " << outfit.outfitId << std::endl;
			}
			else
			{
				pokeAddonsMap[(*it)][outfit.outfitId] = outfit;
				if(!add)
					add = true;
			}
		}

		if(add)
			allPokeAddons.push_back(outfit);
	}

	return true;
}

bool PokeAddons::loadFromXml()
{
	xmlDocPtr doc = xmlParseFile(getFilePath(FILE_TYPE_XML, "pokeaddons.xml").c_str());
	if(!doc)
	{
		std::cout << "[Warning - PokeAddons::loadFromXml] Cannot load PokeAddon file, using defaults." << std::endl;
		std::cout << getLastXMLError() << std::endl;
		return false;
	}

	xmlNodePtr p, root = xmlDocGetRootElement(doc);
	if(xmlStrcmp(root->name,(const xmlChar*)"pokeoutfits"))
	{
		std::cout << "[Error - PokeAddons::loadFromXml] Malformed pokeoutfits file." << std::endl;
		xmlFreeDoc(doc);
		return false;
	}

	p = root->children;
	while(p)
	{
		parseOutfitNode(p);
		p = p->next;
	}

	xmlFreeDoc(doc);
	return true;
}

uint32_t PokeAddons::getOutfitId(uint32_t lookType)
{
	for(PokeAddonList::iterator it = allPokeAddons.begin(); it != allPokeAddons.end(); ++it)
	{
		if(it->lookType == lookType)
			return it->outfitId;
	}

	return 0;
}

bool PokeAddons::getOutfit(uint32_t lookType, PokeAddon& outfit)
{
	for(PokeAddonList::iterator it = allPokeAddons.begin(); it != allPokeAddons.end(); ++it)
	{
		if(it->lookType != lookType)
			continue;

		outfit = *it;
		return true;
	}

	return false;
}

bool PokeAddons::getOutfit(uint32_t outfitId, uint16_t sex, PokeAddon& outfit)
{
	PokeAddonMap map = pokeAddonsMap[sex];
	PokeAddonMap::iterator it = map.find(outfitId);
	if(it == map.end())
		return false;

	outfit = it->second;
	return true;
}
