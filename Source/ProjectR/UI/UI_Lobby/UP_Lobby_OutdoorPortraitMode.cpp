
#include "ProjectR.h"
#include "UP_Lobby_OutdoorPortraitMode.h"

#include "UtilFunctionIntegrated.h"

void UUP_Lobby_OutdoorPortraitMode::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Back->OnClicked.AddDynamic(this, &UUP_Lobby_OutdoorPortraitMode::OnButtonBackClicked);
}

void UUP_Lobby_OutdoorPortraitMode::NativeDestruct()
{
	if (IsValid(Button_Back))
		Button_Back->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UUP_Lobby_OutdoorPortraitMode::OnButtonBackClicked()
{
	UUIFunctionLibrary::GoUIPage(EUIPageEnum::UIPage_LobbyOutdoor);
}