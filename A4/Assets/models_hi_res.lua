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

scene_root = gr.node('root')

front_plane = gr.plane('front_plane')
front_plane:translate(-0.5, -0.5, 0)
front_plane:scale(201, 201, 1)
front_plane:rotate('y', 180)
front_plane:translate(0,0,100)
front_plane:set_material(flatWhite)
scene_root:add_child(front_plane)

back_plane = gr.plane('back_plane')
back_plane:translate(-0.5, -0.5, 0)
back_plane:scale(201, 201, 1)
back_plane:translate(0,0,-100)
back_plane:set_material(flatWhite)
back_plane:set_bitmap(bricks)
scene_root:add_child(back_plane)

top_plane = gr.plane('top_plane')
top_plane:translate(-0.5, -0.5, 0)
top_plane:scale(201, 201, 1)
top_plane:rotate('x', 90)
top_plane:translate(0,100,0)
top_plane:set_material(flatWhite)
scene_root:add_child(top_plane)

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
scene_root:add_child(left_plane)

right_plane = gr.plane('right_plane')
right_plane:translate(-0.5, -0.5, 0)
right_plane:scale(201, 201, 1)
right_plane:rotate('y', -90)
right_plane:translate(100,0,0)
right_plane:set_material(red)
right_plane:set_bitmap(tile)
scene_root:add_child(right_plane)

toy_red = gr.material({1.0, 0.0, 0}, {0.5,0.5,0.5}, 10)

toy_boat = gr.mesh('toy_boat', 'boat.obj')
toy_boat:scale(10,10,10)
toy_boat:rotate('y', 20)
toy_boat:translate(40,-80,20)
toy_boat:set_material(toy_red)
scene_root:add_child(toy_boat)

duck_yellow = gr.material({1.0, 1.0, 0}, {0.5,0.5,0.5}, 10)

duck = gr.mesh('duck', 'duck.obj')
duck:scale(20,20,20)
duck:rotate('y', 135)
duck:translate(-20,-60,-20)
duck:set_material(duck_yellow)
scene_root:add_child(duck)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-80, 80, 80}, {1, 1, 1}, {1, 0, 0})
white_light2 = gr.light({80, 80, -80}, {1, 1, 1}, {1, 0, 0})

white_light_center = gr.light({0, 0, 0}, {1, 1, 1}, {1, 0, 0})
white_light_top = gr.light({0, 90, 90}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.1, 0.1, 0.1}

lights = {white_light_top}

gr.render(scene_root, 'models_1_hi_res.png', 1920, 1920,
	  {-50, -50, 80}, {0, -45, 0}, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'models_2_hi_res.png', 1920, 1920,
	  {-50, 50, 80}, {0, -45, 0}, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'models_3_hi_res.png', 1920, 1920,
	  {50, 50, 80}, {0, -45, 0}, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'models_4_hi_res.png', 1920, 1920,
	  {-50, 50, -80}, {0, -45, 0}, {0, 1, 0}, 90,
	  ambientLight, lights)

gr.render(scene_root, 'models_5_hi_res.png', 1920, 1920,
	  {50, 50, -80}, {0, -45, 0}, {0, 1, 0}, 90,
	  ambientLight, lights)