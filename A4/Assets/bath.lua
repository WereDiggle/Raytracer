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
tile = gr.bitmap("bad_tile.png",10,10)
number_cells = gr.bitmap("number_cells.png",1,1)

grassBumps = gr.bumpmap("grass.png",2.0,2.0, 10)
bumps = gr.bumpmap("testBumps.png", 1.0, 1.0, 10)
bumps2 = gr.bumpmap("testBumps2.png", 1.0, 1.0, 10)
bumps3 = gr.bumpmap("testBumps3.png", 1.0, 1.0, 20)
bumps4 = gr.bumpmap("testBumps4.png", 1.0, 1.0, 10)
bumps5 = gr.bumpmap("testBumps5.png", 1.0, 1.0, 10)
noise = gr.noise(10, 10)

scene_root = gr.node('root')

-- Room
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
back_plane:set_material(flatWhite)
back_plane:set_bitmap(noise)
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

-- bath tub
bath_mat = flatWhite

bath_outer_front = gr.plane('bath_outer_front')
bath_outer_front:translate(-0.5, -0.5, 0)
bath_outer_front:scale(200, 60, 1)
bath_outer_front:translate(0,-70,0)
bath_outer_front:set_material(bath_mat)
scene_root:add_child(bath_outer_front)

bath_top_back = gr.plane('bath_top_back')
bath_top_back:translate(-0.5, -0.5, 0)
bath_top_back:scale(160, 20, 1) 
bath_top_back:rotate('x', -90)
bath_top_back:translate(0, -40, -90)
bath_top_back:set_material(bath_mat)
scene_root:add_child(bath_top_back)

bath_top_front = gr.plane('bath_top_front')
bath_top_front:translate(-0.5, -0.5, 0)
bath_top_front:scale(160, 20, 1) 
bath_top_front:rotate('x', -90)
bath_top_front:translate(0, -40, -10)
bath_top_front:set_material(bath_mat)
scene_root:add_child(bath_top_front)

bath_top_left = gr.plane('bath_top_left')
bath_top_left:translate(-0.5, -0.5, 0)
bath_top_left:scale(20, 100, 1) 
bath_top_left:rotate('x', -90)
bath_top_left:translate(-90, -40, -50)
bath_top_left:set_material(bath_mat)
scene_root:add_child(bath_top_left)

bath_top_right = gr.plane('bath_top_right')
bath_top_right:translate(-0.5, -0.5, 0)
bath_top_right:scale(20, 100, 1) 
bath_top_right:rotate('x', -90)
bath_top_right:translate(90, -40, -50)
bath_top_right:set_material(bath_mat)
scene_root:add_child(bath_top_right)

bath_inner_back = gr.plane('bath_inner_back')
bath_inner_back:translate(-0.5, -0.5, 0)
bath_inner_back:scale(160, 60, 1)
bath_inner_back:translate(0,-70,-80)
bath_inner_back:set_material(bath_mat)
scene_root:add_child(bath_inner_back)

bath_inner_front = gr.plane('bath_inner_front')
bath_inner_front:translate(-0.5, -0.5, 0)
bath_inner_front:scale(160, 60, 1)
bath_inner_front:rotate('y', 180)
bath_inner_front:translate(0,-70,-20)
bath_inner_front:set_material(bath_mat)
scene_root:add_child(bath_inner_front)

bath_inner_left = gr.plane('bath_inner_left')
bath_inner_left:translate(-0.5, -0.5, 0)
bath_inner_left:scale(60, 60, 1)
bath_inner_left:rotate('y', 90)
bath_inner_left:translate(-80,-70,-50)
bath_inner_left:set_material(bath_mat)
scene_root:add_child(bath_inner_left)

bath_inner_right = gr.plane('bath_inner_right')
bath_inner_right:translate(-0.5, -0.5, 0)
bath_inner_right:scale(60, 60, 1)
bath_inner_right:rotate('y', -90)
bath_inner_right:translate(80,-70,-50)
bath_inner_right:set_material(bath_mat)
scene_root:add_child(bath_inner_right)

bath_floor = gr.plane('bath_floor')
bath_floor:translate(-0.5, -0.5, 0)
bath_floor:scale(160, 60, 1) 
bath_floor:rotate('x', -90)
bath_floor:translate(0, -80, -50)
bath_floor:set_material(bath_mat)
scene_root:add_child(bath_floor)

white_light = gr.light({-50, 90, 50}, {2, 2, 2}, {1, 0, 0})
white_light1 = gr.light({-50, 99, -50}, {0.25, 0.25, 0.25}, {1, 0, 0})
white_light2 = gr.light({-50, 99, 50}, {0.25, 0.25, 0.25}, {1, 0, 0})
white_light3 = gr.light({50, 99, -50}, {0.25, 0.25, 0.25}, {1, 0, 0})
white_light4 = gr.light({50, 99, 50}, {0.25, 0.25, 0.25}, {1, 0, 0})

ambientLight = {0.1, 0.1, 0.1}

lights = {white_light}

gr.render(scene_root, 'bath.png', 256, 256,
	  {0, 50, 50}, {0, -70, -50}, {0, 1, 0}, 90,
	  ambientLight, lights)