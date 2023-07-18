#include <dlfcn.h>
#include <map>
#include <string>
#include <maolan/audio/hw.hpp>
#include <maolan/midi/hw.hpp>
#include <maolan/dynamichw.hpp>


using namespace maolan;


int main()
{
  void *handle;
  DynamicHW oss;

  handle = dlopen("./libmaolanoss.so", RTLD_LAZY);
  oss.audio["out"] = (audio::audio_t)dlsym(handle, "createAudioOut");
  oss.audio["in"] = (audio::audio_t)dlsym(handle, "createAudioIn");
  oss.midi["out"] = (midi::midi_t)dlsym(handle, "createMidiOut");
  oss.midi["in"] = (midi::midi_t)dlsym(handle, "createMidiIn");

  oss.audio["out"]("OssAudioOut", "/dev/dsp", 4);
  oss.audio["in"]("OssAudioOut", "/dev/dsp", 4);
  oss.midi["out"]("OssMidiOut", "/dev/dsp");
  oss.midi["in"]("OssMidiIn", "/dev/dsp");
  return 0;
}
