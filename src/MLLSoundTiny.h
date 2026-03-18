/*
 * SPDX-FileCopyrightText: 2025-2026 Juergen Winkler <MobaLedLib@gmx.at>
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
*/

#pragma once
#include <Arduino.h>

#define MLLST_LOG(level,...) if (MLLSoundTiny::LogLevel>=level) Serial.printf(__VA_ARGS__);

class MLLSoundTiny
{
public:
  static uint8_t LogLevel;
};
