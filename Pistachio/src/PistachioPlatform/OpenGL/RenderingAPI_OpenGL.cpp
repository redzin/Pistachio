#include "pch.h"
#include "RenderingAPI_OpenGL.h"

namespace Pistachio
{

  void RenderingAPI_OpenGL::SetClearColor(const glm::vec4& color) const
  {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void RenderingAPI_OpenGL::SetClearDepth(const float depth) const
  {
    glClearDepth(depth);
  }

	void RenderingAPI_OpenGL::SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
	{
		glScissor(x, y, width, height);
	}

  void RenderingAPI_OpenGL::Clear(FramebufferMask mask) const
  {
		glClear(mask);
  }

  void RenderingAPI_OpenGL::Enable(Capability capability) const
  {
    glEnable(static_cast<GLenum>(capability));
  }

  void RenderingAPI_OpenGL::Enable(std::initializer_list<Capability> capabilities) const
  {
    for (auto capability : capabilities)
    {
      glEnable(static_cast<GLenum>(capability));
    }
  }

	std::string GLErrorTypeToString(GLenum Type)
	{
		switch (Type)
		{
		case GL_DEBUG_TYPE_ERROR:								return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "Depecrated Behaviour";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "Undefined Behaviour";
		case GL_DEBUG_TYPE_PORTABILITY:					return "Portability Issue";
		case GL_DEBUG_TYPE_PERFORMANCE:					return "Performance Issue";
		case GL_DEBUG_TYPE_MARKER:							return "Command Stream";
		case GL_DEBUG_TYPE_PUSH_GROUP:					return "Group Push";
		case GL_DEBUG_TYPE_POP_GROUP:						return "Group Pop";
		case GL_DEBUG_TYPE_OTHER:								return "Unknown Error";
		}
	}

	std::string GLSourceTypeToString(GLenum source)
	{
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:							return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "Window System";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "Shader Compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:			return "Third Party";
		case GL_DEBUG_SOURCE_APPLICATION:			return "Application";
		case GL_DEBUG_SOURCE_OTHER:						return "Unknown Source";
		}
	}

