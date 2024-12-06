#pragma once
#include <string>

#include <maolan/audio/oss/base.hpp>

namespace maolan::audio {
class OSSIn : public OSS {
public:
  OSSIn(const std::string &name, const std::string &device,
        const int &frag = defaultFrag);
  virtual ~OSSIn();

  virtual void readhw();
  virtual void process();
  virtual size_t connected() const;

  static OSSIn *_hw;
};
} // namespace maolan::audio
