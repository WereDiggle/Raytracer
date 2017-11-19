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

bluePlane = gr.plane('testPlane')
bluePlane:translate(-0.5,-0.5,0)
bluePlane:scale(100,100,100)
bluePlane:rotate('z', 0)
scene_root:add_child(bluePlane)
bluePlane:set_material(whiteMat)
bluePlane:set_bumpmap(bumps3)

red_light = gr.light({0, 800, 400.0}, {1, 0, 0}, {1, 0, 0})
green_light = gr.light({400.0, -400.0, 400.0}, {0, 1, 0}, {1, 0.0, 0})
blue_light = gr.light({-400, -400, 400.0}, {0, 0, 1}, {1, 0.0, 0})

white_light = gr.light({0, 200, 100}, {1, 1, 1}, {1, 0, 0})

ambientLight = {0.3, 0.3, 0.3}

lights = {white_light}

gr.render(scene_root, 'simple01.png', 512, 512,
	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
	  ambientLight, lights)

--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple02.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple03.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple04.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple05.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple06.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple07.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple08.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
--
--bluePlane:rotate('z', 15)
--
--gr.render(scene_root, 'simple09.png', 512, 512,
--	  {0, 0, 200}, {0, 0, 0}, {0, 1, 0}, 60,
--	  ambientLight, lights)
