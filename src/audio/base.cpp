#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <sys/soundcard.h>
#include <unistd.h>

#include <maolan/audio/input.hpp>
#include <maolan/audio/oss/base.hpp>
#include <maolan/audio/output.hpp>
#include <maolan/config.hpp>
#include <maolan/constants.hpp>

using namespace maolan::audio;

static void checkError(const int &value, const std::string &message) {
  if (value == -1) {
    throw std::invalid_argument(message);
  }
}

/* Calculate frag by giving it minimal size of buffer */
static int size2frag(int x) {
  int frag = 0;
  while ((1 << frag) < x) {
    ++frag;
  }
  return frag;
}

OSS::OSS(const std::string &name, const std::string &device, const int &frag)
    : HW{name, device} {
  int error = 0;
  int tmp;
  _frag = frag;
  _format = AFMT_S32_NE;
  try {
    error = open(_device.data(), O_RDWR, 0);
    checkError(error, "open");
    _fd = error;

    _audioInfo.dev = -1;
    ioctl(_fd, SNDCTL_ENGINEINFO, &_audioInfo);
    _inputs.resize(_audioInfo.max_channels);
    _outputs.resize(_audioInfo.max_channels);

    error = ioctl(_fd, SNDCTL_DSP_GETCAPS, &(_audioInfo.caps));
    checkError(error, "SNDCTL_DSP_GETCAPS");
    if (!(_audioInfo.caps & PCM_CAP_DUPLEX)) {
      fprintf(stderr, "Device doesn't support full duplex!\n");
      exit(1);
    }

    tmp = channels();
    error = ioctl(_fd, SNDCTL_DSP_CHANNELS, &tmp);
    checkError(error, "SNDCTL_DSP_CHANNELS");

    tmp = _format;
    error = ioctl(_fd, SNDCTL_DSP_SETFMT, &tmp);
    checkError(error, "SNDCTL_DSP_SETFMT");
    if (tmp != _format) {
      std::stringstream s;
      s << _device << " doesn't support chosen sample format (";
      s << tmp << ")";
      error = 0;
      checkError(1, s.str());
    }

    tmp = Config::samplerate;
    error = ioctl(_fd, SNDCTL_DSP_SPEED, &tmp);
    checkError(error, "SNDCTL_DSP_SPEED");

    int minFrag = size2frag(sizeof(int) * channels());
    if (_frag < minFrag) {
      _frag = minFrag;
    }
    tmp = _frag;
    error = ioctl(_fd, SNDCTL_DSP_SETFRAGMENT, &tmp);
    checkError(error, "SNDCTL_DSP_SETFRAGMENT");

    error = ioctl(_fd, SNDCTL_DSP_GETOSPACE, &_bufferInfo);
    checkError(error, "SNDCTL_DSP_GETOSPACE");
  } catch (const std::invalid_argument &ex) {
    std::cerr << _type << " error: " << ex.what();
    std::cerr << ' ' << strerror(errno) << '\n';
    exit(1);
  }

  _sampleCount = _bufferInfo.bytes / sizeof(int);
  Config::audioBufferSize = _sampleCount / channels();
  _bytes = new int8_t[_bufferInfo.bytes];

  for (int i = 0; i < _audioInfo.max_channels; ++i) {
    _inputs[i] = new Input();
    _outputs[i] = new Output();
  }
}

nlohmann::json OSS::json() {
  auto data = IO::json();
  data["bits"] = sizeof(int) * 8;
  data["samplerate"] = Config::samplerate;
  return data;
}

OSS::~OSS() {
  close(_fd);
  delete _bytes;
}

size_t OSS::channels() const { return _outputs.size(); }
