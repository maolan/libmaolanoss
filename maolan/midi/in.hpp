#pragma once
#include <string>

#include "maolan/midi/base.hpp"

namespace maolan::midi
{
class OSSIn : public OSS
{
public:
  OSSIn(const std::string &name, const std::string &device);

  virtual void setup();
  virtual void fetch();
  virtual void readhw();
  virtual void process();
  virtual Buffer pull();
};
} // namespace maolan::midi
