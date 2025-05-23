/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: vantorVersion.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <string>

namespace vantor::Core::version
{

    // return: const int major
    int GetMajor();
    // return: const int minor
    int GetMinor();
    // return: const int revision
    int GetRevision();

    std::string GetVersionString();
} // namespace vantor::Core::version