///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "InputSDL.hpp"

LANGULUS_DEFINE_MODULE(
   InputSDL, 0, "InputSDL",
   "Raw input module, using SDL as backend - "
   "allows for raw mouse/joystick/keyboard inputs even on console applications, "
   "by using an external window", "",
   InputSDL, InputGatherer, InputListener
)


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
InputSDL::InputSDL(Runtime* runtime, const Neat&)
   : Resolvable{this}
   , A::Module {runtime}
   , mGatherers{this} {
   // Initialize SDL for input                                          
   VERBOSE_INPUT("Initializing...");
   LANGULUS_ASSERT(SDL_Init(SDL_INIT_GAMEPAD) >= 0, Construct,
      "SDL failed to initialize - no input will be available. SDL_Error: ",
      SDL_GetError()
   );
   VERBOSE_INPUT("Initialized");
}

///                                                                           
InputSDL::~InputSDL() {
   SDL_Quit();
}

/// Module update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the UI requested exit                                  
bool InputSDL::Update(Time deltaTime) {
   // Gather inputs from global SDL events                              
   // Note: SDL_GetRelativeMouseState works only if there's an SDL      
   // window with focus.                                                
   Math::Vec2f relativeMouse;
   auto buttonState = SDL_GetRelativeMouseState(&relativeMouse.x, &relativeMouse.y);

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