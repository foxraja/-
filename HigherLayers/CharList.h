#ifndef _DRAGON_CHARLIST_
#define _DRAGON_CHARLIST_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// Ä³¸¯ÅÍ¿¡ °üÇÑ Å¬·¡½º
template<class T1, class T2, class T3> 
class TCharList : public T1, public T2, public T3
{ 
public:
	void CorrectRiseFall(BYTE nType, int nX, int nY, int& rDamage);
	void CorrectStrikeRange(int nDistance, int& rDamage);
	void CorrectMagicRange(int nDistance, int& rDamage);
	void CorrectCombatRange(int nDistance, int& rDamage);
	
	int CalcAttackSuccessRate(int nStatic = 0, int nDynamic = 0);
	int CalcMagicSuccessRate(int nDefence = 0);
	int CalcPhysicalStrikingPower(int nPercent = 0, int nAdd = 0);
	int CalcPhysicalDefensivePower(int nAttack, int nPercent = 0);
	int CalcMagicStrikingPower(int nHD, int nPercent = 0, int nAdd = 0);
	int CalcMagicDefensivePower(BYTE nType, int nAttack, int nPercent = 0);
	int CalcMagicCastReady(BYTE nMagic);
	int CalcMagicPeriod(BYTE nType, int nContinue);
	int CalcResistPeriod(BYTE nType, int nContinue);
	int CalcMovePower();
	
	bool IsStrikePiercing() const;       // CSD-021024
	bool IsMagicPiercing() const;        // CSD-021024
	bool IsCurseAvoid(BYTE nType) const; // CSD-021024
	
	int GetExtPoisonByItem() const;//¾ÆÀÌÅÛ¿¡ ÀÇÇØ ´õÇØÁö´Â ¸¶¹ý¹æ¾î·Â
	int GetExtCurseByItem() const;
	int GetExtHolyByItem() const;
	int GetExtFireByItem() const;
	int GetExtIceByItem() const;
	int GetExtElectByItem() const;
	
protected:  
	int GetExtPoison() const;//ÃÖÁ¾ ¸¶¹ý¹æ¾î·ÂÀÇ ÇÕ( GetBasicResist(RT_XXX) + GetExtPoisonByItem())
	int GetExtCurse() const;
	int GetExtHoly() const;
	int GetExtFire() const;
	int GetExtIce() const;
	int GetExtElect() const;
};

///////////////////////////////////////////////////////////////////////////////
// Public Method
///////////////////////////////////////////////////////////////////////////////

template<class T1, class T2, class T3> 
void TCharList<T1, T2, T3>::CorrectRiseFall(BYTE nType, int nX, int nY, int& rDamage)
{	// ³ô³·ÀÌ¿¡ µû¸¥ º¸Á¤
	const BYTE nRiseFall = GetRiseFall(nX, nY);	
	const int nTemp = rDamage*0.2; // ³ô³·ÀÌ¿¡ µû¶ó 20% µ¥¹ÌÁö Â÷ÀÌ
	
	switch (nType)
	{
    case 0: // °í->Àú, Àú->°í, °°Àº ³ôÀÌ ¸ðµç °ø°ÝÀÌ °¡´ÉÇÑ °æ¿ì
		{
			if (nRiseFall == 1)  rDamage += nTemp; 
			if (nRiseFall == 2)  rDamage -= nTemp;
			break;
		}
    case 2: // °í->Àú¿Í °°Àº ³ôÀÌ¸¸ °ø°ÝÀÌ °¡´ÉÇÑ °æ¿ì
		{
			if (nRiseFall == 1)  rDamage += nTemp; 
			break;
		}
    case 3: // Àú->°í¿Í °°Àº ³ôÀÌ¸¸ °ø°ÝÀÌ °¡´ÉÇÑ °æ¿ì
		{
			if (nRiseFall == 2)  rDamage -= nTemp;
			break;
		}
	}
}

