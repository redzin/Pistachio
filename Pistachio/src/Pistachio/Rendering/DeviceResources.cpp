#include "pch.h"

namespace Pistachio
{
  //Buffer::~Buffer()
  //{
  //  m_Device.DeleteBuffer(*this);
  //}

  Sampler::~Sampler()
  {
    m_Device.DeleteSampler(*this);
  }

  Buffer::~Buffer()
  {
    m_Device.DeleteBuffer(*this);
  }

  Attachment::~Attachment()
  {
    m_Device.DeleteAttachment(*this);
  }

}

