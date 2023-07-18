#pragma once
#include <maolan/audio/backend.hpp>


namespace maolan::audio
{
class OSSBackend : public Backend
{
  public:
    virtual HW * createOut(const std::string &name, const std::string &device, const size_t &size);
    virtual HW * createIn(const std::string &name, const std::string &device, const size_t &size);
    virtual const std::vector<HW *> list();
};
}
