#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <SFML/System/Vector2.hpp> // For sf::Vector2f

// Forward declare classes if only pointers/references are used in this header's declarations.
// If a class is passed by value (like platformBody was in your original static method),
// or if you need to know its size, then you must include its full header.
class dynamicBody; // Assuming playerbody.hpp defines phys::dynamicBody
// #include "platformbody.hpp" // Actually, platformBody is passed by value in one static method,
                             // so we should include it if that method remains.
                             // For now, let's assume resolveCollisions uses platformBody*
                             // and the static method might be refactored or also use pointers.

namespace phys {

    // Structure to hold information about a specific collision event
    struct collision {
        float time = 0.f;          // Time of impact (often normalized)
        float surfaceArea = 0.f;   // Not commonly used directly in basic resolution, but could be for advanced physics
        int axis = 0;              // Axis of collision (e.g., 0 for X, 1 for Y, or an enum)
        sf::Vector2f normal;       // Normal vector of the collision surface
    };

    // Structure to hold general collision state flags
    struct collisionInfo {
        bool m_collisionTop = false;
        bool m_collisionBottom = false;
        bool m_collisionLeft = false;
        bool m_collisionRight = false;
    };

    // Enum for different types of physics bodies
    enum class bodyType { // Using enum class for better scoping and type safety
        none = 0,
        platform = 1,
        conveyorBelt = 2,
        moving = 3,
        jumpthrough = 4,
        falling = 5,
        vanishing = 6
        // Add other types like 'trap' or 'player' if distinct physics handling is needed
    };

    // Structure to hold information about a physics body involved in a collision
    // This is often the state of the *other* object the player (or dynamic body) collided with.
    struct bodyInfo {
        bodyType m_type = bodyType::none;
        unsigned int m_id = 0; // Unique identifier for the body
        float m_width = 0.f;
        float m_height = 0.f;
        sf::Vector2f m_position = sf::Vector2f(0.f, 0.f);
        float m_surfaceVelocity = 0.f; // e.g., for conveyor belts
        bool m_falling = false;        // Specific state for falling platforms
        // Note: Collision flags (m_collisionTop, etc.) here might be redundant
        // if collisionInfo struct or the static bools are the primary source.
        // Decide on a single source of truth for this information.
    };

    // Forward declaration for platformBody if we only use pointers/references in method signatures here
    class platformBody;

    class collisionSystem {
    public:
        // --- Constructor & Destructor ---
        collisionSystem();
        ~collisionSystem();

        // --- Core Collision Resolution ---
        // Resolves collisions between one dynamic body and an array of platform bodies.
        // The dynamicBody's state (position, velocity) might be modified.
        // Information about the specific collision might be stored (see note on static members).
        void resolveCollisions(dynamicBody* PtrDynamicBody, platformBody platformBodies[], int numberOfPlatformObjects);

        // --- Collision State Management (Currently using static members) ---
        // These methods interact with the static m_bodyInfo and m_collisionInfo.
        // This design implies a single, global "last collision" state.

        // Sets the static m_bodyInfo based on a platform.
        void setBodyInfo(const platformBody& platform); // Pass by const ref if platform isn't modified
        // Gets the current static m_bodyInfo.
        bodyInfo getBodyInfo() const; // Marked const, though it returns a static member

        // Sets the static m_collisionInfo and related static booleans.
        void setCollisionInfo(bool t, bool b, bool l, bool r);
        // Gets the current static m_collisionInfo.
        collisionInfo getCollisionInfo() const; // Marked const

    private:
        // --- Internal Collision Test & Resolution Steps ---
        // These are static, meaning they don't operate on an instance of collisionSystem.
        // They might directly use or modify the static members if not designed carefully.
        // `platformBody` passed by value requires its full definition. If it stays static,
        // platformbody.hpp needs to be included above.
        static bool movingToStaticIntersectionTest(dynamicBody* PtrDynamicBody, const platformBody& platform, collision* outCollision);
        static void resolveStaticCollision(dynamicBody* PtrDynamicBody, const collision& colData, float timesliceRemaining);

    public:
        // --- Static Member Variables ---
        // WARNING: These create a single, global state for collision information.
        // This can be problematic if multiple collisions occur or if you have multiple dynamic bodies.
        // Consider refactoring to return collision data or store it per-instance/per-body.
        static bodyInfo m_bodyInfo;             // Info about the last body collided with.
        static collisionInfo m_collisionInfo;    // General flags about the last collision.

        // These seem redundant if m_collisionInfo already holds this information.
        // If kept, they also need definition in the .cpp file.
        static bool m_collisionTop;
        static bool m_collisionBottom;
        static bool m_collisionLeft;
        static bool m_collisionRight;
    };

} // namespace phys

#endif // COLLISIONSYSTEM_HPP
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





