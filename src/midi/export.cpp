#include "maolan/midi/in.hpp"
#include "maolan/midi/out.hpp"


using namespace maolan::midi;


extern "C" HW * createMidiOut(const std::string &name, const std::string &device)
{
  return new OSSOut{name, device};
}


extern "C" HW * createMidiIn(const std::string &name, const std::string &device)
{
  return new OSSIn{name, device};
}
