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

// Disable symbol overrides so that we can use system headers.
#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include "backends/text-to-speech/linux/linux-text-to-speech.h"

#if defined(USE_LINUX_TTS)
#include <speech-dispatcher/libspeechd.h>
#include "backends/platform/sdl/sdl-sys.h"

#include "common/translation.h"
#include "common/system.h"
#include "common/ustr.h"
#include "common/config-manager.h"

SPDConnection *_connection;

void speech_begin_callback(size_t msg_id, size_t client_id, SPDNotificationType state){
	LinuxTextToSpeechManager *manager =
		static_cast<LinuxTextToSpeechManager *> (g_system->getTextToSpeechManager());
	manager->updateState(LinuxTextToSpeechManager::SPEECH_BEGUN);
}

void speech_end_callback(size_t msg_id, size_t client_id, SPDNotificationType state){
	LinuxTextToSpeechManager *manager =
		static_cast<LinuxTextToSpeechManager *> (g_system->getTextToSpeechManager());
	manager->updateState(LinuxTextToSpeechManager::SPEECH_ENDED);
}

void speech_cancel_callback(size_t msg_id, size_t client_id, SPDNotificationType state){
	LinuxTextToSpeechManager *manager =
		static_cast<LinuxTextToSpeechManager *> (g_system->getTextToSpeechManager());
	manager->updateState(LinuxTextToSpeechManager::SPEECH_CANCELED);
}

void speech_resume_callback(size_t msg_id, size_t client_id, SPDNotificationType state){
	LinuxTextToSpeechManager *manager =
		static_cast<LinuxTextToSpeechManager *> (g_system->getTextToSpeechManager());
	manager->updateState(LinuxTextToSpeechManager::SPEECH_RESUMED);
}

void speech_pause_callback(size_t msg_id, size_t client_id, SPDNotificationType state){
	LinuxTextToSpeechManager *manager =
		static_cast<LinuxTextToSpeechManager *> (g_system->getTextToSpeechManager());
	manager->updateState(LinuxTextToSpeechManager::SPEECH_PAUSED);
}

LinuxTextToSpeechManager::LinuxTextToSpeechManager()
	: _speechState(READY) {
	init();
}

void LinuxTextToSpeechManager::init() {
	_connection = spd_open("ScummVM", "main", NULL, SPD_MODE_THREADED);
	if (_connection == 0) {
		_speechState = BROKEN;
		warning("Couldn't initialize text to speech through speech-dispatcher");
		return;
	}
	_lastSaid = "";

	_connection->callback_begin = speech_begin_callback;
	spd_set_notification_on(_connection, SPD_BEGIN);
	_connection->callback_end = speech_end_callback;
	spd_set_notification_on(_connection, SPD_END);
	_connection->callback_cancel = speech_cancel_callback;
	spd_set_notification_on(_connection, SPD_CANCEL);
	_connection->callback_resume = speech_resume_callback;
	spd_set_notification_on(_connection, SPD_RESUME);
	_connection->callback_pause = speech_pause_callback;
	spd_set_notification_on(_connection, SPD_PAUSE);

	updateVoices();
	_ttsState->_activeVoice = 0;
#ifdef USE_TRANSLATION
	setLanguage(TransMan.getCurrentLanguage());
#else
	setLanguage("en");
#endif
	_speechQueue.clear();
}

LinuxTextToSpeechManager::~LinuxTextToSpeechManager() {
	if (_connection != 0)
		spd_close(_connection);
}

void LinuxTextToSpeechManager::updateState(LinuxTextToSpeechManager::SpeechEvent event) {
	if (_speechState == BROKEN)
		return;
	switch(event) {
	case SPEECH_ENDED:
		_speechQueue.pop_front();
		if (_speechQueue.size() == 0)
			_speechState = READY;
		break;
	case SPEECH_PAUSED:
		_speechState = PAUSED;
		break;
	case SPEECH_CANCELED:
		if (_speechState != PAUSED) {
			_speechState = READY;
		}
		break;
	case SPEECH_RESUMED:
		break;
	case SPEECH_BEGUN:
		_speechState = SPEAKING;
		break;
	}
}

Common::String LinuxTextToSpeechManager::strToUtf8(Common::String str, Common::String charset) {
#if SDL_VERSION_ATLEAST(2, 0, 0)

	char *conv_text = SDL_iconv_string("UTF-8", charset.c_str(), str.c_str(), str.size() + 1);
	Common::String result;
	if (conv_text) {
		result = conv_text;
		SDL_free(conv_text);
	} else if (charset != "ASCII"){
		warning("Could not convert text from %s to UTF-8, trying ASCII", charset.c_str());
		return strToUtf8(str, "ASCII");
	} else
		warning("Could not convert text to UTF-8");

	return result;
#else
	return Common::String();
#endif
}

bool LinuxTextToSpeechManager::say(Common::String str, Action action, Common::String charset) {
	if (_speechState == BROKEN)
		return true;

	if (action == DROP && isSpeaking())
		return true;

	if (action == INTERRUPT_NO_REPEAT && _lastSaid == str && isSpeaking())
		return true;

	if (action == QUEUE_NO_REPEAT && _lastSaid == str && isSpeaking())
		return true;
	
	if (charset.empty()) {
#ifdef USE_TRANSLATION
		charset = TransMan.getCurrentCharset();
#else
		charset = "ASCII";
#endif
	}

	str = strToUtf8(str, charset);

	if (isSpeaking() && (action == INTERRUPT || action == INTERRUPT_NO_REPEAT))
		stop();
	if (!str.empty()) {
		_speechState = SPEAKING;
		_speechQueue.push_back(str);
		_lastSaid = str;
		if(spd_say(_connection, SPD_MESSAGE, str.c_str()) == -1) {
			//restart the connection
			if (_connection != 0)
				spd_close(_connection);
			init();
			return true;
		}
	}

	return false;
}

