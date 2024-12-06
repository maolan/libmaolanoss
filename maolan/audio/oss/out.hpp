#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include <maolan/audio/input.hpp>
#include <maolan/audio/oss/base.hpp>

namespace maolan::audio {
class OSSOut : public OSS {
public:
  OSSOut(const std::string &name, const std::string &device,
         const int &frag = defaultFrag);
  virtual ~OSSOut();

  virtual void fetch();
  virtual void process();
  virtual void writehw();
  virtual size_t connected() const;

  static OSSOut *_hw;
};
} // namespace maolan::audio