template<class T1, class T2, class T3> 
void TCharList<T1, T2, T3>::CorrectStrikeRange(int nDistance, int& rDamage)
{	// ¹°¸®Àû °ø°ÝÀÇ °Å¸®¿¡ µû¸¥ º¸Á¤
	int nPlus = RareEM.GetStaticRareDistant(StaticRare);
	
	if (nPlus > 0)
	{
		int nMin = 0, nMid = 0, nMax = 0;
		
		switch (GetWeaponKind())
		{
		case IK_HURL:      nMin = 3; nMid = 6;  nMax = 8;  break;
		case IK_SHORT_BOW:
		case IK_DUAL_BOW://030102 lsw Dual BOW
		case IK_TWO_ARROW:
		case IK_DUAL_ARROW://030102 lsw Arrow
		case IK_LONG_BOW:  nMin = 4; nMid = 8; nMax = 12; break;
		default:           return;
		}
		
		if (nDistance < nMin)  nPlus = 0;
		else if (nDistance < nMid)	 nPlus = (nPlus>>1);
		// µ¥¹ÌÁö º¸Á¤
		rDamage = rDamage*(100 + nPlus)/100;
	}
}

template<class T1, class T2, class T3> 
void TCharList<T1, T2, T3>::CorrectMagicRange(int nDistance, int& rDamage)
{	// °ø°Ý ¸¶¹ýÀÇ °Å¸®¿¡ µû¸¥ º¸Á¤
	switch (nDistance/6)
	{
    case 0:  rDamage = rDamage;        break;
    case 1:  rDamage = rDamage*90/100; break;
    default: rDamage = rDamage*85/100; break;
	}
}

