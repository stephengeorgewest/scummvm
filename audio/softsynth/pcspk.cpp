/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "audio/softsynth/pcspk.h"
#include "audio/null.h"

#include "common/queue.h"

namespace Audio {

PCSpeaker::Command::Command(WaveForm aWaveForm, float aFrequency, uint32 aLength) :
	waveForm(aWaveForm), frequency(aFrequency), length(aLength) { }

const PCSpeaker::generatorFunc PCSpeaker::generateWave[] =
	{&PCSpeaker::generateSquare, &PCSpeaker::generateSine,
	 &PCSpeaker::generateSaw,    &PCSpeaker::generateTriangle,
	 &PCSpeaker::generateSilence};

PCSpeaker::PCSpeaker(int rate) {
	_rate = rate;
	_wave = kWaveFormSquare;
	_playForever = false;
	_oscLength = 0;
	_oscSamples = 0;
	_remainingSamples = 0;
	_mixedSamples = 0;
	_volume = 255;
	_commandQueue = new Common::Queue<Command>();
	_commandActive = false;
}

PCSpeaker::~PCSpeaker() {
	delete _commandQueue;
}

void PCSpeaker::play(WaveForm wave, int freq, int32 length) {
	Common::StackLock lock(_mutex);

	assert((wave >= kWaveFormSquare) && (wave <= kWaveFormTriangle));

	if (_commandActive || !_commandQueue->empty())
		// Currently playing back a queued instruction. Stop playback and clear
		// the instruction queue.
		stop(0);

	_wave = wave;
	_oscLength = _rate / freq;
	_oscSamples = 0;
	if (length == -1) {
		_remainingSamples = 1;
		_playForever = true;
	} else {
		_remainingSamples = (_rate * length) / 1000;
		_playForever = false;
	}
	_mixedSamples = 0;
}

void PCSpeaker::playQueue(WaveForm wave, float freq, uint32 lengthus) {
	Common::StackLock lock(_mutex);

	// Put the new instruction in the queue. This will be picked up by the
	// readBuffer method.
	_commandQueue->push(Command(wave, freq, lengthus));
}

void PCSpeaker::stop(int32 delay) {
	Common::StackLock lock(_mutex);

	_commandQueue->clear();
	uint32 delaySamples = (_rate * delay) / 1000;
	if (_commandActive) {
		_remainingSamples = MIN(_remainingSamples, delaySamples);
	} else {
		_remainingSamples = delaySamples;
	}
	_playForever = false;
}

void PCSpeaker::setVolume(byte volume) {
	_volume = volume;
}

bool PCSpeaker::isPlaying() const {
	Common::StackLock lock(_mutex);

	return _remainingSamples != 0 || !_commandQueue->empty();
}

int PCSpeaker::readBuffer(int16 *buffer, const int numSamples) {
	Common::StackLock lock(_mutex);

	// The total number of samples generated.
	int generatedSamples = 0;
	// Keep generating samples when not enough have been generated and either
	// the current instruction has not yet finished, or there are more
	// instructions in the queue.
	while (generatedSamples < numSamples && (_remainingSamples > 0 || !_commandQueue->empty())) {
		if (!_commandActive && !_commandQueue->empty()) {
			// No playback instruction is currently being processed, but there
			// are instructions in the queue. Start processing the first queued
			// instruction.
			// Note that this will end playback started by the play method.
			Command command = _commandQueue->pop();
			_wave = command.waveForm;
			_oscLength = (uint32)(_rate / command.frequency);
			_oscSamples = 0;
			// Length is in microseconds.
			_remainingSamples = (_rate * command.length) / 1000000;
			_playForever = false;
			_commandActive = true;
		}

		// The number of samples generated by this playback instruction.
		int commandSamples;

		// Offset the samples for this command by the number of previously
		// generated samples.
		for (commandSamples = 0; _remainingSamples && ((generatedSamples + commandSamples) < numSamples); commandSamples++) {
			buffer[generatedSamples + commandSamples] = generateWave[_wave](_oscSamples, _oscLength) * _volume;
			if (_oscSamples++ >= _oscLength)
				_oscSamples = 0;
			if (!_playForever)
				_remainingSamples--;
			_mixedSamples++;
		}

		generatedSamples += commandSamples;

		if (_remainingSamples == 0)
			// Current playback instruction has finished.
			_commandActive = false;
	}

	// Clear the rest of the buffer
	if (generatedSamples < numSamples)
		memset(buffer + generatedSamples, 0, (numSamples - generatedSamples) * sizeof(int16));

	return numSamples;
}

int8 PCSpeaker::generateSquare(uint32 x, uint32 oscLength) {
	return (x < (oscLength / 2)) ? 127 : -128;
}

int8 PCSpeaker::generateSine(uint32 x, uint32 oscLength) {
	if (oscLength == 0)
		return 0;

	// TODO: Maybe using a look-up-table would be better?
	return CLIP<int16>((int16) (128 * sin(2.0 * M_PI * x / oscLength)), -128, 127);
}

int8 PCSpeaker::generateSaw(uint32 x, uint32 oscLength) {
	if (oscLength == 0)
		return 0;

	return ((x * (65536 / oscLength)) >> 8) - 128;
}

int8 PCSpeaker::generateTriangle(uint32 x, uint32 oscLength) {
	if (oscLength == 0)
		return 0;

	int y = ((x * (65536 / (oscLength / 2))) >> 8) - 128;

	return (x <= (oscLength / 2)) ? y : (256 - y);
}

int8 PCSpeaker::generateSilence(uint32 x, uint32 oscLength) {
	return 0;
}

} // End of namespace Audio


//	Plugin interface
//	(This can only create a null driver since pc speaker support is not part of the
//	midi driver architecture. But we need the plugin for the options menu in the launcher
//	and for MidiDriver::detectDevice() which is more or less used by all engines.)

class PCSpeakerMusicPlugin : public NullMusicPlugin {
public:
	const char *getName() const override {
		return _s("PC Speaker emulator");
	}

	const char *getId() const override {
		return "pcspk";
	}

	MusicDevices getDevices() const override;
};

MusicDevices PCSpeakerMusicPlugin::getDevices() const {
	MusicDevices devices;
	devices.push_back(MusicDevice(this, "", MT_PCSPK));
	return devices;
}

class PCjrMusicPlugin : public NullMusicPlugin {
public:
	const char *getName() const override {
		return _s("IBM PCjr emulator");
	}

	const char *getId() const override {
		return "pcjr";
	}

	MusicDevices getDevices() const override;
};

MusicDevices PCjrMusicPlugin::getDevices() const {
	MusicDevices devices;
	devices.push_back(MusicDevice(this, "", MT_PCJR));
	return devices;
}

//#if PLUGIN_ENABLED_DYNAMIC(PCSPK)
	//REGISTER_PLUGIN_DYNAMIC(PCSPK, PLUGIN_TYPE_MUSIC, PCSpeakerMusicPlugin);
//#else
	REGISTER_PLUGIN_STATIC(PCSPK, PLUGIN_TYPE_MUSIC, PCSpeakerMusicPlugin);
//#endif

//#if PLUGIN_ENABLED_DYNAMIC(PCJR)
	//REGISTER_PLUGIN_DYNAMIC(PCJR, PLUGIN_TYPE_MUSIC, PCjrMusicPlugin);
//#else
	REGISTER_PLUGIN_STATIC(PCJR, PLUGIN_TYPE_MUSIC, PCjrMusicPlugin);
//#endif
