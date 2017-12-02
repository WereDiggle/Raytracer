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
light_gray = gr.material({0.7, 0.7, 0.7}, {0,0,0}, 1)

grass = gr.bitmap("grass.png",2.0,2.0)
number_cells = gr.bitmap("number_cells.png",1,1)

spider_tank = gr.bitmap("spider_tank.png",1,1)
spider_tank_bumps = gr.bumpmap("spider_tank.png",1,1, 2000)

tile = gr.bumpmap("tile.png", 10, 10, 2000)
noise = gr.noise(20, 20)
wood_planks = gr.bitmap("wood_planks.png",1,1)

scene_root = gr.node('root')

-- Room
wall_mat = gr.material({1,1,1}, {0.2,0.2,0.2}, 10)

back_plane = gr.plane('back_plane')
back_plane:translate(-0.5, -0.5, 0)
back_plane:scale(201, 201, 1)
back_plane:translate(0,0,-100)
back_plane:set_material(wall_mat)
back_plane:set_bitmap(noise)
back_plane:set_bumpmap(tile)
back_plane:set_reflectiveness(0.1)
scene_root:add_child(back_plane)

-- gonna make the ceiling and front reflective as a hack to have photons go where the camera's pointing
top_plane = gr.plane('top_plane')
top_plane:translate(-0.5, -0.5, 0)
top_plane:scale(201, 201, 1)
top_plane:rotate('x', 90)
top_plane:translate(0,100,0)
top_plane:set_material(wall_mat)
--top_plane:set_reflectiveness(1)
--top_plane:set_diffuse(0)
scene_root:add_child(top_plane)

front_plane = gr.plane('front_plane')
front_plane:translate(-0.5, -0.5, 0)
front_plane:scale(201, 201, 1)
front_plane:rotate('y', 180)
front_plane:translate(0,0,100)
front_plane:set_material(wall_mat)
--top_plane:set_reflectiveness(1)
--top_plane:set_diffuse(0)
scene_root:add_child(front_plane)


bottom_plane = gr.plane('bottom_plane')
bottom_plane:translate(-0.5, -0.5, 0)
bottom_plane:scale(201, 201, 1)
bottom_plane:rotate('x', -90)
bottom_plane:translate(0,-100,0)
bottom_plane:set_material(flatWhite)
bottom_plane:set_bitmap(wood_planks)
scene_root:add_child(bottom_plane)

left_plane = gr.plane('left_plane')
left_plane:translate(-0.5, -0.5, 0)
left_plane:scale(201, 201, 1)
left_plane:rotate('y', 90)
left_plane:translate(-100,0,0)
left_plane:set_material(flatWhite)
left_plane:set_bitmap(noise)
left_plane:set_bumpmap(tile)
scene_root:add_child(left_plane)

right_plane = gr.plane('right_plane')
right_plane:translate(-0.5, -0.5, 0)
right_plane:scale(201, 201, 1)
right_plane:rotate('y', -90)
right_plane:translate(100,0,0)
right_plane:set_material(flatWhite)
right_plane:set_bitmap(noise)
right_plane:set_bumpmap(tile)
right_plane:set_reflectiveness(0.1)
scene_root:add_child(right_plane)

-- poster

--poster = gr.plane('poster')
--poster:translate(-0.5, -0.5, 0)
--poster:scale(747/10, 816/10, 1)
--poster:translate(0,0,-99)
--poster:set_material(flatWhite)
--poster:set_bitmap(spider_tank)
--poster:set_bumpmap(spider_tank_bumps)
--scene_root:add_child(poster)

-- bath tub
bath_mat = gr.material({1,1,1}, {0.9,0.9,0.9}, 10)

function bath_mod (bath_surface) 
	bath_surface:set_material(bath_mat)
	bath_surface:set_reflectiveness(0.1)
end

bath_outer_front = gr.plane('bath_outer_front')
bath_outer_front:translate(-0.5, -0.5, 0)
bath_outer_front:scale(200, 60, 1)
bath_outer_front:translate(0,-70,0)
bath_mod(bath_outer_front)
scene_root:add_child(bath_outer_front)

bath_top_back = gr.plane('bath_top_back')
bath_top_back:translate(-0.5, -0.5, 0)
bath_top_back:scale(160, 20, 1) 
bath_top_back:rotate('x', -90)
bath_top_back:translate(0, -40, -90)
bath_mod(bath_top_back)
scene_root:add_child(bath_top_back)

bath_top_front = gr.plane('bath_top_front')
bath_top_front:translate(-0.5, -0.5, 0)
bath_top_front:scale(160, 20, 1) 
bath_top_front:rotate('x', -90)
bath_top_front:translate(0, -40, -10)
bath_mod(bath_top_front)
scene_root:add_child(bath_top_front)

bath_top_left = gr.plane('bath_top_left')
bath_top_left:translate(-0.5, -0.5, 0)
bath_top_left:scale(20, 100, 1) 
bath_top_left:rotate('x', -90)
bath_top_left:translate(-90, -40, -50)
bath_mod(bath_top_left)
scene_root:add_child(bath_top_left)

bath_top_right = gr.plane('bath_top_right')
bath_top_right:translate(-0.5, -0.5, 0)
bath_top_right:scale(20, 100, 1) 
bath_top_right:rotate('x', -90)
bath_top_right:translate(90, -40, -50)
bath_mod(bath_top_right)
scene_root:add_child(bath_top_right)

