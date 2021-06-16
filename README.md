# Chameleon

![app](https://github.com/GuitarML/Chameleon/blob/main/resources/Chameleon.jpg)

Chameleon is a guitar plugin using neural networks to create three distinct sounds from a vintage style amp head. EQ and gain were added to 
allow further modification of the three core sounds, named Red (high gain), Gold (crunchy), and Green (crisp and clean). In the same
way a real amp head is used with a cabinet and other effects, this plugin is intended to be used in the signal chain along with IR's (cab sim),
reverb, and any number of guitar effects. 

Chameleon's core sound comes from a neural net inference engine which allows the plugin to disguise itself as a high end
tube amplifier. The engine uses a stateful LSTM model, which improves the sound quality of the previous stateless LSTM used in the SmartAmpPro. It
also improves CPU usage compared to the [SmartAmpPro](https://github.com/GuitarML/SmartAmpPro) and [SmartGuitarAmp](https://github.com/GuitarML/SmartGuitarAmp).

## Installing the plugin

1. Download the appropriate plugin installer (Windows, Mac)
2. Run the installer and follow the instructions. May need to reboot to allow your DAW to recognize the new plugin.

## Info
Re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep
Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models. 

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications. 

## Swapping Models

Even though Chameleon is intended to have three set models, it is easy to swap these for user created json models. Simply overwrite the "red.json", "gold.json", or "green.json" file with the user created model of the same name before running Chameleon. To reset to the originals, delete the custom model. The next time you run Chameleon, the original tones will be written to the tone directory.

The locations of the json tone files are:
```
Windows 10: C:/Users/<username>/AppData/Roaming/GuitarML/Chameleon/tones
Mac:  /Users/<username>/Documents/GuitarML/Chameleon/tones
```
Note: [NeuralPi](https://github.com/GuitarML/NeuralPi) json files are compatible with Chameleon.

## Build Instructions

### Build with Cmake

```bash
# Clone the repository
$ git clone https://github.com/GuitarML/Chameleon.git
$ cd Chameleon

# initialize and set up submodules
$ git submodule update --init --recursive

# build with CMake
$ cmake -Bbuild
$ cmake --build build --config Release
```
The binaries will be located in `Chameleon/build/Chameleon_artefacts/`

### Build with Projucer 

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Initialize and set up submodules
```git submodule update --init --recursive```
4. Open the Chameleon.jucer file and in the appropriate Exporter Header Search Path field, enter the appropriate include paths from the modules folder.
   For example:

```
  	<full-path-to>/Chameleon/modules/json/include
	<full-path-to>/Chameleon/modules/RTNeural
	<full-path-to>/Chameleon/modules/RTNeural/modules/xsimd/include
```
5. Build Chameleon from the Juce Projucer application. 

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.
