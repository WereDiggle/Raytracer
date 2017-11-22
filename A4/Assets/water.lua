mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 10)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 10)

red = gr.material({1.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.5, 0.7, 0.5}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 10)
yellow = gr.material({1.0, 1.0, 0.0}, {0.5, 0.7, 0.5}, 10)
cyan = gr.material({0.0, 1.0, 1.0}, {0.5, 0.7, 0.5}, 10)
magenta = gr.material({1.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 10)

whiteMat = gr.material({1,1,1}, {0.5,0.5,0.5}, 10)

grass = gr.bitmap("grass.png",2.0,2.0)
tile = gr.bitmap("bad_tile.png",10,10)
number_cells = gr.bitmap("number_cells.png",1,1)

grassBumps = gr.bumpmap("grass.png",2.0,2.0, 10)
bumps = gr.bumpmap("testBumps.png", 1.0, 1.0, 10)
bumps2 = gr.bumpmap("testBumps2.png", 1.0, 1.0, 10)
bumps3 = gr.bumpmap("testBumps3.png", 1.0, 1.0, 20)
bumps4 = gr.bumpmap("testBumps4.png", 1.0, 1.0, 10)
bumps5 = gr.bumpmap("testBumps5.png", 1.0, 1.0, 10)
waves = gr.ripple(0.5,0.5,20,20,0.25)

scene_root = gr.node('root')

front_plane = gr.plane('front_plane')
front_plane:translate(-0.5, -0.5, 0)
front_plane:scale(200, 200, 200)
front_plane:rotate('y', 180)
front_plane:translate(0,0,100)
front_plane:set_material(red)
front_plane:set_bitmap(tile)
scene_root:add_child(front_plane)

back_plane = gr.plane('back_plane')
back_plane:translate(-0.5, -0.5, 0)
back_plane:scale(200, 200, 200)
back_plane:translate(0,0,-100)
back_plane:set_material(yellow)
back_plane:set_bitmap(number_cells)
scene_root:add_child(back_plane)

top_plane = gr.plane('top_plane')
top_plane:translate(-0.5, -0.5, 0)
top_plane:scale(200, 200, 200)
top_plane:rotate('x', 90)
top_plane:translate(0,100,0)
top_plane:set_material(green)
top_plane:set_bitmap(tile)
scene_root:add_child(top_plane)

bottom_plane = gr.plane('bottom_plane')
bottom_plane:translate(-0.5, -0.5, 0)
bottom_plane:scale(200, 200, 200)
bottom_plane:rotate('x', -90)
bottom_plane:translate(0,-100,0)
bottom_plane:set_material(cyan)
bottom_plane:set_bitmap(tile)
scene_root:add_child(bottom_plane)

left_plane = gr.plane('left_plane')
left_plane:translate(-0.5, -0.5, 0)
left_plane:scale(200, 200, 200)
left_plane:rotate('y', 90)
left_plane:translate(-100,0,0)
left_plane:set_material(blue)
left_plane:set_bitmap(tile)
scene_root:add_child(left_plane)

right_plane = gr.plane('right_plane')
right_plane:translate(-0.5, -0.5, 0)
right_plane:scale(200, 200, 200)
right_plane:rotate('y', -90)
right_plane:translate(100,0,0)
right_plane:set_material(magenta)
right_plane:set_bitmap(tile)
scene_root:add_child(right_plane)

water_surface = gr.plane('water_surface')
water_surface:translate(-0.5, -0.5, 0)
water_surface:scale(200, 200, 200)
water_surface:rotate('x', -90)
water_surface:translate(0,-30,0)
water_surface:set_material(cyan)
water_surface:set_transparency(1, 1.3)
water_surface:set_reflectiveness(0.1);
water_surface:set_diffuse(0.5)
water_surface:set_bumpmap(waves)
scene_root:add_child(water_surface)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-80, 0, 0}, {1, 1, 1}, {1, 0, 0})
white_light2 = gr.light({80, 80, -80}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.1, 0.1, 0.1}

lights = {white_light}

gr.render(scene_root, 'water.png', 256, 256,
	  {0, 90, 90}, {0, -100, -100}, {0, 1, 0}, 100,
	  ambientLight, lights)