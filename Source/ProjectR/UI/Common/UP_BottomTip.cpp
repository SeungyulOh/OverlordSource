

#include "ProjectR.h"
#include "UP_BottomTip.h"

#include "Utils/UtilFunctionLibrary.h"

void UUP_BottomTip::NativeConstruct()
{
	Super::NativeConstruct();

	if (TipLocalStringKey != NAME_None)
	{
		TipText->SetText(UUtilFunctionLibrary::GetLocalizedString(TipStringTableType, TipLocalStringKey));
	}
}
