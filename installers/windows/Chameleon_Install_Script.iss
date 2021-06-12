[Setup]
AppName=Chameleon
AppVersion=##APPVERSION##
DisableWelcomePage=no
DisableDirPage=yes
DefaultDirName={commoncf64}
DefaultGroupName=Chameleon
OutputBaseFilename="Chameleon-Win-##APPVERSION##"
OutputDir=.
LicenseFile=../../LICENSE

[Types]
Name: "full"; Description: "Full installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "VST3_64"; Description: "VST3 Plugin 64-bit"; Types: full
Name: "VST3_32"; Description: "VST3 Plugin 32-bit"; Types: full
; Name: "AAX"; Description: "AAX Plugin"; Types: full

[Files]
Source: "../../bin/Win64/Chameleon.vst3"; DestDir: "{code:GetDir|VST3_64}"; Components: VST3_64; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "../../bin/Win32/Chameleon.vst3"; DestDir: "{code:GetDir|VST3_32}"; Components: VST3_32; Flags: ignoreversion recursesubdirs createallsubdirs

[Code]
var
  AAXDirPage: TInputDirWizardPage;
  Vst3_64DirPage: TinputDirWizardPage;
  Vst3_32DirPage: TinputDirWizardPage;

procedure InitializeWizard;
begin
  Log('Initializing extra pages')
  //AAX Dir Page
  AAXDirPage := CreateInputDirPage(wpSelectComponents,
    'Select AAX Install Location', 'Where would you like to install the AAX plugin?',
    'AAX plugin will be installed in the following folder.'#13#10#13#10 +
    'To continue, click Next. If you would like to select a different folder, click Browse.',
    False, 'New Folder');

  AAXDirPage.add('');
  AAXDirPage.values[0] := ExpandConstant('{commoncf64}\Avid\Audio\Plug-Ins');

  //VST3 64-bit Dir Page
  Vst3_64DirPage := CreateInputDirPage(AAXDirPage.ID,
    'Select Install Location for VST3 64-bit', 'Where would you like to install the plugin?',
    'VST3 64-bit plugin will be installed in the following folder.'#13#10#13#10 +
    'To continue, click Next. If you would like to select a different folder, click Browse.',
    False, 'New Folder');

  Vst3_64DirPage.add('');
  Vst3_64DirPage.values[0] := ExpandConstant('{commoncf64}\VST3');


  //VST3 32-bit Dir Page
  Vst3_32DirPage := CreateInputDirPage(Vst3_64DirPage.ID,
    'Select Install Location for VST3 32-bit', 'Where would you like to install the plugin?',
    'VST3 32-bit plugin will be installed in the following folder.'#13#10#13#10 +
    'To continue, click Next. If you would like to select a different folder, click Browse.',
    False, 'New Folder');

  Vst3_32DirPage.add('');
  Vst3_32DirPage.values[0] := ExpandConstant('{commoncf32}\VST3');

end;

function IsSelected(Param: String) : Boolean;
begin
  if not (Pos(Param, WizardSelectedComponents(False)) = 0) then // WizardSelectedComponents(False)) then
    Result := True
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  { Skip pages that shouldn't be shown }
  Result := False;

  if (PageID = AAXDirPage.ID) then
  begin
    Result := True;
    Log('Selected 1: ' + WizardSelectedComponents(False)); 

    if IsSelected ('aax') then
    begin
      Log('Not Skipping page');
      Result := False;
    end
  end

  else if (PageID = Vst3_64DirPage.ID) then
  begin
      Result := True;
      Log('Selected 2: ' + WizardSelectedComponents(False));

      if IsSelected ('vst3_64') then
      begin
        Log('Not Skipping');
        Result := False;
      end
  end

  else if (PageID = Vst3_32DirPage.ID) then
  begin
      Result := True;
      Log('Selected 3: ' + WizardSelectedComponents(False));

      if IsSelected ('vst3_32') then
      begin
        Log('Not Skipping');
        Result := False;
      end
  end

end;

function GetDir(Param: String) : String;
begin
  if (Param = 'AAX') then
    Result := AAXDirPage.values[0]
  else if (Param = 'VST3_64') then
    Result := Vst3_64DirPage.values[0]
  else if (Param = 'VST3_32') then
    Result := Vst3_32DirPage.values[0]
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo,
  MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
var
  S: String;
begin
  { Fill the 'Ready Memo' with the normal settings and the custom settings }
  S := '';
  S := S + MemoTypeInfo + NewLine + NewLine;
  S := S + MemoComponentsInfo + NewLine + NewLine;
  S := S + 'Destination Location:' + NewLine;

  if IsSelected('aax') then
    S := S + Space +  GetDir('AAX') + ' (AAX)' + NewLine;

  if IsSelected('vst3_64') then
    S := S + Space +  GetDir('VST3_64') + ' (VST3 64-bit)' + NewLine;


  if IsSelected('vst3_32') then
    S := S + Space +  GetDir('VST3_32') + ' (VST3 32-bit)' + NewLine;

  Result := S;
end;
