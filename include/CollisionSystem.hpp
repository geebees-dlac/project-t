#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>
#include <stdlib.h>
#include <stdbool.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <SFML/System/Vector2.hpp>
// #include <playerbody.hpp>
// #include <platformbody.hpp>
// #include <trapbody.hpp>

namespace phys
{
        struct collision{
                float time = 0.f;
                float surfaceArea = 0.f;
                int axis = 0;
        };

    struct collisionInfo{
        bool m_collisionTop = false;
            bool m_collisionBottom = false;
            bool m_collisionLeft = false;
            bool m_collisionRight = false;
    };

    enum bodyType{
        none = 0,
        platform = 1,
        conveyorBelt = 2,
        moving = 3,
        jumpthrough = 4,
        falling = 5,
        vanishing = 6
    };

    struct bodyInfo{
        unsigned int m_type = phys::bodyType::none;
            unsigned int m_id = 0;
            float m_width = 0.f;
            float m_height = 0.f;
            bool m_collisionTop = false;
            bool m_collisionBottom = false;
            bool m_collisionLeft = false;
            bool m_collisionRight = false;
            bool m_falling = false;
            sf::Vector2f m_position = sf::Vector2f(0,0);
        float m_surfaceVelocity = 0.f;
    };

        class collisionSystem
        {
        public:
                collisionSystem();
                ~collisionSystem();

                void resolveCollisions(class dynamicBody * dynamicBody, class platformBody * platformBodies, int numberOfPlatformObjects);
        void setBodyInfo(platformBody& platform);
        bodyInfo getBodyInfo(){return m_bodyInfo;};

        void setCollisionInfo(bool t, bool b, bool l, bool r);
        collisionInfo getCollisionInfo(){return m_collisionInfo;};

        private:
                static bool movingToStaticIntersectionTest(class dynamicBody * dynamicBody, class platformBody platformBody, struct collision * outCollision);
                static void resolveStaticCollision(class dynamicBody * dynamicBody, struct collision collision, float timesliceRemaining);

        public:
        static bodyInfo m_bodyInfo;
        static collisionInfo m_collisionInfo;
        static bool m_collisionTop;
        static bool m_collisionBottom;
        static bool m_collisionLeft;
        static bool m_collisionRight;
        };
}

#endif
    /*
    enum class ColisionType
    {
        EMPTY = 0, // no solid interaction
        SOLID = 1, //solid interaction
        TRAP = 2, // proceeds to troll
        MOVINGPLATFORM = 3, // moving platform normal
        MOVING = 4, // moving
        PHASING = 5, // false positive
        FALLING = 6, // struct obj falling , player falling
        TRIGGER = 7, //triggering trap or no trap
        TRIGGERED = 8, //triggered trap or no trap
        DISSAPEARING = 9, //dissapearing trap or no trap
        TRIGGEREDTRAP = 11, //triggered trap or no trap
        TRIGGEREDTRAPDISSAPEARING = 12, //triggered trap is appearing
        TRIGGEREDTRAPMOVING = 13, // triggered trap is moving
        TRIGGEREDTRAPMOVINGDISSAPEARING = 14, // triggered trap is moving and dissapearing
        TRIGGEREDTRAPMOVINGPHASING = 15, //trap moving and phasing
        TRIGGEREDTRAPMOVINGPHASINGDISSAPEARING = 16, //will continue if needed on the levels
        TRIGGEREDTRAPMOVINGPHASINGFALLING = 17, //will continue if needed on the levels
        TRIGGEREDTRAPMOVINGPHASINGFALLINGDISSAPEARING = 18, //will continue if needed on the levels
        TRIGGEREDTRAPMOVINGPHASINGFALLINGTRIGGEREDDISSAPEARING = 19, //will continue if needed on the levels
        TRIGGEREDTRAPMOVINGPHASINGFALLINGTRIGGEREDDISSAPEARINGDISSAPEARING = 20 //will continue if needed on the levels

    };
*/

    /*struct CollisionSystem //sampling
    {
        // Collision detection function
        static collisionData detectCollision(const std::vector<ColisionType>& objects, const std::vector<ColisionType>& movingObjects)
        {
            collisionData data;
            for (size_t i = 0; i < objects.size(); ++i)
            {
                if (objects[i] == ColisionType::SOLID)
                {
                    data.moving_collisionTop = true;
                }
                else if (objects[i] == ColisionType::TRAP)
                {
                    data.moving_collisionBottom = true;
                }
                else if (objects[i] == ColisionType::MOVINGPLATFORM)
                {
                    data.moving_collisionLeft = true;
                }
            }
            return data;
        }
*/





