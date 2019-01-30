// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RAI_SkillAICaster.h"
#include "RAI_SkillAICaster_NpcDefault.generated.h"


/** Class needed to support InterfaceCast<IToStringInterface>(Object) */
UCLASS()
class  PROJECTR_API URAI_SkillAICaster_NpcDefault : public URAI_SkillAICaster
{
	GENERATED_BODY()
			
public:
	URAI_SkillAICaster_NpcDefault();

	virtual void		Init(AActor* InOwnerActor, TArray< FName >& InSkillList, float InCheckInterval) override;

	virtual bool		CheckReadyToCastSkill( float DeltaTime ) override;

protected:
	virtual FSkillTableInfo_V2*	GetSkillTableInfo(FName InSkillID);
};
