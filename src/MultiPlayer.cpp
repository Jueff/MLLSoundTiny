/*
 * SPDX-FileCopyrightText: 2025-2026 Juergen Winkler <MobaLedLib@gmx.at>
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @brief MultiPlayer class for controlling sound modules.
 *
 * This class provides an interface to send commands to different sound modules,
 * such as MP3-TF-16P and JQ6500, over a serial stream. It handles command mapping,
 * checksum calculation, and module type switching.
 *
 * Diese Klasse bietet eine Schnittstelle zum Senden von Befehlen an verschiedene
 * Sound-Module, wie MP3-TF-16P und JQ6500, über einen seriellen Stream. Sie handhabt
 * Befehls-Mapping, Prüfsummenberechnung und Modul-Typ-Wechsel.
*/

#include "MultiPlayer.h"
#include "MllSoundTiny.h"

uint16_t MultiPlayer::calculateCheckSum(uint8_t *buffer)
{
  uint16_t sum = 0;
  for (int i=Stack_Version; i<Stack_CheckSum; i++) 
  {
    sum += buffer[i];
  }
  return -sum;
}

void MultiPlayer::disableACK()
{
  _sending[Stack_ACK] = 0x00;
}

bool MultiPlayer::begin(Stream &stream)
{
  _serial = &stream;
  
  disableACK();
  return true;
}

void MultiPlayer::sendMP3TF16P() 
{
  write(_sending, PLAYER_SEND_LENGTH);
  _timeOutTimer = millis();
  _isSending = _sending[Stack_ACK];
  delay(10);
}

void MultiPlayer::sendMP3TF16P(uint8_t command)
{
  sendMP3TF16P(command, 0);
}

void MultiPlayer::sendMP3TF16P(uint8_t command, uint16_t argument)
{
  MLLST_LOG(1, "%2d: MP3-TF-16p command: %d '%s' arg %d\n", _identity, command, toCommandText(command), argument);
  _sending[Stack_Command] = command;
  uint16ToArray(argument, _sending+Stack_Parameter);
  uint16ToArray(calculateCheckSum(_sending), _sending+Stack_CheckSum);
  sendMP3TF16P();
}

void MultiPlayer::sendMP3TF16P(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow)
{
  MLLST_LOG(1, "%2d: MP3-TF-16p command: %d '%s' msb: %d lsb: %d\n", _identity, command, toCommandText(command), argumentHigh, argumentLow);
  uint16_t buffer = argumentHigh;
  buffer <<= 8;
  sendMP3TF16P(command, buffer | argumentLow);
}

void MultiPlayer::uint16ToArray(uint16_t value, uint8_t *array){
  *array = (uint8_t)(value>>8);
  *(array+1) = (uint8_t)(value);
}

void MultiPlayer::setModuleType(uint8_t moduleType) 
{
  if (_currentType != moduleType)
  {
    _currentType = moduleType;
    MLLST_LOG(1, "%2d: Module type changed to: %d\n", _identity, _currentType);
  }
}

void MultiPlayer::setIdentity(uint8_t identity)
{
  _identity = identity;
}

void MultiPlayer::sendCmd(uint8_t command) 
{
  switch (_currentType) {
    case MODULE_MP3_TF_16P:
      sendMP3TF16P(command);
      break;
    case MODULE_JQ6500:
      sendJQ(command);
      break;
    default: unknownModuleType(_currentType, command);
  }
}

void MultiPlayer::sendCmd(uint8_t command, uint8_t argument)
{
  switch (_currentType) {
    case MODULE_MP3_TF_16P:
      sendMP3TF16P(command, argument);
      break;
    case MODULE_JQ6500:
      sendJQ(command, argument);
      break;
    default: unknownModuleType(_currentType, command);
  }
}

void MultiPlayer::sendCmd(uint8_t command, uint16_t argument) 
{
  switch (_currentType) {
    case MODULE_MP3_TF_16P:
      sendMP3TF16P(command, argument);
      break;
    case MODULE_JQ6500:
      sendJQ(command, argument);
      break;
    default: unknownModuleType(_currentType, command);
  }
}

void MultiPlayer::sendCmd(uint8_t command, uint8_t argumentHigh, uint8_t argumentLow) 
{
  switch (_currentType) {
    case MODULE_MP3_TF_16P:
      sendMP3TF16P(command, argumentHigh, argumentLow);
      break;
    case MODULE_JQ6500:
      //sendJQ(command, argumentHigh, argumentLow);
      break;
    default: unknownModuleType(_currentType, command);
  }
}

