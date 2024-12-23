#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <sys/soundcard.h>

#include <maolan/audio/hw.hpp>
#include <maolan/audio/io.hpp>
#include <maolan/constants.hpp>

namespace maolan::audio {
class OSS : public HW {
public:
  OSS(const std::string &name, const std::string &device,
      const int &fragSize = defaultFrag);
  ~OSS();

  virtual size_t channels() const;
  virtual nlohmann::json json();

protected:
  int8_t *_bytes;
  int _format;
  int _frag;
  int _sampleCount;
  oss_audioinfo _audioInfo;
  audio_buf_info _bufferInfo;
};
} // namespace maolan::audio
