// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GameOptionManager.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"




#include "UtilFunctionIntegrated.h"
#include "UI/RWidgetManager.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "Scalability.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Engine/Scene.h"



#if !UE_BUILD_SHIPPING
static TWeakObjectPtr< URGameInstance >	MemoryReportInstance = nullptr;
void CustomMemoryWarningHandler(const FGenericMemoryWarningContext& GenericContext);
#endif


void UGameOptionManager::Initialize(void)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	//SetGameInstance(GameInstance);

	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	SetQualityPreset(RGameUserSetting->QualityPreset);

	ApplyGameOption();

#if !UE_BUILD_SHIPPING	
	SetMemoryCustomWarningHandler();
	MemoryReportInstance = RGAMEINSTANCE(this);
#endif

}

void UGameOptionManager::SetUltimateMatinee(bool IsVisible)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->IsUltimateMatinee = IsVisible;
}

bool UGameOptionManager::IsUltimateMatineeView()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->IsUltimateMatinee;

	return true;
}

void UGameOptionManager::SetStoryMatinee(bool IsAllVisible)
{	
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->IsStoryMatinee = IsAllVisible;
}

bool UGameOptionManager::IsStoryMatineeView()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->IsStoryMatinee;
	return true;
}

void UGameOptionManager::SetModulatedShadow(bool IsOn)
{	
	/*URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->IsModulatedShadow = IsOn;

	ApplyGameOption();

	RefreshOption();*/
}

bool UGameOptionManager::IsModulatedShadowView()
{
	/*URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->IsModulatedShadow;*/

	return true;
}

//by danzang
void UGameOptionManager::ShowParticle(bool show, UWorld* InWorld )
{
	URenderFunctionLibrary::SetEngineFlags(FString("Particles"), show );

	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->ShowEffect = show;
}

void UGameOptionManager::MuteSound(bool InMuteSound)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->MuteSound = InMuteSound;
}

void UGameOptionManager::SetSoundVolume(float InVolume)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->SoundVolume = InVolume;	
}


void UGameOptionManager::SetEffectSoundVolume(float InVolume)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->EffectSoundVolume = InVolume;
}


bool UGameOptionManager::IsMuteSound()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->MuteSound;
	return false;
}

float UGameOptionManager::GetSoundVolume()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->SoundVolume;
	return 1.0f;
}

float UGameOptionManager::GetEffectSoundVolume()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->EffectSoundVolume;
	return 1.0f;
}

bool UGameOptionManager::IsShowParticle()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->ShowEffect;
	return true;
}

EVideoQualityEnum UGameOptionManager::GetQualityType()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	//if (RGameUserSetting)
	//{
		return RGameUserSetting->QualityPreset;
	//}
}

bool UGameOptionManager::DoParticlesShow(UWorld* InWorld)
{
	return URenderFunctionLibrary::GetEngineFlags(FString("Particles"));
}

bool UGameOptionManager::IsSelectImmediately()
{
	/*URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->IsSelectImmediately;*/
	return true;
}

void UGameOptionManager::SetSelectImmediately(bool InSelectImmediately)
{
	/*URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->IsSelectImmediately = InSelectImmediately;

	ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
	if (BasePlayerController)
	{
		BasePlayerController->IsSelectImmediately = InSelectImmediately;
	}*/
}

void UGameOptionManager::ApplyGameOption()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();

	if (RGameUserSetting)
	{
		//APlayerController* PController = UUtilFunctionLibrary::GetPlayerController(RGameInstance->GetWorld());
		//if (PController)
		//{
			//if (RGameUserSetting->IsModulatedShadow)
			//{
			//	PController->ConsoleCommand(TEXT("r.ShadowQuality 2"), true);
			//}
			//else
			//{
			//	PController->ConsoleCommand(TEXT("r.ShadowQuality 0"), true);
			//}
		//}
		
		//RGameUserSetting->SetOverallScalabilityLevel((int32)(RGameUserSetting->QualityPreset));
		//ppp
		RGameUserSetting->FirstExecute = false;
		//RGameUserSetting->FirstExecute = true;

		ApplySetting();
	}
}


//////////////////////////////////////////////////////////////////////////
//Option
//////////////////////////////////////////////////////////////////////////
void UGameOptionManager::SetLanguageType(ELanguageType InLanguageType)
{	
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->LaguageType	= InLanguageType;

	ULocalStringTableManager::GetInstancePtr()->LoadTable(InLanguageType);

#if WITH_EDITOR
	//FTextLocalizationManager::Get().ConfigureGameLocalizationPreviewLanguage(CultureName);
#endif
}

void UGameOptionManager::ApplySetting()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->ApplySettings(true);
}

void UGameOptionManager::RefreshOption()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		RGameUserSetting->ApplySettings(true);
	if (IsValid(RGAMEINSTANCE(this)->RWidgetManager))
	{
		RGAMEINSTANCE(this)->RWidgetManager->RefreshVisibleUI();
	}
}

ELanguageType UGameOptionManager::GetLanguageType()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->LaguageType;
	UE_LOG(LogBattle, Log, TEXT("Game Setting is invalid"));
	return ELanguageType::VE_KOR;
}

