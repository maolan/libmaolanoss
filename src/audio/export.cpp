#include "maolan/audio/in.hpp"
#include "maolan/audio/out.hpp"


using namespace maolan::audio;


extern "C" HW * createAudioOut(const std::string &name, const std::string &device, const size_t &size)
{
  if (size == 1) { return new OSSOut<int8_t>{name, device}; }
  if (size == 2) { return new OSSOut<int16_t>{name, device}; }
  if (size == 4) { return new OSSOut<int32_t>{name, device}; }
  throw std::invalid_argument{"No such size"};
}


extern "C" HW * createAudioIn(const std::string &name, const std::string &device, const size_t &size)
{
  if (size == 1) { return new OSSIn<int8_t>{name, device}; }
  if (size == 2) { return new OSSIn<int16_t>{name, device}; }
  if (size == 4) { return new OSSIn<int32_t>{name, device}; }
  throw std::invalid_argument{"No such size"};
}