template<class T1, class T2, class T3> 
void TCharList<T1, T2, T3>::CorrectCombatRange(int nDistance, int& rDamage)
{	// ÀüÅõ½ºÅ³ÀÇ °Å¸®¿¡ µû¸¥ º¸Á¤
	switch (nDistance/6)
	{
    case 0:  rDamage = rDamage;        break;
    case 1:  rDamage = rDamage*85/100; break;
    default: rDamage = rDamage*80/100; break;
	}
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcAttackSuccessRate(int nStatic, int nDynamic)
{	// ¹°¸®Àû °ø°ÝÀÌ ¼º°øÇÒ È®·ü °è»ê
	if (dwDontMiss > 0)  return 100;
	// ±â´É ¾ÆÀÌÅÛ »ó¼â
	const int nForceValue = RareEM.GetDynamicRareValue(FITEM_STROKE, DynamicRare);
	
	if (nDynamic <= 0 && nForceValue > 0)
	{
		return nForceValue;
	}
	
	const int nItem = item_Hit_Rate + RareEM.GetStaticRareAccurate(StaticRare);
	const int nDex = __min(GetAbility(DEX)/60, 5);
	const int nAttack = nDex + nItem + ((GetActiveCombat()) ? 85:75);
	const int nDefence = __max(nStatic, nDynamic);
	// ¼º°ø·ü °è»ê
	int nResult = nAttack - nDefence;
	if (nResult < 0)    nResult = 0;
	if (nResult > 100)  nResult = 100;
	return nResult;
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcMagicSuccessRate(int nDefence)
{	//< CSD-030306 : ¸¶¹ýÀÌ ¼º°øÇÒ È®·ü °è»ê
	if (dwDontMiss > 0)  return 100;
	
	const int nInt = GetAbility(INT_);
	const int nWis = GetAbility(WIS);
	const int nWsPs = GetAbility(WSPS);
	int nAttack = (nInt + nWis + nWsPs)*0.3 + 80;
	if (nAttack < 0)    nAttack = 0;
	if (nAttack > 100)  nAttack = 100;
	// ¼º°ø·ü °è»ê
	int nResult = nAttack - nDefence;
	if (nResult < 0)    nResult = 0;
	if (nResult > 100)  nResult = 100;
	return nResult;
}	//> CSD-030306

template<class T1, class T2, class T3>
int TCharList<T1, T2, T3>::CalcPhysicalStrikingPower(int nPercent, int nAdd)
{	// ¼ÆËãÎïÀí¹¥»÷Á¦
	if (IsNpc())		
	{	// NPCÀÎ °æ¿ì´Â µ¥ÀÌºí¿¡¼­ Á÷Á¢ ÂüÁ¶
		const int nTemp = GetNpcAttack();
		return (nTemp*(rand()%3 + 9))/10;
	} 
	
	const int nWeapon =	CalcWeaponHit()*GetWeaponSkill(); // ÎäÆ÷¹¥»÷Á¦¼ÆËã
	const int nBase = (GetLevel()<<2) + (nWeapon>>3) + Str/5 + 20 + rand()%20; // CSD-030806 : »ù´¡¹¥»÷Á¦¼ÆËã
	const int nActive = GetActiveCombat();
	const int nPassive = GetPassiveCombat();
	const int nCritial = CalcCriticalHit(nActive, nPassive); // Å©¸®Æ¼Ä® È÷Æ®¿¡ ÀÇÇÑ º¸Á¤
	const int nVigor = RareEM.GetStaticRareVigor(StaticRare); // GÊôÐÔ¼ÆËã
	const int nSum = 100 + nPercent + nCritial + nVigor; // º¸Á¤°ªµéÀÇ ÇÕ(´ÜÀ§ %)
	const int nPlus = RareEM.GetStaticRarePower(StaticRare) +
					  RareEM.GetDynamicRareValue(FITEM_POWER_UP, DynamicRare) +
					  RareEM.GetStaticRareHighPower(StaticRare);
	const int nPower = nAdd + ((GetWeaponSkill()*nPlus)>>3);
	// »ó¼ö¿¡ ÀÇÇÑ º¸Á¤
	return (nBase*nSum/100 + nPower)*1.4;
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcPhysicalDefensivePower(int nAttack, int nPercent)
{	// ¼ÆËãÎïÀí·ÀÓùÁ¦
	if (IsNpc())
	{
		const int nAc = __min(GetNpcDefence(), 100);
		return nAttack*nAc/100;
	}
	
	const int nAc = Ac +
					RareEM.GetStaticRareHardest(StaticRare) +
					RareEM.GetStaticRareHighShield(StaticRare) +
					RareEM.GetDynamicRareValue(FITEM_HEAVY_ARMOR, DynamicRare);
	const int nParrying = (!IsShield()) ? 0:Skill[TACTICS_Parrying]/25;
	const int nBase = __min(nAc, 282); // ÃÖ´ë 235/250 ºñÀ²·Î °è»ê
	const int nSum = 100 + nPercent;               // Ãß°¡ ¹æ¾î·Â(%)
	const int nMax = nAttack*0.95;                 // ÃÖ´ë ¹æ¾î·Â
	return __min((nAttack*nBase/300)*nSum/100, nMax);
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcMagicStrikingPower(int nHD, int nPercent, int nAdd)
{	// Ä§·¨ÉËº¦¼ÆËã
	if (IsNpc())
	{
		return GetNpcMagic() + nHD;
	}
	
	int nTactics = 0, nAbility = 0;
	
	switch (Spell)
	{
    case WIZARD_SPELL:
		{	// Ä§·¨¹¥»÷Á¦¼ÆËã
			nPercent = RareEM.GetStaticRareMagic_power(StaticRare); //°Ù·Ö±ÈÄ§Ôö
			const int nItem = RareEM.GetStaticRareMagic_vigor(StaticRare) + 
							  RareEM.GetDynamicRareValue(FITEM_SPELL_UP, DynamicRare)+
							  RareEM.GetStaticRareHighMagic(StaticRare);
							 // pItem->GetPlusMagic_power;//027030 lsw
			nTactics = (Skill[TACTICS_Magery] + 1)*(nHD + nItem);
			nAbility = GetAbility(INT_);
			
			break;
		}
    case PRIEST_SPELL:
		{	// Ê¥Á¦¹¥»÷Á¦¼ÆËã
			nPercent = RareEM.GetStaticRareNature_power(StaticRare); //°Ù·Ö±ÈÊ¥Ôö
			const int nItem = RareEM.GetStaticRareNature_vigor(StaticRare) + 
						      RareEM.GetDynamicRareValue(FITEM_SPELL_UP, DynamicRare)+
							  RareEM.GetStaticRareHighNature(StaticRare);//027030 lsw
			nTactics = (Skill[TACTICS_Orison] + 1)*(nHD + nItem);
			nAbility = GetAbility(WIS);
			break;
		}
	}
	
	const int nBase = (GetLevel()<<2) + (nTactics>>3) + (nAbility/5) + 30 + rand()%10; // CSD-030806
	const int nSum = 100 + nPercent ;// °Ù·Ö±ÈÔö·ù¼ÆËã
	const int nPlus = nAdd;
	return (nBase*nSum/100 + nPlus)*1.4;
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcMagicDefensivePower(BYTE nType, int nAttack, int nPercent)
{	// ¼ÆËãÄ§¿¹ Ä§·¨·ÀÓùÁ¦
	if (IsNpc())
	{
		const int nAc = __min(GetNpcDefence(nType), 100);
		return nAttack*nAc/100;
	}
	
	int nResist = RareEM.GetDynamicRareValue(FITEM_RESIST_UP, DynamicRare);
	
	switch (nType)
	{
    case RESIST_FIRE:   nResist += GetExtFire();  break;
    case RESIST_ICE:    nResist += GetExtIce();   break;
    case RESIST_ELECT:  nResist += GetExtElect(); break;
    case RESIST_HOLY:   nResist += GetExtHoly();  break;
    case RESIST_IGNORE: nResist = 0;              break;
	}
	
	const int nMax = nAttack*0.95;           // ÃÖ´ë ¹æ¾î·Â
	const int nAc = Ac*5/6;                  // ÃÖÁ¾ AC
	const int nEndu = (GetAbility(ENDU)>>2); // ÃÖÁ¾ ENDU
	// ÃÖ´ë 235/250 ºñÀ²·Î °è»ê
	const int nBase = __min(nResist + nAc + nEndu, 235);
	const int nSum = 100 + nPercent; // Ãß°¡ ¹æ¾î·Â(%)
	return __min((nAttack*nBase/250)*nSum/100, nMax);
}

template<class T1, class T2, class T3>
int TCharList<T1, T2, T3>::CalcMagicCastReady(BYTE nMagic)
{	// ¸¶¹ý ½ÃÀü½Ã°£ °è»ê
	const int nMinus = RareEM.GetStaticRareWise(StaticRare) + 
		RareEM.GetDynamicRareValue(FITEM_SLIGHT, DynamicRare); 
	const int nReady = GetCastReady(nMagic)*(100 - nMinus)/100;
	
	switch (Spell)
	{
    case WIZARD_SPELL: 
		{ // ¸¶¹ý»çÀÎ °æ¿ì
			const int nTime = nReady*(750 - GetAbility(INT_))/650; 
			return __max(nTime, 300);
		}
    case PRIEST_SPELL: 
		{ // ¼ºÁ÷ÀÚÀÎ °æ¿ì
			const int nTime = nReady*(1000 - GetAbility(WIS))/1500 + (nReady>>1);
			return __max(nTime, 600);
		}
	}
	
	return 2000;
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcMagicPeriod(BYTE nType, int nContinue)
{	// ¸¶¹ý À¯Áö½Ã°£ °è»ê
	if (IsNpc())
	{
		return nContinue + rand()%5;
	}
	
	int nItem = 0;
	
	switch (Spell)
	{
    case WIZARD_SPELL: 
		{	// ¸¶¹ý»ç¶ó¸é
			switch (nType/10)
			{
			case MT_PROTECTION: 
				{
					nItem += RareEM.GetStaticRareMagic_stay(StaticRare)/3; 
					nItem += RareEM.GetDynamicRareValue(FITEM_SPELL_TIME_UP, DynamicRare)/3;
					break;
				}
			case MT_ASSISTANCE: 
				{
					nItem += RareEM.GetStaticRareMagic_stay(StaticRare)/2; 
					nItem += RareEM.GetDynamicRareValue(FITEM_SPELL_TIME_UP, DynamicRare)/2;
					break;
				}
			case MT_CURSE: 
				{
					nItem += RareEM.GetStaticRareMagic_stay(StaticRare)/4; 
					nItem += RareEM.GetDynamicRareValue(FITEM_SPELL_TIME_UP, DynamicRare)/4;
					break;
				}
			}
			
			break;
		}
    case PRIEST_SPELL: 
		{	// ¼ºÁ÷ÀÚ¶ó¸é
			switch (nType/10)
			{
			case MT_PROTECTION: 
				{
					nItem += RareEM.GetStaticRareNature_stay(StaticRare)/3; 
					nItem += RareEM.GetDynamicRareValue(FITEM_SPELL_TIME_UP, DynamicRare)/3;
					break;
				}
			case MT_ASSISTANCE: 
				{
					nItem += RareEM.GetStaticRareNature_stay(StaticRare)/2; 
					nItem += RareEM.GetDynamicRareValue(FITEM_SPELL_TIME_UP, DynamicRare)/2;
					break;
				}
			case MT_CURSE: 
				{
					nItem += RareEM.GetStaticRareNature_stay(StaticRare)/4; 
					nItem += RareEM.GetDynamicRareValue(FITEM_SPELL_TIME_UP, DynamicRare)/4;
					break;
				}
			}
			
			break;
		}
	}
	
	const int nInt = GetAbility(INT_);
	const int nWis = GetAbility(WIS);
	const int nWsPs = GetAbility(WSPS);
	const int nBase = nContinue*(nInt + nWis + nWsPs)/1000 + (nContinue>>1);
	const int nSum = 100 + nItem; // º¸Á¤°ªµéÀÇ ÇÕ(´ÜÀ§ %)
	// ·¹¾î ¾ÆÀÌÅÛ¿¡ ÀÇÇÑ ½Ã°£ Áõ°¡
	int nAdd = 0;
	
	switch (nType/10)
	{
    case MT_PROTECTION: 
		{
			nAdd = RareEM.GetStaticRareMasking(StaticRare); 
			break;
		}
    case MT_ASSISTANCE: 
		{
			nAdd = RareEM.GetStaticRareAid(StaticRare);     
			break;
		}
    case MT_CURSE: 
		{
			nAdd = RareEM.GetStaticRareCurse(StaticRare);   
			break;
		}
	}
	
	return nBase*nSum/100 + nAdd;
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcResistPeriod(BYTE nType, int nContinue)
{	// ÀúÇ×·Â¿¡ µû¸¥ À¯Áö½Ã°£ °è»ê
	int nResist = RareEM.GetDynamicRareValue(FITEM_RESIST_UP, DynamicRare);
	
	switch (nType)
	{
    case RESIST_POISON: // µ¶°è¿­ ÀúÁÖ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â 
		{ 
			nResist += GetExtPoison();
			return (__min(nResist, 235)*nContinue)/250;
		}
    case RESIST_CURSE:  // ¸¶ºñ°è¿­ ÀúÁÖ¸¶¹ý¿¡ ´ëÇÑ ÀúÇ×·Â
		{ 
			nResist += GetExtCurse();
			return (__min(nResist, 235)*nContinue)/250;
		}
	}
	
	return 0;
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::CalcMovePower()
{	// ÀÌµ¿·Â °è»ê
	if (dwDontMove > 0)       return 0;  // ÀÌµ¿ºÒ°¡ »óÅÂÀÎ °æ¿ì
	if (nCurrentAction == 1)  return 0;  // ¾É¾Æ ÀÖ´Â °æ¿ì
	// ±Ø¾Ç NKÀÎ °æ¿ì Ã³¸®
	const int nNation = MapInfo[MapNumber].nation;
	if (GetNK(nNation) >= 100)  return 10;
	if (GetNK(nNation) >= 10)   return 25;
	// ÀÌµ¿ ¼Óµµ ±¸ÇÏ±â
	int nMove = GetMovePower(HorseNo), nPlus = 0, nMinus = 0;
	// ¸»À» Å¸°í ÀÖ´ÂÁö ¿©ºÎ
	const bool bHorse = (HorseNo <= 0) ? false:true;
	// ¸»À» Åº °æ¿ì´Â ÀÌµ¿Ä¡¿¡ °üÇÑ ÀüÅõ½ºÅ³, ¸¶¹ý ¹«½Ã
	if (!bHorse)
	{	//< CSD-020710
		switch (GetPassiveCombat())
		{
		case GUST: nPlus = GetCombatValue(GUST); break;
		}
		
		CorrectMovePlus(nMove, nPlus);
		// ·¹¾î¿¡ ´ëÇÑ º¸Á¤
		nMove += (RareEM.GetStaticRareSpeed(StaticRare)>>1);
		nMove += RareEM.GetDynamicRareValue(FITEM_SPEED_UP, DynamicRare);
	}	//> CSD-020710
	
	if (IsFreeze())
	{	//< CSD-021024 : ¾ó¸² »óÅÂÀÎ °æ¿ì¶ó¸é
		nMinus = 50;
	}	//> CSD-021024
	
	CorrectMoveMinus(nMove, nMinus);
	// ¹è°íÇÄ ¼öÄ¡°¡ 0°Å³ª À¯·ÉÀÎ °æ¿ì  ¿òÁ÷ÀÓÀº 1/2·Î ÁÙ¾îµë
	return (IsHungry()) ? (nMove>>1):nMove;
}

template<class T1, class T2, class T3> 
bool TCharList<T1, T2, T3>::IsStrikePiercing() const 
{	//< CSD-021024
	if (IsNpc())
	{
		return (rand()%101 <= GetNpcStrikePiercing()) ? true:false; 
	}
	
	return false;
}	//> CSD-021024

template<class T1, class T2, class T3> 
bool TCharList<T1, T2, T3>::IsMagicPiercing() const
{	//< CSD-021024
	if (IsNpc())
	{
		return (rand()%101 <= GetNpcMagicPiercing()) ? true:false; 
	}
	
	return false;
}	//> CSD-021024

template<class T1, class T2, class T3> 
bool TCharList<T1, T2, T3>::IsCurseAvoid(BYTE nType) const
{	//< CSD-021024
	if (IsNpc())
	{
		switch (nType)
		{
		case 41: return (rand()%101 <= GetNpcStunAvoid()) ? true:false; 
		case 42: return (rand()%101 <= GetNpcSlowAvoid()) ? true:false; 
		case 43: return (rand()%101 <= GetNpcPoisonAvoid()) ? true:false; 
		case 44: return (rand()%101 <= GetNpcBlindAvoid()) ? true:false;
		case 45: return (rand()%101 <= GetNpcDrainAvoid()) ? true:false;
		case 46: return (rand()%101 <= GetNpcFreezeAvoid()) ? true:false;
		}
	}
	
	return false;
}	//> CSD-021024

///////////////////////////////////////////////////////////////////////////////
// Protected Method
///////////////////////////////////////////////////////////////////////////////
template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtPoison() const
{
	return GetBasicResist(RT_POISON) + GetExtPoisonByItem();
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtCurse() const
{
	return GetBasicResist(RT_CURSE) + GetExtCurseByItem();
}

template<class T1, class T2, class T3>
int TCharList<T1, T2, T3>::GetExtHoly() const
{
	return GetBasicResist(RT_HOLY) + GetExtHolyByItem();
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtFire() const
{
	return GetBasicResist(RT_FIRE) + GetExtFireByItem();
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtIce() const
{
	return GetBasicResist(RT_ICE) + GetExtIceByItem();
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtElect() const
{
	return GetBasicResist(RT_ELECT) + GetExtElectByItem();
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtPoisonByItem() const
{
	return GetExtResist(RT_POISON)  + RareEM.GetStaticRareResist_venom(StaticRare);
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtCurseByItem() const//030227 lsw
{
	return GetExtResist(RT_CURSE)+ RareEM.GetStaticRareResist_paralysis(StaticRare);
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtHolyByItem() const
{
	return GetExtResist(RT_HOLY) + RareEM.GetStaticRareResist_holy(StaticRare);
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtFireByItem() const
{
	return GetExtResist(RT_FIRE) + RareEM.GetStaticRareResist_blaze(StaticRare);
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtIceByItem() const
{
	return	 GetExtResist(RT_ICE) + RareEM.GetStaticRareResist_freeze(StaticRare);
}

template<class T1, class T2, class T3> 
int TCharList<T1, T2, T3>::GetExtElectByItem() const
{
	return GetExtResist(RT_ELECT) + RareEM.GetStaticRareResist_lightning(StaticRare);
}

///////////////////////////////////////////////////////////////////////////////
//
typedef TCharList<CCharInfo, CMagicState, CResistState> CHARLIST;
typedef TCharList<CCharInfo, CMagicState, CResistState>* LPCHARLIST;
//
///////////////////////////////////////////////////////////////////////////////
#endif