	void RenderingAPI_OpenGL::EnableDebugOutput() const
	{
#ifdef PSTC_DIST
		return;
#endif

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source,
			GLenum Type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
			{
				//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
				//	(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
				//	type, severity, message);

#ifdef PSTC_OPENGL_DEBUG
				std::string msg = std::string(message, length);

				switch (severity)
				{
				case GL_DEBUG_SEVERITY_HIGH:
					PSTC_CORE_FATAL("OpenGL {0} from {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					PSTC_CORE_ERROR("OpenGL {0} from {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					PSTC_CORE_WARN("OpenGL {0} in {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					PSTC_CORE_INFO("OpenGL {0} from {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				}
				return;
#endif

				switch (Type)
				{
				case GL_DEBUG_TYPE_MARKER:			return;
				case GL_DEBUG_TYPE_PUSH_GROUP:	return;
				case GL_DEBUG_TYPE_POP_GROUP:		return;
				case GL_DEBUG_TYPE_OTHER:				return;
				}

				std::string msg = std::string(message, length);

				switch (severity)
				{
				case GL_DEBUG_SEVERITY_HIGH:
					PSTC_CORE_FATAL("OpenGL {0} from {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					PSTC_CORE_ERROR("OpenGL {0} from {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					PSTC_CORE_WARN("OpenGL {0} in {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					PSTC_CORE_INFO("OpenGL {0} from {1}:\n\t{2}", GLErrorTypeToString(Type), GLSourceTypeToString(source), msg);
					break;
				}

			}, 0);
	}

  void RenderingAPI_OpenGL::Disable(Capability capability) const
  {
    glDisable(static_cast<GLenum>(capability));
  }

  void RenderingAPI_OpenGL::Disable(std::initializer_list<Capability> capabilities) const
  {
    for (auto capability : capabilities)
    {
      glDisable(static_cast<GLenum>(capability));
    }
  }

  void RenderingAPI_OpenGL::DepthMask(bool enable) const
  {
    glDepthMask(enable);
  }

  void RenderingAPI_OpenGL::SetDepthFunc(DepthFunction depthFunc) const
  {
    glDepthFunc(static_cast<GLenum>(depthFunc));
  }


  void RenderingAPI_OpenGL::SetBlendFunction(BlendFunction sfactor, BlendFunction dfactor) const
  {
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
  }

  int RenderingAPI_OpenGL::InitExternalProcAddress(GetRenderAPIProcAddress addressGetPtr) const
  {
		return gladLoadGLLoader((GLADloadproc)addressGetPtr);
  }

  void RenderingAPI_OpenGL::DrawIndexed(const RendererID& inputLayoutID, uint32_t indexCount, uint32_t primitiveType, uint32_t indexBufferBaseType, uint32_t Offset) const
  {
		glBindVertexArray(inputLayoutID);
    glDrawElements(primitiveType, indexCount, indexBufferBaseType, (void*)Offset);
  }

  void RenderingAPI_OpenGL::DrawIndexedInstanced(const RendererID& inputLayoutID, uint32_t indexCount, uint32_t instanceCount, uint32_t primitiveType, uint32_t indexBufferBaseType, uint32_t Offset) const
  {
		glBindVertexArray(inputLayoutID);
		glDrawElementsInstanced(primitiveType, indexCount, indexBufferBaseType, (void*)Offset, instanceCount);
  }

	void RenderingAPI_OpenGL::Draw(const RendererID& inputLayoutID, uint32_t vertexCount, uint32_t primitiveType, uint32_t offset) const
	{
		glBindVertexArray(inputLayoutID);
		glDrawArrays(primitiveType, offset, vertexCount);
	}

  void RenderingAPI_OpenGL::SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height) const
  {
    glViewport(xoffset, yoffset, width, height);
  };



  /////////////////////////////////////////////////////////////////////////
  // Shaders
	/////////////////////////////////////////////////////////////////////////
	constexpr auto VERTEX_SHADER_DECLARATION = "_SHADER_TYPE_VERTEX_SHADER";
	constexpr auto PIXEL_SHADER_DECLARATION = "_SHADER_TYPE_PIXEL_SHADER";
	constexpr auto FRAGMENT_SHADER_DECLARATION = "_SHADER_TYPE_FRAGMENT_SHADER";

	static GLenum ShaderTypeFromString(const std::string& Type)
	{
		if (Type == VERTEX_SHADER_DECLARATION) return GL_VERTEX_SHADER;
		if (Type == FRAGMENT_SHADER_DECLARATION || Type == PIXEL_SHADER_DECLARATION) return GL_FRAGMENT_SHADER;

		PSTC_CORE_ASSERT(false, "Invalid shader type!");

		return 0;

	}

	static std::string ShaderStringFromType(GLenum Type)
	{
		if (Type == GL_VERTEX_SHADER) return "Vertex";
		if (Type == GL_FRAGMENT_SHADER) return "Fragment/Pixel";

		PSTC_CORE_ASSERT(false, "Invalid shader type!");

		return "";

	}

	static std::string ShaderDefStringFromType(GLenum Type)
	{
		if (Type == GL_VERTEX_SHADER) return VERTEX_SHADER_DECLARATION;
		if (Type == GL_FRAGMENT_SHADER) return FRAGMENT_SHADER_DECLARATION;

		PSTC_CORE_ASSERT(false, "Invalid shader type!");

		return "";

	}

	static std::vector<GLenum> GetShaderTypes(const std::string& source)
	{
		std::vector<GLenum> shaderTypes;

		size_t pos = source.find_first_of("_SHADER_TYPE", 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);

			std::string def = source.substr(pos, eol - pos);
			shaderTypes.push_back(ShaderTypeFromString(def));

			pos = source.find("_SHADER_TYPE", eol);
		}

		return shaderTypes;
	}

	static uint32_t CompileShader(const std::string& shaderSrc, std::vector<GLenum>& shaderTypes, const std::string& prependSource = "")
	{

		PSTC_CORE_ASSERT(shaderTypes.size() <= 2, "Cannot compile more than 3 shaders per shader program!");

		std::array<GLuint, 2> shaders{0,0};

		int shaderIndex = 0;
		for (GLenum shaderType : shaderTypes)
		{
			//std::string shaderDefString = "#version 430 core\n#extension GL_ARB_buffer_storage : require\n#define " + ShaderDefStringFromType(shaderType) + "\n";
			std::string shaderDefString = "#version 430 core\n#define " + ShaderDefStringFromType(shaderType) + "\n";
			std::string completeSrc = shaderDefString + prependSource + shaderSrc;

			GLuint shader = glCreateShader(shaderType);
			const GLchar* source = (const GLchar*)(completeSrc).c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);

				PSTC_CORE_ERROR("{0}", infoLog.data());
				PSTC_CORE_ASSERT(false, "{0} shader compilation error!", ShaderStringFromType(shaderType));

				return 0;
			}
			shaders[shaderIndex++] = shader;
		}


		GLuint program = glCreateProgram();
		for (GLuint shader : shaders)
			glAttachShader(program, shader);
		glLinkProgram(program);


		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (GLuint shader : shaders)
				glDeleteShader(shader);

			PSTC_CORE_ERROR("{0}", infoLog.data());
			PSTC_CORE_ASSERT(false, "{0} shader linking error!");

			return 0;
		}

		for (GLuint shader : shaders)
			glDetachShader(program, shader);

		return program;
	}

	RendererID RenderingAPI_OpenGL::CreateShader(ShaderDescriptor shaderDescriptor) const
	{
		auto shaderSrc = ReadFile(shaderDescriptor.Path);
		auto shaderTypes = GetShaderTypes(shaderSrc);
		auto rendererId = CompileShader(shaderSrc, shaderTypes, shaderDescriptor.PrependSource);

		PSTC_CORE_ASSERT(rendererId, "Failed to compile shader!");

		return rendererId;
	}

	RendererID RenderingAPI_OpenGL::CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) const
	{
		std::string completeSrc = "#ifdef ";
		completeSrc += VERTEX_SHADER_DECLARATION;
		completeSrc += "\n" + vertexSrc;
		completeSrc += "#endif\n";
		completeSrc += "#ifdef ";
		completeSrc += FRAGMENT_SHADER_DECLARATION;
		completeSrc += "\n" + fragmentSrc;
		auto shaderTypes = GetShaderTypes(completeSrc);
		auto rendererId = CompileShader(completeSrc, shaderTypes);

		PSTC_CORE_ASSERT(rendererId, "Failed to compile shader!");

		return rendererId;
	}

