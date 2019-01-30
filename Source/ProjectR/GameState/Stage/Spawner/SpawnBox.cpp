
#include "ProjectR.h"
#include "SpawnBox.h"


#include "GlobalIntegrated.h"






#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"






#include "UtilFunctionIntegrated.h"

#include "Table/GrowthTables/GrowthHeroMultiplier.h"

#include "CustomActors/BaseSequenceActor.h"


ASpawnBox::ASpawnBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_UNREAL_DEVELOPER_TOOLS
	UArrowComponent*		ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (IsValid(ArrowComponent))
		ArrowComponent->SetupAttachment(RootComponent);
	UTextRenderComponent*	TextRenderComponent = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	if (IsValid(TextRenderComponent))
	{
		TextRenderComponent->SetupAttachment(RootComponent);
		TextRenderComponent->SetText(FText::FromString(GetName()));
		TextRenderComponent->SetTextRenderColor(FColor::Blue);
	}

	UShapeComponent*		ShapeComponent = GetCollisionComponent();
	ShapeComponent->ShapeColor = FColor::Red;
#endif

}

ASpawnBox::~ASpawnBox()
{

}

