

#include "ProjectR.h"
#include "RGameViewportClient.h"

#include "GlobalIntegrated.h"

#include "UtilFunctionIntegrated.h"


void URGameViewportClient::PostRender(UCanvas* Canvas)
{
	Super::PostRender(Canvas);

	// Fade if requested, you could use the same DrawScreenFade method from any canvas such as the HUD
	if (bFading)
	{
		DrawScreenFade(Canvas);
	}
}

void URGameViewportClient::ClearFade()
{
	bFading = false;
}

void URGameViewportClient::Fade(const float Duration)
{
	if (World)
	{
		bFading = true;
		FadeDuration = (float)(Duration * 0.5f);
		FadeStartTime = World->GetTimeSeconds();
		bToBlack = true;
	}
}

void URGameViewportClient::DrawScreenFade(UCanvas* Canvas)
{
	if (bFading)
	{
		if (World)
		{
			const float Time = World->GetTimeSeconds();
			float Alpha = FMath::Clamp((Time - FadeStartTime) / FadeDuration, 0.f, 1.f);

			// Make sure that we stay black in a fade to black
			if (1.0f == Alpha)
			{
				if (bToBlack == false)
				{
					bFading = false;
					return;
				}
				URGameInstance* RGameInstance = RGAMEINSTANCE(this);
				if( IsValid(RGameInstance->EventManager))
				{
					RGameInstance->EventManager->OnBeforeFadeIn_Delegate.Broadcast();
				}

				bToBlack = !bToBlack;
				FadeStartTime = World->GetTimeSeconds();
				Alpha = FMath::Clamp((Time - FadeStartTime) / FadeDuration, 0.f, 1.f);
			}
// 			else
			{
				FColor OldColor = Canvas->DrawColor;
				FLinearColor FadeColor = FLinearColor::Black;
				FadeColor.A = bToBlack ? Alpha : 1 - Alpha;
				Canvas->DrawColor = FadeColor.ToFColor(true); // TheJamsh: "4.10 cannot convert directly to FColor, so need to use FLinearColor::ToFColor() :)
				Canvas->DrawTile(Canvas->DefaultTexture, 0, 0, Canvas->ClipX, Canvas->ClipY, 0, 0, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY());
				Canvas->DrawColor = OldColor;
			}
		}
	}
}