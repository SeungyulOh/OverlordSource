// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "Party.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"

#include "UtilFunctionIntegrated.h"


UParty::UParty()
{
}

void UParty::Initialize()
{
	MemberPlayerUKList.Empty();
}

void UParty::AddMember(int32 PlayerUK)
{
	if (!MemberPlayerUKList.Contains(PlayerUK))
	{
		MemberPlayerUKList.Emplace(PlayerUK);
	}
}

void UParty::RemoveMember(int32 PlayerUK)
{
	if (MemberPlayerUKList.Contains(PlayerUK))
	{
		MemberPlayerUKList.Remove(PlayerUK);
	}
}

bool UParty::IsMyPartyPlayer(int32 PlayerUK)
{
	if (MemberPlayerUKList.Contains(PlayerUK))
		return true;

	return false;
}

bool UParty::IsPartyAllDie()
{
	if (0 < MemberPlayerUKList.Num())
		return false;

	return true;
}

void UParty::GetPartyMember(TArray<TWeakObjectPtr<AActor>>& outPartyMemberList)
{
	outPartyMemberList.Empty();

	AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
	if (pGameMode)
	{
		pGameMode->ObjectHelper.GetObjectList(MemberPlayerUKList, outPartyMemberList);
	}

}

//////////////////////////////////////////////////////////////////////////


void UParty::StopSkillEffect()
{
	TArray<TWeakObjectPtr<AActor>> team_member_list;
	GetPartyMember(team_member_list);

	USkillEntityComponent* skill_comp = nullptr;
	UBattleEntityComponent* battle_comp = nullptr;
	for (auto member : team_member_list)
	{
		skill_comp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(member.Get());
		if (skill_comp)
		{
			skill_comp->EndSkillActionForced_V2();
			skill_comp->ForceClearSkillEffect();
		}

		battle_comp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(member.Get());
		if (battle_comp)
		{
			battle_comp->StopBattleEntityForced();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// TeamAI
void UParty::StartPartyAI(bool auto_active_skill)
{
	TArray<TWeakObjectPtr<AActor>> team_member_list;
	GetPartyMember(team_member_list);

	for (auto& Element : team_member_list)
	{
		if (Element.IsValid())
		{
			UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Element.Get());
			if (IsValid(AIComp))
			{
				AIComp->AIRestart();
			}
		}
	}
}

void UParty::StopPartyAI()
{
	TArray<TWeakObjectPtr<AActor>> team_member_list;
	GetPartyMember(team_member_list);

	for (auto& Element : team_member_list)
	{
		if (Element.IsValid())
		{
			UEntityAIComponent* AIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(Element.Get());
			if (IsValid(AIComp))
			{
				AIComp->AIStop();
			}
		}
	}
}



//////////////////////////////////////////////////////////////////////////
