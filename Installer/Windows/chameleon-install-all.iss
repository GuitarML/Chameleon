[Setup]
AppName=Chameleon
AppVersion=1.0.0
AppPublisher=GuitarML
AppPublisherURL=https://guitarml.com/
DefaultDirName={cf}
DefaultGroupName=Chameleon
OutputBaseFilename=Chameleon-Install-Win10
SetupIconFile=guitarml.ico
ArchitecturesInstallIn64BitMode=x64
 
[Types]
Name: "VST3"; Description: "VST3 Plugin install"
Name: "Standalone"; Description: "Standalone app install"
 
[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; \
    GroupDescription: "{cm:AdditionalIcons}"
 
[Components]
Name: "vst3"; Description: "VST3 Plugin"; Types: VST3
Name: "standalone"; Description: "Standalone app"; Types: Standalone
 
[Files]
;Source: "Chameleon.vst3"; DestDir: "{app}\Steinberg\VST3"
Source: "Chameleon.exe"; DestDir: "{pf64}\GuitarML"; Components: standalone
Source: "guitarml.ico"; DestDir: "{pf64}\GuitarML"; Components: standalone
Source: "Chameleon.vst3"; DestDir: "{app}\VST3"; Components: vst3

[Icons]
Name: "{userdesktop}\Chameleon"; Filename: "{pf64}\GuitarML\Chameleon.exe"; \
    IconFilename: "{pf64}\GuitarML\guitarml.ico"; Tasks: desktopicon