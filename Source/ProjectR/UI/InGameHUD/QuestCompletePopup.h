#pragma once

#include "UI/RBaseWidget.h"

#include "QuestCompletePopup.generated.h"


UCLASS()
class PROJECTR_API UQuestCompletePopup : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
};