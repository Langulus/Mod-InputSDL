///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus.hpp>
#include <Langulus/Input.hpp>
#include <Math/Vector.hpp>

using namespace Langulus;

struct InputSDL;
struct InputGatherer;
struct InputListener;

#if 0
   #define VERBOSE_INPUT(...)      Logger::Verbose(Self(), __VA_ARGS__)
   #define VERBOSE_INPUT_TAB(...)  const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})
#else
   #define VERBOSE_INPUT(...)      LANGULUS(NOOP)
   #define VERBOSE_INPUT_TAB(...)  LANGULUS(NOOP)
#endif

/// Include SDL                                                               
#include <SDL3/SDL.h>
