#include "RasterSource.h"
#include "RasterTile.h"
#include "ITerrain.h"

namespace argeo
{
    RasterSource::RasterSource(ITerrain *terrain)
    : m_terrain(terrain)
    { }

	RasterTile* RasterSource::get_tile(RasterTileIdentifier identifier)
	{
		std::unordered_map<RasterTileIdentifier, RasterTile*, RasterTileIdentifierHasher>::const_iterator itr =
			m_active_tiles.find(identifier);

		if (itr != m_active_tiles.end())
		{
			return itr->second;
		}
		else
		{
			// New tiles are not initially active.  They become active when loaded.
			return new RasterTile(this, identifier);
		}
	}

	void RasterSource::activate_tile(RasterTile* tile)
	{
		m_active_tiles[tile->get_identifier()] = tile;
	}

	void RasterSource::deactivate_tile(RasterTile* tile)
	{
		m_active_tiles.erase(tile->get_identifier());
	}

	std::function<void()> RasterSource::add_listener(
			std::function<void(vec3d&)> function,
			const Geodetic3D& geodetic)
	{
        RasterSourceListenerEntry* entry = new RasterSourceListenerEntry(function, geodetic);
		m_listeners.insert(entry).second;
        return [&, entry]()
        {
            m_listeners.erase(entry);
        };
	}

	void RasterSource::on_tile_changed(const RasterTile* tile)
	{
        // TODO: MUTEX
        m_tiles_changed.insert(tile);
        m_terrain->notify_dirty(tile);
	}

    void RasterSource::update(FrameState* frame_state)
    {
        MapProjection* projection = frame_state->get_map_projection();
        Ellipsoid ellipsoid       = projection->get_ellipsoid();

        for (const RasterTile* tile : m_tiles_changed)
        {
            for (RasterSourceListenerEntry* listener : m_listeners)
            {
                Geodetic3D geodetic = listener->position;
                Geodetic2D geodetic2D(
                        geodetic.get_longitude(),
                        geodetic.get_latitude());

                if (!tile->get_extent().contains(geodetic2D))
                {
                    continue;
                }

                Geocentric3D position_on_ellipsoid = ellipsoid.to_geocentric3D(geodetic2D);
                Ray ray;

                if (frame_state->get_mode() == SceneMode::Scene3D)
                {
                    ray.direction = ellipsoid.geodetic_surface_normal(position_on_ellipsoid.to_vec3d()).to_vec3d();

                    // Try to find the intersection point between the surface normal and z-axis.
                    // minimum height (-11500.0) for the terrain set, need to get this information from the terrain provider
                    ray.origin    = ellipsoid.get_surface_normal_intersection_with_z_axis(position_on_ellipsoid, 11500).to_vec3d();

                    // Theoretically, not with Earth datums, the intersection point can be outside the ellipsoid
                    if (vec_undefined(ray.origin))
                    {
                        // intersection point is outside the ellipsoid, try other value
                        // minimum height (-11500.0) for the terrain set, need to get this information from the terrain provider
                        double magnitude = ::fmin(tile->get_minimum_height(), -11500.0);

                        // multiply by the *positive* value of the magnitude
                        vec3d vector_to_minimum_point =  ray.direction * (std::abs(magnitude) + 1);
                        ray.origin = position_on_ellipsoid.to_vec3d() - vector_to_minimum_point;
                    }
                }
                else
                {
                    vec3d projected_position = projection->project(geodetic).to_vec3d();
                    projected_position = vec3d(
                            projected_position.z,
                            projected_position.x,
                            projected_position.y);

                    ray.origin    = projected_position;
                    ray.direction = vec3d::unit_x();
                }

                // TODO: Pick!!
                vec3d position = m_terrain->pick_from_tile(
                        tile,
                        ray,
                        frame_state->get_mode(),
                        projection,
                        false);

                if (!vec_undefined(position))
                {
                    listener->function(position);
                }
            }
        }

        m_tiles_changed.clear();
    }

}