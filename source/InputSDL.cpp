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
   InputSDL, InputGatherer, InputListener, Anticipator
)


DMeta TranslateKey(SDL_Scancode);
DMeta TranslateMouse(Uint8);

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
   Math::Vec2f mouseMovement;
   Math::Vec2f mouseScroll;
   SDL_Event e;
   while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_EVENT_QUIT:
         // User requests quit                                          
         return false;
      case SDL_EVENT_JOYSTICK_AXIS_MOTION:
         Logger::Input("Joystick axis motion");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_BALL_MOTION:
         Logger::Input("Joystick ball motion");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
         Logger::Input("Joystick button down");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_BUTTON_UP:
         Logger::Input("Joystick button up");
         TODO();
         break;
      case SDL_EVENT_JOYSTICK_HAT_MOTION:
         Logger::Input("Joystick hat motion");
         TODO();
         break;
      case SDL_EVENT_CLIPBOARD_UPDATE:
         Logger::Input("Clipboard change detected");
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
         Event newEvent;
         newEvent.mType = TranslateMouse(e.button.button);
         newEvent.mState = EventState::Begin;
         Logger::Input("Mouse button pressed: ", newEvent.mType.GetToken());
         PushEvent(newEvent);
         break;
      }
      case SDL_EVENT_MOUSE_BUTTON_UP: {
         // Mouse key was released                                      
         Event newEvent;
         newEvent.mType = TranslateMouse(e.button.button);
         newEvent.mState = EventState::End;
         Logger::Input("Mouse button released: ", newEvent.mType.GetToken());
         PushEvent(newEvent);
         break;
      }
      case SDL_EVENT_WINDOW_FOCUS_LOST: {
         // Input focus lost - pause game, etc.?                        
         Logger::Input("Focus lost");
         PushEvent(Events::WindowUnfocus {});
         break;
      }
      case SDL_EVENT_WINDOW_FOCUS_GAINED: {
         // Input focus gained - resume game?                           
         Logger::Input("Focus gained");
         PushEvent(Events::WindowFocus {});
         break;
      }
      case SDL_EVENT_KEY_DOWN: {
         // Keyboard key was pressed down                               
         Event newEvent;
         newEvent.mType = TranslateKey(e.key.keysym.scancode);
         newEvent.mState = EventState::Begin;
         Logger::Input("Keyboard button pressed: ", newEvent.mType.GetToken());
         PushEvent(newEvent);
         break;
      }
      case SDL_EVENT_KEY_UP: {
         // Keyboard key was released                                   
         Event newEvent;
         newEvent.mType = TranslateKey(e.key.keysym.scancode);
         newEvent.mState = EventState::End;
         Logger::Input("Keyboard button released: ", newEvent.mType.GetToken());
         PushEvent(newEvent);
         break;
      }}
   }

   // Dispatch gathered mouse movement events                           
   if (mouseMovement.x) {
      Logger::Input("Mouse moved horizontally: ", *mouseMovement.x);
      PushEvent(Events::MouseMoveHorizontal {
         EventState::Point, *mouseScroll.x});
   }

   if (mouseMovement.y) {
      Logger::Input("Mouse moved vertically: ", *mouseMovement.y);
      PushEvent(Events::MouseMoveVertical {
         EventState::Point, *mouseScroll.y});
   }

   if (mouseMovement.x or mouseMovement.y) {
      PushEvent(Events::MouseMove {
         EventState::Point, mouseScroll});
   }
   
   // Dispatch gathered mouse scroll events                             
   if (mouseScroll.x) {
      Logger::Input("Mouse scrolled horizontally: ", *mouseScroll.x);
      PushEvent(Events::MouseScrollHorizontal {
         EventState::Point, *mouseScroll.x});
   }

   if (mouseScroll.y) {
      Logger::Input("Mouse scrolled vertically: ", *mouseScroll.y);
      PushEvent(Events::MouseScrollVertical {
         EventState::Point, *mouseScroll.y});
   }

   if (mouseScroll.x or mouseScroll.y) {
      PushEvent(Events::MouseScroll {
         EventState::Point, mouseScroll});
   }

   // Update all gatherers                                              
   for (auto& gatherer : mGatherers)
      gatherer.Update(deltaTime, mGlobalEvents);

   mGlobalEvents.Clear();
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void InputSDL::Create(Verb& verb) {
   mGatherers.Create(verb);
}

