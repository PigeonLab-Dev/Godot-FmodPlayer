extends SceneTree

func _initialize() -> void:
	TranslationServer.set_locale("zh_Hans")
	var plugin_script := load("res://addons/fmod_player/fmod_player_main.gd")
	var plugin: EditorPlugin = plugin_script.new()
	plugin.call("_add_fmod_property_translations")

	var domain := TranslationServer.get_or_add_domain("godot.properties")
	var banner := String(domain.translate("Show Startup Banner"))
	var dsp_buffer_size := String(domain.translate("Dsp Buffer Size"))
	var init_flags := String(domain.translate("Init Flags"))
	print("FMOD_TRANSLATION_PROBE:Show Startup Banner=", banner == "显示启动横幅", " hex=", banner.to_utf8_buffer().hex_encode(), " expected=", "显示启动横幅".to_utf8_buffer().hex_encode())
	print("FMOD_TRANSLATION_PROBE:Dsp Buffer Size=", dsp_buffer_size == "DSP 缓冲区大小", " hex=", dsp_buffer_size.to_utf8_buffer().hex_encode(), " expected=", "DSP 缓冲区大小".to_utf8_buffer().hex_encode())
	print("FMOD_TRANSLATION_PROBE:Init Flags=", init_flags == "初始化标志", " hex=", init_flags.to_utf8_buffer().hex_encode(), " expected=", "初始化标志".to_utf8_buffer().hex_encode())
	quit()
