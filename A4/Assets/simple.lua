-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({1.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 25)
mat5 = gr.material({0.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 25)

scene_root = gr.node('root')

-- pale green sphere, fourth
-- s1 = gr.nh_sphere('s1', {0, 0, 0}, 25)
-- scene_root:add_child(s1)
-- s1:set_material(mat1)

-- Test triangle
-- triangle = gr.mesh('triangle', 'Assets/triangle.obj')
-- scene_root:add_child(triangle)
-- triangle:set_material(mat1)

-- Test cube 
cube = gr.mesh('cube', 'Assets/cube.obj')
scene_root:add_child(cube)
cube:set_material(mat1)

-- gray sphere, first
s2 = gr.nh_sphere('s2', {100, 0, 0}, 25)
scene_root:add_child(s2)
s2:set_material(mat2)

-- orangy sphere, fifth
s3 = gr.nh_sphere('s3', {0, 100, 0}, 25)
scene_root:add_child(s3)
s3:set_material(mat3)

-- red sphere, third
s4 = gr.nh_sphere('s4', {-100, 25, -300}, 25)
scene_root:add_child(s4)
s4:set_material(mat4)

-- red box 
s4 = gr.nh_box('s4', {300, 300, 300}, 25)
scene_root:add_child(s4)
s4:set_material(mat4)

-- blue sphere, second
s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat5)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'simple.png', 512, 512,
	  {10, 10, 10}, {0, 0, 0}, {0, 1, 0}, 30,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})

gr.render(scene_root, 'simple_back.png', 512, 512,
	  {-10, -10, -10}, {0, 0, 0}, {0, 1, 0}, 30,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})