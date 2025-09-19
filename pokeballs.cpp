enum POKEBALL_TYPE
{
	TYPE_BYTE = 0,
	TYPE_STRING = 1,
	TYPE_SHORT = 2,
	TYPE_LONG = 3,
};

enum POKEBALL_TYPE_BYTE
{
	COLOR1 = 0,
	COLOR2 = 1,
	COLOR3 = 2,
	COLOR4 = 3,
	UNIQUE = 4,
	BOOST = 5,
	TASK = 6,
	IV_HP = 7,
	IV_ATK = 8,
	IV_DEF = 9,
	IV_STK = 10,
	IV_SDF = 11,
	IV_SPD = 12,
	EV_HP = 13,
	EV_ATK = 14,
	EV_DEF = 15,
	EV_STK = 16,
	EV_SDF = 17,
	EV_SPD = 18,
    EV_P_HP = 19,
	EV_P_ATK = 20,
	EV_P_DEF = 21,
	EV_P_STK = 22,
	EV_P_SDF = 23,
	EV_P_SPD = 24,
	CD1 = 25,
	CD2 = 26,
	CD3 = 27,
	CD4 = 28,
	CD5 = 29,
	CD6 = 30,
	CD7 = 31,
	CD8 = 32,
	CD9 = 33,
	CD10 = 34,
	CD11 = 35,
	CD12 = 36,  

	POKEBALL_TYPE_BYTE_FIRST = COLOR1,
	POKEBALL_TYPE_BYTE_LAST = CD12,
};

enum POKEBALL_TYPE_STRING
{
	POKE = 0,
	NICK = 1,
	EHDITTO = 2,
	NATURE = 3,
	SERIAL = 4,
	AURA = 5,

	POKEBALL_TYPE_STRING_FIRST = POKE,
	POKEBALL_TYPE_STRING_LAST = AURA,
};

enum POKEBALL_TYPE_SHORT
{
	HEALTH = 0,
	HELDY = 1,
	HELDX = 2,
	ADDONNOW = 3,

	PPOKEBALL_TYPE_SHORT_FIRST = HEALTH,
	POKEBALL_TYPE_SHORT_LAST = ADDONNOW,
};

PokeBall::PokeBall(uint16_t _type) : Item(_type)
{
   // poke = ""; //nome pokemon
	//health = 0; //health atual pokemon
	//nick = ""; //apelido pokemon
	//boost = 0; //boost do pokemon
	//heldy = 0;
	//heldx = 0;
	//ehditto = "";
	//nature = ""; //nature do pokemon
	
	//serial = "";
	//unique = false;
	//color1 = 0;
	//color2 = 0;
	//color3 = 0;
	//color4 = 0;
	//addonNow = 0;
	hands = 0;
	//aura = "";
	control
	//task
	lock
	
	
	//iv_hp = random_range(0, 31); //iv do health
	//iv_atk = random_range(0, 31); //iv do atk
	//iv_def = random_range(0, 31); //iv do def
	//iv_stk = random_range(0, 31); //iv do special atk
	//iv_sdf = random_range(0, 31); //iv do special def
	//iv_spd = random_range(0, 31); //iv do speed
	//ev_hp = 0; //ev do health
	//ev_atk = 0; //ev do atk
	//ev_def = 0; //ev do def
	//ev_stk = 0; //ev do special atk
	//ev_sdf = 0; //ev do special def
	//ev_spd = 0; //ev do speed
	//ev_p_hp = 0; //ev_p do health
	//ev_p_atk = 0; //ev_p do atk
	//ev_p_def = 0; //ev_p do def
	//ev_p_stk = 0; //ev_p do special atk
	//ev_p_sdf = 0; //ev_p do special def
	//ev_p_spd = 0; //ev_p do speed
	
	
	burn
	burndmg
	poison
	poisondmg
	confuse
	sleep
	miss
	missSpell
	missEff
	fear
	fearSkill
	silence
	silenceEff
	stun
	stunEff
	stunSpell
	paralyze
	paralyzeEff
	slow
	slowEff
	leech
	leechdmg
	Buff1
	Buff2
	Buff3
	Buff1skill
    Buff2skill
	Buff3skill
	
	
	
	
	
}