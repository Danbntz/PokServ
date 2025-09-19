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

#ifndef __IOMARKET__
#define __IOMARKET__
#include "otsystem.h"
#include "player.h"
#include "database.h"

#include "creature.h"
#include "account.h" 
#include "group.h"
#include "protocol.h"
#include "outputmessage.h"

struct MarketList
{
	uint32_t id, player_id, item_Attributes_Size; 
	uint64_t item_Price, m_Time;
	uint16_t item_Id, item_Count;
	const char* item_Attributes;
	bool type_Money, price_all;
	std::map<std::string, std::string> attributesList;
	typedef std::map<std::string, std::pair<std::string, uint8_t> > AttributesListNew;
	//std::map<std::string, std::pair<std::string, uint8_t> > attributesListNew;
	AttributesListNew attributesListNew;
	MarketList(uint32_t _id, uint32_t _player_id, uint64_t _item_Price, uint16_t _item_Id, uint16_t _item_Count, const char* _item_Attributes, uint32_t _item_Attributes_Size, int64_t _m_Time, bool _type_Money, bool _price_all, std::map<std::string, std::string> _attributesList)
    {
        id = _id;
		player_id = _player_id;
		item_Price = _item_Price;
		item_Id = _item_Id;
		item_Count = _item_Count;
		item_Attributes = _item_Attributes;
		item_Attributes_Size = _item_Attributes_Size;
		m_Time = _m_Time;
		type_Money = _type_Money;
		price_all = _price_all;
		attributesList = _attributesList;
    }
	 
	MarketList()
    {
        id = 0;
		player_id = 0;
		item_Price = 0;
		item_Id = 0;
		item_Count = 0;
		item_Attributes = "";
		item_Attributes_Size = 0;
		m_Time = 0;
		type_Money = false;
		price_all = false;
		attributesList.clear();
    }
};

class IOMarket
{
	public:
		virtual ~IOMarket() {}
		static IOMarket* getMarket()
		{
			static IOMarket market;
			return &market; 
		}
		
		uint64_t getMoneyMarket(Player* player, bool type); 
		bool removeMoneyMarket(Player* player, bool type, uint64_t price);
		bool addMoneyMarket(std::string player, bool type, uint64_t value);
		
		bool buyMarket(Player* player, uint16_t id, uint16_t count);
		bool sellMarket(Player* player, Item* item, uint16_t count, uint64_t preco, bool money, bool stack);
		bool cancelSellMarket(Player* player, uint16_t id);
		
		bool loadXmlMarket();
		bool loadItemsMarket();
		bool saveItemsMarket();
		
		bool getMarketItem(const uint32_t id) const;
		bool setMarketItem(uint32_t id, MarketList marketList);
		bool setUpdateMarketItem(uint32_t id, MarketList marketList);
		
		virtual void Begin() {marketMap.begin();}
		virtual void End() {marketMap.end();}
		virtual void eraseMarket(const uint32_t id) {marketMap.erase(id);}
		
		
		//Buy Item List 
		uint16_t buyListSizeMarket(uint8_t PAGFIRST, std::string searchText);  
		bool buyListMarket(Player* player, uint8_t PAGFIRST, std::string searchText, NetworkMessage_ptr msg);
		
		//Sell Item List 
		uint8_t sellListSizeMarket(Player* player); 
		bool sellListMarket(Player* player, NetworkMessage_ptr msg);
		
		//Name and Description Item
		std::string getItemListName(MarketList marketList);
		std::string getItemListDescription(MarketList marketList);
		void unserializeListAtributes(Item* item, MarketList marketList);
		
		//unserialize Atributos 
		bool unserialize(PropStream& stream, std::map<std::string, uint8_t>& s);
		bool unserializeMap(PropStream& stream, MarketList& marketList);
		Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream, MarketList& marketList);
		bool unserializeAttr(PropStream& propStream, MarketList& marketList);
		
		

	protected: 
		IOMarket() {} 
		typedef std::map<uint32_t, MarketList> MarketMap; 
		MarketMap marketMap;
		//uint32_t nextMarketId;
		std::map<std::string, std::string> xmlDescription;
		//typedef std::map<std::string, uint8_t> AttributesNew;
		//AttributesNew attributesNew;
		
	private:
		char data[sizeof(std::string)];
		enum Type
		{
			NONE = 0,
			STRING = 1,
			INTEGER = 2,
			FLOAT = 3,
			BOOLEAN = 4
		} type;
};
#endif
