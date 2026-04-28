#include "fmod_audio_preview_property.h"

#include <cstring>

#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {
	void FmodAudioPreviewProperty::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioPreviewProperty::set_stream);
		ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioPreviewProperty::get_stream);
		ClassDB::bind_method(D_METHOD("_draw_waveform"), &FmodAudioPreviewProperty::_draw_waveform);
		ClassDB::bind_method(D_METHOD("_draw_indicator"), &FmodAudioPreviewProperty::_draw_indicator);
		ClassDB::bind_method(D_METHOD("_on_indicator_gui_input", "event"), &FmodAudioPreviewProperty::_on_indicator_gui_input);
		ClassDB::bind_method(D_METHOD("_play"), &FmodAudioPreviewProperty::_play);
		ClassDB::bind_method(D_METHOD("_stop"), &FmodAudioPreviewProperty::_stop);
		ClassDB::bind_method(D_METHOD("_on_finished"), &FmodAudioPreviewProperty::_on_finished);
		ClassDB::bind_method(D_METHOD("_generate_waveform_async"), &FmodAudioPreviewProperty::_generate_waveform_async);
	}

	FmodAudioPreviewProperty::FmodAudioPreviewProperty() {
		set_custom_minimum_size(Vector2(1, 100));
		set_process(false);

		_player = memnew(FmodAudioStreamPlayer);
		_player->connect("finished", callable_mp(this, &FmodAudioPreviewProperty::_on_finished));
		add_child(_player);

		VBoxContainer* vbox = memnew(VBoxContainer);
		vbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		add_child(vbox);

		_preview = memnew(ColorRect);
		_preview->set_v_size_flags(Control::SIZE_EXPAND_FILL);
		_preview->connect("draw", callable_mp(this, &FmodAudioPreviewProperty::_draw_waveform));
		vbox->add_child(_preview);

		_indicator = memnew(Control);
		_indicator->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		_indicator->set_mouse_filter(Control::MOUSE_FILTER_STOP);
		_indicator->connect("draw", callable_mp(this, &FmodAudioPreviewProperty::_draw_indicator));
		_indicator->connect("gui_input", callable_mp(this, &FmodAudioPreviewProperty::_on_indicator_gui_input));
		_preview->add_child(_indicator);

		HBoxContainer* hbox = memnew(HBoxContainer);
		hbox->add_theme_constant_override("separation", 0);
		vbox->add_child(hbox);

		_play_button = memnew(Button);
		_play_button->set_flat(true);
		_play_button->set_focus_mode(Control::FOCUS_ACCESSIBILITY);
		_play_button->set_accessibility_name("Play");
		_play_button->connect("pressed", callable_mp(this, &FmodAudioPreviewProperty::_play));
		hbox->add_child(_play_button);

		_stop_button = memnew(Button);
		_stop_button->set_flat(true);
		_stop_button->set_focus_mode(Control::FOCUS_ACCESSIBILITY);
		_stop_button->set_accessibility_name("Stop");
		_stop_button->connect("pressed", callable_mp(this, &FmodAudioPreviewProperty::_stop));
		hbox->add_child(_stop_button);

		_current_label = memnew(Label);
		_current_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
		_current_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		_current_label->set_modulate(Color(1, 1, 1, 0.5));
		hbox->add_child(_current_label);

		_duration_label = memnew(Label);
		_duration_label->set_modulate(Color(1, 1, 1, 0.5));
		hbox->add_child(_duration_label);

		_update_time_labels();
	}

	FmodAudioPreviewProperty::~FmodAudioPreviewProperty() {
	}

	void FmodAudioPreviewProperty::_notification(int p_what) {
		switch (p_what) {
			case NOTIFICATION_THEME_CHANGED: {
				if (_play_button) {
					_play_button->set_button_icon(FmodUtils::get_editor_theme_icon(is_playing ? "Pause" : "MainPlay"));
				}
				if (_stop_button) {
					_stop_button->set_button_icon(FmodUtils::get_editor_theme_icon("Stop"));
				}
				if (_preview) {
					_preview->set_color(FmodUtils::get_editor_theme_color("dark_color_2", "Editor"));
				}
				set_color(FmodUtils::get_editor_theme_color("dark_color_1", "Editor"));
				_queue_preview_redraw();
			} break;

			case NOTIFICATION_PROCESS: {
				_process_playback();
			} break;

			case NOTIFICATION_READY: {
				_update_preview();
			} break;

			case NOTIFICATION_RESIZED: {
				_queue_preview_redraw();
			} break;

			case NOTIFICATION_VISIBILITY_CHANGED: {
				if (!is_visible_in_tree()) {
					_stop();
				}
				_queue_preview_redraw();
			} break;
		}
	}

	void FmodAudioPreviewProperty::set_stream(const Ref<FmodAudioStream>& p_stream) {
		if (stream == p_stream) {
			return;
		}

		stream = p_stream;
		_update_preview();
	}

	Ref<FmodAudioStream> FmodAudioPreviewProperty::get_stream() const {
		return stream;
	}

	void FmodAudioPreviewProperty::_process_playback() {
		if (!is_playing || _player == nullptr) {
			return;
		}

		playback_position = _player->get_playback_position();
		_update_time_labels();
		_queue_preview_redraw();

		if (!_player->is_playing()) {
			_on_finished();
		}
	}

	void FmodAudioPreviewProperty::_update_time_labels() {
		if (_current_label) {
			_current_label->set_text(String::num(playback_position, 2).pad_decimals(2) + " /");
		}
		if (_duration_label) {
			_duration_label->set_text(String::num(stream_duration, 2).pad_decimals(2) + "s");
		}
	}

	void FmodAudioPreviewProperty::_queue_preview_redraw() {
		if (_preview) {
			_preview->queue_redraw();
		}
		if (_indicator) {
			_indicator->queue_redraw();
		}
	}

	void FmodAudioPreviewProperty::_update_preview() {
		if (_player != nullptr) {
			_player->stop();
		}

		is_playing = false;
		is_pausing = false;
		is_dragging = false;
		playback_position = 0.0f;
		stream_duration = 0.0f;
		set_process(false);

		if (_play_button) {
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
		}

		if (stream.is_valid()) {
			if (_player != nullptr) {
				_player->set_stream(stream);
			}
			if (stream->is_data_loaded() && stream->preload()) {
				stream_duration = stream->get_length();
			}
		}

		set_visible(stream.is_valid());
		_update_time_labels();
		_queue_preview_redraw();
	}

	void FmodAudioPreviewProperty::_draw_waveform() {
		if (stream.is_null() || !stream->is_data_loaded()) {
			return;
		}

		Size2 size = _preview->get_size();
		int width = (int)size.width;
		if (width <= 0 || size.height <= 0) {
			return;
		}

		if (cached_waveform_stream != stream || cached_waveform_points.is_empty()) {
			if (!waveform_generating) {
				waveform_generating = true;
				call_deferred("_generate_waveform_async");
			}
			if (cached_waveform_points.is_empty()) {
				return;
			}
		}

		PackedVector2Array scaled_points;
		scaled_points.resize(width * 2);

		const int source_width = cached_waveform_points.size() / 2;
		for (int i = 0; i < width; i++) {
			int source_index = CLAMP((int)((float)i / MAX(width - 1, 1) * MAX(source_width - 1, 0)), 0, MAX(source_width - 1, 0));
			float min_y_norm = cached_waveform_points[source_index * 2].y;
			float max_y_norm = cached_waveform_points[source_index * 2 + 1].y;

			scaled_points.set(i * 2, Vector2(i + 1, min_y_norm * size.height));
			scaled_points.set(i * 2 + 1, Vector2(i + 1, max_y_norm * size.height));
		}

		PackedColorArray colors = { FmodUtils::get_editor_theme_color("contrast_color_2", "Editor") };
		RenderingServer::get_singleton()->canvas_item_add_multiline(_preview->get_canvas_item(), scaled_points, colors);
	}

	void FmodAudioPreviewProperty::_generate_waveform_async() {
		if (stream.is_null() || !stream->is_data_loaded()) {
			waveform_generating = false;
			return;
		}

		Ref<FmodSystem> system = FmodServer::get_main_system();
		if (system.is_null() || system->system_is_null()) {
			waveform_generating = false;
			return;
		}

		unsigned int decode_mode = FmodSystem::FMOD_MODE_OPENMEMORY | FmodSystem::FMOD_MODE_CREATESAMPLE;
		Ref<FmodSound> temp_sound = system->create_sound_from_memory(stream->get_audio_data(), decode_mode);
		if (temp_sound.is_null()) {
			waveform_generating = false;
			return;
		}

		Dictionary format_info = temp_sound->get_format();
		int channels = format_info.get("channels", 1);
		int bits = format_info.get("bits", 16);
		int sound_format = format_info.get("format", FmodSound::FMOD_SOUND_FORMAT_PCM16);

		double pcm_bytes = temp_sound->get_length(FmodSystem::FMOD_TIME_UNIT_PCMBYTES);
		if (pcm_bytes <= 0) {
			waveform_generating = false;
			return;
		}

		const unsigned int max_read_bytes = 30 * 1024 * 1024;
		unsigned int read_length = (unsigned int)MIN(pcm_bytes, (double)max_read_bytes);

		PackedByteArray data;
		Ref<FmodSoundLock> lock = temp_sound->lock(0, read_length);
		if (lock.is_valid()) {
			data = lock->get_data1();
			PackedByteArray data2 = lock->get_data2();
			if (!data2.is_empty()) {
				int offset = data.size();
				data.resize(offset + data2.size());
				memcpy(data.ptrw() + offset, data2.ptr(), data2.size());
			}
		}

		if (data.is_empty()) {
			waveform_generating = false;
			return;
		}

		int bytes_per_sample = bits / 8;
		if (bytes_per_sample <= 0) {
			bytes_per_sample = 2;
		}

		int frame_size = bytes_per_sample * MAX(channels, 1);
		int total_frames = data.size() / frame_size;
		if (total_frames <= 0) {
			waveform_generating = false;
			return;
		}

		const int waveform_width = 1000;
		PackedVector2Array points;
		points.resize(waveform_width * 2);

		for (int i = 0; i < waveform_width; i++) {
			int start_frame = (int)(i * (float)total_frames / waveform_width);
			int end_frame = (int)((i + 1) * (float)total_frames / waveform_width);
			end_frame = MIN(end_frame, total_frames);
			start_frame = CLAMP(start_frame, 0, MAX(end_frame - 1, 0));

			float max_val = -1.0f;
			float min_val = 1.0f;

			for (int frame = start_frame; frame < end_frame; frame++) {
				int frame_offset = frame * frame_size;
				float sample_sum = 0.0f;

				for (int ch = 0; ch < channels; ch++) {
					int sample_offset = frame_offset + ch * bytes_per_sample;
					if (sample_offset + bytes_per_sample > data.size()) {
						continue;
					}

					const uint8_t* ptr = data.ptr() + sample_offset;
					float sample_val = 0.0f;

					switch (sound_format) {
						case FmodSound::FMOD_SOUND_FORMAT_PCM8:
							sample_val = (float)(*ptr) / 127.5f - 1.0f;
							break;

						case FmodSound::FMOD_SOUND_FORMAT_PCM16: {
							int16_t val = (int16_t)(ptr[0] | (ptr[1] << 8));
							sample_val = (float)val / 32767.0f;
							break;
						}

						case FmodSound::FMOD_SOUND_FORMAT_PCM24: {
							int32_t val = (int32_t)(ptr[0] | (ptr[1] << 8) | (ptr[2] << 16));
							if (val & 0x800000) {
								val |= 0xFF000000;
							}
							sample_val = (float)val / 8388607.0f;
							break;
						}

						case FmodSound::FMOD_SOUND_FORMAT_PCM32: {
							int32_t val = (int32_t)(ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24));
							sample_val = (float)val / 2147483647.0f;
							break;
						}

						case FmodSound::FMOD_SOUND_FORMAT_PCMFLOAT:
							memcpy(&sample_val, ptr, sizeof(float));
							break;

						default:
							if (bits == 8) {
								sample_val = (float)(*ptr) / 127.5f - 1.0f;
							}
							else {
								int16_t val = (int16_t)(ptr[0] | (ptr[1] << 8));
								sample_val = (float)val / 32767.0f;
							}
							break;
					}

					sample_sum += sample_val;
				}

				float sample_avg = channels > 0 ? sample_sum / channels : sample_sum;
				max_val = MAX(max_val, sample_avg);
				min_val = MIN(min_val, sample_avg);
			}

			float min_y = min_val * 0.5f + 0.5f;
			float max_y = max_val * 0.5f + 0.5f;
			if (max_y - min_y < 0.01f) {
				float mid = (max_y + min_y) * 0.5f;
				min_y = mid - 0.005f;
				max_y = mid + 0.005f;
			}

			points.set(i * 2, Vector2((float)i / waveform_width, CLAMP(min_y, 0.0f, 1.0f)));
			points.set(i * 2 + 1, Vector2((float)i / waveform_width, CLAMP(max_y, 0.0f, 1.0f)));
		}

		cached_waveform_points = points;
		cached_waveform_stream = stream;
		waveform_generating = false;
		_queue_preview_redraw();
	}

	void FmodAudioPreviewProperty::_draw_indicator() {
		if (stream.is_null() || stream_duration <= 0.0f || _indicator == nullptr) {
			return;
		}

		Size2 size = _indicator->get_size();
		if (size.width <= 0 || size.height <= 0) {
			return;
		}

		float ofs_x = CLAMP(playback_position / stream_duration * size.width, 0.0f, size.width);
		Color color = FmodUtils::get_editor_theme_color("accent_color", "Editor");
		Ref<Texture2D> icon = FmodUtils::get_editor_theme_icon("TimelineIndicator");

		RID canvas_item = _indicator->get_canvas_item();
		RenderingServer::get_singleton()->canvas_item_add_line(canvas_item, Point2(ofs_x, 0), Point2(ofs_x, size.height), color, 2.0f);

		if (icon.is_valid()) {
			RenderingServer::get_singleton()->canvas_item_add_texture_rect(
				canvas_item,
				Rect2(ofs_x - icon->get_width() * 0.5f, 0, icon->get_width(), icon->get_height()),
				icon->get_rid(),
				false,
				color
			);
		}
	}

	void FmodAudioPreviewProperty::_on_indicator_gui_input(const Ref<InputEvent>& p_event) {
		Ref<InputEventMouseButton> mb = p_event;
		if (mb.is_valid() && mb->get_button_index() == MOUSE_BUTTON_LEFT) {
			if (mb->is_pressed()) {
				_seek_to(mb->get_position().x);
			}
			is_dragging = mb->is_pressed();
		}

		Ref<InputEventMouseMotion> mm = p_event;
		if (mm.is_valid() && is_dragging) {
			_seek_to(mm->get_position().x);
		}
	}

	void FmodAudioPreviewProperty::_update_playback_position(float p_position) {
		playback_position = CLAMP(p_position, 0.0f, MAX(stream_duration, 0.0f));
		_update_time_labels();
		_queue_preview_redraw();
	}

	void FmodAudioPreviewProperty::_seek_to(float p_x) {
		if (stream.is_null() || stream_duration <= 0.0f || _indicator == nullptr) {
			return;
		}

		float width = _indicator->get_size().x;
		if (width <= 0.0f) {
			return;
		}

		_update_playback_position((p_x / width) * stream_duration);
		if (_player != nullptr && is_playing) {
			_player->seek(playback_position);
		}
	}

	void FmodAudioPreviewProperty::_play() {
		if (stream.is_null() || !stream->is_data_loaded() || _player == nullptr) {
			return;
		}

		if (is_playing) {
			is_pausing = true;
			playback_position = _player->get_playback_position();
			_player->stop();
			is_playing = false;
			set_process(false);
			if (_play_button) {
				_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
			}
			_update_playback_position(playback_position);
		}
		else {
			is_pausing = false;
			_player->play(playback_position);
			is_playing = true;
			set_process(true);
			if (_play_button) {
				_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("Pause"));
			}
		}
	}

	void FmodAudioPreviewProperty::_stop() {
		if (_player != nullptr) {
			_player->stop();
		}

		is_playing = false;
		is_pausing = false;
		is_dragging = false;
		set_process(false);

		if (_play_button) {
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
		}

		_update_playback_position(0.0f);

		if (_player != nullptr && stream.is_valid()) {
			_player->set_stream(stream);
		}
	}

	void FmodAudioPreviewProperty::_on_finished() {
		is_playing = false;
		set_process(false);

		if (_play_button) {
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
		}

		if (!is_pausing) {
			_update_playback_position(0.0f);
		}
		else {
			_update_playback_position(playback_position);
		}
		is_pausing = false;
	}
}
