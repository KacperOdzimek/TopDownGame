function on_init(e)
    _e_add_sprite(e, "sprite", "mod/textures/sprite_sheet", 1, "body")
end

function on_update(e, dt)
    speed = 4

    x_change = _i_axis("move_right") * dt * speed
    y_change = _i_axis("move_forward") * dt * speed

    x, y = _e_get_location(e)
    _e_sweep(e, x + x_change, y + y_change)
end