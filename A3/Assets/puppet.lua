
rootNode = gr.node('root')

-- colours
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.5, 0.5, 0.5}, 10)

darkGray = gr.material({0.1, 0.1, 0.1}, {0.0, 0.0, 0.0}, 10)

lightBlue = gr.material({0.0, 0.6, 1.0}, {0.1, 0.1, 0.1}, 10)

function makeLeg(side)

    socketSize = 0.2
    innerLegLength = 0.5
    legMaxRot = 45
    legMinRot = -10

    if side <= 0 then
        yLimits = {-legMaxRot,0,-legMinRot}
    else
        yLimits = {legMinRot,0,legMaxRot}
    end

    -- Joint that connects to the main body to inner leg
    innerLegJoint = gr.joint('innerLegJoint', {-45,0,45}, yLimits)
    -- innerLegJoint:rotate('')

        -- Leg socket to the main body
        socket = gr.mesh('sphere', 'socket')
        socket:scale(socketSize,socketSize,socketSize)
        socket:set_material(lightBlue)
        innerLegJoint:add_child(socket)

            -- the inner leg
            innerLeg = gr.mesh('sphere', 'innerLeg')
            innerLeg:translate(0,0,1)
            innerLeg:scale(1.0/socketSize, 1.0/socketSize, 1.0/socketSize)
            innerLeg:scale(socketSize, socketSize, innerLegLength)
            innerLeg:set_material(white)
            socket:add_child(innerLeg)

        -- joint that connects outer leg to inner leg
        outerLeg = makeOuterLeg()
        outerLeg:rotate('x',90)
        outerLeg:translate(0,0,1)
        innerLegJoint:add_child(outerLeg)


    return innerLegJoint
end

function makeOuterLeg()

    outerLegLength = 1
    outerLegWidth = 0.4
    outerLegDepth = 0.2

    wheelDepth = 0.1
    wheelSize = 0.2

    -- joint that connects outer leg to inner leg
    outerLegJoint = gr.joint('outerLegJoint', {-45,0,45}, {-45,0,45})

        -- the outer leg
        outerLeg = gr.mesh('sphere', 'outerLeg')
        outerLeg:translate(0,0.3,0.8)
        outerLeg:scale(outerLegWidth,outerLegDepth,outerLegLength)
        outerLeg:set_material(lightBlue) 
        outerLegJoint:add_child(outerLeg)

            -- the wheel
            wheel = gr.mesh('sphere', 'wheel')
            wheel:scale(1.0/outerLegWidth, 1.0/outerLegDepth, 1.0/outerLegLength)
            wheel:scale(wheelSize, wheelDepth, wheelSize)
            wheel:translate(0,-0.9,0.9)
            wheel:set_material(black)
            outerLeg:add_child(wheel)

    return outerLegJoint
end

function makeArm(side)

    armLength = 0.5

    fingerApart = 0.125
    fingerUpArm = 0.8

    armMaxRot = 60

    if side <= 0 then
        yLimits = {-armMaxRot,0,15}
    else
        yLimits = {-15,0,armMaxRot}
    end

    armJoint = gr.joint('armJoint', {-45,0,60}, yLimits)

        arm = gr.mesh('sphere', 'arm')
        arm:translate(0,0,1)
        arm:scale(0.2,0.2,armLength)
        arm:set_material(lightBlue)
        armJoint:add_child(arm)

        finger = makeFinger()
        finger:translate(0, fingerApart, fingerUpArm)
        finger:rotate('z', 0)
        armJoint:add_child(finger)

        finger = makeFinger()
        finger:translate(0, fingerApart, fingerUpArm)
        finger:rotate('z', 120)
        armJoint:add_child(finger)

        finger = makeFinger()
        finger:translate(0, fingerApart, fingerUpArm)
        finger:rotate('z', 240)
        armJoint:add_child(finger)

    return armJoint

end

function makeFinger()

    baseFingerLength = 0.2
    baseFingerThickness = 0.04

    tipFingerLength = 0.1
    tipFingerThickness = 0.03

    baseFingerJoint = gr.joint('baseFingerJoint', {-45,0,15}, {0,0,0})

        baseFinger = gr.mesh('sphere', 'baseFinger')
        baseFinger:translate(0,0,0.6)
        baseFinger:scale(baseFingerThickness, baseFingerThickness, baseFingerLength)
        baseFinger:set_material(white)
        baseFingerJoint:add_child(baseFinger)

            --tipFingerJoint = gr.joint('tipFingerJoint', {0,0,15}, {0,0,0})
            --tipFingerJoint:translate(0,0,0.2)
            --baseFingerJoint:add_child(tipFingerJoint)

            --    tipFinger = gr.mesh('sphere', 'tipFinger')
            --    tipFinger:translate(0,0,0.8)
            --    tipFinger:scale(tipFingerThickness, tipFingerThickness, tipFingerLength)
            --    tipFinger:set_material(white)
            --    tipFingerJoint:add_child(tipFinger)

    return baseFingerJoint

