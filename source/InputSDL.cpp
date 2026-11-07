///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "InputSDL.hpp"
#include <Langulus/Profiler.hpp>

LANGULUS_DEFINE_MODULE(
   InputSDL, 0, "InputSDL",
   "Raw input module, using SDL as backend - "
   "allows for raw mouse/joystick/keyboard inputs even on console applications, "
   "by using an external window", "",
   InputSDL, InputGatherer, InputListener, Anticipator
)


auto TranslateKey(SDL_Scancode) -> RTTI::DMeta;
auto TranslateMouse(Uint8) -> RTTI::DMeta;


/// Module construction                                                       
///   @param runtime the runtime that owns the module                         
///   @param descriptor instructions for configuring the module               
InputSDL::InputSDL(Runtime* runtime, Neat const&)
   : Downcastable {this}
   , Module       {runtime} {
   // Reflect all event tokens                                          
   Langulus::RegisterEvents();

   // Initialize SDL for input                                          
   VERBOSE_INPUT("Initializing...");
   LglsAssert(SDL_Init(SDL_INIT_GAMEPAD) >= 0,
      "SDL failed to initialize - no input will be available. SDL_Error: ",
      SDL_GetError()
   );
   VERBOSE_INPUT("Initialized");
}

///                                                                           
InputSDL::~InputSDL() {
   mEventQueue.Reset();
   mGatherers.Reset();

   SDL_Quit();
}

/// First stage destruction                                                   
void InputSDL::Teardown() {
   mGatherers.Teardown();
}

/// Module update routine                                                     
///   @param deltaTime time between updates                                   
///   @return false if the UI requested exit                                  
bool InputSDL::Update(Time deltaTime) {
   LglsProfile();
   Math::Vec2f mouseMovement;
   Math::Vec2f mouseScroll;
   SDL_Event e;

   while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_EVENT_QUIT:
         // User requests quit                                          
         return false;
      case SDL_EVENT_JOYSTICK_AXIS_MOTION:
         VERBOSE_INPUT("Joystick axis motion");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_BALL_MOTION:
         VERBOSE_INPUT("Joystick ball motion");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
         VERBOSE_INPUT("Joystick button down");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_BUTTON_UP:
         VERBOSE_INPUT("Joystick button up");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_HAT_MOTION:
         VERBOSE_INPUT("Joystick hat motion");
         TODO();
         break;
      case SDL_EVENT_CLIPBOARD_UPDATE:
         VERBOSE_INPUT("Clipboard change detected");
         TODO();
         break;
      case SDL_EVENT_MOUSE_MOTION:
         // Mouse moved                                                 
         mouseMovement.x += e.motion.xrel;
         mouseMovement.y += e.motion.yrel;
         break;
      case SDL_EVENT_MOUSE_WHEEL:
         // Mouse scrolled                                              
         mouseScroll.x += e.wheel.x;
         mouseScroll.y += e.wheel.y;
         break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
         // Mouse key was pressed                                       
         auto newEvent = Event::From(
            TranslateMouse(e.button.button),
            EventState::Begin
         );
         VERBOSE_INPUT("Mouse button pressed: ", newEvent.GetEvent());
         PushEvent(newEvent);
         break;
      }
      case SDL_EVENT_MOUSE_BUTTON_UP: {
         // Mouse key was released                                      
         auto newEvent = Event::From(
            TranslateMouse(e.button.button),
            EventState::End
         );
         VERBOSE_INPUT("Mouse button released: ", newEvent.GetEvent());
         PushEvent(newEvent);
         break;
      }
      case SDL_EVENT_WINDOW_FOCUS_LOST: {
         // Input focus lost - pause game, etc.?                        
         VERBOSE_INPUT("Focus lost");
         PushEvent(Events::WindowUnfocus {});
         break;
      }
      case SDL_EVENT_WINDOW_FOCUS_GAINED: {
         // Input focus gained - resume game?                           
         VERBOSE_INPUT("Focus gained");
         PushEvent(Events::WindowFocus {});
         break;
      }
      case SDL_EVENT_KEY_DOWN: {
         // Keyboard key was pressed down                               
         auto newEvent = Event::From(
            TranslateKey(e.key.scancode),
            EventState::Begin
         );
         VERBOSE_INPUT("Keyboard button pressed: ", newEvent.GetEvent());
         PushEvent(newEvent);
         break;
      }
      case SDL_EVENT_KEY_UP: {
         // Keyboard key was released                                   
         auto newEvent = Event::From(
            TranslateKey(e.key.scancode),
            EventState::End
         );
         VERBOSE_INPUT("Keyboard button released: ", newEvent.GetEvent());
         PushEvent(newEvent);
         break;
      }}
   }

   // Dispatch gathered mouse movement events                           
   if (mouseMovement)
      PushEvent(Events::MouseMove   {EventState::Point, mouseMovement});
   
   // Dispatch gathered mouse scroll events                             
   if (mouseScroll)
      PushEvent(Events::MouseScroll {EventState::Point, mouseScroll});

   // Update all gatherers                                              
   for (auto& gatherer : mGatherers)
      gatherer.Update(deltaTime, mEventQueue);

   mEventQueue.Clear();
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb the creation/destruction verb                               
void InputSDL::Create(Verb& verb) {
   mGatherers.Create(this, verb);
}

