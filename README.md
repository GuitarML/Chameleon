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

1. Download the appropriate plugin installer (Windows 10, Mac, Ubuntu Linux)
2. Run the installer and follow the instructions. May need to reboot to allow your DAW to recognize the new plugin.

## Info
Re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep
Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [AI-GuitarAmpModelling](https://github.com/Alec-Wright/AI-GuitarAmpModelling) project was used to train the .json models. 

## Build Instructions

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Download and extract [NumCpp](https://github.com/dpilger26/NumCpp) This is a c++ implementation of the Python Numpy library.
4. Download and extract [Boost](https://www.boost.org/)  (NumCpp requires Boost)
5. Download and extract: [json](https://github.com/nlohmann/json) Json for c++.
6. Open the Chameleon.jucer file and in the appropriate Exporter Header Search Path field, enter the appropriate include paths.
   For example:

```
  	<full-path-to>/json-develop/include
	<full-path-to>/NumCpp-master/include
	<full-path-to>/boost_1_75_0/boost_1_75_0
```
7. Build Chameleon from the Juce Projucer application. 

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.
