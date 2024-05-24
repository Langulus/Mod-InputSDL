///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "InputSDL.hpp"
#include <Flow/Verbs/Interact.hpp>
#include <Entity/Event.hpp>
#include <Math/Vector.hpp>

LANGULUS_DEFINE_MODULE(
   InputSDL, 0, "InputSDL",
   "Raw input module, using SDL as backend", "",
   InputSDL, InputGatherer, InputListener
)


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
InputSDL::InputSDL(Runtime* runtime, const Neat&)
   : Resolvable{this}
   , A::Module {runtime}
   , mGatherers{this} {
   VERBOSE_INPUT("Initializing...");

   // Initialize SDL for input                                          
   // FTXUI handles input events, but only in the context of the        
   // console window. If we want direct mouse state access for free     
   // look, or joystick support, we need SDL for it.                    
   LANGULUS_ASSERT(SDL_Init(SDL_INIT_GAMEPAD) >= 0, Construct,
      "SDL failed to initialize - no input will be available. SDL_Error: ",
      SDL_GetError()
   );

   // Create an invisible window so that we can capture and track       
   // the global mouse                                                  
   mInputFocus = SDL_CreateWindow(
      "Game Input Handle", 1, 1,
      SDL_WINDOW_BORDERLESS|SDL_WINDOW_INPUT_FOCUS);
   LANGULUS_ASSERT(mInputFocus, Construct,
      "SDL failed to create input window. SDL_Error: ",
      SDL_GetError()
   );

   LANGULUS_ASSERT(SDL_SetRelativeMouseMode(true) >= 0, Construct,
      "SDL failed to set relative mouse mode. SDL_Error: ",
      SDL_GetError()
   );

   VERBOSE_INPUT("Initialized");
}

///                                                                           
InputSDL::~InputSDL() {
   if (mInputFocus)
      SDL_DestroyWindow(mInputFocus);
   SDL_Quit();
}

/// Module update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the UI requested exit                                  
bool InputSDL::Update(Time deltaTime) {
   // Gather inputs                                                     
   Math::Vec2f relativeMouse;
   auto buttonState = SDL_GetRelativeMouseState(&relativeMouse.x, &relativeMouse.y);
   Verbs::Interact interact {Events::MouseMove {relativeMouse}};
   GetRuntime()->GetOwner()->Run(interact);

   // Handle events on the SDL queue                                    
   SDL_Event e;
   while (SDL_PollEvent(&e) != 0) {
      // User requests quit                                             
      switch (e.type) {
      case SDL_EVENT_QUIT:
         Logger::Input("Quit");
         break;
      case SDL_EVENT_JOYSTICK_ADDED:
         Logger::Input("Joystick added");
         break;
      case SDL_EVENT_CLIPBOARD_UPDATE:
         Logger::Input("Clipboard change detected");
         break;
      case SDL_EVENT_GAMEPAD_ADDED:
         Logger::Input("Gamepad added");
         break;
      }
   }

   // Update all GUI systems                                            
   for (auto& gatherer : mGatherers)
      gatherer.Update(deltaTime);
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void InputSDL::Create(Verb& verb) {
   mGatherers.Create(verb);
}