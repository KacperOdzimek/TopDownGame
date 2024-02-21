function on_init(e)
    _d_set_b("moving_up", true)
    _e_add_sprite(e, "sprite", "mod/textures/cat_texture", 0, "body")
end

function on_update(e, dt)
    moving_up = _d_get("moving_up")
    x, y = _e_get_location(e)

    if (y > 3)  then moving_up = false end
    if (y < -3) then moving_up = true  end

    _d_set_b("moving_up", moving_up)

    if moving_up then
        _e_sweep(e, x, y + 3 * dt)
    else
        _e_sweep(e, x, y - 3 * dt)
    end
end