void UGameOptionManager::ApplyOptionsAfterLoadLevel( UWorld* InWorld )
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		bool bUsePostProcessing = true;
		// Scalability
		switch (RGameUserSetting->QualityPreset)
		{
			case EVideoQualityEnum::VE_VERYHIGH:
				break;
			case EVideoQualityEnum::VE_HIGH:
			case EVideoQualityEnum::VE_MID:
			case EVideoQualityEnum::VE_LOW:
				break;
		}

		EnablePostprocess(InWorld, bUsePostProcessing);
	}
}

//////////////////////////////////////////////////////////////////////////
//UI String Manager
//////////////////////////////////////////////////////////////////////////


void UGameOptionManager::SetQualityPreset(EVideoQualityEnum InQualityType)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		RGameUserSetting->QualityPreset = InQualityType;

		bool bUsePostProcessing = true;
		// Scalability
		switch (InQualityType)
		{
			case EVideoQualityEnum::VE_VERYHIGH:			
				RGameUserSetting->SetShadowQuality(3);
				RGameUserSetting->SetPostProcessingQuality(1);
				RGameUserSetting->SetTextureQuality(3);
				RGameUserSetting->SetVisualEffectQuality(3);
				RGameUserSetting->SetResolutionScaleValueEx(100.0f);
				RGameUserSetting->SetAntiAliasingQuality(0);
				RGameUserSetting->SetViewDistanceQuality(3);
				RGameUserSetting->SetFoliageQuality(0);
				break;
				// TODO : test postprocess performance
			case EVideoQualityEnum::VE_HIGH:
				RGameUserSetting->SetShadowQuality(2);
				RGameUserSetting->SetPostProcessingQuality(1);
				RGameUserSetting->SetTextureQuality(3);
				RGameUserSetting->SetVisualEffectQuality(2);
				RGameUserSetting->SetResolutionScaleValueEx(100.0f);
				RGameUserSetting->SetAntiAliasingQuality(0);
				RGameUserSetting->SetViewDistanceQuality(3);
				RGameUserSetting->SetFoliageQuality(0);
				break;
			case EVideoQualityEnum::VE_MID:
				RGameUserSetting->SetShadowQuality(1);
				RGameUserSetting->SetPostProcessingQuality(1);
				RGameUserSetting->SetTextureQuality(2);
				RGameUserSetting->SetVisualEffectQuality(1);
				RGameUserSetting->SetResolutionScaleValueEx(100.0f);
				RGameUserSetting->SetAntiAliasingQuality(0);
				RGameUserSetting->SetViewDistanceQuality(3);
				RGameUserSetting->SetFoliageQuality(0);
				break;
			case EVideoQualityEnum::VE_LOW:
				RGameUserSetting->SetShadowQuality(0);
				RGameUserSetting->SetPostProcessingQuality(0);
				RGameUserSetting->SetTextureQuality(1);
				RGameUserSetting->SetVisualEffectQuality(0);
				RGameUserSetting->SetResolutionScaleValueEx(100.0f);
				RGameUserSetting->SetAntiAliasingQuality(0);
				RGameUserSetting->SetViewDistanceQuality(3);
				RGameUserSetting->SetFoliageQuality(0);
				break;
		}
				
		EnablePostprocess(RGAMEINSTANCE(this)->GetWorld(), bUsePostProcessing );

		// Shadow
// 		if (InQualityType == EVideoQualityEnum::VE_LOW)
// 			EnableDynamicShadow(false);
// 		else
		EnableDynamicShadow(true);

		// Particle
// 		switch (InQualityType)
// 		{
// 			case EVideoQualityEnum::VE_VERYHIGH:
// 				ShowParticle(true, RGAMEINSTANCE(this)->GetWorld() );
// 				UParticleSystemComponent::EnableParticleCulling(false);
// 				UParticleSystemComponent::SetParticleSpawnMaxDistance(2000.0f);
// 				UParticleSystemComponent::SetParticleSpawnMaxNum(100);
// 				break;
// 			case EVideoQualityEnum::VE_HIGH:
// 				ShowParticle(true, RGAMEINSTANCE(this)->GetWorld());
// 				UParticleSystemComponent::EnableParticleCulling(true);
// 				UParticleSystemComponent::SetParticleSpawnMaxDistance(1500.0f);
// 				UParticleSystemComponent::SetParticleSpawnMaxNum(12);
// 				break;
// 			case EVideoQualityEnum::VE_MID:
// 				ShowParticle(true, RGAMEINSTANCE(this)->GetWorld());
// 				UParticleSystemComponent::EnableParticleCulling(true);
// 				UParticleSystemComponent::SetParticleSpawnMaxDistance(3500.0f);
// 				UParticleSystemComponent::SetParticleSpawnMaxNum(8);
// 				break;
// 			case EVideoQualityEnum::VE_LOW:
// 				ShowParticle(false, RGAMEINSTANCE(this)->GetWorld());
// 				break;
// 		}
	}
}

