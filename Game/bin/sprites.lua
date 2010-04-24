-- here are the spriteloading facility
-- be gentle with it

sprites = {
    -- dude
    {   name = "dude",
        path = "gfx/objects.png",
        x = 3 * 32, y = 0, w = 32, h = 32, },
        
    -- island
    {   name = "tropic",
        path = "gfx/objects.png",
        x = 32, y = 0, w = 52, h = 43, },
        
    -- action
    {   name = "valid_action",
        path = "gfx/objects.png",
        color = 0xff47aa34,
        x = 0, y = 0, w = 32, h = 32, },
    {   name = "invalid_action",
        path = "gfx/objects.png",
        color = 0xff9f132a,
        x = 0, y = 0, w = 32, h = 32, },
    {   name = "action_shadow",
        path = "gfx/objects.png",
        color = 0xff2d2d2d,
        x = 0, y = 32, w = 32, h = 32, },
}
