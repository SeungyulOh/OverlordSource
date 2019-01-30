// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectREditor.h"
#include "JSONImportFactory.h"

#include "UnrealEd.h"
//#include "ModuleManager.h"
#include "JSONString.h"

#define LOCTEXT_NAMESPACE "JSONImportFactory"

static bool IsJSONFile(const TCHAR* Extension)
{
	return FCString::Stricmp(Extension, TEXT("json")) == 0;
}

UJSONImportFactory::UJSONImportFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	bCreateNew = false;
	bEditAfterNew = true;
	SupportedClass = UJSONString::StaticClass();

	bEditorImport = true;
	bText = true;

	Formats.Add(TEXT("json;JSON File"));
}

bool UJSONImportFactory::FactoryCanImport(const FString& Filename)
{
	return IsJSONFile(*FPaths::GetExtension(Filename));
}

UObject* UJSONImportFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	UJSONString* NewAsset = NULL;

	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	if (IsJSONFile(Type))
	{
		FString String;
		//const int32 BufferSize = BufferEnd - Buffer;
		//appBufferToString( String, Buffer, BufferSize );
		int32 NumChars = (BufferEnd - Buffer);
		TArray<TCHAR>& StringChars = String.GetCharArray();
		StringChars.AddUninitialized(NumChars + 1);
		FMemory::Memcpy(StringChars.GetData(), Buffer, NumChars*sizeof(TCHAR));
		StringChars.Last() = 0;

		NewAsset = NewObject<UJSONString>(InParent, InName, Flags);
		if (IsValid(NewAsset))
		{
			NewAsset->AssetImportData->Update(CurrentFilename);
			NewAsset->JsonString = String;
		}
	}

	FEditorDelegates::OnAssetPostImport.Broadcast(this, NewAsset);

	return NewAsset;
}

#undef LOCTEXT_NAMESPACE
