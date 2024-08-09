#include <unistd.h>

#include <maolan/audio/oss/in.hpp>
#include <maolan/constants.hpp>
#include <maolan/audio/output.hpp>

using namespace maolan::audio;

template <class T>
OSSIn<T>::OSSIn(const std::string &name, const std::string &device,
                const int &frag)
    : OSS{name, device, sizeof(T), frag} {
  _type = "AudioOSSIn";
}

template <class T> void OSSIn<T>::readhw() {
  read(_fd, _bytes, _bufferInfo.bytes);
}

template <class T> void OSSIn<T>::process() {
  const auto &chs = channels();
  size_t channel;
  size_t index;
  T *samples = (T *)_bytes;
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

namespace maolan::audio {
template class OSSIn<int32_t>;
template class OSSIn<int16_t>;
template class OSSIn<int8_t>;
} // namespace maolan::audio
