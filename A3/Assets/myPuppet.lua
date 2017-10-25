
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

    fingerApart = 0.125
    fingerUpArm = 1.2

    armJoint = gr.joint('armJoint', {-45,0,45}, {-45,0,45})

        arm = gr.mesh('sphere', 'arm')
        arm:translate(1.0,0,0)
        arm:scale(0.7,0.2,0.2)
        arm:set_material(lightBlue)
        armJoint:add_child(arm)

        finger = makeFinger()
        finger:translate(fingerUpArm, fingerApart, 0.0)
        finger:rotate('x', 0)
        armJoint:add_child(finger)

        finger = makeFinger()
        finger:translate(fingerUpArm, fingerApart, 0.0)
        finger:rotate('x', 120)
        armJoint:add_child(finger)

        finger = makeFinger()
        finger:translate(fingerUpArm, fingerApart, 0.0)
        finger:rotate('x', 240)
        armJoint:add_child(finger)

    return armJoint

end

function makeFinger()

    baseFingerLength = 0.2
    baseFingerThickness = 0.04

    baseFingerJoint = gr.joint('baseFingerJoint', {-45,0,45}, {-45,0,45})

        baseFinger = gr.mesh('sphere', 'baseFinger')
        baseFinger:translate(1.0,0,0)
        baseFinger:scale(baseFingerLength, baseFingerThickness, baseFingerThickness)
        baseFinger:set_material(white)
        baseFingerJoint:add_child(baseFinger)

    return baseFingerJoint

end

function makeHead()

    headSize = 0.3
    eyeSize = 0.1
    eyeApart = 0.15

    headJoint = gr.joint('headJoint', {-45,0,45}, {-45,0,45})

        head = gr.mesh('sphere', 'head')
        head:scale(headSize, headSize, headSize)
        head:set_material(white)
        headJoint:add_child(head)

        eye = gr.mesh('sphere', 'eye')
        eye:scale(eyeSize, eyeSize, eyeSize)
        eye:translate(1,eyeApart,0)
        eye:rotate('x', 0)
        eye:set_material(black)
        head:add_child(eye)

        eye = gr.mesh('sphere', 'eye')
        eye:scale(eyeSize, eyeSize, eyeSize)
        eye:translate(1,eyeApart,0)
        eye:rotate('x', 120)
        eye:set_material(black)
        head:add_child(eye)

        eye = gr.mesh('sphere', 'eye')
        eye:scale(eyeSize, eyeSize, eyeSize)
        eye:translate(1,eyeApart,0)
        eye:rotate('x', 240)
        eye:set_material(black)
        head:add_child(eye)

    return headJoint
end

-- torso
torso = gr.mesh('sphere', 'torso')
torso:scale(1.0, 0.75, 1.0)
torso:translate(0.0, 0.0, 0.0)
torso:set_material(lightBlue)
rootNode:add_child(torso)

block = gr.mesh('cube', 'block')
block:scale(1.5,2,1)
block:translate(0,0.5,-1.2)
block:set_material(lightBlue)
rootNode:add_child(block)

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

-- make some arms
armsDepth = -0.5
armsOut = 0.7
armsApart = 0.4

arm = makeArm()
arm:rotate('y', -90)
arm:translate(armsApart,armsDepth,armsOut)
rootNode:add_child(arm)

arm = makeArm()
arm:rotate('y', -90)
arm:translate(-armsApart,armsDepth,armsOut)
rootNode:add_child(arm)

armSocket = gr.mesh('sphere', 'armSocket')
armSocket:scale(0.3,0.3,0.3)
armSocket:translate(armsApart, armsDepth, armsOut)
armSocket:set_material(white)
rootNode:add_child(armSocket)

armSocket = gr.mesh('sphere', 'armSocket')
armSocket:scale(0.3,0.3,0.3)
armSocket:translate(-armsApart, armsDepth, armsOut)
armSocket:set_material(white)
rootNode:add_child(armSocket)

-- make some heads
headsDepth = 0.5
headsOut = 0.5

head = makeHead()
head:rotate('z', 90)
head:rotate('x', 45)
head:translate(0,headsDepth,headsOut)
rootNode:add_child(head)

head = makeHead()
head:rotate('z', 90)
head:rotate('x', 45)
head:translate(0,headsDepth,headsOut)
head:rotate('y', 90)
rootNode:add_child(head)

head = makeHead()
head:rotate('z', 90)
head:rotate('x', 45)
head:translate(0,headsDepth,headsOut)
head:rotate('y', -90)
rootNode:add_child(head)

return rootNode