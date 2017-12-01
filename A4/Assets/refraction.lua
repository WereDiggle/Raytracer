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

flatWhite = gr.material({1,1,1}, {0,0,0}, 1)

grass = gr.bitmap("grass.png",2.0,2.0)
tile = gr.bumpmap("tile.png",10,10,2000)
number_cells = gr.bitmap("number_cells.png",1,1)

grassBumps = gr.bumpmap("grass.png",2.0,2.0, 10)
bumps = gr.bumpmap("testBumps.png", 1.0, 1.0, 10)
bumps2 = gr.bumpmap("testBumps2.png", 1.0, 1.0, 10)
bumps3 = gr.bumpmap("testBumps3.png", 1.0, 1.0, 20)
bumps4 = gr.bumpmap("testBumps4.png", 1.0, 1.0, 10)
bumps5 = gr.bumpmap("testBumps5.png", 1.0, 1.0, 10)
noise = gr.noise(10, 10)

scene_root = gr.node('root')

front_plane = gr.plane('front_plane')
front_plane:translate(-0.5, -0.5, 0)
front_plane:scale(201, 201, 1)
front_plane:rotate('y', 180)
front_plane:translate(0,0,100)
front_plane:set_material(red)
scene_root:add_child(front_plane)

back_plane = gr.plane('back_plane')
back_plane:translate(-0.5, -0.5, 0)
back_plane:scale(201, 201, 1)
back_plane:translate(0,0,-100)
back_plane:set_material(yellow)
back_plane:set_bumpmap(tile)
scene_root:add_child(back_plane)

top_plane = gr.plane('top_plane')
top_plane:translate(-0.5, -0.5, 0)
top_plane:scale(201, 201, 1)
top_plane:rotate('x', 90)
top_plane:translate(0,100,0)
top_plane:set_material(green)
scene_root:add_child(top_plane)

bottom_plane = gr.plane('bottom_plane')
bottom_plane:translate(-0.5, -0.5, 0)
bottom_plane:scale(201, 201, 1)
bottom_plane:rotate('x', -90)
bottom_plane:translate(0,-100,0)
bottom_plane:set_material(cyan)
scene_root:add_child(bottom_plane)

left_plane = gr.plane('left_plane')
left_plane:translate(-0.5, -0.5, 0)
left_plane:scale(201, 201, 1)
left_plane:rotate('y', 90)
left_plane:translate(-100,0,0)
left_plane:set_material(blue)
scene_root:add_child(left_plane)

right_plane = gr.plane('right_plane')
right_plane:translate(-0.5, -0.5, 0)
right_plane:scale(201, 201, 1)
right_plane:rotate('y', -90)
right_plane:translate(100,0,0)
right_plane:set_material(magenta)
scene_root:add_child(right_plane)

front_sphere = gr.sphere('front_sphere')
front_sphere:scale(20,20,20)
front_sphere:translate(0,-30,40)
front_sphere:set_material(flatWhite)
front_sphere:set_transparency(1, 1.05)
front_sphere:set_diffuse(0)
scene_root:add_child(front_sphere)

bottom_left_sphere = gr.sphere('back_sphere')
bottom_left_sphere:scale(30,30,30)
bottom_left_sphere:translate(-65,-65,-65)
bottom_left_sphere:set_material(flatWhite)
bottom_left_sphere:set_transparency(1, 1.05)
bottom_left_sphere:set_diffuse(0)
scene_root:add_child(bottom_left_sphere)

bottom_right_sphere = gr.sphere('back_sphere')
bottom_right_sphere:scale(30,30,30)
bottom_right_sphere:translate(65,-65,-65)
bottom_right_sphere:set_material(flatWhite)
bottom_right_sphere:set_transparency(1, 1.05)
bottom_right_sphere:set_diffuse(0)
scene_root:add_child(bottom_right_sphere)

top_left_sphere = gr.sphere('back_sphere')
top_left_sphere:scale(30,30,30)
top_left_sphere:translate(-65,65,-65)
top_left_sphere:set_material(flatWhite)
top_left_sphere:set_transparency(1, 1.05)
top_left_sphere:set_diffuse(0)
scene_root:add_child(top_left_sphere)

top_right_sphere = gr.sphere('back_sphere')
top_right_sphere:scale(30,30,30)
top_right_sphere:translate(65,65,-65)
top_right_sphere:set_material(flatWhite)
top_right_sphere:set_transparency(1, 1.05)
top_right_sphere:set_diffuse(0)
scene_root:add_child(top_right_sphere)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-80, 80, 80}, {1, 1, 1}, {1, 0, 0})
white_light2 = gr.light({80, 80, -80}, {1, 1, 1}, {1, 0, 0})

white_light_center = gr.light({0, 0, 0}, {1, 1, 1}, {1, 0, 0})
white_light_top = gr.light({50, 90, 90}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.1, 0.1, 0.1}

lights = {white_light_top}

gr.render(scene_root, 'refraction_1_05_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.1)
bottom_right_sphere:set_transparency(1, 1.1)
top_left_sphere:set_transparency(1, 1.1)
bottom_left_sphere:set_transparency(1, 1.1)
front_sphere:set_transparency(1, 1.1)

gr.render(scene_root, 'refraction_1_1_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.2)
bottom_right_sphere:set_transparency(1, 1.2)
top_left_sphere:set_transparency(1, 1.2)
bottom_left_sphere:set_transparency(1, 1.2)
front_sphere:set_transparency(1, 1.2)

gr.render(scene_root, 'refraction_1_2_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.3)
bottom_right_sphere:set_transparency(1, 1.3)
top_left_sphere:set_transparency(1, 1.3)
bottom_left_sphere:set_transparency(1, 1.3)
front_sphere:set_transparency(1, 1.3)

gr.render(scene_root, 'refraction_1_3_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.4)
bottom_right_sphere:set_transparency(1, 1.4)
top_left_sphere:set_transparency(1, 1.4)
bottom_left_sphere:set_transparency(1, 1.4)
front_sphere:set_transparency(1, 1.4)

gr.render(scene_root, 'refraction_1_4_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.5)
bottom_right_sphere:set_transparency(1, 1.5)
top_left_sphere:set_transparency(1, 1.5)
bottom_left_sphere:set_transparency(1, 1.5)
front_sphere:set_transparency(1, 1.5)

gr.render(scene_root, 'refraction_1_5_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.6)
bottom_right_sphere:set_transparency(1, 1.6)
top_left_sphere:set_transparency(1, 1.6)
bottom_left_sphere:set_transparency(1, 1.6)
front_sphere:set_transparency(1, 1.6)

gr.render(scene_root, 'refraction_1_6_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.7)
bottom_right_sphere:set_transparency(1, 1.7)
top_left_sphere:set_transparency(1, 1.7)
bottom_left_sphere:set_transparency(1, 1.7)
front_sphere:set_transparency(1, 1.7)

gr.render(scene_root, 'refraction_1_7_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

top_right_sphere:set_transparency(1, 1.8)
bottom_right_sphere:set_transparency(1, 1.8)
top_left_sphere:set_transparency(1, 1.8)
bottom_left_sphere:set_transparency(1, 1.8)
front_sphere:set_transparency(1, 1.8)

gr.render(scene_root, 'refraction_1_8_hi_res.png', 1920, 1080,
	  {0, -50, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)
