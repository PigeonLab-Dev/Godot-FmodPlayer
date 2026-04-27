using System;
using FmodPlayer;
using Godot;
using Godot.Collections;

public partial class CSharpWrapperSmokeTest : Node
{
	private int _failures;
	private Godot.FileAccess _logFile;

	public override void _Ready()
	{
		_logFile = Godot.FileAccess.Open("res://tests/csharp_wrapper_smoke_test_result.txt", Godot.FileAccess.ModeFlags.Write);
		Log("CSHARP_WRAPPER_SMOKE_TEST_BEGIN");

		try
		{
			RunSmokeTests();
		}
		catch (Exception exception)
		{
			Fail($"Unhandled exception: {exception}");
		}

		Log($"CSHARP_WRAPPER_SMOKE_TEST_SUMMARY failures={_failures}");
		Log("CSHARP_WRAPPER_SMOKE_TEST_END");
		_logFile?.Flush();
		_logFile?.Dispose();

		GetTree().Quit(_failures == 0 ? 0 : 1);
	}

	private void RunSmokeTests()
	{
		var system = FmodServer.GetMainSystem();
		Check(system != null, "FmodServer.GetMainSystem");
		if (system == null)
		{
			return;
		}

		Dictionary version = system.GetVersion();
		Check(version.Count > 0, "FmodSystem.GetVersion");

		long stereoChannels = system.GetSpeakerModeChannels(FmodSystem.FmodSpeakerMode.Stereo);
		Check(stereoChannels == 2, $"FmodSystem.GetSpeakerModeChannels stereo={stereoChannels}");

		float[] stereoMatrix = system.GetDefaultMixMatrix(
			FmodSystem.FmodSpeakerMode.Stereo,
			FmodSystem.FmodSpeakerMode.Stereo,
			0,
			0
		);
		Check(stereoMatrix.Length == 4, $"FmodSystem.GetDefaultMixMatrix length={stereoMatrix.Length}");

		var masterGroup = FmodServer.GetMasterChannelGroup();
		Check(masterGroup != null && masterGroup.ChannelGroupIsValid(), "FmodServer.GetMasterChannelGroup");
		if (masterGroup != null)
		{
			double previousVolume = masterGroup.VolumeDb;
			masterGroup.VolumeDb = -3.0;
			Check(Math.Abs(masterGroup.VolumeDb - -3.0) < 0.1, $"FmodChannelGroup.VolumeDb set/get={masterGroup.VolumeDb}");
			masterGroup.VolumeDb = previousVolume;
		}

		var fader = system.CreateDspByType((long)FmodDSP.FmodDspType.Fader);
		Check(fader != null, "FmodSystem.CreateDspByType(Fader)");
		if (fader != null)
		{
			long parameterCount = fader.GetNumParameters();
			Check(parameterCount >= 2, $"FmodDSP.GetNumParameters count={parameterCount}");

			Dictionary info = fader.GetParameterInfo(0);
			Check(info.Count > 0 && info.ContainsKey("name"), "FmodDSP.GetParameterInfo");

			fader.SetParameterFloat(0, -6.0);
			double gain = fader.GetParameterFloat(0);
			Check(Math.Abs(gain - -6.0) < 0.1, $"FmodDSP.Set/GetParameterFloat gain={gain}");
		}

		var oscillator = system.CreateDspByType((long)FmodDSP.FmodDspType.Oscillator);
		Check(oscillator != null, "FmodSystem.CreateDspByType(Oscillator)");
		if (oscillator != null && masterGroup != null)
		{
			oscillator.SetParameterFloat(1, 440.0);
			var channel = system.PlayDsp(oscillator, masterGroup, true);
			Check(channel != null && channel.ChannelControlIsValid(), "FmodSystem.PlayDsp");
			if (channel != null)
			{
				channel.VolumeDb = -24.0;
				Check(Math.Abs(channel.VolumeDb - -24.0) < 0.1, $"FmodChannel.VolumeDb set/get={channel.VolumeDb}");
				channel.Stop();
			}
			oscillator.Release();
		}

		if (fader != null)
		{
			fader.Release();
		}

		var soundGroup = system.CreateSoundGroup("CSharpWrapperSmokeSoundGroup");
		Check(soundGroup != null && soundGroup.SoundGroupIsValid(), "FmodSystem.CreateSoundGroup");
		if (soundGroup != null)
		{
			soundGroup.VolumeDb = -5.0;
			Check(Math.Abs(soundGroup.VolumeDb - -5.0) < 0.1, $"FmodSoundGroup.VolumeDb set/get={soundGroup.VolumeDb}");
			Check(soundGroup.GetName() == "CSharpWrapperSmokeSoundGroup", $"FmodSoundGroup.GetName={soundGroup.GetName()}");
			soundGroup.Release();
		}

		var childGroup = system.CreateChannelGroup("CSharpWrapperSmokeChannelGroup");
		Check(childGroup != null && childGroup.ChannelGroupIsValid(), "FmodSystem.CreateChannelGroup");
		if (childGroup != null)
		{
			Check(childGroup.GetName() == "CSharpWrapperSmokeChannelGroup", $"FmodChannelGroup.GetName={childGroup.GetName()}");
			childGroup.Release();
		}

		var reverb = system.CreateReverb3d();
		Check(reverb != null && reverb.Reverb3dIsValid(), "FmodSystem.CreateReverb3d");
		if (reverb != null)
		{
			reverb.DecayTime = 1200.0;
			Check(Math.Abs(reverb.DecayTime - 1200.0) < 0.1, $"FmodReverb3D.DecayTime set/get={reverb.DecayTime}");
			reverb.Set3dAttributes(new Vector3(1, 2, 3), 2.0, 8.0);
			Dictionary attributes = reverb.Get3dAttributes();
			Check(attributes.Count > 0, "FmodReverb3D.Get3dAttributes");
			reverb.Release();
		}
	}

	private void Check(bool condition, string label)
	{
		if (condition)
		{
			Log($"PASS {label}");
		}
		else
		{
			Fail(label);
		}
	}

	private void Fail(string label)
	{
		_failures++;
		Log($"FAIL {label}");
		GD.PushError(label);
	}

	private void Log(string message)
	{
		GD.Print(message);
		_logFile?.StoreLine(message);
		_logFile?.Flush();
	}
}
