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

  void Buffer::Lock()
  {
    if (gSync)
    {
      m_Device.DeleteSync(gSync);
    }
    gSync = m_Device.FenceSync();
  }

  void Buffer::Wait()
  {
    if ((void*)gSync)
    {
      while (true)
      {
        FenceSignal waitReturn = m_Device.ClientWaitSync(gSync, 1);
        if (waitReturn == FENCE_ALREADY_SIGNALED || waitReturn == FENCE_CONDITION_SATISFIED)
          return;
      }
    }
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

  Hash GetHash(ShaderDescriptor descriptor)
  {
    Hasher hasher;
    for (const auto& c : descriptor.Path)
      hasher.hash(c);
    for (const auto& c : descriptor.PrependSource)
      hasher.hash(c);
    return hasher.get();
  }

}