void UGameOptionManager::EnableDynamicShadow(const bool enable)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;

	UWorld * world = RGAMEINSTANCE(this)->GetWorld();
	for (TObjectIterator<UDirectionalLightComponent> It; It; ++It)
	{
		UDirectionalLightComponent * directionallight = *It;
		if (directionallight->GetScene() == world->Scene && directionallight->GetOwner() != nullptr)
		{
			if (enable == true)
			{
				directionallight->CastDynamicShadows = 1;
				directionallight->MarkRenderStateDirty();
			}
			else
			{
				directionallight->CastDynamicShadows = 0;
				directionallight->MarkRenderStateDirty();
			}
		}
	}
}

void UGameOptionManager::SetBloomIntensity( UWorld* InWorld, float InIntensity )
{
	if (IsValid(InWorld) == false)
		return;

// 	for (auto& postProcessVolume : InWorld->PostProcessVolumes)
// 	{
// 		if (postProcessVolume && postProcessVolume->GetProperties().Settings )
// 			postProcessVolume->GetProperties().Settings->BloomIntensity = InIntensity;
// 	}
}

float UGameOptionManager::GetTooltipDelayTime()
{
	/*URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		return RGameUserSetting->Tooltip_DelayTime;
	}*/

	return 0.0f;
}

void UGameOptionManager::SetPlayIntroMovie(bool InbPlay)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		RGameUserSetting->bPlayIntroMovie = InbPlay;
	}
	ApplyGameOption();
}

bool UGameOptionManager::IsPlayIntroMovie()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		return RGameUserSetting->bPlayIntroMovie;
	}
	return true;
}

void UGameOptionManager::EnablePostprocess( UWorld* InWorld, bool InEnable)
{
	if (IsValid(InWorld) == false)
		return;

	for (auto& postProcessVolume : InWorld->PostProcessVolumes)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
		if (PPVolume)
		{
			for (FWeightedBlendable& WeightedBlend : PPVolume->Settings.WeightedBlendables.Array)
			{
				WeightedBlend.Weight = (InEnable) ? 1.0f : 0.0f;
			}
			//PPVolume->bEnabled = InEnable;			
		}
	}
}

void UGameOptionManager::SetUserNick(FString InNick)
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
	{
		if (RGameUserSetting->UserNick.Compare(InNick) == 0)
			return;

		RGameUserSetting->UserNick = InNick;
		RGameUserSetting->ApplySettings(true);
	}
}

FString UGameOptionManager::GetUserNick()
{
	URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
	if (RGameUserSetting)
		return RGameUserSetting->UserNick;

	return FString();
}


#if !UE_BUILD_SHIPPING
void UGameOptionManager::SetMemoryCustomWarningHandler()
{
	// Set the memory warning handler
	FPlatformMisc::SetMemoryWarningHandler(CustomMemoryWarningHandler);
}


void UGameOptionManager::DumpMemreport()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	APlayerController* PController = UUtilFunctionLibrary::GetPlayerController(RGAMEINSTANCE(this)->GetWorld());
	if (PController)
	{
		PController->ConsoleCommand(TEXT("memreport -full"), true);
	}
}


#include "RHI.h"

void CustomMemoryWarningHandler(const FGenericMemoryWarningContext& GenericContext)
{
	FPlatformMemoryStats Stats = FPlatformMemory::GetStats();

	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("EngineMemoryWarningHandler: Mem Used %.2f MB, Texture Memory %.2f MB, Render Target memory %.2f MB, OS Free %.2f MB\n"),
		Stats.UsedPhysical / 1048576.0f,
		GCurrentTextureMemorySize / 1048576.0f,
		GCurrentRendertargetMemorySize / 1048576.0f,
		Stats.AvailablePhysical / 1048576.0f);

	UE_LOG(LogBattle, Log, TEXT("EngineMemoryWarningHandler: Mem Used %.2f MB, Texture Memory %.2f MB, Render Target memory %.2f MB, OS Free %.2f MB\n"),
		Stats.UsedPhysical / 1048576.0f,
		GCurrentTextureMemorySize / 1048576.0f,
		GCurrentRendertargetMemorySize / 1048576.0f,
		Stats.AvailablePhysical / 1048576.0f);


	if (MemoryReportInstance.IsValid())
	{
		APlayerController* PController = UUtilFunctionLibrary::GetPlayerController(MemoryReportInstance->GetWorld());
		if (PController)
		{
			PController->ConsoleCommand(TEXT("memreport -full"), true);
		}
#if ENABLE_FORCE_GC
		if(MemoryReportInstance->GetWorld() )
			MemoryReportInstance->GetWorld()->ForceGarbageCollection(true);
#endif
	}
	

#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	static const auto OOMMemReportVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("Debug.OOMMemReport"));
	const int32 OOMMemReport = OOMMemReportVar ? OOMMemReportVar->GetValueOnAnyThread() : false;
	if (OOMMemReport)
	{
		GEngine->Exec(NULL, TEXT("OBJ LIST"));
		GEngine->Exec(NULL, TEXT("MEM FROMREPORT"));
	}
#endif

	//GLastMemoryWarningTime = FPlatformTime::Seconds();
}

#endif