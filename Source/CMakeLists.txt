#add_subdirectory(headless)

target_sources(Chameleon PRIVATE
	Eq4Band.cpp
	Eq4Band.h
	myLookAndFeel.cpp
	myLookAndFeel.h
	PluginEditor.cpp
	PluginEditor.h
	PluginProcessor.cpp
	PluginProcessor.h
	RTNeuralLSTM.cpp
	RTNeuralLSTM.h

)

#target_precompile_headers(Chameleon PRIVATE pch.h)
