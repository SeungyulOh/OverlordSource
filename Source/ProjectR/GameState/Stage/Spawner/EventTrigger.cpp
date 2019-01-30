
#include "ProjectR.h"
#include "EventTrigger.h"

#include "GlobalIntegrated.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"



#include "UtilFunctionIntegrated.h"


AEventTrigger::AEventTrigger(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UBoxComponent* boxComponent = Cast<UBoxComponent>(GetCollisionComponent());
	if (IsValid(boxComponent))
	{

		boxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AEventTrigger::OnBeginOverlap);
		boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEventTrigger::OnBeginOverlap);
		boxComponent->OnComponentEndOverlap.RemoveDynamic(this, &AEventTrigger::OnEndOverlap);
		boxComponent->OnComponentEndOverlap.AddDynamic(this, &AEventTrigger::OnEndOverlap);
	}

#if WITH_UNREAL_DEVELOPER_TOOLS 
	UTextRenderComponent*	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	if (IsValid(TextRenderComponent))
	{
		TextRenderComponent->SetupAttachment(RootComponent);
		TextRenderComponent->SetText(FText::FromString(GetName()));
		TextRenderComponent->SetTextRenderColor(FColor::Blue);
	}
	UShapeComponent*	ShapeComponent = GetCollisionComponent();
	ShapeComponent->ShapeColor = FColor::Yellow;
#endif
}

AEventTrigger::~AEventTrigger()
{

}

void AEventTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && IsValid(OtherComp))
	{
		if (OtherActor != this)
		{
			UShapeComponent* ShapeComp = Cast<UShapeComponent>(OtherComp);
			if (!IsValid(ShapeComp))
				return;

			URGameInstance* RGameInstance = RGAMEINSTANCE(this);
			if (IsValid(RGameInstance) /*&& IsValid(RGameInstance->GameObjectMgr)*/)
			{
				IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(OtherActor);
				if (BaseProperty)
				{
// 					if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero && BaseProperty->GetPartyID() == RGameInstance->GameObjectMgr->GetMyPartyID())
					if (BaseProperty->GetCharacterCategory() == ECharacterCategoryTypeEnum::VE_Hero && BaseProperty->GetPartyID() == UObjectFunctionLibrary::GetMyPartyID())
					{
						OnOverlapEvent.Broadcast(ActorID);
						SetActorEnableCollision(false);
					}
				}
			}
		}
	}
}

void AEventTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
