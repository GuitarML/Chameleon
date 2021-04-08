# Build Linux Installer Instructions (.vst3 only)

The Linux installer uses a Binary Payload script as described here: 
https://www.linuxjournal.com/content/add-binary-payload-your-shell-scripts

1. Copy the Linux Chameleon.vst3 to this folder. 

2. Run the build script using "./build_linux_installer.sh" in the terminal.

3. The output .exe installer will be generated in "Builds/".

4. Run installer using "./Chameleon-Install-VST3-Linux.sh" in the terminal.

Note: You may need to apply proper permissions to the installer, for example: 
       "chmod 775 Chameleon-Install-VST3-Linux.sh"

Note: The .vst3 will be installed to the user's home folder "~/.vst3/"
      The script will untar the payload to the current directory first, so if the install fails you can
	  manually copy the "Chameleon.vst" folder to the desired location.
	  
	 
