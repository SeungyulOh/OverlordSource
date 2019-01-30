// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "UtilFunctionIntegrated.h"

#include "GlobalIntegrated.h"

#include "Table/SkillResourceTableInfo.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"
#include "RSkillCutscene.h"

#define APPENDSTRING_ACTIVESKILL	FString(TEXT("_Context_Name"))
#define APPENDSTRING_CONTEXTSKILL	FString(TEXT("_Name"))

void URSkillCutscene::PrepareSkillCutsceneInfo(FName InSkillName, UTexture2D* InSkillPotrait, ESkillCategoryTypeEnum InSkillControlType)
{
	if (IsValid(InSkillPotrait))
	{
		//return;
	}
	
	auto RGameInstance = RGAMEINSTANCE(this);
	ELanguageType CurLanguageType = ELanguageType::VE_MAX;
	
	{
		CurLanguageType = RGameInstance->OptionManager->GetLanguageType();
	}
	
	//UE_LOG(LogBattle, Error, TEXT("SkillCutRow - %s"), *(InSkillName.ToString()));
	FString SkillStringRow = InSkillName.ToString();

	switch (InSkillControlType)
	{
		case ESkillCategoryTypeEnum::VE_ActiveSkill01:
			SkillStringRow.Append(APPENDSTRING_ACTIVESKILL);
		break;
		case ESkillCategoryTypeEnum::VE_ActiveSkill02:
			SkillStringRow.Append(APPENDSTRING_CONTEXTSKILL);
		break;
	}
	//UE_LOG(LogBattle, Error, TEXT("SkillCutRowf - %s"), *(SkillStringRow));

	FText SkillText = UUtilFunctionLibrary::GetLocalizedString( EStringTableType::VE_Skill, FName(*SkillStringRow));
	OnVisible(SkillText, InSkillPotrait);
}