bath_inner_back = gr.plane('bath_inner_back')
bath_inner_back:translate(-0.5, -0.5, 0)
bath_inner_back:scale(161, 60, 1)
bath_inner_back:translate(0,-70,-80)
bath_mod(bath_inner_back)
scene_root:add_child(bath_inner_back)

bath_inner_front = gr.plane('bath_inner_front')
bath_inner_front:translate(-0.5, -0.5, 0)
bath_inner_front:scale(161, 60, 1)
bath_inner_front:rotate('y', 180)
bath_inner_front:translate(0,-70,-20)
bath_mod(bath_inner_front)
scene_root:add_child(bath_inner_front)

bath_inner_left = gr.plane('bath_inner_left')
bath_inner_left:translate(-0.5, -0.5, 0)
bath_inner_left:scale(61, 60, 1)
bath_inner_left:rotate('y', 90)
bath_inner_left:translate(-80,-70,-50)
bath_mod(bath_inner_left)
scene_root:add_child(bath_inner_left)

bath_inner_right = gr.plane('bath_inner_right')
bath_inner_right:translate(-0.5, -0.5, 0)
bath_inner_right:scale(61, 60, 1)
bath_inner_right:rotate('y', -90)
bath_inner_right:translate(80,-70,-50)
bath_mod(bath_inner_right)
scene_root:add_child(bath_inner_right)

bath_floor = gr.plane('bath_floor')
bath_floor:translate(-0.5, -0.5, 0)
bath_floor:scale(160, 60, 1) 
bath_floor:rotate('x', -90)
bath_floor:translate(0, -80, -50)
bath_mod(bath_floor)
scene_root:add_child(bath_floor)

-- water surface
waves = gr.ripple(0.5,0.5,10*8,10*3,25)

water_surface = gr.plane('water_surface')
water_surface:translate(-0.5, -0.5, 0)
water_surface:scale(160, 60, 1)
water_surface:rotate('x', -90)
water_surface:translate(0,-45,-50)
water_surface:set_material(cyan)
water_surface:set_transparency(1, 1.3)
water_surface:set_reflectiveness(0.15);
water_surface:set_diffuse(0.05)
water_surface:set_bumpmap(waves)
scene_root:add_child(water_surface)

-- duck
yellow_rubber = gr.material({1.0, 1.0, 0.0}, {0.2,0.2,0.2}, 10)

duck = gr.mesh('Scout_the_duck', 'duck.obj')
duck:scale(10,10,10)
duck:rotate('y', 135)
duck:translate(0, -45, -50)
duck:set_material(yellow_rubber)
scene_root:add_child(duck)

-- boat

red_plastic = gr.material({1.0, 0.0, 0.0}, {0.9,0.9,0.9}, 10)

boat = gr.mesh('toy_boat', 'boat.obj')
boat:scale(5,5,5)
boat:translate(70, -35, -90)
boat:set_material(red_plastic)
scene_root:add_child(boat)

-- faucet

metal = gr.material({0.2, 0.2, 0.2}, {1,1,1}, 10)

faucet = gr.cube('faucet')
faucet:translate(-0.5, -0.5, -0.5)
faucet:scale(50,10,10)
faucet:translate(85, -35, -50)
faucet:set_material(metal)
faucet:set_reflectiveness(0.5)
scene_root:add_child(faucet)

knob_1 = gr.cube('knob_1')
knob_1:translate(-0.5, -0.5, -0.5)
knob_1:scale(8,10,8)
knob_1:translate(88, -35, -30)
knob_1:set_material(metal)
knob_1:set_reflectiveness(0.5)
scene_root:add_child(knob_1)

handle_1 = gr.sphere('handle_1')
handle_1:scale(14,3,3)
handle_1:translate(78, -32, -30)
handle_1:set_material(metal)
handle_1:set_reflectiveness(0.5)
scene_root:add_child(handle_1)

knob_2 = gr.cube('knob_2')
knob_2:translate(-0.5, -0.5, -0.5)
knob_2:scale(8,10,8)
knob_2:translate(88, -35, -70)
knob_2:set_material(metal)
knob_2:set_reflectiveness(0.5)
scene_root:add_child(knob_2)

handle_2 = gr.sphere('handle_2')
handle_2:scale(14,3,3)
handle_2:translate(78, -32, -70)
handle_2:set_material(metal)
handle_2:set_reflectiveness(0.5)
scene_root:add_child(handle_2)


white_light_left = gr.light({-90, 90, -90}, {0.5, 0.5, 0.5}, {1, 0, 0})
white_light_right = gr.light({90, 90, -90}, {0.5, 0.5, 0.5}, {1, 0, 0})
white_light_front = gr.light({0, 90, 90}, {0.5, 0.5, 0.5}, {1, 0, 0})
white_light_center = gr.light({0, 0, 0}, {1, 1, 1}, {1, 0, 0})
white_light_front_left = gr.light({-50, 90, 30}, {0.8, 0.8, 0.8}, {1, 0, 0})

white_light_close = gr.light({-20, -30, -40}, {1, 1, 1}, {1, 0, 0})


ambientLight = {0.2, 0.2, 0.2}

lights = {white_light_front_left}

camera_origin = {-50, 0, 10}
camera_dest = {85, -45, -50}

gr.render(scene_root, 'bath_128_128.png', 128, 128,
	  camera_origin, camera_dest, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'bath_256_256.png', 256, 256,
	  camera_origin, camera_dest, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'bath_512_512.png', 512, 512,
	  camera_origin, camera_dest, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'bath_1920_1920.png', 1920, 1920,
	  camera_origin, camera_dest, {0, 1, 0}, 90,
	  ambientLight, lights)