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
    Pistachio::RendererID id_temp = RendererID;
    m_Device.DeleteBuffer(*this);
    PSTC_INFO("GPU mapped buffer ({0}) unmapped and destroyed!", id_temp);
  }

  Attachment::~Attachment()
  {
    m_Device.DeleteAttachment(*this);
  }

}

