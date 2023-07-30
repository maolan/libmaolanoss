#include <iostream>

#include <maolan/dynamichw.hpp>

using namespace maolan;

int main() {
  DynamicHW oss{"./libmaolanoss.so"};

  oss.audio.out("OssAudioOut", "/dev/dsp", 4);
  oss.audio.in("OssAudioOut", "/dev/dsp", 4);
  oss.midi.out("OssMidiOut", "/dev/dsp");
  oss.midi.in("OssMidiIn", "/dev/dsp");

  auto *audioDevices = oss.audio.list();
  for (const auto &device : *audioDevices) {
    std::cout << device->name() << ' ' << device->device() << std::endl;
  }
  delete audioDevices;

  auto *midiDevices = oss.midi.list();
  for (const auto &device : *midiDevices) {
    std::cout << device->name() << ' ' << device->device() << std::endl;
  }
  delete midiDevices;
  return 0;
}
