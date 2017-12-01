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

grass = gr.bitmap("grass.png",1.0,1.0)
fur = gr.bitmap("fur.png",1.0,1.0)
jeans = gr.bitmap("jeans.png",1.0,1.0)
bricks = gr.bitmap("bricks.png",1.0,1.0)
wood_planks = gr.bitmap("wood_planks.png",1.0,1.0)
tile = gr.bitmap("tile.png",10,10)
number_cells = gr.bitmap("number_cells.png",1,1)

spider_bumps = gr.bumpmap("spider_tank.png",1.0,1.0,2000)
tile_bumps = gr.bumpmap("tile.png", 10, 10, 20)
stars_bumps = gr.bumpmap("stars.png", 1, 1, 2000)
spiral_bumps = gr.bumpmap("spiral.png", 10, 10, 2000)
rough_bumps = gr.bumpmap("rough.png", 1, 1, 2000)

scene_root = gr.node('root')

front_plane = gr.plane('front_plane')
front_plane:translate(-0.5, -0.5, 0)
front_plane:scale(201, 201, 1)
front_plane:rotate('y', 180)
front_plane:translate(0,0,100)
front_plane:set_material(whiteMat)
scene_root:add_child(front_plane)

back_plane = gr.plane('back_plane')
back_plane:translate(-0.5, -0.5, 0)
back_plane:scale(201, 201, 1)
back_plane:translate(0,0,-100)
back_plane:set_material(whiteMat)
back_plane:set_bumpmap(spider_bumps)
scene_root:add_child(back_plane)

top_plane = gr.plane('top_plane')
top_plane:translate(-0.5, -0.5, 0)
top_plane:scale(201, 201, 1)
top_plane:rotate('x', 90)
top_plane:translate(0,100,0)
top_plane:set_material(whiteMat)
scene_root:add_child(top_plane)

bottom_plane = gr.plane('bottom_plane')
bottom_plane:translate(-0.5, -0.5, 0)
bottom_plane:scale(201, 201, 1)
bottom_plane:rotate('x', -90)
bottom_plane:translate(0,-100,0)
bottom_plane:set_material(green)
bottom_plane:set_bumpmap(rough_bumps)
scene_root:add_child(bottom_plane)

left_plane = gr.plane('left_plane')
left_plane:translate(-0.5, -0.5, 0)
left_plane:scale(201, 201, 1)
left_plane:rotate('y', 90)
left_plane:translate(-100,0,0)
left_plane:set_material(whiteMat)
left_plane:set_bumpmap(stars_bumps)
scene_root:add_child(left_plane)

right_plane = gr.plane('right_plane')
right_plane:translate(-0.5, -0.5, 0)
right_plane:scale(201, 201, 1)
right_plane:rotate('y', -90)
right_plane:translate(100,0,0)
right_plane:set_material(red)
right_plane:set_bumpmap(spiral_bumps)
scene_root:add_child(right_plane)

back_sphere = gr.sphere('back_sphere')
back_sphere:scale(40,40,40)
back_sphere:translate(-20,-60,-20)
back_sphere:set_material(flatWhite)
back_sphere:set_bumpmap(tile_bumps)
scene_root:add_child(back_sphere)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-80, 80, 80}, {1, 1, 1}, {1, 0, 0})
white_light2 = gr.light({80, 80, -80}, {1, 1, 1}, {1, 0, 0})

white_light_center = gr.light({0, 0, 0}, {1, 1, 1}, {1, 0, 0})
white_light_top = gr.light({0, 90, 90}, {1, 1, 1}, {1, 0, 0})

white_light_1 = gr.light({90, 50, 50}, {1, 1, 1}, {1, 0, 0})
white_light_2 = gr.light({-90, 50, 50}, {1, 1, 1}, {1, 0, 0})
white_light_3 = gr.light({0, 50, 50}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.1, 0.1, 0.1}

lights = {white_light_1}

gr.render(scene_root, 'bump_maps_1.png', 256, 256,
	  {0, 0, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

lights = {white_light_2}

gr.render(scene_root, 'bump_maps_2.png', 256, 256,
	  {0, 0, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)

lights = {white_light_3}

gr.render(scene_root, 'bump_maps_3.png', 256, 256,
	  {0, 0, 99}, {0, -50, -100}, {0, 1, 0}, 90,
	  ambientLight, lights)
