#include <unistd.h>

#include <maolan/audio/oss/in.hpp>
#include <maolan/audio/output.hpp>
#include <maolan/constants.hpp>

using namespace maolan::audio;

OSSIn::OSSIn(const std::string &name, const std::string &device,
                const int &frag)
    : OSS{name, device, frag} {
  _type = "AudioOSSIn";
}

void OSSIn::readhw() {
  read(_fd, _bytes, _bufferInfo.bytes);
}

void OSSIn::process() {
  const auto &chs = channels();
  size_t channel;
  size_t index;
  int *samples = (int *)_bytes;
  for (size_t i = 0; i < chs; ++i) {
    // TODO: this is memory leak, fix it!
    _outputs[i] = new Output();
  }
  for (size_t i = 0; i < Config::audioBufferSize; ++i) {
    channel = i % chs;
    index = i / chs;
    _outputs[channel]->buffer()->data()[index] = samples[i] / floatMaxInt;
  }
}

size_t OSSIn::connected() const { return 0; }
