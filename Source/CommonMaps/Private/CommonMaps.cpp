// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonMaps.h"
#include "CommonMapsStyle.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ISettingsModule.h"
#include "ISettingsContainer.h"

#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"

static const FName CommonMapsTabName("CommonMaps");

#define LOCTEXT_NAMESPACE "FCommonMapsModule"

void FCommonMapsModule::StartupModule()
{
	FCommonMapsStyle::Initialize();
	FCommonMapsStyle::ReloadTextures();

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCommonMapsModule::RegisterMenus));
}

void FCommonMapsModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FCommonMapsStyle::Shutdown();
}

void FCommonMapsModule::OpenCommonMap_Clicked(const FString MapPath)
{
	if (ensure(MapPath.Len()))
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(MapPath);
	}
}

void FCommonMapsModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	/** Common maps menu bar */
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = Menu->AddSection("PlayCommonMaps", TAttribute<FText>(), FToolMenuInsert("Play", EToolMenuInsertType::After));
	FToolMenuEntry CommonMapEntry = FToolMenuEntry::InitComboButton(
		"CommonMapOptions",
		FUIAction(
			FExecuteAction(),
			FCanExecuteAction::CreateRaw(this, &FCommonMapsModule::HasNoPlayWorld),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateRaw(this, &FCommonMapsModule::CanShowCommonMaps)),
		FOnGetContent::CreateRaw(this, &FCommonMapsModule::GetCommonMapsDropdown),
		LOCTEXT("CommonMaps_Label", "Common Maps"),
		LOCTEXT("CommonMaps_ToolTip", "Some commonly desired maps while using the editor"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level")
	);
	CommonMapEntry.StyleNameOverride = "CalloutToolbar";
	Section.AddEntry(CommonMapEntry);
}

bool FCommonMapsModule::HasPlayWorld()
{
	return GEditor->PlayWorld != nullptr;
}

bool FCommonMapsModule::HasNoPlayWorld()
{
	return !HasPlayWorld();
}

bool FCommonMapsModule::HasPlayWorldAndRunning()
{
	return HasPlayWorld() && !GUnrealEd->PlayWorld->bDebugPauseExecution;
}

bool FCommonMapsModule::CanShowCommonMaps()
{
	return HasNoPlayWorld();
}

TSharedRef<SWidget> FCommonMapsModule::GetCommonMapsDropdown()
{
	FMenuBuilder MenuBuilder(true, nullptr);
	
	MenuBuilder.AddSubMenu(
		LOCTEXT("CommonMaps_UserEditorMaps", "Common User Maps"),
		LOCTEXT("CommonMaps_UserEditorMapsToolTip", "User defined common maps"), 
		FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder) {
			for (const FSoftObjectPath& Path : GetDefault<UCommonMapsUserSettings>()->CommonMaps)
			{
				if (!Path.IsValid())
				{
					continue;
				}
		
				const FText DisplayName = FText::FromString(Path.GetAssetName());
				MenuBuilder.AddMenuEntry(
					DisplayName,
					LOCTEXT("CommonMaps_PathTooltip", "Opens this map in the editor"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateRaw(this, &FCommonMapsModule::OpenCommonMap_Clicked, Path.ToString()),
						FCanExecuteAction::CreateRaw(this, &FCommonMapsModule::HasNoPlayWorld),
						FIsActionChecked(),
						FIsActionButtonVisible::CreateRaw(this, &FCommonMapsModule::HasNoPlayWorld)
					)
				);
			}

			MenuBuilder.AddSeparator();
			MenuBuilder.AddMenuEntry(
				LOCTEXT("CommonMaps_AddMap", "Add a common map"), 
				LOCTEXT("CommonMaps_AddMapTooltip", "Adds a new common map to this menu"), 
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.PlusCircle"),
				FUIAction(
					FExecuteAction::CreateLambda([]() {
						FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer("Editor", "General", "CommonMapsUserSettings");
					}), 
					FCanExecuteAction(),
					FIsActionChecked(),
					FIsActionButtonVisible()
				)
			);
		})
	);

	MenuBuilder.AddSubMenu(
		LOCTEXT("CommonMaps_ProjectEditorMaps", "Common Project Maps"),
		LOCTEXT("CommonMaps_ProjectEditorMapsToolTip", "Project defined common maps"), 
		FNewMenuDelegate::CreateLambda([&](FMenuBuilder& MenuBuilder) {
			for (const FSoftObjectPath& Path : GetDefault<UCommonMapsProjectSettings>()->CommonMaps)
			{
				if (!Path.IsValid())
				{
					continue;
				}
		
				const FText DisplayName = FText::FromString(Path.GetAssetName());
				MenuBuilder.AddMenuEntry(
					DisplayName,
					LOCTEXT("CommonMaps_PathTooltip", "Opens this map in the editor"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateRaw(this, &FCommonMapsModule::OpenCommonMap_Clicked, Path.ToString()),
						FCanExecuteAction::CreateRaw(this, &FCommonMapsModule::HasNoPlayWorld),
						FIsActionChecked(),
						FIsActionButtonVisible::CreateRaw(this, &FCommonMapsModule::HasNoPlayWorld)
					)
				);
			}

			MenuBuilder.AddSeparator();
			MenuBuilder.AddMenuEntry(
				LOCTEXT("CommonMaps_AddMap", "Add a common map"), 
				LOCTEXT("CommonMaps_AddMapTooltip", "Adds a new common map to this menu"), 
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.PlusCircle"),
				FUIAction(
					FExecuteAction::CreateLambda([]() {
						FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer("Project", "Editor", "CommonMapsProjectSettings");
					}), 
					FCanExecuteAction(),
					FIsActionChecked(),
					FIsActionButtonVisible()
				)
			);
		})
	);

	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonMapsModule, CommonMaps)