/// Push a global event, that will be propagated to all gatherers             
///   @param e event to push                                                  
void InputSDL::PushEvent(Event const& e) {
   const auto signature = EventSignature {e.GetEvent(), e.GetEventState()};
   auto found = mEventQueue.Find(signature);
   if (found) {
      // Event-state pair already exists, merge payload           
      found.GetVal().Concat(e.GetPayload());
   }
   else {
      // Event-state pair doesn't exist yet                       
      mEventQueue.Merge(signature, e);
   }
}

/// SDL3 keyboard event -> Langulus event translator                          
///   @param i the code to translate                                          
///   @return the translated event                                            
auto TranslateKey(SDL_Scancode i) -> RTTI::DMeta {
   switch (i) {
   case SDL_SCANCODE_A:             return MetaDataOf<Keys::A>();
   case SDL_SCANCODE_B:             return MetaDataOf<Keys::B>();
   case SDL_SCANCODE_C:             return MetaDataOf<Keys::C>();
   case SDL_SCANCODE_D:             return MetaDataOf<Keys::D>();
   case SDL_SCANCODE_E:             return MetaDataOf<Keys::E>();
   case SDL_SCANCODE_F:             return MetaDataOf<Keys::F>();
   case SDL_SCANCODE_G:             return MetaDataOf<Keys::G>();
   case SDL_SCANCODE_H:             return MetaDataOf<Keys::H>();
   case SDL_SCANCODE_I:             return MetaDataOf<Keys::I>();
   case SDL_SCANCODE_J:             return MetaDataOf<Keys::J>();
   case SDL_SCANCODE_K:             return MetaDataOf<Keys::K>();
   case SDL_SCANCODE_L:             return MetaDataOf<Keys::L>();
   case SDL_SCANCODE_M:             return MetaDataOf<Keys::M>();
   case SDL_SCANCODE_N:             return MetaDataOf<Keys::N>();
   case SDL_SCANCODE_O:             return MetaDataOf<Keys::O>();
   case SDL_SCANCODE_P:             return MetaDataOf<Keys::P>();
   case SDL_SCANCODE_Q:             return MetaDataOf<Keys::Q>();
   case SDL_SCANCODE_R:             return MetaDataOf<Keys::R>();
   case SDL_SCANCODE_S:             return MetaDataOf<Keys::S>();
   case SDL_SCANCODE_T:             return MetaDataOf<Keys::T>();
   case SDL_SCANCODE_U:             return MetaDataOf<Keys::U>();
   case SDL_SCANCODE_V:             return MetaDataOf<Keys::V>();
   case SDL_SCANCODE_W:             return MetaDataOf<Keys::W>();
   case SDL_SCANCODE_X:             return MetaDataOf<Keys::X>();
   case SDL_SCANCODE_Y:             return MetaDataOf<Keys::Y>();
   case SDL_SCANCODE_Z:             return MetaDataOf<Keys::Z>();

   case SDL_SCANCODE_1:             return MetaDataOf<Keys::Main1>();
   case SDL_SCANCODE_2:             return MetaDataOf<Keys::Main2>();
   case SDL_SCANCODE_3:             return MetaDataOf<Keys::Main3>();
   case SDL_SCANCODE_4:             return MetaDataOf<Keys::Main4>();
   case SDL_SCANCODE_5:             return MetaDataOf<Keys::Main5>();
   case SDL_SCANCODE_6:             return MetaDataOf<Keys::Main6>();
   case SDL_SCANCODE_7:             return MetaDataOf<Keys::Main7>();
   case SDL_SCANCODE_8:             return MetaDataOf<Keys::Main8>();
   case SDL_SCANCODE_9:             return MetaDataOf<Keys::Main9>();
   case SDL_SCANCODE_0:             return MetaDataOf<Keys::Main0>();

   case SDL_SCANCODE_RETURN:        return MetaDataOf<Keys::Enter>(); 
   case SDL_SCANCODE_ESCAPE:        return MetaDataOf<Keys::Escape>();
   case SDL_SCANCODE_BACKSPACE:     return MetaDataOf<Keys::Back>();  
   case SDL_SCANCODE_TAB:           return MetaDataOf<Keys::Tab>();   
   case SDL_SCANCODE_SPACE:         return MetaDataOf<Keys::Space>(); 
   case SDL_SCANCODE_MINUS:         return MetaDataOf<Keys::Minus>(); 
   case SDL_SCANCODE_EQUALS:        TODO(); return {}; //missing
   case SDL_SCANCODE_LEFTBRACKET:   return MetaDataOf<Keys::LeftBracket>(); 
   case SDL_SCANCODE_RIGHTBRACKET:  return MetaDataOf<Keys::RightBracket>();
   case SDL_SCANCODE_BACKSLASH:
   case SDL_SCANCODE_NONUSHASH:     return MetaDataOf<Keys::Hack>();           
   case SDL_SCANCODE_SEMICOLON:     return MetaDataOf<Keys::Semicolon>();      
   case SDL_SCANCODE_APOSTROPHE:    return MetaDataOf<Keys::Apostrophe>();     
   case SDL_SCANCODE_GRAVE:         return MetaDataOf<Keys::Tilde>();          
   case SDL_SCANCODE_COMMA:         return MetaDataOf<Keys::Comma>();          
   case SDL_SCANCODE_PERIOD:        return MetaDataOf<Keys::Period>();         
   case SDL_SCANCODE_SLASH:         return MetaDataOf<Keys::Slash>();          

   case SDL_SCANCODE_CAPSLOCK:      return MetaDataOf<Keys::CapsLock>();       

   case SDL_SCANCODE_F1:            return MetaDataOf<Keys::F1>();             
   case SDL_SCANCODE_F2:            return MetaDataOf<Keys::F2>();             
   case SDL_SCANCODE_F3:            return MetaDataOf<Keys::F3>();             
   case SDL_SCANCODE_F4:            return MetaDataOf<Keys::F4>();             
   case SDL_SCANCODE_F5:            return MetaDataOf<Keys::F5>();             
   case SDL_SCANCODE_F6:            return MetaDataOf<Keys::F6>();             
   case SDL_SCANCODE_F7:            return MetaDataOf<Keys::F7>();             
   case SDL_SCANCODE_F8:            return MetaDataOf<Keys::F8>();             
   case SDL_SCANCODE_F9:            return MetaDataOf<Keys::F9>();             
   case SDL_SCANCODE_F10:           return MetaDataOf<Keys::F10>();            
   case SDL_SCANCODE_F11:           return MetaDataOf<Keys::F11>();            
   case SDL_SCANCODE_F12:           return MetaDataOf<Keys::F12>();            

   case SDL_SCANCODE_PRINTSCREEN:   return MetaDataOf<Keys::Print>();          
   case SDL_SCANCODE_SCROLLLOCK:    return MetaDataOf<Keys::ScrollLock>();     
   case SDL_SCANCODE_PAUSE:         return MetaDataOf<Keys::Pause>();          
   case SDL_SCANCODE_INSERT:        return MetaDataOf<Keys::Insert>();         
   case SDL_SCANCODE_HOME:          return MetaDataOf<Keys::Home>();           
   case SDL_SCANCODE_PAGEUP:        return MetaDataOf<Keys::PageUp>();         
   case SDL_SCANCODE_DELETE:        return MetaDataOf<Keys::Delete>();         
   case SDL_SCANCODE_END:           return MetaDataOf<Keys::End>();            
   case SDL_SCANCODE_PAGEDOWN:      return MetaDataOf<Keys::PageDown>();       
   case SDL_SCANCODE_RIGHT:         return MetaDataOf<Keys::Right>();          
   case SDL_SCANCODE_LEFT:          return MetaDataOf<Keys::Left>();           
   case SDL_SCANCODE_DOWN:          return MetaDataOf<Keys::Down>();           
   case SDL_SCANCODE_UP:            return MetaDataOf<Keys::Up>();             

   case SDL_SCANCODE_NUMLOCKCLEAR:  return MetaDataOf<Keys::NumLock>();        
   case SDL_SCANCODE_KP_DIVIDE:     return MetaDataOf<Keys::NumpadDivide>();   
   case SDL_SCANCODE_KP_MULTIPLY:   return MetaDataOf<Keys::NumpadMultiply>(); 
   case SDL_SCANCODE_KP_MINUS:      return MetaDataOf<Keys::NumpadSubtract>(); 
   case SDL_SCANCODE_KP_PLUS:       return MetaDataOf<Keys::NumpadAdd>();      
   case SDL_SCANCODE_KP_ENTER:      return MetaDataOf<Keys::NumpadEnter>();    
   case SDL_SCANCODE_KP_1:          return MetaDataOf<Keys::Numpad1>();        
   case SDL_SCANCODE_KP_2:          return MetaDataOf<Keys::Numpad2>();        
   case SDL_SCANCODE_KP_3:          return MetaDataOf<Keys::Numpad3>();        
   case SDL_SCANCODE_KP_4:          return MetaDataOf<Keys::Numpad4>();        
   case SDL_SCANCODE_KP_5:          return MetaDataOf<Keys::Numpad5>();        
   case SDL_SCANCODE_KP_6:          return MetaDataOf<Keys::Numpad6>();        
   case SDL_SCANCODE_KP_7:          return MetaDataOf<Keys::Numpad7>();        
   case SDL_SCANCODE_KP_8:          return MetaDataOf<Keys::Numpad8>();        
   case SDL_SCANCODE_KP_9:          return MetaDataOf<Keys::Numpad9>();        
   case SDL_SCANCODE_KP_0:          return MetaDataOf<Keys::Numpad0>();        
   case SDL_SCANCODE_KP_PERIOD:     return MetaDataOf<Keys::NumpadDecimal>();  

   case SDL_SCANCODE_NONUSBACKSLASH:return MetaDataOf<Keys::Hack>();           
   case SDL_SCANCODE_APPLICATION:   TODO(); return {}; //missing
   case SDL_SCANCODE_POWER:         TODO(); return {}; //missing
   case SDL_SCANCODE_KP_EQUALS:     return MetaDataOf<Keys::NumpadEqual>();    
   case SDL_SCANCODE_F13:           return MetaDataOf<Keys::F13>();            
   case SDL_SCANCODE_F14:           return MetaDataOf<Keys::F14>();            
   case SDL_SCANCODE_F15:           return MetaDataOf<Keys::F15>();            
   case SDL_SCANCODE_F16:           return MetaDataOf<Keys::F16>();            
   case SDL_SCANCODE_F17:           return MetaDataOf<Keys::F17>();            
   case SDL_SCANCODE_F18:           return MetaDataOf<Keys::F18>();            
   case SDL_SCANCODE_F19:           return MetaDataOf<Keys::F19>();            
   case SDL_SCANCODE_F20:           return MetaDataOf<Keys::F20>();            
   case SDL_SCANCODE_F21:           return MetaDataOf<Keys::F21>();            
   case SDL_SCANCODE_F22:           return MetaDataOf<Keys::F22>();            
   case SDL_SCANCODE_F23:           return MetaDataOf<Keys::F23>();            
   case SDL_SCANCODE_F24:           return MetaDataOf<Keys::F24>();            

   case SDL_SCANCODE_EXECUTE:       TODO(); return {}; //missing
   case SDL_SCANCODE_HELP:          TODO(); return {}; //missing
   case SDL_SCANCODE_MENU:          TODO(); return {}; //missing
   case SDL_SCANCODE_SELECT:        TODO(); return {}; //missing
   case SDL_SCANCODE_STOP:          TODO(); return {}; //missing
   case SDL_SCANCODE_AGAIN:         TODO(); return {}; //missing
   case SDL_SCANCODE_UNDO:          TODO(); return {}; //missing
   case SDL_SCANCODE_CUT:           TODO(); return {}; //missing
   case SDL_SCANCODE_COPY:          TODO(); return {}; //missing
   case SDL_SCANCODE_PASTE:         TODO(); return {}; //missing
   case SDL_SCANCODE_FIND:          TODO(); return {}; //missing
   case SDL_SCANCODE_MUTE:          TODO(); return {}; //missing
   case SDL_SCANCODE_VOLUMEUP:      TODO(); return {}; //missing
   case SDL_SCANCODE_VOLUMEDOWN:    TODO(); return {}; //missing
   case SDL_SCANCODE_KP_COMMA:      TODO(); return {}; //missing
   case SDL_SCANCODE_KP_EQUALSAS400:TODO(); return {}; //missing

   case SDL_SCANCODE_INTERNATIONAL1:TODO(); return {}; /**< used on Asian keyboards, see footnotes in USB doc */
   case SDL_SCANCODE_INTERNATIONAL2:TODO(); return {}; 
   case SDL_SCANCODE_INTERNATIONAL3:TODO(); return {}; /**< Yen */
   case SDL_SCANCODE_INTERNATIONAL4:TODO(); return {}; 
   case SDL_SCANCODE_INTERNATIONAL5:TODO(); return {}; 
   case SDL_SCANCODE_INTERNATIONAL6:TODO(); return {}; 
   case SDL_SCANCODE_INTERNATIONAL7:TODO(); return {}; 
   case SDL_SCANCODE_INTERNATIONAL8:TODO(); return {}; 
   case SDL_SCANCODE_INTERNATIONAL9:TODO(); return {}; 

   case SDL_SCANCODE_LANG1:         TODO(); return {}; /**< Hangul/English toggle */
   case SDL_SCANCODE_LANG2:         TODO(); return {}; /**< Hanja conversion */
   case SDL_SCANCODE_LANG3:         TODO(); return {}; /**< Katakana */
   case SDL_SCANCODE_LANG4:         TODO(); return {}; /**< Hiragana */
   case SDL_SCANCODE_LANG5:         TODO(); return {}; /**< Zenkaku/Hankaku */
   case SDL_SCANCODE_LANG6:         TODO(); return {}; /**< reserved */
   case SDL_SCANCODE_LANG7:         TODO(); return {}; /**< reserved */
   case SDL_SCANCODE_LANG8:         TODO(); return {}; /**< reserved */
   case SDL_SCANCODE_LANG9:         TODO(); return {}; /**< reserved */

   case SDL_SCANCODE_ALTERASE:      TODO(); return {}; /**< Erase-Eaze */
   case SDL_SCANCODE_SYSREQ:        TODO(); return {}; 
   case SDL_SCANCODE_CANCEL:        TODO(); return {}; /**< AC Cancel */
   case SDL_SCANCODE_CLEAR:         TODO(); return {}; 
   case SDL_SCANCODE_PRIOR:         TODO(); return {}; 
   case SDL_SCANCODE_RETURN2:       TODO(); return {}; 
   case SDL_SCANCODE_SEPARATOR:     TODO(); return {}; 
   case SDL_SCANCODE_OUT:           TODO(); return {}; 
   case SDL_SCANCODE_OPER:          TODO(); return {}; 
   case SDL_SCANCODE_CLEARAGAIN:    TODO(); return {}; 
   case SDL_SCANCODE_CRSEL:         TODO(); return {}; 
   case SDL_SCANCODE_EXSEL:         TODO(); return {}; 

   case SDL_SCANCODE_KP_00:               TODO(); return {}; 
   case SDL_SCANCODE_KP_000:              TODO(); return {}; 
   case SDL_SCANCODE_THOUSANDSSEPARATOR:  TODO(); return {}; 
   case SDL_SCANCODE_DECIMALSEPARATOR:    TODO(); return {}; 
   case SDL_SCANCODE_CURRENCYUNIT:        TODO(); return {}; 
   case SDL_SCANCODE_CURRENCYSUBUNIT:     TODO(); return {}; 
   case SDL_SCANCODE_KP_LEFTPAREN:        TODO(); return {}; 
   case SDL_SCANCODE_KP_RIGHTPAREN:       TODO(); return {}; 
   case SDL_SCANCODE_KP_LEFTBRACE:        TODO(); return {}; 
   case SDL_SCANCODE_KP_RIGHTBRACE:       TODO(); return {}; 
   case SDL_SCANCODE_KP_TAB:              TODO(); return {}; 
   case SDL_SCANCODE_KP_BACKSPACE:        TODO(); return {}; 
   case SDL_SCANCODE_KP_A:                TODO(); return {}; 
   case SDL_SCANCODE_KP_B:                TODO(); return {}; 
   case SDL_SCANCODE_KP_C:                TODO(); return {}; 
   case SDL_SCANCODE_KP_D:                TODO(); return {}; 
   case SDL_SCANCODE_KP_E:                TODO(); return {}; 
   case SDL_SCANCODE_KP_F:                TODO(); return {}; 
   case SDL_SCANCODE_KP_XOR:              TODO(); return {}; 
   case SDL_SCANCODE_KP_POWER:            TODO(); return {}; 
   case SDL_SCANCODE_KP_PERCENT:          TODO(); return {}; 
   case SDL_SCANCODE_KP_LESS:             TODO(); return {}; 
   case SDL_SCANCODE_KP_GREATER:          TODO(); return {}; 
   case SDL_SCANCODE_KP_AMPERSAND:        TODO(); return {}; 
   case SDL_SCANCODE_KP_DBLAMPERSAND:     TODO(); return {}; 
   case SDL_SCANCODE_KP_VERTICALBAR:      TODO(); return {}; 
   case SDL_SCANCODE_KP_DBLVERTICALBAR:   TODO(); return {}; 
   case SDL_SCANCODE_KP_COLON:            TODO(); return {}; 
   case SDL_SCANCODE_KP_HASH:             TODO(); return {}; 
   case SDL_SCANCODE_KP_SPACE:            TODO(); return {}; 
   case SDL_SCANCODE_KP_AT:               TODO(); return {}; 
   case SDL_SCANCODE_KP_EXCLAM:           TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMSTORE:         TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMRECALL:        TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMCLEAR:         TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMADD:           TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMSUBTRACT:      TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMMULTIPLY:      TODO(); return {}; 
   case SDL_SCANCODE_KP_MEMDIVIDE:        TODO(); return {}; 
   case SDL_SCANCODE_KP_PLUSMINUS:        TODO(); return {}; 
   case SDL_SCANCODE_KP_CLEAR:            TODO(); return {}; 
   case SDL_SCANCODE_KP_CLEARENTRY:       TODO(); return {}; 
   case SDL_SCANCODE_KP_BINARY:           TODO(); return {}; 
   case SDL_SCANCODE_KP_OCTAL:            TODO(); return {}; 
   case SDL_SCANCODE_KP_DECIMAL:          TODO(); return {}; 
   case SDL_SCANCODE_KP_HEXADECIMAL:      TODO(); return {}; 

   case SDL_SCANCODE_LCTRL:            return MetaDataOf<Keys::LeftControl>(); 
   case SDL_SCANCODE_LSHIFT:           return MetaDataOf<Keys::LeftShift>();   
   case SDL_SCANCODE_LALT:             return MetaDataOf<Keys::LeftAlt>();     
   case SDL_SCANCODE_LGUI:             return MetaDataOf<Keys::LeftGUI>();
   case SDL_SCANCODE_RCTRL:            return MetaDataOf<Keys::RightControl>();
   case SDL_SCANCODE_RSHIFT:           return MetaDataOf<Keys::RightShift>();  
   case SDL_SCANCODE_RALT:             return MetaDataOf<Keys::RightAlt>();    
   case SDL_SCANCODE_RGUI:             return MetaDataOf<Keys::RightGUI>();

   case SDL_SCANCODE_MODE:             TODO(); return {}; //missing

   case SDL_SCANCODE_MEDIA_NEXT_TRACK:       TODO(); return {}; // missing
   case SDL_SCANCODE_MEDIA_PREVIOUS_TRACK:   TODO(); return {}; // missing
   case SDL_SCANCODE_MEDIA_STOP:             TODO(); return {}; // missing
   case SDL_SCANCODE_MEDIA_PLAY:             TODO(); return {}; // missing
   case SDL_SCANCODE_MEDIA_SELECT:           TODO(); return {}; // missing
   case SDL_SCANCODE_MEDIA_REWIND:           TODO(); return {}; // missing
   case SDL_SCANCODE_MEDIA_FAST_FORWARD:     TODO(); return {}; // missing

   case SDL_SCANCODE_AC_SEARCH:              TODO(); return {}; // missing
   case SDL_SCANCODE_AC_HOME:                TODO(); return {}; // missing
   case SDL_SCANCODE_AC_BACK:                TODO(); return {}; // missing
   case SDL_SCANCODE_AC_FORWARD:             TODO(); return {}; // missing
   case SDL_SCANCODE_AC_STOP:                TODO(); return {}; // missing
   case SDL_SCANCODE_AC_REFRESH:             TODO(); return {}; // missing
   case SDL_SCANCODE_AC_BOOKMARKS:           TODO(); return {}; // missing

   case SDL_SCANCODE_MEDIA_EJECT:            TODO(); return {}; // missing
   case SDL_SCANCODE_SLEEP:                  TODO(); return {}; // missing

   case SDL_SCANCODE_SOFTLEFT:               TODO(); return {}; // missing
   case SDL_SCANCODE_SOFTRIGHT:              TODO(); return {}; // missing
   case SDL_SCANCODE_CALL:                   TODO(); return {}; // missing
   case SDL_SCANCODE_ENDCALL:                TODO(); return {}; // missing
   default:
      LglsError("Missing keyboard event");
      return {};
   }
}

auto TranslateMouse(Uint8 e) -> RTTI::DMeta {
   switch (e) {
   case 0: return MetaDataOf<Keys::LeftMouse>();
   case 1: return MetaDataOf<Keys::RightMouse>();
   case 2: return MetaDataOf<Keys::MiddleMouse>();
   case 3: return MetaDataOf<Keys::Mouse4>();
   case 4: return MetaDataOf<Keys::Mouse5>();
   case 5: return MetaDataOf<Keys::Mouse6>();
   case 6: return MetaDataOf<Keys::Mouse7>();
   case 7: return MetaDataOf<Keys::Mouse8>();
   default:
      LglsError("Missing mouse button event");
      return {};
   }
}
