#pragma once

#include "GeodeticExtent.h"
#include "RasterTileIdentifier.h"
#include "RasterLevelCollection.h"
#include "RasterSourceTileData.h"
#include "Texture2D.h"
#include "FrameState.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>

namespace argeo
{
	class RasterTile;
    class ITerrain;

	struct RasterSourceListenerEntry
	{
		RasterSourceListenerEntry(
                std::function<void(vec3d&)> function,
                const Geodetic3D& position)
        : function(function)
        , position(position)
		{ }

        std::function<void(vec3d&)> function;
        Geodetic3D position;
	};

	class RasterSource
	{
	public:

        void update(FrameState* frame_state);

		virtual double get_maximum_height() const = 0;
		virtual double get_minimum_height() const = 0;
		virtual RasterLevelCollection get_levels() const  = 0;

		virtual GeodeticExtent get_extent() const = 0;

		virtual RasterSourceTileData load_tile_data(RasterTileIdentifier identifier) = 0;

		RasterTile* get_tile(RasterTileIdentifier identifier);

		void activate_tile(RasterTile* tile);
		void deactivate_tile(RasterTile* tile);

        std::function<void()> add_listener(
                std::function<void(vec3d&)> function,
                const Geodetic3D& geodetic);
		
		void on_tile_changed(const RasterTile* tile);

    protected:
        RasterSource(ITerrain* terrain);

	private:
		std::unordered_set<RasterSourceListenerEntry*> m_listeners;
		std::unordered_map<RasterTileIdentifier, RasterTile*, RasterTileIdentifierHasher> m_active_tiles;
        std::unordered_set<const RasterTile*> m_tiles_changed;
        ITerrain* m_terrain;

	};
}