bool LinuxTextToSpeechManager::stop() {
	if (_speechState == READY || _speechState == BROKEN)
		return true;
	_speechState = READY;
	_speechQueue.clear();
	return spd_cancel(_connection) == -1;
}

bool LinuxTextToSpeechManager::pause() {
	if (_speechState == READY || _speechState == PAUSED || _speechState == BROKEN)
		return true;
	_speechState = PAUSED;
	bool result = spd_cancel_all(_connection) == -1;
	if (result)
		return true;
	if (result)
		return true;
	return false;
}

bool LinuxTextToSpeechManager::resume() {
	if (_speechState == READY || _speechState == SPEAKING || _speechState == BROKEN)
		return true;
	if (_speechQueue.size()) {
		_speechState = SPEAKING;
		for (Common::List<Common::String>::iterator i = _speechQueue.begin(); i != _speechQueue.end(); i++) {
			if (spd_say(_connection, SPD_MESSAGE, i->c_str()) == -1) {
				if (_connection != 0)
					spd_close(_connection);
				init();
				return true;
			}
		}
	}
	else
		_speechState = READY;
	return false;
}

bool LinuxTextToSpeechManager::isSpeaking() {
	return _speechState == SPEAKING;
}

bool LinuxTextToSpeechManager::isPaused() {
	return _speechState == PAUSED;
}

bool LinuxTextToSpeechManager::isReady() {
	return _speechState == READY;
}

void LinuxTextToSpeechManager::setVoice(unsigned index) {
	if (_speechState == BROKEN)
		return;
	assert(index < _ttsState->_availableVoices.size());
	Common::TTSVoice voice = _ttsState->_availableVoices[index];
	spd_set_voice_type(_connection, *(SPDVoiceType *)(voice.getData()));
	_ttsState->_activeVoice = index;
}

void LinuxTextToSpeechManager::setRate(int rate) {
	if (_speechState == BROKEN)
		return;
	assert(rate >= -100 && rate <= 100);
	spd_set_voice_rate(_connection, rate);
	_ttsState->_rate = rate;
}

void LinuxTextToSpeechManager::setPitch(int pitch) {
	if (_speechState == BROKEN)
		return;
	assert(pitch >= -100 && pitch <= 100);
	spd_set_voice_pitch(_connection, pitch);
	_ttsState->_pitch = pitch;
}

void LinuxTextToSpeechManager::setVolume(unsigned volume) {
	if (_speechState == BROKEN)
		return;
	assert(volume <= 100);
	spd_set_volume(_connection, (volume - 50) * 2);
	_ttsState->_volume = volume;
}

void LinuxTextToSpeechManager::setLanguage(Common::String language) {
	if (_speechState == BROKEN)
		return;
	spd_set_language(_connection, language.c_str());
	_ttsState->_language = language;
	setVoice(_ttsState->_activeVoice);
}

void LinuxTextToSpeechManager::createVoice(int typeNumber, Common::TTSVoice::Gender gender, Common::TTSVoice::Age age, char *description) {
	SPDVoiceType *type = (SPDVoiceType *) malloc(sizeof(SPDVoiceType));
	*type = static_cast<SPDVoiceType>(typeNumber);
	Common::TTSVoice voice(gender, age, (void *) type, description);
	_ttsState->_availableVoices.push_back(voice);
}

void LinuxTextToSpeechManager::updateVoices() {
	if (_speechState == BROKEN)
		return;
	/* just use these voices:
	   SPD_MALE1, SPD_MALE2, SPD_MALE3,
	   SPD_FEMALE1, SPD_FEMALE2, SPD_FEMALE3,
	   SPD_CHILD_MALE, SPD_CHILD_FEMALE

	   it depends on the user to map them to the right voices in speech-dispatcher
	   configuration
	*/
	_ttsState->_availableVoices.clear();

	char **voiceInfo = spd_list_voices(_connection);

	createVoice(SPD_MALE1, Common::TTSVoice::MALE, Common::TTSVoice::ADULT, voiceInfo[0]);
	createVoice(SPD_MALE2, Common::TTSVoice::MALE, Common::TTSVoice::ADULT, voiceInfo[1]);
	createVoice(SPD_MALE3, Common::TTSVoice::MALE, Common::TTSVoice::ADULT, voiceInfo[2]);
	createVoice(SPD_FEMALE1, Common::TTSVoice::FEMALE, Common::TTSVoice::ADULT, voiceInfo[3]);
	createVoice(SPD_FEMALE2, Common::TTSVoice::FEMALE, Common::TTSVoice::ADULT, voiceInfo[4]);
	createVoice(SPD_FEMALE3, Common::TTSVoice::FEMALE, Common::TTSVoice::ADULT, voiceInfo[5]);
	createVoice(SPD_CHILD_MALE, Common::TTSVoice::MALE, Common::TTSVoice::CHILD, voiceInfo[6]);
	createVoice(SPD_CHILD_FEMALE, Common::TTSVoice::FEMALE, Common::TTSVoice::CHILD, voiceInfo[7]);

	for (int i = 0; i < 8; i++)
		free(voiceInfo[i]);

	free(voiceInfo);
}

void LinuxTextToSpeechManager::freeVoiceData(void *data) {
	free(data);
}


#endif