end

function makeHead()

    headSize = 0.35
    eyeSize = 0.05
    eyeApart = 0.1

    headJoint = gr.joint('headJoint', {-45,0,45}, {-1800,0,1800})

        head = gr.mesh('sphere', 'head')
        head:scale(headSize, headSize, headSize)
        head:set_material(white)
        headJoint:add_child(head)

        eye = gr.mesh('sphere', 'eye')
        eye:scale(eyeSize, eyeSize, eyeSize)
        eye:translate(0,1,eyeApart)
        eye:rotate('y', 0)
        eye:set_material(black)
        head:add_child(eye)

        eye = gr.mesh('sphere', 'eye')
        eye:scale(eyeSize, eyeSize, eyeSize)
        eye:translate(0,1,eyeApart)
        eye:rotate('y', 120)
        eye:set_material(black)
        head:add_child(eye)

        eye = gr.mesh('sphere', 'eye')
        eye:scale(eyeSize, eyeSize, eyeSize)
        eye:translate(0,1,eyeApart)
        eye:rotate('y', 240)
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

ring = gr.mesh('sphere', 'ring')
ring:scale(1.02, 0.2, 1.02)
ring:translate(0.0, 0.0, 0.0)
ring:set_material(white)
rootNode:add_child(ring)

block = gr.mesh('cube', 'block')
block:scale(1.5,2,1)
block:translate(0,0.5,-1.2)
block:set_material(lightBlue)
rootNode:add_child(block)

-- making some legs
legsDepth = -0.5
legsOut = 0.8
legsApart = 0.3
legsAngle = 15

-- right side legs
leg = makeLeg(-1)
leg:rotate('y', 90)
leg:rotate('y', -legsAngle)
leg:translate(legsOut,legsDepth,legsApart)
rootNode:add_child(leg)

leg = makeLeg(1)
leg:rotate('y', 90)
leg:rotate('y', legsAngle)
leg:translate(legsOut,legsDepth,-legsApart)
--leg:rotate('x', legsAngle)
rootNode:add_child(leg)

-- left side legs
leg = makeLeg(1)
leg:rotate('y', -90)
leg:rotate('y', legsAngle)
leg:translate(-legsOut,legsDepth,legsApart)
--leg:rotate('x', legsAngle)
rootNode:add_child(leg)

leg = makeLeg(-1)
leg:rotate('y', -90)
leg:rotate('y', -legsAngle)
leg:translate(-legsOut,legsDepth,-legsApart)
--leg:rotate('x', legsAngle)
rootNode:add_child(leg)

-- make some arms
armsDepth = -0.3
armsOut = 0.75
armsApart = 0.5

arm = makeArm(1)
arm:translate(armsApart,armsDepth,armsOut)
rootNode:add_child(arm)

arm = makeArm(-1)
arm:translate(-armsApart,armsDepth,armsOut)
rootNode:add_child(arm)

armSocket = gr.mesh('sphere', 'armSocket')
armSocket:scale(0.2,0.2,0.2)
armSocket:translate(armsApart, armsDepth, armsOut)
armSocket:set_material(white)
rootNode:add_child(armSocket)

armSocket = gr.mesh('sphere', 'armSocket')
armSocket:scale(0.2,0.2,0.2)
armSocket:translate(-armsApart, armsDepth, armsOut)
armSocket:set_material(white)
rootNode:add_child(armSocket)

-- make some heads
headsDepth = 0.35
headsOut = 0.6
headsAngle = 37

head = makeHead()
head:rotate('x', headsAngle)
head:translate(0,headsDepth,headsOut)
rootNode:add_child(head)

head = makeHead()
head:rotate('x', headsAngle)
head:translate(0,headsDepth,headsOut)
head:rotate('y', 90)
rootNode:add_child(head)

head = makeHead()
head:rotate('x', headsAngle)
head:translate(0,headsDepth,headsOut)
head:rotate('y', -90)
rootNode:add_child(head)

--TEST LIMB

--testJoint = gr.joint('testJoint', {-45,0,45}, {-90,0,90});
--testJoint:translate(2,0,0)
--rootNode:add_child(testJoint)
--
--testLimb = gr.mesh('sphere', 'testLimb')
--testLimb:translate(0,0,1)
--testLimb:scale(1, 0.5, 2)
--testLimb:set_material(darkGray)
--testJoint:add_child(testLimb)
--
--testJoint = gr.joint('testJoint', {-45,0,45}, {-90,0,90});
--testJoint:rotate('x', 90)
--testJoint:translate(-2,0,0)
--rootNode:add_child(testJoint)
--
--testLimb = gr.mesh('sphere', 'testLimb')
--testLimb:translate(0,1,0)
--testLimb:scale(1, 2, 0.5)
--testLimb:set_material(darkGray)
--testJoint:add_child(testLimb)

return rootNode