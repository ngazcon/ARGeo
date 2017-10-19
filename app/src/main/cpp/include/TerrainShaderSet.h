#pragma once

#include "SceneMode.h"
#include "ShaderProgram.h"

#include <string>
#include <map>

namespace argeo
{
    class  ITerrain;
    struct TerrainShader
    {
        TerrainShader(
                const SceneMode& mode,
                ShaderProgram* program)
        : mode(mode)
        , program(program)
        { }

        SceneMode mode;
        ShaderProgram* program;
    };

    class TerrainShaderSet
    {
    public:
        TerrainShaderSet(
                const std::string& base_vertex_shader,
                const std::string& base_fragment_shader);
        ~TerrainShaderSet();

        ShaderProgram* get_shader_program(
                ITerrain* terrain,
                FrameState* frame_state,
                const bool& use_double_precision = false,
                const bool& use_web_mercator_projection = false);

        ShaderProgram* get_pick_shader_program(
                ITerrain* terrain,
                FrameState* frame_state,
                const bool& use_double_precision = false,
                const bool& use_web_mercator_projection = false);

    private:
        std::string get_position_mode(const SceneMode& mode);
        std::string get_2D_Y_position_fraction(const bool& use_web_mercator_projection);



    private:
        std::string m_base_vertex_source;
        std::string m_base_fragment_source;

        std::map<SceneMode, TerrainShader*> m_shaders_by_flag;
        std::map<SceneMode, TerrainShader*> m_pick_shaders_by_flag;
    };
}

