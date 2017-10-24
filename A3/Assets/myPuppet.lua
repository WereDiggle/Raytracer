
rootNode = gr.node('root')

-- colours
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

darkGray = gr.material({0.1, 0.1, 0.1}, {0.1, 0.1, 0.1}, 10)

-- TODO: needs work
lightBlue = gr.material({0.5, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

function makeLeg(side)

    socketSize = 0.2

    innerLegLength = 0.5

    outerLegLength = 1.2
    outerLegWidth = 0.4
    outerLegDepth = 0.15

    wheelDepth = 0.2
    wheelSize = 0.4

    -- Joint that connects to the main body to inner leg
    innerLegJoint = gr.joint('innerLegJoint', {-45,0,45}, {-45,0,45})
    -- innerLegJoint:rotate('')

        -- Leg socket to the main body
        socket = gr.mesh('sphere', 'socket')
        socket:scale(socketSize,socketSize,socketSize)
        socket:set_material(lightBlue)
        innerLegJoint:add_child(socket)

            -- the inner leg
            innerLeg = gr.mesh('sphere', 'innerLeg')
            innerLeg:scale(1.0/socketSize, 1.0/socketSize, 1.0/socketSize)
            innerLeg:scale(innerLegLength, socketSize, socketSize)
            innerLeg:translate(1.6,0,0)
            innerLeg:set_material(blue)
            socket:add_child(innerLeg)

        -- joint that connects outer leg to inner leg
        outerLegJoint = gr.joint('outerLegJoint', {-45,0,45}, {-135,-90,-45})
        outerLegJoint:translate(1,0,0)
        innerLegJoint:add_child(outerLegJoint)

            -- the outer leg
            outerLeg = gr.mesh('sphere', 'outerLeg')
            outerLeg:scale(outerLegDepth,outerLegLength,outerLegWidth)
            outerLeg:translate(0,-1,0)
            outerLeg:rotate('z',90)
            outerLeg:set_material(lightBlue)
            outerLegJoint:add_child(outerLeg)

                -- the wheel
                wheel = gr.mesh('sphere', 'wheel')
                wheel:scale(1.0/outerLegDepth, 1.0/outerLegLength, 1.0/outerLegWidth)
                wheel:scale(wheelDepth, wheelSize, wheelSize)
                wheel:translate(-1,-0.9,0)
                wheel:set_material(black)
                outerLeg:add_child(wheel)


    return innerLegJoint
end

function makeArm()

    armJoint = gr.joint('armJoint');

end

-- torso
torso = gr.mesh('sphere', 'torso')
torso:scale(1.0, 0.75, 1.0)
torso:translate(0.0, 0.0, 0.0)
torso:set_material(lightBlue)
rootNode:add_child(torso)

-- making some legs
legsDepth = -0.4
legsOut = 1
legsApart = 0.5
legsAngle = 30

-- right side legs
leg = makeLeg(1.0)
leg:translate(legsOut,legsDepth,legsApart)
rootNode:add_child(leg)

leg = makeLeg(1.0)
leg:translate(legsOut,legsDepth,-legsApart)
--leg:rotate('x', legsAngle)
rootNode:add_child(leg)

-- left side legs
leg = makeLeg(-1.0)
leg:rotate('y', 180)
leg:translate(-legsOut,legsDepth,legsApart)
--leg:rotate('x', legsAngle)
rootNode:add_child(leg)

leg = makeLeg(-1.0)
leg:rotate('y', 180)
leg:translate(-legsOut,legsDepth,-legsApart)
--leg:rotate('x', legsAngle)
rootNode:add_child(leg)

return rootNode