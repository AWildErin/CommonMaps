// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine/DeveloperSettings.h"

#include "CommonMaps.generated.h"

class FToolBarBuilder;
class FMenuBuilder;

/**
* @class UCommonMapsUserSettings
* User settings for common maps
*/
UCLASS(config = EditorPerProjectUserSettings)
class UCommonMapsUserSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	/** A list of common maps defined for just you */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (AllowedClasses = "/Script/Engine.World"))
	TArray<FSoftObjectPath> CommonMaps;
};

/**
 * @class UCommonMapsProjectSettings
 * Project settings for common maps
 */
UCLASS(config = Editor, defaultconfig)
class UCommonMapsProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	/** A list of common maps defined for the whole project */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (AllowedClasses = "/Script/Engine.World"))
	TArray<FSoftObjectPath> CommonMaps;
};

class FCommonMapsModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void OpenCommonMap_Clicked(const FString MapPath);

private:
	void RegisterMenus();

	bool HasPlayWorld();
	bool HasNoPlayWorld();
	bool HasPlayWorldAndRunning();
	bool CanShowCommonMaps();

	TSharedRef<SWidget> GetCommonMapsDropdown();
};
