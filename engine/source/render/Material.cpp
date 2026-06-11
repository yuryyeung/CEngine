#include "render/Material.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"

namespace CEngine
{
    void Material::SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
    {
        m_shaderProgram = shaderProgram;
    }

    void Material::SetParam(const std::string& name, float value)
    {
        m_floatParams[name] = value;
    }

    void Material::SetParam(const std::string &name, float v0, float v1)
    {
        m_float2Params[name] = {v0, v1};
    }

    void Material::SetParam(const std::string &name, const std::shared_ptr<Texture> texture)
    {
        m_textures[name] = texture;
    }

    void Material::Bind()
    {
        if (!m_shaderProgram)
        {
            return;
        }

        m_shaderProgram->Bind();
        for (auto& param : m_floatParams)
        {
            m_shaderProgram->SetUniform(param.first, param.second);
        }

        for (auto& param : m_float2Params)
        {
            m_shaderProgram->SetUniform(param.first, param.second.first, param.second.second);
        }

        for (auto& parm : m_textures)
        {
            m_shaderProgram->SetTexture(parm.first, parm.second.get());
        }
    }

    ShaderProgram *Material::GetShaderProgram()
    {
        return m_shaderProgram.get();
    }
}