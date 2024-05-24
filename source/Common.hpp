///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus.hpp>
#include <Langulus/Input.hpp>

using namespace Langulus;

struct InputSDL;
struct InputGatherer;
struct InputListener;

#define VERBOSE_INPUT(...)      Logger::Verbose(Self(), __VA_ARGS__)
#define VERBOSE_INPUT_TAB(...)  const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})

/// Include SDL                                                               
#include <SDL3/SDL.h>
