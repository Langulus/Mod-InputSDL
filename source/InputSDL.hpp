///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "InputGatherer.hpp"
#include <Flow/Verbs/Create.hpp>


///                                                                           
///   Raw input module using SDL                                              
///                                                                           
/// Manages and produces input gatherers                                      
///                                                                           
struct InputSDL final : A::InputModule {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::InputModule);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created input gatherers                                   
   TFactory<InputGatherer> mGatherers;

public:
    InputSDL(Runtime*, const Neat&);
   ~InputSDL();

   bool Update(Time);
   void Create(Verb&);
};


/// SDL3 keyboard event -> Langulus event translator                          
///   @param i - the code to translate                                        
///   @return the translated event                                            
Event TranslateKeyboard(SDL_Scancode i) {
   Event r;
   switch (i) {
   case SDL_SCANCODE_A:             r.mType = MetaOf<Keys::A>();              break;
   case SDL_SCANCODE_B:             r.mType = MetaOf<Keys::B>();              break;
   case SDL_SCANCODE_C:             r.mType = MetaOf<Keys::C>();              break;
   case SDL_SCANCODE_D:             r.mType = MetaOf<Keys::D>();              break;
   case SDL_SCANCODE_E:             r.mType = MetaOf<Keys::E>();              break;
   case SDL_SCANCODE_F:             r.mType = MetaOf<Keys::F>();              break;
   case SDL_SCANCODE_G:             r.mType = MetaOf<Keys::G>();              break;
   case SDL_SCANCODE_H:             r.mType = MetaOf<Keys::H>();              break;
   case SDL_SCANCODE_I:             r.mType = MetaOf<Keys::I>();              break;
   case SDL_SCANCODE_J:             r.mType = MetaOf<Keys::J>();              break;
   case SDL_SCANCODE_K:             r.mType = MetaOf<Keys::K>();              break;
   case SDL_SCANCODE_L:             r.mType = MetaOf<Keys::L>();              break;
   case SDL_SCANCODE_M:             r.mType = MetaOf<Keys::M>();              break;
   case SDL_SCANCODE_N:             r.mType = MetaOf<Keys::N>();              break;
   case SDL_SCANCODE_O:             r.mType = MetaOf<Keys::O>();              break;
   case SDL_SCANCODE_P:             r.mType = MetaOf<Keys::P>();              break;
   case SDL_SCANCODE_Q:             r.mType = MetaOf<Keys::Q>();              break;
   case SDL_SCANCODE_R:             r.mType = MetaOf<Keys::R>();              break;
   case SDL_SCANCODE_S:             r.mType = MetaOf<Keys::S>();              break;
   case SDL_SCANCODE_T:             r.mType = MetaOf<Keys::T>();              break;
   case SDL_SCANCODE_U:             r.mType = MetaOf<Keys::U>();              break;
   case SDL_SCANCODE_V:             r.mType = MetaOf<Keys::V>();              break;
   case SDL_SCANCODE_W:             r.mType = MetaOf<Keys::W>();              break;
   case SDL_SCANCODE_X:             r.mType = MetaOf<Keys::X>();              break;
   case SDL_SCANCODE_Y:             r.mType = MetaOf<Keys::Y>();              break;
   case SDL_SCANCODE_Z:             r.mType = MetaOf<Keys::Z>();              break;

   case SDL_SCANCODE_1:             r.mType = MetaOf<Keys::Main1>();          break;
   case SDL_SCANCODE_2:             r.mType = MetaOf<Keys::Main2>();          break;
   case SDL_SCANCODE_3:             r.mType = MetaOf<Keys::Main3>();          break;
   case SDL_SCANCODE_4:             r.mType = MetaOf<Keys::Main4>();          break;
   case SDL_SCANCODE_5:             r.mType = MetaOf<Keys::Main5>();          break;
   case SDL_SCANCODE_6:             r.mType = MetaOf<Keys::Main6>();          break;
   case SDL_SCANCODE_7:             r.mType = MetaOf<Keys::Main7>();          break;
   case SDL_SCANCODE_8:             r.mType = MetaOf<Keys::Main8>();          break;
   case SDL_SCANCODE_9:             r.mType = MetaOf<Keys::Main9>();          break;
   case SDL_SCANCODE_0:             r.mType = MetaOf<Keys::Main0>();          break;

   case SDL_SCANCODE_RETURN:        r.mType = MetaOf<Keys::Enter>();          break;
   case SDL_SCANCODE_ESCAPE:        r.mType = MetaOf<Keys::Escape>();         break;
   case SDL_SCANCODE_BACKSPACE:     r.mType = MetaOf<Keys::Back>();           break;
   case SDL_SCANCODE_TAB:           r.mType = MetaOf<Keys::Tab>();            break;
   case SDL_SCANCODE_SPACE:         r.mType = MetaOf<Keys::Space>();          break;
   case SDL_SCANCODE_MINUS:         r.mType = MetaOf<Keys::Minus>();          break;
   case SDL_SCANCODE_EQUALS:        TODO(); //missing
   case SDL_SCANCODE_LEFTBRACKET:   r.mType = MetaOf<Keys::LeftBracket>();    break;
   case SDL_SCANCODE_RIGHTBRACKET:  r.mType = MetaOf<Keys::RightBracket>();   break;
   case SDL_SCANCODE_BACKSLASH:
   case SDL_SCANCODE_NONUSHASH:     r.mType = MetaOf<Keys::Hack>();           break;
   case SDL_SCANCODE_SEMICOLON:     r.mType = MetaOf<Keys::Semicolon>();      break;
   case SDL_SCANCODE_APOSTROPHE:    r.mType = MetaOf<Keys::Apostrophe>();     break;
   case SDL_SCANCODE_GRAVE:         r.mType = MetaOf<Keys::Tilde>();          break;
   case SDL_SCANCODE_COMMA:         r.mType = MetaOf<Keys::Comma>();          break;
   case SDL_SCANCODE_PERIOD:        r.mType = MetaOf<Keys::Period>();         break;
   case SDL_SCANCODE_SLASH:         r.mType = MetaOf<Keys::Slash>();          break;

   case SDL_SCANCODE_CAPSLOCK:      r.mType = MetaOf<Keys::CapsLock>();       break;

   case SDL_SCANCODE_F1:            r.mType = MetaOf<Keys::F1>();             break;
   case SDL_SCANCODE_F2:            r.mType = MetaOf<Keys::F2>();             break;
   case SDL_SCANCODE_F3:            r.mType = MetaOf<Keys::F3>();             break;
   case SDL_SCANCODE_F4:            r.mType = MetaOf<Keys::F4>();             break;
   case SDL_SCANCODE_F5:            r.mType = MetaOf<Keys::F5>();             break;
   case SDL_SCANCODE_F6:            r.mType = MetaOf<Keys::F6>();             break;
   case SDL_SCANCODE_F7:            r.mType = MetaOf<Keys::F7>();             break;
   case SDL_SCANCODE_F8:            r.mType = MetaOf<Keys::F8>();             break;
   case SDL_SCANCODE_F9:            r.mType = MetaOf<Keys::F9>();             break;
   case SDL_SCANCODE_F10:           r.mType = MetaOf<Keys::F10>();            break;
   case SDL_SCANCODE_F11:           r.mType = MetaOf<Keys::F11>();            break;
   case SDL_SCANCODE_F12:           r.mType = MetaOf<Keys::F12>();            break;

   case SDL_SCANCODE_PRINTSCREEN:   r.mType = MetaOf<Keys::Print>();          break;
   case SDL_SCANCODE_SCROLLLOCK:    r.mType = MetaOf<Keys::ScrollLock>();     break;
   case SDL_SCANCODE_PAUSE:         r.mType = MetaOf<Keys::Pause>();          break;
   case SDL_SCANCODE_INSERT:        r.mType = MetaOf<Keys::Insert>();         break;
   case SDL_SCANCODE_HOME:          r.mType = MetaOf<Keys::Home>();           break;
   case SDL_SCANCODE_PAGEUP:        r.mType = MetaOf<Keys::PageUp>();         break;
   case SDL_SCANCODE_DELETE:        r.mType = MetaOf<Keys::Delete>();         break;
   case SDL_SCANCODE_END:           r.mType = MetaOf<Keys::End>();            break;
   case SDL_SCANCODE_PAGEDOWN:      r.mType = MetaOf<Keys::PageDown>();       break;
   case SDL_SCANCODE_RIGHT:         r.mType = MetaOf<Keys::Right>();          break;
   case SDL_SCANCODE_LEFT:          r.mType = MetaOf<Keys::Left>();           break;
   case SDL_SCANCODE_DOWN:          r.mType = MetaOf<Keys::Down>();           break;
   case SDL_SCANCODE_UP:            r.mType = MetaOf<Keys::Up>();             break;

   case SDL_SCANCODE_NUMLOCKCLEAR:  r.mType = MetaOf<Keys::NumLock>();        break;
   case SDL_SCANCODE_KP_DIVIDE:     r.mType = MetaOf<Keys::NumpadDivide>();   break;
   case SDL_SCANCODE_KP_MULTIPLY:   r.mType = MetaOf<Keys::NumpadMultiply>(); break;
   case SDL_SCANCODE_KP_MINUS:      r.mType = MetaOf<Keys::NumpadSubtract>(); break;
   case SDL_SCANCODE_KP_PLUS:       r.mType = MetaOf<Keys::NumpadAdd>();      break;
   case SDL_SCANCODE_KP_ENTER:      r.mType = MetaOf<Keys::NumpadEnter>();    break;
   case SDL_SCANCODE_KP_1:          r.mType = MetaOf<Keys::Numpad1>();        break;
   case SDL_SCANCODE_KP_2:          r.mType = MetaOf<Keys::Numpad2>();        break;
   case SDL_SCANCODE_KP_3:          r.mType = MetaOf<Keys::Numpad3>();        break;
   case SDL_SCANCODE_KP_4:          r.mType = MetaOf<Keys::Numpad4>();        break;
   case SDL_SCANCODE_KP_5:          r.mType = MetaOf<Keys::Numpad5>();        break;
   case SDL_SCANCODE_KP_6:          r.mType = MetaOf<Keys::Numpad6>();        break;
   case SDL_SCANCODE_KP_7:          r.mType = MetaOf<Keys::Numpad7>();        break;
   case SDL_SCANCODE_KP_8:          r.mType = MetaOf<Keys::Numpad8>();        break;
   case SDL_SCANCODE_KP_9:          r.mType = MetaOf<Keys::Numpad9>();        break;
   case SDL_SCANCODE_KP_0:          r.mType = MetaOf<Keys::Numpad0>();        break;
   case SDL_SCANCODE_KP_PERIOD:     r.mType = MetaOf<Keys::NumpadDecimal>();  break;

   case SDL_SCANCODE_NONUSBACKSLASH:r.mType = MetaOf<Keys::Hack>();           break;
   case SDL_SCANCODE_APPLICATION:   TODO(); //missing
   case SDL_SCANCODE_POWER:         TODO(); //missing
   case SDL_SCANCODE_KP_EQUALS:     r.mType = MetaOf<Keys::NumpadEqual>();    break;
   case SDL_SCANCODE_F13:           r.mType = MetaOf<Keys::F13>();            break;
   case SDL_SCANCODE_F14:           r.mType = MetaOf<Keys::F14>();            break;
   case SDL_SCANCODE_F15:           r.mType = MetaOf<Keys::F15>();            break;
   case SDL_SCANCODE_F16:           r.mType = MetaOf<Keys::F16>();            break;
   case SDL_SCANCODE_F17:           r.mType = MetaOf<Keys::F17>();            break;
   case SDL_SCANCODE_F18:           r.mType = MetaOf<Keys::F18>();            break;
   case SDL_SCANCODE_F19:           r.mType = MetaOf<Keys::F19>();            break;
   case SDL_SCANCODE_F20:           r.mType = MetaOf<Keys::F20>();            break;
   case SDL_SCANCODE_F21:           r.mType = MetaOf<Keys::F21>();            break;
   case SDL_SCANCODE_F22:           r.mType = MetaOf<Keys::F22>();            break;
   case SDL_SCANCODE_F23:           r.mType = MetaOf<Keys::F23>();            break;
   case SDL_SCANCODE_F24:           r.mType = MetaOf<Keys::F24>();            break;

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

   case SDL_SCANCODE_LCTRL:      r.mType = MetaOf<Keys::LeftControl>();    break;
   case SDL_SCANCODE_LSHIFT:     r.mType = MetaOf<Keys::LeftShift>();      break;
   case SDL_SCANCODE_LALT:       r.mType = MetaOf<Keys::LeftAlt>();        break;
   case SDL_SCANCODE_LGUI:       TODO(); //missing
   case SDL_SCANCODE_RCTRL:      r.mType = MetaOf<Keys::RightControl>();   break;
   case SDL_SCANCODE_RSHIFT:     r.mType = MetaOf<Keys::RightShift>();     break;
   case SDL_SCANCODE_RALT:       r.mType = MetaOf<Keys::RightAlt>();       break;
   case SDL_SCANCODE_RGUI:       TODO(); //missing

   case SDL_SCANCODE_MODE:       TODO(); //missing

   case SDL_SCANCODE_AUDIONEXT = 258,
   case SDL_SCANCODE_AUDIOPREV = 259,
   case SDL_SCANCODE_AUDIOSTOP = 260,
   case SDL_SCANCODE_AUDIOPLAY = 261,
   case SDL_SCANCODE_AUDIOMUTE = 262,
   case SDL_SCANCODE_MEDIASELECT = 263,
   case SDL_SCANCODE_WWW = 264,             /**< AL Internet Browser */
   case SDL_SCANCODE_MAIL = 265,
   case SDL_SCANCODE_CALCULATOR = 266,      /**< AL Calculator */
   case SDL_SCANCODE_COMPUTER = 267,
   case SDL_SCANCODE_AC_SEARCH = 268,       /**< AC Search */
   case SDL_SCANCODE_AC_HOME = 269,         /**< AC Home */
   case SDL_SCANCODE_AC_BACK = 270,         /**< AC Back */
   case SDL_SCANCODE_AC_FORWARD = 271,      /**< AC Forward */
   case SDL_SCANCODE_AC_STOP = 272,         /**< AC Stop */
   case SDL_SCANCODE_AC_REFRESH = 273,      /**< AC Refresh */
   case SDL_SCANCODE_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

         /* @} *//* Usage page 0x0C */

         /**
          *  \name Walther keys
          *
          *  These are values that Christian Walther added (for mac keyboard?).
          */
          /* @{ */

         SDL_SCANCODE_BRIGHTNESSDOWN = 275,
         SDL_SCANCODE_BRIGHTNESSUP = 276,
         SDL_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                                switch, video mode switch */
         SDL_SCANCODE_KBDILLUMTOGGLE = 278,
         SDL_SCANCODE_KBDILLUMDOWN = 279,
         SDL_SCANCODE_KBDILLUMUP = 280,
         SDL_SCANCODE_EJECT = 281,
         SDL_SCANCODE_SLEEP = 282,           /**< SC System Sleep */

         SDL_SCANCODE_APP1 = 283,
         SDL_SCANCODE_APP2 = 284,

         /* @} *//* Walther keys */

         /**
          *  \name Usage page 0x0C (additional media keys)
          *
          *  These values are mapped from usage page 0x0C (USB consumer page).
          */
          /* @{ */

         SDL_SCANCODE_AUDIOREWIND = 285,
         SDL_SCANCODE_AUDIOFASTFORWARD = 286,

         /* @} *//* Usage page 0x0C (additional media keys) */

         /**
          *  \name Mobile keys
          *
          *  These are values that are often used on mobile phones.
          */
          /* @{ */

         SDL_SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                           used as a multi-function feature key for selecting
                                           a software defined function shown on the bottom left
                                           of the display. */
         SDL_SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                            used as a multi-function feature key for selecting
                                            a software defined function shown on the bottom right
                                            of the display. */
         SDL_SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
         SDL_SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */
   }
}