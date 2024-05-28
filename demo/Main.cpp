///                                                                           
/// Langulus::Module::InputSDL                                                
/// Copyright (c) 2024 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include <Langulus.hpp>
#include <Langulus/Input.hpp>
#include <Flow/Time.hpp>
#include <thread>

using namespace Langulus;
using namespace std::chrono;
using dsec = duration<double>;
constexpr int FPS = 60;

LANGULUS_RTTI_BOUNDARY(RTTI::MainBoundary)


int main(int, char**) {
   // Create root entity                                                
   Thing root;
   root.SetName("ROOT");
   root.CreateRuntime();
   root.LoadMod("InputSDL");

   // Create user interface                                             
   root.CreateUnit<A::InputGatherer>();
   root.CreateUnit<A::InputListener>();

   auto invFpsLimit = round<system_clock::duration>(dsec {1. / FPS});
   auto m_BeginFrame = system_clock::now();
   auto m_EndFrame = m_BeginFrame + invFpsLimit;
   //unsigned frame_count_per_second = 0;
   auto prev_time_in_seconds = time_point_cast<seconds>(m_BeginFrame);

   while (true) {
      // Update until quit                                              
      if (not root.Update(16ms))
         break;

      // This part is just measuring if we're keeping the frame rate    
      // It is not necessary to keep the frame rate                     
      const auto time_in_seconds = time_point_cast<seconds>(system_clock::now());
      //++frame_count_per_second;
      if (time_in_seconds > prev_time_in_seconds) {
         //Logger::Info("FPS: ", frame_count_per_second);
         //frame_count_per_second = 0;
         prev_time_in_seconds = time_in_seconds;
      }

      // This part keeps the frame rate                                 
      std::this_thread::sleep_until(m_EndFrame);
      m_BeginFrame = m_EndFrame;
      m_EndFrame = m_BeginFrame + invFpsLimit;
   }

   return 0;
}
