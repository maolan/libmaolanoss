#pragma once
#include <maolan/midi/backend.hpp>


namespace maolan::midi
{
class OSSBackend : public Backend
{
  public:
    virtual HW * createOut(const std::string &name, const std::string &device, const size_t &size);
    virtual HW * createIn(const std::string &name, const std::string &device, const size_t &size);
    virtual const std::vector<HW *> list();
};
}