	void RenderingAPI_OpenGL::DeleteShader(RendererID& shader) const
	{
		glDeleteProgram(shader);
	}

	void RenderingAPI_OpenGL::UseShaderProgram(const RendererID& shader) const
	{
		glUseProgram(shader);
	}



  /////////////////////////////////////////////////////////////////////////
  // Buffers
	/////////////////////////////////////////////////////////////////////////

	//RendererID RenderingAPI_OpenGL::CreateBuffer(const BufferDescriptor& descriptor) const
	//{
	//	RendererID rendererId;
	//	glCreateBuffers(1, &rendererId);
	//	glNamedBufferData(rendererId, descriptor.Size, nullptr, descriptor.Usage);
	//
	//	return rendererId;
	//}
	//
	//RendererID RenderingAPI_OpenGL::CreateBuffer(const BufferDescriptor& descriptor, const void* data) const
	//{
	//	RendererID rendererId;
	//	glCreateBuffers(1, &rendererId);
	//	glNamedBufferData(rendererId, descriptor.Size, data, descriptor.Usage);
	//
	//	return rendererId;
	//}

	RendererID RenderingAPI_OpenGL::CreatePersistenBuffer(const BufferDescriptor descriptor) const
	{
		RendererID rendererId;
		glCreateBuffers(1, &rendererId);
		glNamedBufferStorage(rendererId, descriptor.Size, nullptr, descriptor.Flags);
		return rendererId;
	}

	RendererID RenderingAPI_OpenGL::CreatePersistenBuffer(const BufferDescriptor descriptor, const void* data) const
	{
		RendererID rendererId;
		glCreateBuffers(1, &rendererId);
		glNamedBufferStorage(rendererId, descriptor.Size, data, descriptor.Flags);
		return rendererId;
	}

	void* RenderingAPI_OpenGL::MapBuffer(RendererID rendererId, uint32_t offset, uint32_t size, uint32_t accessFlags) const
	{
		return glMapNamedBufferRange(rendererId, offset, size, accessFlags);
	}

	void RenderingAPI_OpenGL::UnmapBuffer(RendererID rendererId) const
	{
		glUnmapNamedBuffer(rendererId);
	}

	//void RenderingAPI_OpenGL::UploadBufferData(const RendererID& rendererId, const BufferDescriptor& buffer, uint32_t Offset, const void* data) const
	//{
	//	glNamedBufferSubData(rendererId, Offset, buffer.Size, data);
	//}

	//void RenderingAPI_OpenGL::UploadBufferData(const RendererID& rendererId, uint32_t size, uint32_t offset, const void* data) const
	//{
	//	glNamedBufferSubData(rendererId, offset, size, data);
	//}

