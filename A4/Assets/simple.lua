-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 10)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 10)
mat4 = gr.material({1.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 10)
mat5 = gr.material({0.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 10)

whiteMat = gr.material({1,1,1}, {0.5,0.5,0.5}, 10)

scene_root = gr.node('root')

-- testing phong lighting
-- phongTest = gr.nh_sphere('phongTest', {0, 0, 0}, 100)
-- scene_root:add_child(phongTest)
-- phongTest:set_material(whiteMat)

-- pale green sphere, fourth
-- s1 = gr.nh_sphere('s1', {0, 0, 0}, 25)
-- scene_root:add_child(s1)
-- s1:set_material(mat1)

-- Test box
-- phongBox = gr.nh_box('phongBox', {-50, -50, -50}, 100);
-- scene_root:add_child(phongBox)
-- phongBox:set_material(whiteMat)

-- Test triangle
-- triangle = gr.mesh('triangle', 'triangle.obj')
-- scene_root:add_child(triangle)
-- triangle:set_material(whiteMat)

-- Test cube 
cube = gr.mesh('cube', 'cube.obj')
cube:scale(100, 100, 100)
cube:rotate('z', 45)
cube:translate(200, 100, 300)
cube:set_material(mat1)
scene_root:add_child(cube)
-- 
-- gray sphere, first
-- s2 = gr.nh_sphere('s2', {100, 0, 0}, 25)
-- scene_root:add_child(s2)
-- s2:set_material(mat2)
-- 
-- orangy sphere, fifth
s3 = gr.nh_sphere('s3', {0, -1200, 0}, 1000)
scene_root:add_child(s3)
s3:set_material(mat3)
-- 
-- red sphere, third
s4 = gr.nh_sphere('s4', {100, 50, 0}, 50)
s3:add_child(s4)
s4:set_material(mat4)

-- red box 
redBox = gr.nh_box('s4', {-0, 0, 0}, 100)
redBox:scale(2, 2, 1)
redBox:rotate('z', 45)
redBox:translate(-100, 150, 200)
s3:add_child(redBox)
redBox:set_material(mat4)
-- 
-- -- blue sphere, second
-- s5 = gr.nh_sphere('s5', {100, 0, -100}, 25)
-- scene_root:add_child(s5)
-- s5:set_material(mat5)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({0, 800, 300}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.3, 0.3, 0.3}

lights = {white_light}

gr.render(scene_root, 'simple.png', 512, 512,
	  {0, 800, 400}, {0, 0, 0}, {0, 1, 0}, 60,
	  ambientLight, lights)

--gr.render(scene_root, 'simple_back.png', 512, 512,
--	  {-10, -10, -10}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)