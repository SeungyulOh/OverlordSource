#pragma once

#include "UnrealEd.h"
#include "XmlString.h"
#include "AssetTypeActions_Base.h"

class PROJECTREDITOR_API FAssetTypeActions_XmlString : public FAssetTypeActions_Base
{
public:
	virtual FColor GetTypeColor() const override { return FColor(62, 140, 35); }
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
	virtual bool IsImportedAsset() const override { return true; }

	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_XmlString", "Xml String"); }
	virtual UClass* GetSupportedClass() const override { return UXmlString::StaticClass(); }
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override;
	// End IAssetTypeActions

private:
	/** Can we execute a reimport for the selected objects? */
	bool CanExecuteReimport(const TArray<TWeakObjectPtr<UXmlString>> Objects) const;

	/** Handler for when Reimport is selected */
	void ExecuteReimport(const TArray<TWeakObjectPtr<UXmlString>> Objects) const;

protected:
	/** Handler for opening the source file for this asset */
	void ExecuteFindSourceFileInExplorer(TArray<FString> Filenames, TArray<FString> OverrideExtensions);

	/** Determine whether the find source file in explorer editor command can execute or not */
	bool CanExecuteFindSourceFileInExplorer(TArray<FString> Filenames, TArray<FString> OverrideExtensions) const;

	/** Verify the specified filename exists */
	bool VerifyFileExists(const FString& InFileName) const;
};