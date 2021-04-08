[Setup]
AppName=Chameleon
AppVersion=1.0.0
AppPublisher=GuitarML
AppPublisherURL=https://guitarml.com/
DefaultDirName={cf}
DefaultGroupName=Chameleon
OutputBaseFilename=Chameleon-Install-Win10-Standalone
SetupIconFile=guitarml.ico
ArchitecturesInstallIn64BitMode=x64
 
[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; \
    GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
 
[Files]
;Source: "Chameleon.vst3"; DestDir: "{app}\Steinberg\VST3"
Source: "Chameleon.exe"; DestDir: "{pf64}\GuitarML"
Source: "guitarml.ico"; DestDir: "{pf64}\GuitarML"

[Icons]
Name: "{userdesktop}\Chameleon"; Filename: "{pf64}\GuitarML\Chameleon.exe"; \
    IconFilename: "{pf64}\GuitarML\guitarml.ico"; Tasks: desktopicon