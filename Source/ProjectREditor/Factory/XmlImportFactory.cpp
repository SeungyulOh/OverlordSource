// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectREditor.h"
#include "XmlImportFactory.h"

#include "UnrealEd.h"
//#include "ModuleManager.h"
#include "XmlString.h"

#define LOCTEXT_NAMESPACE "XmlImportFactory"

static bool IsXmlFile(const TCHAR* Extension)
{
	return FCString::Stricmp(Extension, TEXT("xml")) == 0;
}

UXmlImportFactory::UXmlImportFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	bCreateNew = false;
	bEditAfterNew = true;
	SupportedClass = UXmlString::StaticClass();

	bEditorImport = true;
	bText = true;

	Formats.Add(TEXT("xml;XML File"));
}

bool UXmlImportFactory::FactoryCanImport(const FString& Filename)
{
	return IsXmlFile(*FPaths::GetExtension(Filename));
}

UObject* UXmlImportFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	UXmlString* NewAsset = NULL;

	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	if (IsXmlFile(Type))
	{
		FString String;
		//const int32 BufferSize = BufferEnd - Buffer;
		//appBufferToString( String, Buffer, BufferSize );
		int32 NumChars = (BufferEnd - Buffer);
		TArray<TCHAR>& StringChars = String.GetCharArray();
		StringChars.AddUninitialized(NumChars + 1);
		FMemory::Memcpy(StringChars.GetData(), Buffer, NumChars*sizeof(TCHAR));
		StringChars.Last() = 0;

		NewAsset = NewObject<UXmlString>(InParent, InName, Flags);
		if (IsValid(NewAsset))
		{
			NewAsset->AssetImportData->Update(CurrentFilename);
			NewAsset->XmlString = String;
		}
	}

	FEditorDelegates::OnAssetPostImport.Broadcast(this, NewAsset);

	return NewAsset;
}

#undef LOCTEXT_NAMESPACE