	void RenderingAPI_OpenGL::DeleteBuffer(RendererID& rendererId) const
	{
		glDeleteBuffers(1, &rendererId);
	}

	void RenderingAPI_OpenGL::BindBuffer(BufferBindingTarget bindingTarget, const RendererID& buffer, const uint32_t& bindingSlot) const
	{
		glBindBufferBase(bindingTarget, bindingSlot, buffer);
	}

	//void RenderingAPI_OpenGL::SetBufferUniformBinding(const RendererID& buffer, const uint32_t& binding) const
	//{
	//	glBindBufferRange(GL_UNIFORM_BUFFER, binding, buffer, offset, size);
	//}

	DeviceMemoryFence RenderingAPI_OpenGL::FenceSync() const
	{
		GLsync gSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		return gSync;
	}

	void RenderingAPI_OpenGL::DeleteSync(DeviceMemoryFence gSync) const
	{
		glDeleteSync(static_cast<GLsync>(gSync));
	}

	FenceSignal RenderingAPI_OpenGL::ClientWaitSync(DeviceMemoryFence gSync, uint64_t timeout) const
	{
		return glClientWaitSync(static_cast<GLsync>(gSync), GL_SYNC_FLUSH_COMMANDS_BIT, timeout);
	}




	/////////////////////////////////////////////////////////////////////////
	// Input Layout / Vertex Arrays
	/////////////////////////////////////////////////////////////////////////

