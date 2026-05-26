
#include <graphics/TileMap.hpp>
#include "graphics/Texture.hpp"

graphics::TileMap::TileMap
    (
    std::shared_ptr<Texture> texture,
    int world_width_tiles, int world_height_tiles,
    int tile_width_world, int tile_height_world,
    int tile_width_pixels, int tile_height_pixels
    )
    : texture(texture)
    , world_width_tiles(world_width_tiles)
    , world_height_tiles(world_height_tiles)
    , tile_width_world(tile_width_world)
    , tile_height_world(tile_height_world)
    , tile_width_pixels(tile_width_pixels)
    , tile_height_pixels(tile_height_world)
    , chunk_width_tiles(world_width_tiles)
    , chunk_height_tiles(world_height_tiles)
{
    create();
}

graphics::TileMap::TileMap
    (
    std::shared_ptr<Texture> texture,
    int world_width_tiles, int world_height_tiles,
    int tile_width_world, int tile_height_world,
    int tile_width_pixels, int tile_height_pixels,
    int chunk_width_tiles, int chunk_height_tiles
    )
    : texture(texture)
    , world_width_tiles(world_width_tiles)
    , world_height_tiles(world_height_tiles)
    , tile_width_world(tile_width_world)
    , tile_height_world(tile_height_world)
    , tile_width_pixels(tile_width_pixels)
    , tile_height_pixels(tile_height_world)
    , chunk_width_tiles(world_width_tiles)
    , chunk_height_tiles(world_height_tiles)
{
    create();
}


void graphics::TileMap::create()
{
    // Empty grid
    std::vector<int> grid;
    int chunk_size_tiles = chunk_width_tiles * chunk_height_tiles;
    grid.reserve(chunk_size_tiles);
    for (int i = 0; i < chunk_size_tiles; ++i) grid.emplace_back(0);
    
    int world_width_chunks = world_width_tiles / chunk_height_tiles;
    int world_height_chunks = world_height_tiles / chunk_height_tiles;
    
    // Column major order
    for (int x = 0; x < world_width_chunks; ++x)
    {
        for (int y = 0; y < world_height_chunks; ++y)
        {
            chunks.emplace_back(grid);
        }
    }
}
