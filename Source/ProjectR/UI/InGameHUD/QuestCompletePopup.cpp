
#include "ProjectR.h"
#include "QuestCompletePopup.h"



void UQuestCompletePopup::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAni(TEXT("CompleteAni"));
}

void UQuestCompletePopup::NativeDestruct()
{

	Super::NativeDestruct();
}

