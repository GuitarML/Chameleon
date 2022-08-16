juce_add_binary_data(BinaryData SOURCES
	chameleon_amp.jpg
	CMakeLists.txt
	knob_70_black.png
	led_24_red.png
	led_gold.png
	led_gold_on.png
	led_green.png
	led_green_on.png
	led_red.png
	led_red_off.png
	led_red_on.png
	logo.png
	power_switch_down.png
	power_switch_mid.png
	power_switch_up.png
      ../models/red.json
      ../models/gold.json
      ../models/green.json
)

# Need to build BinaryData with -fPIC flag on Linux
set_target_properties(BinaryData PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE)
