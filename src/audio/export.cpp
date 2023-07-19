#include <vector>
#include <sys/nv.h>
#include <sys/sndstat.h>
#include <fcntl.h>

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


extern "C" std::vector<HW *> * listAudio()
{
  int fd;
  struct sndstioc_nv_arg arg;
  const nvlist_t * const *di;
  size_t i, nitems;
  nvlist_t *nvl;
  HW *hw;
  auto *devices = new std::vector<HW *>;

  /* Open sndstat node in read-only first */
  fd = open("/dev/sndstat", O_RDONLY);

  if (ioctl(fd, SNDSTIOC_REFRESH_DEVS, NULL))
  {
    throw std::invalid_argument{"SNDSTIOC_REFRESH_DEVS"};
  }

  /* Get the size of snapshot, when nbytes = 0 */
  arg.nbytes = 0;
  arg.buf = NULL;
  if (ioctl(fd, SNDSTIOC_GET_DEVS, &arg))
  {
    throw std::invalid_argument{"SNDSTIOC_GET_DEVS"};

  }

  /* Get snapshot data */
  arg.buf = malloc(arg.nbytes);
  if (arg.buf == NULL)
  {
    throw std::invalid_argument{"malloc"};
  }
  if (ioctl(fd, SNDSTIOC_GET_DEVS, &arg))
  {
    throw std::invalid_argument{"SNDSTIOC_GET_DEVS"};
  }

  /* Deserialize the nvlist stream */
  nvl = nvlist_unpack(arg.buf, arg.nbytes, 0);
  free(arg.buf);

  /* Get DSPs array */
  di = nvlist_get_nvlist_array(nvl, SNDST_DSPS, &nitems);
  for (i = 0; i < nitems; ++i)
  {
    std::string devnode;
    std::string desc;
    devnode = nvlist_get_string(di[i], SNDST_DSPS_DEVNODE);
    desc = nvlist_get_string(di[i], SNDST_DSPS_DESC);
    hw = new HW(desc, "/dev/" + devnode);
    devices->push_back(hw);
  }

  nvlist_destroy(nvl);
  return devices;
}
