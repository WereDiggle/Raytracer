
rootNode = gr.node('root')

-- colours
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- TODO: needs work
lightBlue = gr.material({0.25, 0.25, 1.0}, {0.1, 0.1, 0.1}, 10)

function makeLeg(psx, psy, psz)

    socketSize = 0.2

    innerLegLength = 0.5

    outerLegLength = 1.2
    outerLegWidth = 0.4
    outerLegDepth = 0.15

    wheelDepth = 0.2
    wheelSize = 0.4

    -- Leg socket to the main body
    socket = gr.mesh('sphere', 'socket')
    socket:scale(1.0/psx, 1.0/psy, 1.0/psz)
    socket:scale(socketSize,socketSize,socketSize)
    socket:set_material(lightBlue)

    -- joint that connects Leg socket to inner leg
    innerLegJoint = gr.joint('innerLegJoint', {0,45,90}, {0,45,90})
    socket:add_child(innerLegJoint)

    -- the inner leg
    innerLeg = gr.mesh('sphere', 'innerLeg')
    innerLeg:scale(1.0/socketSize, 1.0/socketSize, 1.0/socketSize)
    innerLeg:scale(innerLegLength, socketSize, socketSize)
    innerLeg:translate(1.6,0,0)
    innerLeg:set_material(blue)
    innerLegJoint:add_child(innerLeg)

    -- joint that connects outer leg to inner leg
    outerLegJoint = gr.joint('outerLegJoint', {0,45,90}, {0,45,90})
    outerLegJoint:translate(1,0,0)
    innerLeg:add_child(outerLegJoint)

    -- the outer leg
    outerLeg = gr.mesh('sphere', 'outerLeg')
    outerLeg:scale(1.0/innerLegLength, 1.0/socketSize, 1.0/socketSize)
    outerLeg:scale(outerLegDepth,outerLegLength,outerLegWidth)
    outerLeg:translate(0,-5,0)
    outerLeg:set_material(lightBlue)
    outerLegJoint:add_child(outerLeg)

    -- the wheel
    wheel = gr.mesh('sphere', 'wheel')
    wheel:scale(1.0/outerLegDepth, 1.0/outerLegLength, 1.0/outerLegWidth)
    wheel:scale(wheelDepth, wheelSize, wheelSize)
    wheel:translate(-1,-0.9,0)
    wheel:set_material(blue)
    outerLeg:add_child(wheel)


    return socket
end

-- torso
torso = gr.mesh('sphere', 'torso')
torso:scale(1.0, 0.75, 1.0)
torso:translate(0.0, -1.0, 0.0)
torso:set_material(lightBlue)

-- making some legs
legsDepth = -0.4
legsOut = 3
legsApart = 0.5
legsAngle = 30
leg = makeLeg(1.0, 0.75, 1.0)
leg:rotate('y', -legsAngle)
leg:translate(legsOut,legsDepth,legsApart)
torso:add_child(leg)

leg = makeLeg(1.0, 0.75, 1.0)
leg:translate(legsOut,legsDepth,-legsApart)
--leg:rotate('x', legsAngle)
torso:add_child(leg)

leg = makeLeg(1.0, 0.75, 1.0)
leg:translate(-legsOut,legsDepth,legsApart)
--leg:rotate('x', legsAngle)
torso:add_child(leg)

leg = makeLeg(1.0, 0.75, 1.0)
leg:translate(-legsOut,legsDepth,-legsApart)
--leg:rotate('x', legsAngle)
torso:add_child(leg)

rootNode:add_child(torso)

return rootNode