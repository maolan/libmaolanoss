#include <maolan/dynamichw.hpp>


using namespace maolan;


int main()
{
  DynamicHW oss{"./libmaolanoss.so"};

  oss.audio.out("OssAudioOut", "/dev/dsp", 4);
  oss.audio.in("OssAudioOut", "/dev/dsp", 4);
  oss.midi.out("OssMidiOut", "/dev/dsp");
  oss.midi.in("OssMidiIn", "/dev/dsp");
  return 0;
}
