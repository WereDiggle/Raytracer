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

sphere = gr.sphere('sphere')
sphere:scale(10,10,10)
sphere:translate(0,0,70)
sphere:set_material(red)
sphere:set_transparency(1, 1.5)
--sphere:set_reflectiveness(0.2)
sphere:set_diffuse(0)
scene_root:add_child(sphere)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-80, 80, 80}, {1, 1, 1}, {1, 0, 0})
white_light2 = gr.light({80, 80, -80}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.1, 0.1, 0.1}

lights = {white_light}

gr.render(scene_root, 'simple1.png', 256, 256,
	  {0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
	  ambientLight, lights)

--gr.debug_render(scene_root, 'simple1.png', 256, 256,
--	  {0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
--	  ambientLight, lights, 128, 128)

--gr.render(scene_root, 'simple2.png', 256, 256,
--	  {0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
--	  ambientLight, lights)

--glass_pane = gr.plane('glass_pane')
--glass_pane:translate(-0.5, -0.5, 0)
--glass_pane:scale(200, 200, 200)
--glass_pane:rotate('x', -90)
--glass_pane:translate(0,-60,0)
--glass_pane:set_material(cyan)
--glass_pane:set_transparency(1, 1.1)
--glass_pane:set_diffuse(0.2)
--scene_root:add_child(glass_pane)
--
--gr.render(scene_root, 'water_refraction_1.png', 256, 256,
--	  {0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
--	  ambientLight, lights)
--
--glass_pane:set_transparency(1, 1.2);
--
--gr.render(scene_root, 'water_refraction_2.png', 256, 256,
--	  {0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
--	  ambientLight, lights)
--
--glass_pane:set_transparency(1, 1.3);
--
--gr.render(scene_root, 'water_refraction_3.png', 256, 256,
--	  {0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
--	  ambientLight, lights)

--gr.debug_render(scene_root, 'simple.png', 256, 256,
--			{0, 0, 90}, {0, 0, 0}, {0, 1, 0}, 100,
--			ambientLight, lights, 100, 150)

