rootNode = gr.node('root')

-- colours
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- torso
torso = gr.mesh('sphere', 'torso')
torso:scale(1, 0.8, 1)
torso:translate(0.0, 0.25, 0.0)
torso:set_material(blue)

leg = gr.mesh('sphere', 'leg')
leg:scale(1,0.1,0.1)
leg:translate(1,0,0)
leg:set_material(blue)

rootNode:add_child(leg)

rootNode:add_child(torso)


return rootNode