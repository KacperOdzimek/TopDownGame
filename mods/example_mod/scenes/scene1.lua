function on_init()
    tilemap = _e_create()
    _e_add_tilemap(tilemap, "tilemap", "mod/tilemaps/tilemap", "mod/textures/tileset")

    player = _e_create()
    _e_add_behavior(player, "behavior", "mod/behaviors/move_left")
end

function on_update(dt)

end

function on_destroy()

end