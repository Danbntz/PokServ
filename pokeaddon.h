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

#ifndef __POKEADDON__
#define __POKEADDON__

#include "otsystem.h"
#include "enums.h"

#define POKEADDON_MAX_NUMBER 25

struct PokeAddon
{
	PokeAddon()
	{
		isDefault = true;
		outfitId = lookType = addons = storageId = fly = surf = ride = 0;
	}

	bool isDefault;
	uint16_t addons, fly, surf, ride;
	uint32_t outfitId, lookType, storageId;
	std::string name, storageValue;
};

typedef std::list<PokeAddon> PokeAddonList;
typedef std::map<uint32_t, PokeAddon> PokeAddonMap;

class PokeAddons
{
	public:
          
		virtual ~PokeAddons() {}
		static PokeAddons* getInstance()
		{
			static PokeAddons instance;
			return &instance;
		}

		bool loadFromXml();
		bool parseOutfitNode(xmlNodePtr p);

		bool getOutfit(uint32_t outfitId, uint16_t sex, PokeAddon& outfit);
		bool getOutfit(uint32_t lookType, PokeAddon& outfit);
		
	/*	inline PokeAddonMap::const_iterator getBegin() const 
        {
            return allPokeAddons.begin();
        }
		PokeAddonMap::iterator end()
        {
            return allPokeAddons.end();
        }*/

		uint32_t getOutfitId(uint32_t lookType);
		PokeAddonList allPokeAddons;
		

	private:
		PokeAddons() {}

		 
		std::map<uint16_t, PokeAddonMap> pokeAddonsMap;
};
#endif
