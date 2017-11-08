mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 10)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 10)
mat4 = gr.material({1.0, 0.0, 0.0}, {0.5, 0.7, 0.5}, 10)
mat5 = gr.material({0.0, 0.0, 1.0}, {0.5, 0.7, 0.5}, 10)

whiteMat = gr.material({1,1,1}, {0.1,0.1,0.1}, 10)
gray = gr.material({0.7,0.7,0.7}, {0.2,0.2,0.2}, 10) 
green = gr.material({0.2, 0.8, 0.2}, {0.1, 0.1, 0.1}, 5)

grayFlat = gr.material({0.3,0.3,0.3}, {0,0,0}, 10) 

laserMat = gr.material({1,0,0}, {1,1,1}, 1)

scene_root = gr.node('destruction_of_Alderaan')

planet = gr.nh_sphere('s3', {400, -1200, -400}, 1200)
scene_root:add_child(planet)
planet:set_material(green)

cow = gr.mesh( 'dodec', 'cow.obj' )
cow:translate(-0.5,-0.5,-0.5)
cow:scale(5, 5, 5)
cow:translate(60, 120, -60)
cow:set_material(whiteMat)
scene_root:add_child(cow)

cow2 = gr.mesh( 'dodec', 'cow.obj' )
cow2:translate(-10,1,5)
cow2:set_material(whiteMat)
cow:add_child(cow2)

cow3 = gr.mesh( 'dodec', 'cow.obj' )
cow3:translate(-10,1,-5)
cow3:set_material(whiteMat)
cow:add_child(cow3)

mickey = gr.mesh( 'dodec', 'mickey.obj' )
mickey:translate(-0.5,-0.5,-0.5)
mickey:scale(100, 100, 100)
mickey:rotate('x', -90)
mickey:rotate('z', -90)
mickey:translate(-30, 90, 0)
mickey:set_material(whiteMat)
scene_root:add_child(mickey)

laser = gr.nh_sphere('laser', {0, 0, 0}, 1)
laser:scale(1,0.05,0.05)
laser:translate(1.9,0.5,0.4)
mickey:add_child(laser)
laser:set_material(laserMat)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({-1000, 1000, 1000}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.3, 0.3, 0.3}

lights = {white_light}

gr.render(scene_root, 'sample.png', 1920, 1080,
	  {200, -50, 200}, {0, 0, 0}, {0, 1, 0}, 60,
	  ambientLight, lights)