void MultiPlayer::sendJQ(uint8_t command) 
{
  MLLST_LOG(1, "%2d: JQ6500 cmd %d '%s'\n", _identity, command, toCommandText(command));

  command = mapJQ6500Command(command);
  _sendJQ[1] = 2;
  _sendJQ[2] = command;
  _sendJQ[3] = 0xef;
  write(_sendJQ, 4);
}

uint8_t MultiPlayer::mapJQ6500Command(uint8_t command)
{
  if (command == PLAYER_PLAY_MP3)
  {
    // map to normal play command
    command = PLAYER_PLAY_TRACK;
  }
  return command;
}

void MultiPlayer::sendJQ(uint8_t command, uint8_t argument) 
{
  MLLST_LOG(1, "%2d: JQ6500 cmd %d '%s' arg %d\n", _identity, command, toCommandText(command), argument);

  command = mapJQ6500Command(command);
  _sendJQ[1] = 3;
  _sendJQ[2] = command;
  _sendJQ[3] = argument;
  _sendJQ[4] = 0xef;
  write(_sendJQ, 5);
}

void MultiPlayer::sendJQ(uint8_t command, uint16_t argument) 
{
  MLLST_LOG(1, "%2d: JQ6500 cmd %d '%s' larg %d\n", _identity, command, toCommandText(command), argument);
  command = mapJQ6500Command(command);
  _sendJQ[1] = 4;
  _sendJQ[2] = command;
  _sendJQ[3] = (uint8_t) (argument>>8);
  _sendJQ[4] = (uint8_t) argument;
  _sendJQ[5] = 0xef;
  write(_sendJQ, 6);
}

size_t MultiPlayer::write(const uint8_t* buffer, size_t size)
{
  MLLST_IS_LOG(4) debugOutput(buffer, size);
  return _serial->write(buffer, size);
}

void MultiPlayer::debugOutput(const uint8_t* data, uint8_t dataLength)
{
  Serial.print("SEND DATA: ");
  for (int i = 0; i < dataLength; i++)
  {
    Serial.printf("%2x ", (uint8_t)data[i]);
  }
  Serial.println();
}

void MultiPlayer::unknownModuleType(uint8_t moduleType, uint8_t command)
{
  MLLST_LOG(1, "Unknown module type: %d for command: %d '%s'\n", moduleType, command, toCommandText(command));
}

const char* MultiPlayer::toCommandText(uint8_t command)
{
  switch (command)
  {
    case PLAYER_NO_COMMAND: return "No command";
    case PLAYER_NEXT: return "Next track";
    case PLAYER_PREV: return "Previous track";
    case PLAYER_VOLUME_INCREASE: return "Volume increase";
    case PLAYER_VOLUME_DECREASE: return "Volume decrease";
    case PLAYER_STANDBY: return "Standby / Sleep";
    case PLAYER_RESET: return "Reset device";
    case PLAYER_PLAYBACK: return "Play";
    case PLAYER_PAUSE: return "Pause";
    case PLAYER_STOP_ADVERTISEMENT: return "Stop advertisement";
    case PLAYER_STOP: return "Stop playback";
    case PLAYER_NORMAL: return "Normal mode";
    case PLAYER_SET_RANDOM: return "Set random playback";

    case PLAYER_SET_VOLUME: return "Set volume";
    case PLAYER_EQ: return "Set equalizer";
    case PLAYER_SOURCE: return "Set source";
    case PLAYER_REPEAT: return "Set repeat";
    case PLAYER_SET_DAC: return "Set DAC";
    case PLAYER_SET_REPEAT_CURRENT: return "Set repeat current track";

    case PLAYER_PLAY_TRACK: return "Play track (16-bit)";
    case PLAYER_SINGLE_REPEAT: return "Single repeat mode";
    case PLAYER_PLAY_MP3: return "Play MP3 (16-bit)";
    case PLAYER_ADVERTISEMENT: return "Insert advertisement";
    case PLAYER_FOLDER_REPEAT: return "Folder repeat";

    case PLAYER_PLAY_FOLDER: return "Play folder track";
    case PLAYER_ADJUST_VOLUME: return "Adjust volume/amplification";

    case PLAYER_PLAY_FOLDER_TRACK: return "Play folder/track (4+12 bit)";

    case PLAYER_PLAY_TRACK_ON: return "Play track on module";
    case PLAYER_PLAY_MP3_ON: return "Play MP3 on module";
    case PLAYER_PLAY_TRACK_REPEAT_ON: return "Play track on module (repeat)";

    case PLAYER_SET_MODULE_TYPE: return "Set module type";
    case PLAYER_SET_ACTIVE_MODULE: return "Set active module";

    default: return "unknown command";
  }
}