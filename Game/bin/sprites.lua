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
    {   name = "shoe",
        path = "gfx/objects.png",
        x = 70, y = 73, w = 35, h = 25, },
    {   name = "mushroom",
        path = "gfx/objects.png",
        x = 32 * 4, y = 0, w = 32, h = 43, },
    {   name = "castle",
        path = "gfx/objects.png",
        x = 32, y = 58, w = 37, h = 43, },
        
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
