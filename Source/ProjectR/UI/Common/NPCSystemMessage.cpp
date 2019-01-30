

#include "ProjectR.h"
#include "NPCSystemMessage.h"


void UNPCSystemMessage::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(MessageButton))
	{
		MessageButton->OnClicked.RemoveDynamic(this, &UNPCSystemMessage::OnClickMessageButton);
		MessageButton->OnClicked.AddDynamic(this, &UNPCSystemMessage::OnClickMessageButton);
	}

	ShowTextOverlay(false);
}

void UNPCSystemMessage::NativeDestruct()
{
	if (IsValid(MessageButton))
		MessageButton->OnClicked.RemoveDynamic(this, &UNPCSystemMessage::OnClickMessageButton);


	Super::NativeDestruct();
}

void UNPCSystemMessage::SetNPCMessageText(FText InText)
{
	if (IsValid(MessageButton))
		MessageButton->SetVisibility(ESlateVisibility::Visible);

	if (IsValid(NPCMessageText))
		NPCMessageText->SetText(InText);

	ShowTextOverlay(true);
}

void UNPCSystemMessage::OnClickMessageButton()
{
	ShowTextOverlay(false);

	//go contents ?
}

void UNPCSystemMessage::ShowTextOverlay(bool bShow)
{
	if (IsValid(MessageButton))
		MessageButton->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (IsValid(MessageOverlay))
		MessageOverlay->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
