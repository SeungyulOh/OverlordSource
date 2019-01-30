// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"

#include "LeaderControlHelper.h"
#include "UtilFunctionIntegrated.h"

#include "RCancelIcon.h"
#include "RSkillSet_New.h"

void URCancelIcon::NativeConstruct()
{
	Super::NativeConstruct();

	if(CancelButton)
	{
		CancelButton->OnHovered.RemoveDynamic(this, &URCancelIcon::BtnEventOnHovered);
		CancelButton->OnHovered.AddDynamic(this, &URCancelIcon::BtnEventOnHovered);
		CancelButton->OnUnhovered.RemoveDynamic(this, &URCancelIcon::BtnEventOnUnhovered);
		CancelButton->OnUnhovered.AddDynamic(this, &URCancelIcon::BtnEventOnUnhovered);
	}
}

void URCancelIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void URCancelIcon::BeginDestroy()
{
	Super::BeginDestroy();
}

bool URCancelIcon::IsReadyForFinishDestroy()
{
	bool Result = Super::IsReadyForFinishDestroy();

	return Result;
}

void URCancelIcon::BtnEventOnHovered()
{
	PlayAni("Click_Pressed");

	URSkillSet_New* skillset = Cast<URSkillSet_New>(GetParentWidget());
	if (skillset->IsValidLowLevel())
		skillset->Blackboard.bFingerOnCancelButton = true;
}

void URCancelIcon::BtnEventOnUnhovered()
{
	PlayAni("Click_Pressed", true);
	URSkillSet_New* skillset = Cast<URSkillSet_New>(GetParentWidget());
	if (skillset->IsValidLowLevel())
		skillset->Blackboard.bFingerOnCancelButton = false;
}

