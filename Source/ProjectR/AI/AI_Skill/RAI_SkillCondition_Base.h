#pragma once
#include "Table/SkillConditionTableInfo.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "RAI_SkillCondition_Base.generated.h"


UCLASS()
class  PROJECTR_API URAI_SkillCondition_Base : public UObject
{
	GENERATED_BODY()

public:
	URAI_SkillCondition_Base();

	void		Init(AActor* InOwnerActor, float InCheckInterval = 1.0f );
	void		AddSkillConditionID(FName InConditionID);

	bool		CheckConditions( EPriorityTypeEnum InAISearchPreferType );
	void		ClearConditionList();

	AActor*		GetCheckTargetActor();

protected:

	bool		CheckCondition(FSkillConditionTableInfo* InSkillConditionInfo, EPriorityTypeEnum InAISearchPreferType);

	bool		CheckConditonMEType(FSkillConditionTableInfo* InSkillConditionInfo);
	bool		CheckConditonTargetType(FSkillConditionTableInfo* InSkillConditionInfo, EPriorityTypeEnum InAISearchPreferType);

	AActor*		GetAISearchTarget(EPriorityTypeEnum InAISearchPreferType, EFriendshipTypeEnum FriendshipType = EFriendshipTypeEnum::VE_None);

	bool		CheckTargetState(FSkillConditionTableInfo* InSkillConditionInfo, AActor* CheckTarget);

protected:


	TArray< FSkillConditionTableInfo* >		SkillContionTableInfoList;	
	
	TWeakObjectPtr< AActor >					CheckTargetActor;

	/*
	Cache
	*/
	TWeakObjectPtr< AActor >					OwnerActor;
	TWeakObjectPtr< UBattleEntityComponent >	OwnerBattleEntityComponent;
	TWeakObjectPtr< USkillEntityComponent >		OwnerSkillEntityComponent;
};

