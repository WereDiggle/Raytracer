-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 10)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 10)
mat4 = gr.material({1.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 10)
mat5 = gr.material({0.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 10)

whiteMat = gr.material({1,1,1}, {0.5,0.5,0.5}, 10)

--grass = gr.bitmap("grass.png",2.0,2.0)
grassBumps = gr.bumpmap("grass.png",2.0,2.0, 10)
bumps = gr.bumpmap("testBumps.png", 1.0, 1.0, 10)
bumps2 = gr.bumpmap("testBumps2.png", 1.0, 1.0, 10)
bumps3 = gr.bumpmap("testBumps3.png", 1.0, 1.0, 20)
bumps4 = gr.bumpmap("testBumps4.png", 1.0, 1.0, 10)
bumps5 = gr.bumpmap("testBumps5.png", 1.0, 1.0, 10)

scene_root = gr.node('root')

back_plane = gr.plane('back_plane')
back_plane:translate(-0.5, -0.5, 0)
back_plane:scale(200, 200, 200)
back_plane:translate(0,0,-100)
back_plane:set_material(whiteMat)
back_plane:set_bumpmap(bumps3)
scene_root:add_child(back_plane)

top_plane = gr.plane('top_plane')
top_plane:translate(-0.5, -0.5, 0)
top_plane:scale(200, 200, 200)
top_plane:rotate('x', 90)
top_plane:translate(0,100,0)
top_plane:set_material(whiteMat)
top_plane:set_bumpmap(bumps3)
scene_root:add_child(top_plane)

bottom_plane = gr.plane('bottom_plane')
bottom_plane:translate(-0.5, -0.5, 0)
bottom_plane:scale(200, 200, 200)
bottom_plane:rotate('x', -90)
bottom_plane:translate(0,-100,0)
bottom_plane:set_material(whiteMat)
bottom_plane:set_bumpmap(bumps3)
scene_root:add_child(bottom_plane)

left_plane = gr.plane('left_plane')
left_plane:translate(-0.5, -0.5, 0)
left_plane:scale(200, 200, 200)
left_plane:rotate('y', 90)
left_plane:translate(-100,0,0)
left_plane:set_material(whiteMat)
left_plane:set_bumpmap(bumps3)
scene_root:add_child(left_plane)

right_plane = gr.plane('right_plane')
right_plane:translate(-0.5, -0.5, 0)
right_plane:scale(200, 200, 200)
right_plane:rotate('y', -90)
right_plane:translate(100,0,0)
right_plane:set_material(whiteMat)
right_plane:set_bumpmap(bumps3)
scene_root:add_child(right_plane)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-80, -80, -80}, {1, 1, 1}, {1, 0, 0})
white_light2 = gr.light({80, -80, -80}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0, 0, 0}

lights = {white_light, white_light2}

gr.render(scene_root, 'plane-bumps.png', 192, 108,
	  {0, 99, 200}, {0, -100, -100}, {0, 1, 0}, 60,
	  ambientLight, lights)
