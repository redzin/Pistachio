#include "pch.h"
#include "Shader_OpenGL.h"

#include <fstream>

namespace Pistacio
{
	constexpr auto VERTEX_SHADER_DECLARATION = "_SHADER_TYPE_VERTEX_SHADER";
	constexpr auto PIXEL_SHADER_DECLARATION = "_SHADER_TYPE_PIXEL_SHADER";
	constexpr auto FRAGMENT_SHADER_DECLARATION = "_SHADER_TYPE_FRAGMENT_SHADER";


	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == VERTEX_SHADER_DECLARATION) return GL_VERTEX_SHADER;
		if (type == FRAGMENT_SHADER_DECLARATION || type == PIXEL_SHADER_DECLARATION) return GL_FRAGMENT_SHADER;

		PSTC_CORE_ASSERT(false, "Invalid shader type!");

		return 0;

	}

	static std::string ShaderStringFromType(GLenum type)
	{
		if (type == GL_VERTEX_SHADER) return "Vertex";
		if (type == GL_FRAGMENT_SHADER) return "Fragment/Pixel";

		PSTC_CORE_ASSERT(false, "Invalid shader type!");

		return "";

	}

	static std::string ShaderDefStringFromType(GLenum type)
	{
		if (type == GL_VERTEX_SHADER) return VERTEX_SHADER_DECLARATION;
		if (type == GL_FRAGMENT_SHADER) return FRAGMENT_SHADER_DECLARATION;

		PSTC_CORE_ASSERT(false, "Invalid shader type!");

		return "";

	}

	Shader_OpenGL::Shader_OpenGL(const std::string& filePath)
	{
		Name = remove_extension(base_name(filePath));

		auto shaderSrc = ReadFile(filePath);
		auto shaderTypes = GetShaderTypes(shaderSrc);
		Compile(shaderSrc, shaderTypes);

	}

	Shader_OpenGL::Shader_OpenGL(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) : m_RendererId(0), Name(name)
	{
		std::string completeSrc = "#ifdef ";
		completeSrc += VERTEX_SHADER_DECLARATION;
		completeSrc += "\n" + vertexSource;
		completeSrc += "#endif\n";
		completeSrc += "#ifdef ";
		completeSrc += FRAGMENT_SHADER_DECLARATION;
		completeSrc += "\n" + fragmentSource;
		std::vector<GLenum> shaderTypes = GetShaderTypes(completeSrc);
		Compile(completeSrc, shaderTypes);
	}

	Shader_OpenGL::~Shader_OpenGL()
	{
		glDeleteProgram(m_RendererId);
	}

	const std::string& Shader_OpenGL::GetName() const
	{
		return Name;
	}

	const RendererID Shader_OpenGL::GetRendererID() const
	{
		return m_RendererId;
	}

	std::string Shader_OpenGL::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				PSTC_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			PSTC_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::vector<GLenum> Shader_OpenGL::GetShaderTypes(const std::string& source)
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

	void Shader_OpenGL::Compile(const std::string& shaderSrc, std::vector<GLenum>& shaderTypes)
	{
		
		PSTC_CORE_ASSERT(shaderTypes.size() <= 3, "Cannot compile more than 3 shaders per shader program!");

		std::array<GLuint, 3> shaders;

		int shaderIndex = 0;
		for (GLenum shaderType : shaderTypes)
		{
			std::string shaderDefString = "#version 430 core \n #define " + ShaderDefStringFromType(shaderType) + "\n";
			std::string completeSrc = shaderDefString + shaderSrc;

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

				return;
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

			return;
		}

		for (GLuint shader : shaders)
			glDetachShader(program, shader);

		m_RendererId = program;
	}

}

