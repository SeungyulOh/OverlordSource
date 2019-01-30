

// Entity base property interface
//=========================================================================================
// Implement IEntityBaseProperty
//=========================================================================================

EEntityTypeEnum ImplClass::GetEntityType()
{
	return EntityBasePropertyInfo.EntityType; 
}

void ImplClass::SetEntityType(EEntityTypeEnum InEntityType)
{
	EntityBasePropertyInfo.EntityType = InEntityType;
}

int32 ImplClass::GetLevel()
{
	return EntityBasePropertyInfo.Level;
}

void ImplClass::SetLevel(int32 InLevel)
{
	EntityBasePropertyInfo.Level = InLevel;
}

int32 ImplClass::GetGrade()
{
	return EntityBasePropertyInfo.Grade;
}

void ImplClass::SetGrade(int32 InGrade)
{
	EntityBasePropertyInfo.Grade = InGrade;
}

EClassTypeEnum ImplClass::GetJobClass()
{
	return EntityBasePropertyInfo.JobClassType;
}

void ImplClass::SetJobClass(EClassTypeEnum InJobClass)
{
	EntityBasePropertyInfo.JobClassType = InJobClass;
}

bool ImplClass::IsReady()
{
	return EntityBasePropertyInfo.bIsReady;
}

void ImplClass::SetReady(bool InReady)
{
	if (InReady)
	{
		if (EntityBasePropertyInfo.bIsReady != InReady)
		{
			EntityBasePropertyInfo.bIsReady = InReady;

			auto EntityAIComponentInstance = GetEntityAIComponent();
			if (EntityAIComponentInstance 
				&& EntityAIComponentInstance->GetAIControlMode() != EAIControlModeEnum::VE_ManualMode)
			{
				EntityAIComponentInstance->AIRestart();
			}
		}

		if (IsValid(SkillEntityComponent))
		{
			SkillEntityComponent->ActivateSkill(true);
		}
	}
	else
	{
		EntityBasePropertyInfo.bIsReady = InReady;

		auto EntityAIComponentInstance = GetEntityAIComponent();
		if (EntityAIComponentInstance)
		{
			EntityAIComponentInstance->AIStop();
		}
	}
}


FName ImplClass::GetCharacterID()
{
	return EntityBasePropertyInfo.CharacterID;
}

void ImplClass::SetCharacterID(FName InCharacterID)
{
	EntityBasePropertyInfo.CharacterID = InCharacterID;
}

ECharacterCategoryTypeEnum	ImplClass::GetCharacterCategory()
{
	return EntityBasePropertyInfo.CharacterCategory;
}

void ImplClass::SetCharacterCategory(ECharacterCategoryTypeEnum InCharacterCategory)
{
	EntityBasePropertyInfo.CharacterCategory = InCharacterCategory;
}

int32 ImplClass::GetUserKID()
{
	return EntityBasePropertyInfo.UserKID;
}

void ImplClass::SetUserKID(int32 InUserKID)
{
	EntityBasePropertyInfo.UserKID = InUserKID;
}

int32 ImplClass::GetPlayerUK()
{
	return EntityBasePropertyInfo.PlayerUK;
}

void ImplClass::SetPlayerUK(int32 InPlayerUK)
{
	EntityBasePropertyInfo.PlayerUK = InPlayerUK;
}

int32 ImplClass::GetPartyID()
{
	return EntityBasePropertyInfo.PartyID;
}

void ImplClass::SetPartyID(int32 InPartyID)
{
	EntityBasePropertyInfo.PartyID = InPartyID;
}

int32 ImplClass::GetGroupKey()
{
	return EntityBasePropertyInfo.GroupKey;
}

void ImplClass::SetGroupKey(int32 InGroupKey)
{
	EntityBasePropertyInfo.GroupKey = InGroupKey;
}

FString	ImplClass::GetHeroUD()
{
	return EntityBasePropertyInfo.HeroUD;
}

void ImplClass::SetHeroUD(FString InHeroUD)
{
	EntityBasePropertyInfo.HeroUD = InHeroUD;
}

ECharacterPropertyTypeEnum	ImplClass::GetAttachPropertyType()
{
	return EntityBasePropertyInfo.AttackPropertyType;
}

void ImplClass::SetAttachPropertyType(ECharacterPropertyTypeEnum InPropertyType)
{
	EntityBasePropertyInfo.AttackPropertyType = InPropertyType;
}

bool ImplClass::GetAccountCharacter()
{
	return EntityBasePropertyInfo.AccountCharacter;
}

void ImplClass::SetAccountCharacter(bool InFlag)
{
	EntityBasePropertyInfo.AccountCharacter = InFlag;
}

int32 ImplClass::GetRealTimeParam()
{
	return EntityBasePropertyInfo.RealTimeParam;
}

void ImplClass::SetRealTimeParam(int32 InParam)
{
	if (EntityBasePropertyInfo.RealTimeParam < InParam)
	{
		EntityBasePropertyInfo.RealTimeParam = InParam;

#if LOG_NET_REALTIME
		UE_LOG(LogRTS, Log, TEXT("[EntityBaseProperty] SetRealTimeParam >> Entity %d Medals %d"), EntityBasePropertyInfo.PlayerUK, EntityBasePropertyInfo.RealTimeParam);
#endif

		auto BattleEntityComp = GetBattleEntityComponent();
		if (BattleEntityComp)
		{
			BattleEntityComp->OnRefreshRealTimeParam.Broadcast(InParam);
		}
	}
}

void ImplClass::ReSetRealTimeParam()
{
	EntityBasePropertyInfo.RealTimeParam = 0;

	auto BattleEntityComp = GetBattleEntityComponent();
	if (BattleEntityComp)
	{
		BattleEntityComp->OnRefreshRealTimeParam.Broadcast(0);
	}
}

FEntityBasePropertyInfo* ImplClass::GetEntityBasePropertyInfo()
{
	return &EntityBasePropertyInfo;
}