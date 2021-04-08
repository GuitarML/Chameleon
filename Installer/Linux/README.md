# Build Windows Installer Instructions

This uses InnoSetup to build Windows installers. The .iss scripts are used by InnoSetup.

1. Download InnoSetup: https://jrsoftware.org/isdl.php

2. Copy the Windows Chameleon.vst3 or Chameleon.exe to this folder. 

3. Open the .iss script (either VST3 or Standalone) with InnoSetup.

4. Run the script from InnoSetup. The output .exe installer will be generated in "Output/".

Note: These are for 64 bit Windows installers. The .vst3 will be installed to "C:\Program Files\Common Files\VST3"
      The standalone will be installed to "C:\Program Files\GuitarML" with the option of creating a Desktop shortcut.