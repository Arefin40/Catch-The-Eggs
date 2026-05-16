#pragma once

#pragma region Includes
#include <algorithm>
#include <array>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <direct.h>
inline bool audioGetCwd(char *b, size_t n) { return _getcwd(b, (int)n) != nullptr; }
#else
#include <unistd.h>
inline bool audioGetCwd(char *b, size_t n) { return getcwd(b, n) != nullptr; }
#endif

#include "miniaudio.h"
#pragma endregion

namespace SOUND
{
   enum Type
   {
      GOLDEN_EGG,
      POOP,
      PERK,
      BOMB,
      GAME_OVER,
      NEW_HIGH_SCORE,
      COUNT
   };
}

namespace Audio
{
   namespace detail
   {

      constexpr ma_uint32 kSoundFlags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH;
      constexpr ma_uint32 kMusicFlags = MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH;
      constexpr float kMusicVolume = 0.15f;
      constexpr size_t kVoicePool = 16;

      constexpr struct
      {
         SOUND::Type id;
         const char *file;
      } kSounds[] = {
          {SOUND::GOLDEN_EGG, "golden-egg.wav"},
          {SOUND::POOP, "poop.wav"},
          {SOUND::PERK, "gift.wav"},
          {SOUND::BOMB, "bomb.wav"},
          {SOUND::GAME_OVER, "game-over.wav"},
          {SOUND::NEW_HIGH_SCORE, "new-highscore.wav"},
      };

      struct VoiceSlot
      {
         ma_sound sound{};
         bool active = false;
      };

      struct State
      {
         std::mutex mutex;
         ma_engine engine{};
         std::array<ma_sound, SOUND::COUNT> templates{};
         std::array<bool, SOUND::COUNT> templateReady{};
         std::vector<VoiceSlot> voices;
         ma_sound music{};
         bool musicReady = false;
         std::atomic<bool> initialized{false};
         float masterVolume = 1.0f;
         float musicVolume = kMusicVolume;
      };

      inline State &state()
      {
         static State s;
         return s;
      }

      inline std::string basePath()
      {
         char cwd[1024];
         return audioGetCwd(cwd, sizeof(cwd)) ? std::string(cwd) + "/assets/sounds/" : "assets/sounds/";
      }

      template <typename T>
      inline void uninitAndZero(T &obj, void (*uninit)(T *))
      {
         uninit(&obj);
         std::memset(&obj, 0, sizeof(obj));
      }

      inline void releaseVoice(VoiceSlot &slot)
      {
         if (!slot.active)
            return;
         uninitAndZero(slot.sound, ma_sound_uninit);
         slot.active = false;
      }

      inline void reclaimVoices(State &s)
      {
         for (auto &v : s.voices)
            if (v.active && (!ma_sound_is_playing(&v.sound) || ma_sound_at_end(&v.sound)))
               releaseVoice(v);
      }

      inline VoiceSlot *acquireVoice(State &s)
      {
         VoiceSlot *oldest = nullptr;
         for (auto &v : s.voices)
         {
            if (!v.active)
               return &v;
            if (!oldest)
               oldest = &v;
         }

         if (oldest)
         {
            ma_sound_stop(&oldest->sound);
            releaseVoice(*oldest);
         }
         return oldest;
      }

      inline bool loadTemplate(State &s, SOUND::Type id)
      {
         std::string path = basePath() + kSounds[id].file;
         bool ok = ma_sound_init_from_file(&s.engine, path.c_str(), kSoundFlags,
                                           nullptr, nullptr, &s.templates[id]) == MA_SUCCESS;
         return s.templateReady[id] = ok;
      }

      inline bool initUnlocked(State &s)
      {
         if (s.initialized.load(std::memory_order_acquire))
            return true;

         ma_engine_config cfg = ma_engine_config_init();
         if (ma_engine_init(&cfg, &s.engine) != MA_SUCCESS)
            return false;

         s.voices.assign(kVoicePool, {});
         s.templateReady.fill(false);

         bool any = false;
         for (auto &def : kSounds)
            any |= loadTemplate(s, def.id);

         if (!any)
         {
            uninitAndZero(s.engine, ma_engine_uninit);
            return false;
         }

         ma_engine_set_volume(&s.engine, s.masterVolume);
         s.initialized.store(true, std::memory_order_release);
         return true;
      }

   }