	void BindVertexBufferToVertexArray(const RendererID& vertexBufferRendererId, const uint32_t divisor, const BufferMemoryLayout& layout, uint32_t bindingFirst, uint32_t bindingCount)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferRendererId);

		for (const auto& entry : layout)
		{
			for (int binding = bindingFirst; binding < bindingFirst + bindingCount; binding++)
			{
				glEnableVertexAttribArray(binding);

				uint32_t baseType = ShaderDataTypeToOpenGLBaseType(entry.Type);
				uint32_t componentCount = glm::min(4u, DataTypeComponentCount(entry.Type));
				uint32_t stride = layout.GetStride();
				
				glVertexAttribPointer(
					binding,
					componentCount,
					baseType,
					entry.Normalized ? GL_TRUE : GL_FALSE,
					stride,
					(const void*)(entry.Offset + (binding-bindingFirst) * 16)
				);
				glVertexAttribDivisor(binding, divisor);
			}
		}
	}

	RendererID RenderingAPI_OpenGL::CreateAttributeLayout(const AttributeLayoutDescriptor& attributeDescriptor) const
	{
		RendererID rendererId;
		glCreateVertexArrays(1, &rendererId);
		glBindVertexArray(rendererId);

		int index = 0;
		for (const auto& attribute : attributeDescriptor)
		{
			BindVertexBufferToVertexArray(attribute.Buffer->RendererID, attribute.Buffer->Descriptor.AttributeDivisor, attribute.MemoryLayout, attribute.BindingFirst, attribute.BindingCount);
		}

		glBindVertexArray(0);

		return rendererId;
	}

	RendererID RenderingAPI_OpenGL::CreateAttributeLayout(RendererID indexBuffer, const AttributeLayoutDescriptor& attributeDescriptor) const
	{
		RendererID rendererId;
		glCreateVertexArrays(1, &rendererId);
		glBindVertexArray(rendererId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		for (const auto& attribute : attributeDescriptor)
		{
			BindVertexBufferToVertexArray(attribute.Buffer->RendererID, attribute.Buffer->Descriptor.AttributeDivisor, attribute.MemoryLayout, attribute.BindingFirst, attribute.BindingCount);
		}

		glBindVertexArray(0);

		return rendererId;

	}

	//RendererID RenderingAPI_OpenGL::CreateAttributeLayout(RendererID indexBuffer, const std::unordered_map<std::string, Ref<BufferResource>>& vertexBuffers, const std::unordered_map<std::string, Ref<BufferResource>>& vertexBuffers) const
	//{
	//	RendererID rendererId;
	//	glCreateVertexArrays(1, &rendererId);
	//	glBindVertexArray(rendererId);
	//
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//
	//	int index = 0;
	//	for (const auto& [name, vertexBuffer] : vertexBuffers)
	//	{
	//		BindVertexBufferToVertexArray(vertexBuffer->Buffer->RendererID, vertexBuffer->Buffer->Descriptor.AttributeDivisor, vertexBuffer->Layout, index);
	//	}
	//	for (const auto& [name, vertexBuffer] : vertexBuffers)
	//	{
	//		BindVertexBufferToVertexArray(vertexBuffer->Buffer->RendererID, vertexBuffer->Buffer->Descriptor.AttributeDivisor, vertexBuffer->Layout, index);
	//	}
	//
	//	glBindVertexArray(0);
	//
	//	return rendererId;
	//}

	void RenderingAPI_OpenGL::DeleteInputLayout(RendererID& rendererId) const
	{
		glDeleteVertexArrays(1, &rendererId);
	}



	/////////////////////////////////////////////////////////////////////////
	// Textures
	/////////////////////////////////////////////////////////////////////////

	static void UploadSamplerDataInternal(RendererID rendererID, const SamplerDescriptor& samplerDescriptor, const Image& image, const uint32_t layer = 0)
	{
		switch (samplerDescriptor.TextureDescriptor.Type)
		{
		case TEXTURE_1D:
			glTextureSubImage1D(
				rendererID,
				0, 0,
				image.Width,
				static_cast<GLenum>(image.Format),
				static_cast<GLenum>(image.DataType),
				image.Data
			);
			glGenerateTextureMipmap(rendererID);
			break;
		case TEXTURE_1D_ARRAY:
			PSTC_CORE_ASSERT(false, "Texture 1D array not yet implemented!");
			break;
		case TEXTURE_BUFFER:
			PSTC_CORE_ASSERT(false, "Texture buffer not yet implemented!");
			break;
		case TEXTURE_2D:
		case TEXTURE_2D_MULTISAMPLE:
			glTextureSubImage2D(
				rendererID,
				0, 0, 0,
				image.Width, image.Height,
				static_cast<GLenum>(image.Format),
				static_cast<GLenum>(image.DataType),
				image.Data
			);
			glGenerateTextureMipmap(rendererID);
			break;
		case TEXTURE_3D:
			PSTC_CORE_ASSERT(false, "Texture 3D not yet implemented!");
			break;
		case TEXTURE_2D_ARRAY:
		case TEXTURE_2D_MULTISAMPLE_ARRAY:
			glTextureSubImage3D(
				rendererID,
				0, 0, 0, layer,
				image.Width, image.Height, 1,
				static_cast<GLenum>(image.Format),
				static_cast<GLenum>(image.DataType),
				image.Data
			);
			//glGenerateTextureMipmap(sampler.RendererID);
			break;
		}
	}

	RendererID RenderingAPI_OpenGL::CreateSampler(const std::string& path, SamplerDescriptor& samplerDescriptor) const
	{
		Image image = LoadImageFromFile(path);

		if (image.Format == IMAGE_FORMAT_RGB)
		{
			samplerDescriptor.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGB8;
		}
		else
		{
			samplerDescriptor.TextureDescriptor.InternalFormat = INTERNAL_FORMAT_RGBA8;
		}

		samplerDescriptor.TextureDescriptor.SizeX = image.Width;
		samplerDescriptor.TextureDescriptor.SizeY = image.Height;

		RendererID rendererId;

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX, samplerDescriptor.TextureDescriptor.SizeY);

		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, samplerDescriptor.MinFilter);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, samplerDescriptor.MagFilter);

		UploadSamplerDataInternal(rendererId, samplerDescriptor, image);

		return rendererId;
	}

	RendererID RenderingAPI_OpenGL::CreateSampler(const SamplerDescriptor& samplerDescriptor) const
	{
		PSTC_CORE_ASSERT(samplerDescriptor.TextureDescriptor.Type != TEXTURE_BUFFER, "Sampler cannot be of type TEXTURE_BUFFER!");

		RendererID rendererId;
		glCreateTextures(samplerDescriptor.TextureDescriptor.Type, 1, &rendererId);

		switch (samplerDescriptor.TextureDescriptor.Type)
		{
		case TEXTURE_1D:
			glTextureStorage1D(rendererId, samplerDescriptor.MipLevels, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX);
		case TEXTURE_2D:
			glTextureStorage2D(rendererId, samplerDescriptor.MipLevels, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX, samplerDescriptor.TextureDescriptor.SizeY);
			break;
		case TEXTURE_2D_MULTISAMPLE:
			glTextureStorage2DMultisample(rendererId, samplerDescriptor.TextureDescriptor.Samples, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX, samplerDescriptor.TextureDescriptor.SizeY, GL_FALSE);
			break;
		case TEXTURE_3D:
		case TEXTURE_2D_ARRAY:
			glTextureStorage3D(rendererId, samplerDescriptor.MipLevels, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX, samplerDescriptor.TextureDescriptor.SizeY, samplerDescriptor.TextureDescriptor.Layers);
			break;
		case TEXTURE_2D_MULTISAMPLE_ARRAY:
			glTextureStorage3DMultisample(rendererId, samplerDescriptor.TextureDescriptor.Samples, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX, samplerDescriptor.TextureDescriptor.SizeY, samplerDescriptor.TextureDescriptor.SizeZ, GL_FALSE);
			break;
		case TEXTURE_1D_ARRAY:
			glTextureStorage2D(rendererId, samplerDescriptor.MipLevels, samplerDescriptor.TextureDescriptor.InternalFormat, samplerDescriptor.TextureDescriptor.SizeX, samplerDescriptor.TextureDescriptor.Layers);
			break;
		}

		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, samplerDescriptor.MagFilter);
		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, samplerDescriptor.MinFilter);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return rendererId;
	}

	void RenderingAPI_OpenGL::UploadSamplerData(const Sampler& sampler, const Image& image, const uint32_t layer) const
	{
		UploadSamplerDataInternal(sampler.RendererID, sampler.SamplerDescriptor, image, layer);
	}

	void RenderingAPI_OpenGL::BindSampler(const RendererID& rendererId, uint32_t unit) const
	{
		glBindTextureUnit(unit, rendererId);
	}

	void RenderingAPI_OpenGL::DeleteSampler(RendererID& id) const
	{
		glDeleteTextures(1, &id);
	}


	/////////////////////////////////////////////////////////////////////////
	// Framebuffers
	/////////////////////////////////////////////////////////////////////////

	RendererID RenderingAPI_OpenGL::CreateAttachment(const TextureDescriptorBase& descriptor) const
	{
		RendererID rendererId;

		if (descriptor.Samples > 1)
		{
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &rendererId);
			glTextureStorage2DMultisample(rendererId, descriptor.Samples, descriptor.InternalFormat, descriptor.SizeX, descriptor.SizeY, GL_FALSE);
		}
		else
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
			glTextureStorage2D(rendererId, 1, descriptor.InternalFormat, descriptor.SizeX, descriptor.SizeY);
		}

		return rendererId;
	}

	void RenderingAPI_OpenGL::DeleteAttachment(RendererID& id) const
	{
		glDeleteTextures(1, &id);
	}

	RendererID RenderingAPI_OpenGL::CreateFramebuffer() const
	{
		RendererID rendererId;
		glCreateFramebuffers(1, &rendererId);
		
		return rendererId;
	}

	void RenderingAPI_OpenGL::CheckFramebufferCompleteness(const RendererID& rendererId) const
	{
		GLenum framebufferComplete = glCheckNamedFramebufferStatus(rendererId, GL_FRAMEBUFFER);
		PSTC_CORE_ASSERT(framebufferComplete == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");
	}

	void RenderingAPI_OpenGL::InvalidateFramebuffer(const RendererID framebufferId, uint32_t invalidAttachmentCount, const AttachmentPoint* attachmentPointArrayData) const
	{
		glInvalidateNamedFramebufferData(framebufferId, invalidAttachmentCount, attachmentPointArrayData);
	}

	void RenderingAPI_OpenGL::BindAttachment(const RendererID& framebufferId, const RendererID& attachmentId, const AttachmentPoint& attachmentPoint) const
	{
		glNamedFramebufferTexture(framebufferId, attachmentPoint, attachmentId, 0);
	}

	void RenderingAPI_OpenGL::UnbindFramebuffer() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingAPI_OpenGL::BindFramebuffer(const RendererID& rendererId) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);
	}

	void RenderingAPI_OpenGL::DeleteFramebuffer(RendererID& rendererId) const
	{
		glDeleteFramebuffers(1, &rendererId);

	}

	void RenderingAPI_OpenGL::BlitFramebuffer(
		const RendererID& sourceId, uint32_t sourceWidth, uint32_t sourceHeight,
		const RendererID& destId, uint32_t destWidth, uint32_t destHeight,
		TextureFilter filter, FramebufferMask mask) const
	{
		glBlitNamedFramebuffer(
			sourceId, destId,
			0, 0,
			sourceWidth,
			sourceHeight,
			0, 0,
			destWidth,
			destHeight,
			mask, filter
		);
	}


}

