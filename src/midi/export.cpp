#include <glob.h>

#include <maolan/midi/oss/in.hpp>
#include <maolan/midi/oss/out.hpp>

using namespace maolan::midi;

extern "C" HW *createMidiOut(const std::string &name,
                             const std::string &device) {
  return new OSSOut{name, device};
}

extern "C" HW *createMidiIn(const std::string &name,
                            const std::string &device) {
  return new OSSIn{name, device};
}

extern "C" std::vector<HW *> *listMidi() {
  HW *hw;
  glob_t g = {0};
  auto *devices = new std::vector<HW *>;
  std::string midistat = "/dev/midistat";

  glob("/dev/midi*", GLOB_DOOFFS, nullptr, &g);
  glob("/dev/umidi*", GLOB_DOOFFS | GLOB_APPEND, nullptr, &g);
  for (size_t i = 0; i < g.gl_pathc; ++i) {
    if (g.gl_pathv[i] == midistat) {
      continue;
    }
    hw = new HW(g.gl_pathv[i], g.gl_pathv[i]);
    devices->push_back(hw);
  }
  return devices;
}
