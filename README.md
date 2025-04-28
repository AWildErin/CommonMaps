# CommonMaps
Customised version of the Common Maps feature found in the Lyra game sample from Epic Games.

## Usage
There's two ways to add maps, either via the included settings UI found in Project and Editor settings or by editing the `DefaultEditor.ini` and `DefaultEditorPerProjectUserSettings.ini`.

`DefaultEditor.ini` example:
```ini
[/Script/CommonMaps.CommonMapsProjectSettings]
+CommonMaps=/Game/SCPCB/Maps/Entry_P.Entry_P
+CommonMaps=/Game/SCPCB/Maps/TheSite_P.TheSite_P
```

`DefaultEditorPerProjectUserSettings.ini` example:
```ini
[/Script/CommonMaps.CommonMapsUserSettings]
+CommonMaps=/Game/SCPCB/Maps/Dev/Dev_Interaction_P.Dev_Interaction_P
```