   inline void shutdown()
   {
      using namespace detail;
      State &s = state();
      std::lock_guard lock(s.mutex);
      if (!s.initialized.load(std::memory_order_acquire))
         return;

      for (auto &v : s.voices)
         releaseVoice(v);
      if (s.musicReady)
      {
         uninitAndZero(s.music, ma_sound_uninit);
         s.musicReady = false;
      }
      for (int i = 0; i < SOUND::COUNT; ++i)
         if (s.templateReady[i])
         {
            uninitAndZero(s.templates[i], ma_sound_uninit);
            s.templateReady[i] = false;
         }

      uninitAndZero(s.engine, ma_engine_uninit);
      s.voices.clear();
      s.initialized.store(false, std::memory_order_release);
   }

   inline bool init()
   {
      using namespace detail;
      State &s = state();
      std::lock_guard lock(s.mutex);
      bool ok = initUnlocked(s);
      static bool hooked = false;
      if (ok && !hooked)
      {
         std::atexit(+[]
                     { shutdown(); });
         hooked = true;
      }
      return ok;
   }

   inline void play(SOUND::Type id)
   {
      using namespace detail;
      if (id < 0 || id >= SOUND::COUNT)
         return;
      State &s = state();
      std::lock_guard lock(s.mutex);
      if (!s.initialized.load(std::memory_order_acquire) && !initUnlocked(s))
         return;
      if (!s.templateReady[id] && !loadTemplate(s, id))
         return;

      reclaimVoices(s);
      VoiceSlot *slot = acquireVoice(s);
      if (!slot)
         return;

      if (ma_sound_init_copy(&s.engine, &s.templates[id], kSoundFlags, nullptr, &slot->sound) != MA_SUCCESS)
         return;

      slot->active = true;
      ma_sound_set_volume(&slot->sound, s.masterVolume);
      if (ma_sound_start(&slot->sound) != MA_SUCCESS)
         releaseVoice(*slot);
   }

   inline void initBackgroundMusic()
   {
      using namespace detail;
      State &s = state();
      std::lock_guard lock(s.mutex);
      if (!s.initialized.load(std::memory_order_acquire) && !initUnlocked(s))
         return;
      if (s.musicReady && ma_sound_is_playing(&s.music))
         return;

      if (!s.musicReady)
      {
         std::string path = basePath() + "background-music.wav";
         if (ma_sound_init_from_file(&s.engine, path.c_str(), kMusicFlags,
                                     nullptr, nullptr, &s.music) != MA_SUCCESS)
            return;
         ma_sound_set_looping(&s.music, MA_TRUE);
         s.musicReady = true;
      }
      ma_sound_set_volume(&s.music, s.musicVolume);
      ma_sound_start(&s.music);
   }

   inline void stopMusic()
   {
      using namespace detail;
      State &s = state();
      std::lock_guard lock(s.mutex);
      if (s.musicReady)
         ma_sound_stop(&s.music);
   }

   inline void setVolume(float v)
   {
      detail::state().masterVolume = std::clamp(v, 0.f, 1.f);
      std::lock_guard lock(detail::state().mutex);
      if (detail::state().initialized)
         ma_engine_set_volume(&detail::state().engine, detail::state().masterVolume);
   }
   inline float getVolume() { return detail::state().masterVolume; }
   inline void setMusicVolume(float v)
   {
      auto &s = detail::state();
      s.musicVolume = std::clamp(v, 0.f, 1.f);
      std::lock_guard lock(s.mutex);
      if (s.musicReady)
         ma_sound_set_volume(&s.music, s.musicVolume);
   }
   inline float getMusicVolume() { return detail::state().musicVolume; }

}