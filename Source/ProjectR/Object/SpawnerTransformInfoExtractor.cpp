// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SpawnerTransformInfoExtractor.h"

#include "Object/SpawnerTransform.h"
#include "Object/WallActor.h"
#include "Object/TriggerActor.h"

#include "UtilFunctionIntegrated.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"



// Sets default values
ASpawnerTransformInfoExtractor::ASpawnerTransformInfoExtractor()
{ 	
	
}

bool ASpawnerTransformInfoExtractor::SaveFile(FString InFileName, FString InFileContents )
{
	FString FullPath = FPaths::ProjectSavedDir() + InFileName + ".xml";
	if (FFileHelper::SaveStringToFile(InFileContents, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8 ))
	{
		UE_LOG(LogBattle, Log, TEXT("ASpawnerTransformInfoExtractor::SaveFile %s Save"), *FullPath);
		return true;
	}

	return false;
}

bool ASpawnerTransformInfoExtractor::Save()
{
	FString InFileName = UGameplayStatics::GetCurrentLevelName(this);
	FString OutputString = TEXT("<TransformInfo>\n");

	AddSpawnerTransform(OutputString);
	AddWall(OutputString);
	AddTrigger(OutputString);

	OutputString += TEXT("</TransformInfo>\n");

	FString FullPath = FPaths::ProjectSavedDir() + InFileName + ".xml";
	if (FFileHelper::SaveStringToFile(OutputString, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8))
	{
		UE_LOG(LogBattle, Log, TEXT("ASpawnerTransformInfoExtractor::SaveFile %s Save"), *FullPath);

		FString strText;
		strText = FString::Printf(TEXT("%s Save"), *FullPath);
		UKismetSystemLibrary::PrintString(this, strText);

		return true;
	}

	return false;
}

void ASpawnerTransformInfoExtractor::AddSpawnerTransform(FString& output)
{
	for (TActorIterator<ASpawnerTransform> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ASpawnerTransform* SpawnerActor = *ActorItr;
		if (IsValid(SpawnerActor))
		{
			output += FString::Printf(TEXT("\t<Transform ID = \"%d\" Pos = \"%f,%f,%f\" Rot = \"%f,%f,%f\" />\n"),
				SpawnerActor->SpawnerTransformID,
				SpawnerActor->GetActorLocation().X, SpawnerActor->GetActorLocation().Y, SpawnerActor->GetActorLocation().Z,
				SpawnerActor->GetActorRotation().Roll, SpawnerActor->GetActorRotation().Pitch, SpawnerActor->GetActorRotation().Yaw);
		}
	}
}

void ASpawnerTransformInfoExtractor::AddWall(FString& output)
{
	for (TActorIterator<AWallActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AWallActor* SpawnerActor = *ActorItr;
		if (IsValid(SpawnerActor))
		{
			FName fileName = UUtilFunctionLibrary::GetActorFileName(SpawnerActor);
			if (!fileName.IsNone())
			{
				output += FString::Printf(TEXT("\t<Wall ID = \"%d\" Color = \"%d\" Path = \"%s\" Pos = \"%f,%f,%f\" Rot = \"%f,%f,%f\" Scale = \"%f,%f,%f\" />\n"),
					SpawnerActor->ActorID,
					SpawnerActor->ColorTypeID,
					*fileName.ToString(),
					SpawnerActor->GetActorLocation().X, SpawnerActor->GetActorLocation().Y, SpawnerActor->GetActorLocation().Z,
					SpawnerActor->GetActorRotation().Roll, SpawnerActor->GetActorRotation().Pitch, SpawnerActor->GetActorRotation().Yaw,
					SpawnerActor->GetActorScale().X, SpawnerActor->GetActorScale().Y, SpawnerActor->GetActorScale().Z);
			}
		}
	}
}

void ASpawnerTransformInfoExtractor::AddTrigger(FString& output)
{
	for (TActorIterator<ATriggerActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ATriggerActor* SpawnerActor = *ActorItr;
		if (IsValid(SpawnerActor))
		{
			UBoxComponent* BoxComponent = Cast<UBoxComponent>(SpawnerActor->GetCollisionComponent());
			if (IsValid(BoxComponent))
			{
				FName fileName = UUtilFunctionLibrary::GetActorFileName(SpawnerActor);
				if (!fileName.IsNone())
				{
					output += FString::Printf(TEXT("\t<Trigger ID = \"%d\" Path = \"%s\" Pos = \"%f,%f,%f\" Rot = \"%f,%f,%f\" BoxExtent = \"%f,%f,%f\" />\n"),
						SpawnerActor->ActorID,
						*fileName.ToString(),
						SpawnerActor->GetActorLocation().X, SpawnerActor->GetActorLocation().Y, SpawnerActor->GetActorLocation().Z,
						SpawnerActor->GetActorRotation().Roll, SpawnerActor->GetActorRotation().Pitch, SpawnerActor->GetActorRotation().Yaw,
						BoxComponent->GetUnscaledBoxExtent().X, BoxComponent->GetUnscaledBoxExtent().Y, BoxComponent->GetUnscaledBoxExtent().Z);
				}
			}
		}
	}
}
