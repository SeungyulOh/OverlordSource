#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.generated.h"

class UUC_GenericPopup;

/**
* BP: popup with modal backdrop covering the screen. with popup animation.
*/
UCLASS()
class PROJECTR_API UUC_GenericPopupContent : public URBaseWidget
{
	GENERATED_BODY()
public:

	UPROPERTY()
	UUC_GenericPopup* ParentPopup;
};
