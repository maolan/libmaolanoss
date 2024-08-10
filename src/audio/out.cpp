#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/soundcard.h>
#include <unistd.h>

#include <maolan/audio/oss/out.hpp>
#include <maolan/audio/output.hpp>
#include <maolan/constants.hpp>

using namespace maolan::audio;

template <typename T>
OSSOut<T>::OSSOut(const std::string &name, const std::string &device,
                  const int &frag)
    : OSS{name, device, sizeof(T), frag} {
  _type = "AudioOSSOut";
}

template <typename T> void OSSOut<T>::fetch() {
  OSS::fetch();
  for (size_t i = 0; i < channels(); ++i) {
    _outputs[i]->buffer(_inputs[i]->pull());
  }
}

template <typename T> void OSSOut<T>::process() {
  T *samples = (T *)_bytes;
  auto chs = channels();
  for (size_t channel = 0; channel < chs; ++channel) {
    auto out = _outputs[channel];
    if (out == nullptr) {
      for (std::size_t i = 0; i < Config::audioBufferSize; ++i) {
        samples[i * chs + channel] = 0;
      }
    } else {
      const auto &buffer = out->buffer();
      const auto &data = buffer->data();
      for (size_t i = 0; i < buffer->size(); ++i) {
        auto &sample = data[i];
        if (sample <= -1.0) {
          sample = -1.0;
        } else if (sample >= 1.0) {
          sample = 1.0;
        }
        samples[i * chs + channel] = sample * floatMaxInt;
      }
    }
  }
}

template <class T> void OSSOut<T>::writehw() {
  write(_fd, _bytes, _bufferInfo.bytes);
}

namespace maolan::audio {
template class OSSOut<int32_t>;
template class OSSOut<int16_t>;
template class OSSOut<int8_t>;
} // namespace maolan::audio
