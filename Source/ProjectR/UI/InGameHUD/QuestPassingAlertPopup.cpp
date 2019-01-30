
#include "ProjectR.h"
#include "QuestPassingAlertPopup.h"

#include "Global/TableManager.h"
#include "Global/RGameInstance.h"
#include "Global/CampaignManager.h"

#include "Utils/UtilFunctionLibrary.h"

#include "UI/RWidgetManager.h"
void UQuestPassingAlertPopup::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Cancel->OnClicked.RemoveDynamic(this, &UQuestPassingAlertPopup::OnClickCancel);
	Button_Cancel->OnClicked.AddDynamic(this, &UQuestPassingAlertPopup::OnClickCancel);
	Button_Close->OnClicked.RemoveDynamic(this, &UQuestPassingAlertPopup::OnClickCancel);
	Button_Close->OnClicked.AddDynamic(this, &UQuestPassingAlertPopup::OnClickCancel);
	Button_OK->OnClicked.RemoveDynamic(this, &UQuestPassingAlertPopup::OnClickOK);
	Button_OK->OnClicked.AddDynamic(this, &UQuestPassingAlertPopup::OnClickOK);
}

void UQuestPassingAlertPopup::NativeDestruct()
{

	Super::NativeDestruct();
}

void UQuestPassingAlertPopup::OnClickOK()
{
	OnClickCancel();
}

void UQuestPassingAlertPopup::OnClickCancel()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	RGameInstance->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_QuestPassingAlertPopup);
}

void UQuestPassingAlertPopup::SetPopupText(int32 InQuestID)
{
	FFloorQuestTableInfo* pTableData = UTableManager::GetInstancePtr()->GetFloorQuestRow(FName(*FText::AsNumber(InQuestID).ToString()));
	if (pTableData)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		ImportantMissionTargetText->SetRichText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, pTableData->TargetStringKey));
		ImportantCurrentText->SetText(FText::AsNumber(RGameInstance->CampaignManager->TowerStatus.floorQuestStatus.progress));
		ImportantMaxText->SetText(FText::AsNumber(pTableData->ConditionGoal));
	}
}

