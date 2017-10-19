#include "TerrainShaderSet.h"
#include "ITerrain.h"

namespace argeo
{

    TerrainShaderSet::TerrainShaderSet(
            const std::string& base_vertex_shader,
            const std::string& base_fragment_shader)
    : m_base_vertex_source(base_vertex_shader)
    , m_base_fragment_source(base_fragment_shader)
    { }

    TerrainShaderSet::~TerrainShaderSet()
    { }

    std::string TerrainShaderSet::get_position_mode(const SceneMode& mode)
    {
        std::string get_position_3D_mode       = "vec4 getPosition(vec3 position, float height, vec2 textureCoordinates) { return getPosition3DMode(position, height, textureCoordinates); }";
        std::string get_positionCV_and_2D_mode = "vec4 getPosition(vec3 position, float height, vec2 textureCoordinates) { return getPositionColumbusViewMode(position, height, textureCoordinates); }";
        std::string get_position_morphing_mode = "vec4 getPosition(vec3 position, float height, vec2 textureCoordinates) { return getPositionMorphingMode(position, height, textureCoordinates); }";

        std::string position_mode;

        switch (mode)
        {
            case SceneMode::Scene3D:
                position_mode = get_position_3D_mode;
                break;

            case SceneMode::Scene2D:
            case SceneMode::SceneCV:
                position_mode = get_positionCV_and_2D_mode;
                break;

            case SceneMode::Morphing:
                position_mode = get_position_morphing_mode;
                break;
        }

        return position_mode;
    }

    std::string TerrainShaderSet::get_2D_Y_position_fraction(const bool& use_web_mercator_projection)
    {
        std::string get_2D_y_position_fraction_geographic_projection = "float get2DYPositionFraction(vec2 textureCoordinates) { return get2DGeographicYPositionFraction(textureCoordinates); }";
        std::string get_2D_y_position_fraction_mercator_projection   = "float get2DYPositionFraction(vec2 textureCoordinates) { return get2DMercatorYPositionFraction(textureCoordinates); }";
        return use_web_mercator_projection ? get_2D_y_position_fraction_mercator_projection : get_2D_y_position_fraction_geographic_projection;
    }

    ShaderProgram* TerrainShaderSet::get_shader_program(
            ITerrain* terrain,
            FrameState* frame_state,
            const bool& use_double_precision,
            const bool& use_web_mercator_projection)
    {
        TerrainShader* terrain_shader = terrain->get_shader_program();

        const SceneMode mode = frame_state->get_mode();
        if (terrain_shader != nullptr)
        {
            if (terrain_shader->mode == frame_state->get_mode())
            {
               return terrain_shader->program;
            }
        }

        terrain_shader = m_shaders_by_flag[frame_state->get_mode()];

        if (terrain_shader == nullptr)
        {
            std::string vs;

            if (use_double_precision)
            {
                vs += "#define ENABLE_DOUBLE_PRECISION 1\n";
            }

            vs += m_base_vertex_source;
            vs += get_position_mode(mode);
            vs += get_2D_Y_position_fraction(use_web_mercator_projection);

            std::string fs = m_base_fragment_source;
            fs  = terrain->get_material()->get_shader_source() + fs;

            ShaderProgram* shader = frame_state->get_context()->get_shader_cache()->get_shader_program(
                    vs,
                    fs);

            terrain_shader = m_shaders_by_flag[mode] = new TerrainShader(mode, shader);
        }

        terrain->set_shader_program(terrain_shader);
        return  terrain_shader->program;
    }

    ShaderProgram* TerrainShaderSet::get_pick_shader_program(
            ITerrain* terrain,
            FrameState* frame_state,
            const bool& use_double_precision,
            const bool& use_web_mercator_projection)
    {
        TerrainShader* terrain_shader = terrain->get_shader_program();

        const SceneMode mode = frame_state->get_mode();
        if (terrain_shader != nullptr)
        {
            if (terrain_shader->mode == frame_state->get_mode())
            {
                return terrain_shader->program;
            }
        }

        terrain_shader = m_shaders_by_flag[frame_state->get_mode()];

        if (terrain_shader == nullptr)
        {
            std::string vs = m_base_vertex_source;
            std::string fs = m_base_fragment_source;

            vs += "#define APPLY_NORMAL_MAPPING ";
            vs += (true > 0 ? "1" : "0");
            vs += "\n";

            vs += get_position_mode(mode);
            vs += get_2D_Y_position_fraction(use_web_mercator_projection);

            ShaderProgram* shader = frame_state->get_context()->get_shader_cache()->get_shader_program(
                    vs,
                    fs);

            terrain_shader = m_shaders_by_flag[mode] = new TerrainShader(mode, shader);
        }

        terrain->set_shader_program(terrain_shader);
        return  terrain_shader->program;
    }
}