/// Push a global event, that will be propagated to all gatherers             
///   @param e - event to push                                                
void InputSDL::PushEvent(const Event& e) {
   const auto foundEvent = mGlobalEvents.FindIt(e.mType);
   if (foundEvent) {
      const auto foundState = foundEvent.mValue->FindIt(e.mState);
      if (foundState) {
         // Event already exists, merge payload                         
         foundState.mValue->mPayload += e.mPayload;
         return;
      }
      foundEvent.mValue->Insert(e.mState, e);
   }
   else {
      mGlobalEvents.Insert(e.mType);
      auto& newGroup = mGlobalEvents[e.mType];
      newGroup.Insert(e.mState, e);
   }
}

/// SDL3 keyboard event -> Langulus event translator                          
///   @param i - the code to translate                                        
///   @return the translated event                                            
DMeta TranslateKey(SDL_Scancode i) {
   switch (i) {
   case SDL_SCANCODE_A:             return MetaOf<Keys::A>();
   case SDL_SCANCODE_B:             return MetaOf<Keys::B>();
   case SDL_SCANCODE_C:             return MetaOf<Keys::C>();
   case SDL_SCANCODE_D:             return MetaOf<Keys::D>();
   case SDL_SCANCODE_E:             return MetaOf<Keys::E>();
   case SDL_SCANCODE_F:             return MetaOf<Keys::F>();
   case SDL_SCANCODE_G:             return MetaOf<Keys::G>();
   case SDL_SCANCODE_H:             return MetaOf<Keys::H>();
   case SDL_SCANCODE_I:             return MetaOf<Keys::I>();
   case SDL_SCANCODE_J:             return MetaOf<Keys::J>();
   case SDL_SCANCODE_K:             return MetaOf<Keys::K>();
   case SDL_SCANCODE_L:             return MetaOf<Keys::L>();
   case SDL_SCANCODE_M:             return MetaOf<Keys::M>();
   case SDL_SCANCODE_N:             return MetaOf<Keys::N>();
   case SDL_SCANCODE_O:             return MetaOf<Keys::O>();
   case SDL_SCANCODE_P:             return MetaOf<Keys::P>();
   case SDL_SCANCODE_Q:             return MetaOf<Keys::Q>();
   case SDL_SCANCODE_R:             return MetaOf<Keys::R>();
   case SDL_SCANCODE_S:             return MetaOf<Keys::S>();
   case SDL_SCANCODE_T:             return MetaOf<Keys::T>();
   case SDL_SCANCODE_U:             return MetaOf<Keys::U>();
   case SDL_SCANCODE_V:             return MetaOf<Keys::V>();
   case SDL_SCANCODE_W:             return MetaOf<Keys::W>();
   case SDL_SCANCODE_X:             return MetaOf<Keys::X>();
   case SDL_SCANCODE_Y:             return MetaOf<Keys::Y>();
   case SDL_SCANCODE_Z:             return MetaOf<Keys::Z>();

   case SDL_SCANCODE_1:             return MetaOf<Keys::Main1>();
   case SDL_SCANCODE_2:             return MetaOf<Keys::Main2>();
   case SDL_SCANCODE_3:             return MetaOf<Keys::Main3>();
   case SDL_SCANCODE_4:             return MetaOf<Keys::Main4>();
   case SDL_SCANCODE_5:             return MetaOf<Keys::Main5>();
   case SDL_SCANCODE_6:             return MetaOf<Keys::Main6>();
   case SDL_SCANCODE_7:             return MetaOf<Keys::Main7>();
   case SDL_SCANCODE_8:             return MetaOf<Keys::Main8>();
   case SDL_SCANCODE_9:             return MetaOf<Keys::Main9>();
   case SDL_SCANCODE_0:             return MetaOf<Keys::Main0>();

   case SDL_SCANCODE_RETURN:        return MetaOf<Keys::Enter>(); 
   case SDL_SCANCODE_ESCAPE:        return MetaOf<Keys::Escape>();
   case SDL_SCANCODE_BACKSPACE:     return MetaOf<Keys::Back>();  
   case SDL_SCANCODE_TAB:           return MetaOf<Keys::Tab>();   
   case SDL_SCANCODE_SPACE:         return MetaOf<Keys::Space>(); 
   case SDL_SCANCODE_MINUS:         return MetaOf<Keys::Minus>(); 
   case SDL_SCANCODE_EQUALS:        TODO(); //missing
   case SDL_SCANCODE_LEFTBRACKET:   return MetaOf<Keys::LeftBracket>(); 
   case SDL_SCANCODE_RIGHTBRACKET:  return MetaOf<Keys::RightBracket>();
   case SDL_SCANCODE_BACKSLASH:
   case SDL_SCANCODE_NONUSHASH:     return MetaOf<Keys::Hack>();           
   case SDL_SCANCODE_SEMICOLON:     return MetaOf<Keys::Semicolon>();      
   case SDL_SCANCODE_APOSTROPHE:    return MetaOf<Keys::Apostrophe>();     
   case SDL_SCANCODE_GRAVE:         return MetaOf<Keys::Tilde>();          
   case SDL_SCANCODE_COMMA:         return MetaOf<Keys::Comma>();          
   case SDL_SCANCODE_PERIOD:        return MetaOf<Keys::Period>();         
   case SDL_SCANCODE_SLASH:         return MetaOf<Keys::Slash>();          

   case SDL_SCANCODE_CAPSLOCK:      return MetaOf<Keys::CapsLock>();       

   case SDL_SCANCODE_F1:            return MetaOf<Keys::F1>();             
   case SDL_SCANCODE_F2:            return MetaOf<Keys::F2>();             
   case SDL_SCANCODE_F3:            return MetaOf<Keys::F3>();             
   case SDL_SCANCODE_F4:            return MetaOf<Keys::F4>();             
   case SDL_SCANCODE_F5:            return MetaOf<Keys::F5>();             
   case SDL_SCANCODE_F6:            return MetaOf<Keys::F6>();             
   case SDL_SCANCODE_F7:            return MetaOf<Keys::F7>();             
   case SDL_SCANCODE_F8:            return MetaOf<Keys::F8>();             
   case SDL_SCANCODE_F9:            return MetaOf<Keys::F9>();             
   case SDL_SCANCODE_F10:           return MetaOf<Keys::F10>();            
   case SDL_SCANCODE_F11:           return MetaOf<Keys::F11>();            
   case SDL_SCANCODE_F12:           return MetaOf<Keys::F12>();            

   case SDL_SCANCODE_PRINTSCREEN:   return MetaOf<Keys::Print>();          
   case SDL_SCANCODE_SCROLLLOCK:    return MetaOf<Keys::ScrollLock>();     
   case SDL_SCANCODE_PAUSE:         return MetaOf<Keys::Pause>();          
   case SDL_SCANCODE_INSERT:        return MetaOf<Keys::Insert>();         
   case SDL_SCANCODE_HOME:          return MetaOf<Keys::Home>();           
   case SDL_SCANCODE_PAGEUP:        return MetaOf<Keys::PageUp>();         
   case SDL_SCANCODE_DELETE:        return MetaOf<Keys::Delete>();         
   case SDL_SCANCODE_END:           return MetaOf<Keys::End>();            
   case SDL_SCANCODE_PAGEDOWN:      return MetaOf<Keys::PageDown>();       
   case SDL_SCANCODE_RIGHT:         return MetaOf<Keys::Right>();          
   case SDL_SCANCODE_LEFT:          return MetaOf<Keys::Left>();           
   case SDL_SCANCODE_DOWN:          return MetaOf<Keys::Down>();           
   case SDL_SCANCODE_UP:            return MetaOf<Keys::Up>();             

   case SDL_SCANCODE_NUMLOCKCLEAR:  return MetaOf<Keys::NumLock>();        
   case SDL_SCANCODE_KP_DIVIDE:     return MetaOf<Keys::NumpadDivide>();   
   case SDL_SCANCODE_KP_MULTIPLY:   return MetaOf<Keys::NumpadMultiply>(); 
   case SDL_SCANCODE_KP_MINUS:      return MetaOf<Keys::NumpadSubtract>(); 
   case SDL_SCANCODE_KP_PLUS:       return MetaOf<Keys::NumpadAdd>();      
   case SDL_SCANCODE_KP_ENTER:      return MetaOf<Keys::NumpadEnter>();    
   case SDL_SCANCODE_KP_1:          return MetaOf<Keys::Numpad1>();        
   case SDL_SCANCODE_KP_2:          return MetaOf<Keys::Numpad2>();        
   case SDL_SCANCODE_KP_3:          return MetaOf<Keys::Numpad3>();        
   case SDL_SCANCODE_KP_4:          return MetaOf<Keys::Numpad4>();        
   case SDL_SCANCODE_KP_5:          return MetaOf<Keys::Numpad5>();        
   case SDL_SCANCODE_KP_6:          return MetaOf<Keys::Numpad6>();        
   case SDL_SCANCODE_KP_7:          return MetaOf<Keys::Numpad7>();        
   case SDL_SCANCODE_KP_8:          return MetaOf<Keys::Numpad8>();        
   case SDL_SCANCODE_KP_9:          return MetaOf<Keys::Numpad9>();        
   case SDL_SCANCODE_KP_0:          return MetaOf<Keys::Numpad0>();        
   case SDL_SCANCODE_KP_PERIOD:     return MetaOf<Keys::NumpadDecimal>();  

   case SDL_SCANCODE_NONUSBACKSLASH:return MetaOf<Keys::Hack>();           
   case SDL_SCANCODE_APPLICATION:   TODO(); //missing
   case SDL_SCANCODE_POWER:         TODO(); //missing
   case SDL_SCANCODE_KP_EQUALS:     return MetaOf<Keys::NumpadEqual>();    
   case SDL_SCANCODE_F13:           return MetaOf<Keys::F13>();            
   case SDL_SCANCODE_F14:           return MetaOf<Keys::F14>();            
   case SDL_SCANCODE_F15:           return MetaOf<Keys::F15>();            
   case SDL_SCANCODE_F16:           return MetaOf<Keys::F16>();            
   case SDL_SCANCODE_F17:           return MetaOf<Keys::F17>();            
   case SDL_SCANCODE_F18:           return MetaOf<Keys::F18>();            
   case SDL_SCANCODE_F19:           return MetaOf<Keys::F19>();            
   case SDL_SCANCODE_F20:           return MetaOf<Keys::F20>();            
   case SDL_SCANCODE_F21:           return MetaOf<Keys::F21>();            
   case SDL_SCANCODE_F22:           return MetaOf<Keys::F22>();            
   case SDL_SCANCODE_F23:           return MetaOf<Keys::F23>();            
   case SDL_SCANCODE_F24:           return MetaOf<Keys::F24>();            

   case SDL_SCANCODE_EXECUTE:             TODO(); //missing
   case SDL_SCANCODE_HELP:                TODO(); //missing
   case SDL_SCANCODE_MENU:                TODO(); //missing
   case SDL_SCANCODE_SELECT:              TODO(); //missing
   case SDL_SCANCODE_STOP:                TODO(); //missing
   case SDL_SCANCODE_AGAIN:               TODO(); //missing
   case SDL_SCANCODE_UNDO:                TODO(); //missing
   case SDL_SCANCODE_CUT:                 TODO(); //missing
   case SDL_SCANCODE_COPY:                TODO(); //missing
   case SDL_SCANCODE_PASTE:               TODO(); //missing
   case SDL_SCANCODE_FIND:                TODO(); //missing
   case SDL_SCANCODE_MUTE:                TODO(); //missing
   case SDL_SCANCODE_VOLUMEUP:            TODO(); //missing
   case SDL_SCANCODE_VOLUMEDOWN:          TODO(); //missing
   case SDL_SCANCODE_KP_COMMA:            TODO(); //missing
   case SDL_SCANCODE_KP_EQUALSAS400:      TODO(); //missing

   case SDL_SCANCODE_INTERNATIONAL1:      TODO(); /**< used on Asian keyboards, see footnotes in USB doc */
   case SDL_SCANCODE_INTERNATIONAL2:      TODO();
   case SDL_SCANCODE_INTERNATIONAL3:      TODO(); /**< Yen */
   case SDL_SCANCODE_INTERNATIONAL4:      TODO();
   case SDL_SCANCODE_INTERNATIONAL5:      TODO();
   case SDL_SCANCODE_INTERNATIONAL6:      TODO();
   case SDL_SCANCODE_INTERNATIONAL7:      TODO();
   case SDL_SCANCODE_INTERNATIONAL8:      TODO();
   case SDL_SCANCODE_INTERNATIONAL9:      TODO();

   case SDL_SCANCODE_LANG1:               TODO(); /**< Hangul/English toggle */
   case SDL_SCANCODE_LANG2:               TODO(); /**< Hanja conversion */
   case SDL_SCANCODE_LANG3:               TODO(); /**< Katakana */
   case SDL_SCANCODE_LANG4:               TODO(); /**< Hiragana */
   case SDL_SCANCODE_LANG5:               TODO(); /**< Zenkaku/Hankaku */
   case SDL_SCANCODE_LANG6:               TODO(); /**< reserved */
   case SDL_SCANCODE_LANG7:               TODO(); /**< reserved */
   case SDL_SCANCODE_LANG8:               TODO(); /**< reserved */
   case SDL_SCANCODE_LANG9:               TODO(); /**< reserved */

   case SDL_SCANCODE_ALTERASE:            TODO(); /**< Erase-Eaze */
   case SDL_SCANCODE_SYSREQ:              TODO();
   case SDL_SCANCODE_CANCEL:              TODO(); /**< AC Cancel */
   case SDL_SCANCODE_CLEAR:               TODO();
   case SDL_SCANCODE_PRIOR:               TODO();
   case SDL_SCANCODE_RETURN2:             TODO();
   case SDL_SCANCODE_SEPARATOR:           TODO();
   case SDL_SCANCODE_OUT:                 TODO();
   case SDL_SCANCODE_OPER:                TODO();
   case SDL_SCANCODE_CLEARAGAIN:          TODO();
   case SDL_SCANCODE_CRSEL:               TODO();
   case SDL_SCANCODE_EXSEL:               TODO();

   case SDL_SCANCODE_KP_00:               TODO();
   case SDL_SCANCODE_KP_000:              TODO();
   case SDL_SCANCODE_THOUSANDSSEPARATOR:  TODO();
   case SDL_SCANCODE_DECIMALSEPARATOR:    TODO();
   case SDL_SCANCODE_CURRENCYUNIT:        TODO();
   case SDL_SCANCODE_CURRENCYSUBUNIT:     TODO();
   case SDL_SCANCODE_KP_LEFTPAREN:        TODO();
   case SDL_SCANCODE_KP_RIGHTPAREN:       TODO();
   case SDL_SCANCODE_KP_LEFTBRACE:        TODO();
   case SDL_SCANCODE_KP_RIGHTBRACE:       TODO();
   case SDL_SCANCODE_KP_TAB:              TODO();
   case SDL_SCANCODE_KP_BACKSPACE:        TODO();
   case SDL_SCANCODE_KP_A:                TODO();
   case SDL_SCANCODE_KP_B:                TODO();
   case SDL_SCANCODE_KP_C:                TODO();
   case SDL_SCANCODE_KP_D:                TODO();
   case SDL_SCANCODE_KP_E:                TODO();
   case SDL_SCANCODE_KP_F:                TODO();
   case SDL_SCANCODE_KP_XOR:              TODO();
   case SDL_SCANCODE_KP_POWER:            TODO();
   case SDL_SCANCODE_KP_PERCENT:          TODO();
   case SDL_SCANCODE_KP_LESS:             TODO();
   case SDL_SCANCODE_KP_GREATER:          TODO();
   case SDL_SCANCODE_KP_AMPERSAND:        TODO();
   case SDL_SCANCODE_KP_DBLAMPERSAND:     TODO();
   case SDL_SCANCODE_KP_VERTICALBAR:      TODO();
   case SDL_SCANCODE_KP_DBLVERTICALBAR:   TODO();
   case SDL_SCANCODE_KP_COLON:            TODO();
   case SDL_SCANCODE_KP_HASH:             TODO();
   case SDL_SCANCODE_KP_SPACE:            TODO();
   case SDL_SCANCODE_KP_AT:               TODO();
   case SDL_SCANCODE_KP_EXCLAM:           TODO();
   case SDL_SCANCODE_KP_MEMSTORE:         TODO();
   case SDL_SCANCODE_KP_MEMRECALL:        TODO();
   case SDL_SCANCODE_KP_MEMCLEAR:         TODO();
   case SDL_SCANCODE_KP_MEMADD:           TODO();
   case SDL_SCANCODE_KP_MEMSUBTRACT:      TODO();
   case SDL_SCANCODE_KP_MEMMULTIPLY:      TODO();
   case SDL_SCANCODE_KP_MEMDIVIDE:        TODO();
   case SDL_SCANCODE_KP_PLUSMINUS:        TODO();
   case SDL_SCANCODE_KP_CLEAR:            TODO();
   case SDL_SCANCODE_KP_CLEARENTRY:       TODO();
   case SDL_SCANCODE_KP_BINARY:           TODO();
   case SDL_SCANCODE_KP_OCTAL:            TODO();
   case SDL_SCANCODE_KP_DECIMAL:          TODO();
   case SDL_SCANCODE_KP_HEXADECIMAL:      TODO();

   case SDL_SCANCODE_LCTRL:            return MetaOf<Keys::LeftControl>(); 
   case SDL_SCANCODE_LSHIFT:           return MetaOf<Keys::LeftShift>();   
   case SDL_SCANCODE_LALT:             return MetaOf<Keys::LeftAlt>();     
   case SDL_SCANCODE_LGUI:             TODO(); //missing
   case SDL_SCANCODE_RCTRL:            return MetaOf<Keys::RightControl>();
   case SDL_SCANCODE_RSHIFT:           return MetaOf<Keys::RightShift>();  
   case SDL_SCANCODE_RALT:             return MetaOf<Keys::RightAlt>();    
   case SDL_SCANCODE_RGUI:             TODO(); //missing

   case SDL_SCANCODE_MODE:             TODO(); //missing

   case SDL_SCANCODE_AUDIONEXT:        TODO(); //missing
   case SDL_SCANCODE_AUDIOPREV:        TODO(); //missing
   case SDL_SCANCODE_AUDIOSTOP:        TODO(); //missing
   case SDL_SCANCODE_AUDIOPLAY:        TODO(); //missing
   case SDL_SCANCODE_AUDIOMUTE:        TODO(); //missing
   case SDL_SCANCODE_MEDIASELECT:      TODO(); //missing
   case SDL_SCANCODE_WWW:              TODO(); //missing
   case SDL_SCANCODE_MAIL:             TODO(); //missing
   case SDL_SCANCODE_CALCULATOR:       TODO(); //missing
   case SDL_SCANCODE_COMPUTER:         TODO(); //missing
   case SDL_SCANCODE_AC_SEARCH:        TODO(); //missing
   case SDL_SCANCODE_AC_HOME:          TODO(); //missing
   case SDL_SCANCODE_AC_BACK:          TODO(); //missing
   case SDL_SCANCODE_AC_FORWARD:       TODO(); //missing
   case SDL_SCANCODE_AC_STOP:          TODO(); //missing
   case SDL_SCANCODE_AC_REFRESH:       TODO(); //missing
   case SDL_SCANCODE_AC_BOOKMARKS:     TODO(); //missing

   case SDL_SCANCODE_BRIGHTNESSDOWN:   TODO(); // missing
   case SDL_SCANCODE_BRIGHTNESSUP:     TODO(); // missing
   case SDL_SCANCODE_DISPLAYSWITCH:    TODO(); // missing
   case SDL_SCANCODE_KBDILLUMTOGGLE:   TODO(); // missing
   case SDL_SCANCODE_KBDILLUMDOWN:     TODO(); // missing
   case SDL_SCANCODE_KBDILLUMUP:       TODO(); // missing
   case SDL_SCANCODE_EJECT:            TODO(); // missing
   case SDL_SCANCODE_SLEEP:            TODO(); // missing
   case SDL_SCANCODE_APP1:             TODO(); // missing
   case SDL_SCANCODE_APP2:             TODO(); // missing

   case SDL_SCANCODE_AUDIOREWIND:      TODO(); // missing
   case SDL_SCANCODE_AUDIOFASTFORWARD: TODO(); // missing
   case SDL_SCANCODE_SOFTLEFT:         TODO(); // missing
   case SDL_SCANCODE_SOFTRIGHT:        TODO(); // missing
   case SDL_SCANCODE_CALL:             TODO(); // missing
   case SDL_SCANCODE_ENDCALL:          TODO(); // missing
   default:
      LANGULUS_OOPS(Meta, "Missing keyboard event");
   }
   return {};
}

DMeta TranslateMouse(Uint8 e) {
   switch (e) {
   case 0: return MetaOf<Keys::LeftMouse>();
   case 1: return MetaOf<Keys::RightMouse>();
   case 2: return MetaOf<Keys::MiddleMouse>();
   case 3: return MetaOf<Keys::Mouse4>();
   case 4: return MetaOf<Keys::Mouse5>();
   case 5: return MetaOf<Keys::Mouse6>();
   case 6: return MetaOf<Keys::Mouse7>();
   case 7: return MetaOf<Keys::Mouse8>();
   default:
      LANGULUS_OOPS(Meta, "Missing mouse button event");
   }
}
