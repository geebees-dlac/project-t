#include "CollisionSystem.hpp" // Must include its own header (adjust path if CollisionSystem.hpp is in ../include/)
                               // e.g., #include "../include/CollisionSystem.hpp" if this .cpp is in src/

#include "playerbody.hpp"    // Or dynamicbody.hpp - Needed for dynamicBody definition
#include "platformbody.hpp"  // Needed for platformBody definition (especially if used by value or members accessed)

#include <iostream> // For any debugging output

namespace phys {

    // --- Definition of Static Member Variables ---
    // These lines allocate storage for the static members.
    bodyInfo      collisionSystem::m_bodyInfo;
    collisionInfo collisionSystem::m_collisionInfo;
    bool          collisionSystem::m_collisionTop = false;    // Initialize if desired
    bool          collisionSystem::m_collisionBottom = false; // Initialize if desired
    bool          collisionSystem::m_collisionLeft = false;   // Initialize if desired
    bool          collisionSystem::m_collisionRight = false;  // Initialize if desired


    // --- Constructor ---
    collisionSystem::collisionSystem() {
        std::cout << "CollisionSystem created." << std::endl;
        // Initialize any non-static members here if you had them.
        // Static members are initialized where they are defined (above).
    }

    // --- Destructor ---
    collisionSystem::~collisionSystem() {
        std::cout << "CollisionSystem destroyed." << std::endl;
    }

    // --- Core Collision Resolution ---
    void collisionSystem::resolveCollisions(dynamicBody* PtrDynamicBody, platformBody platformBodies[], int numberOfPlatformObjects) {
        if (!PtrDynamicBody) return; // Basic null check

        // Reset global collision flags for this resolution pass (important with static state)
        // This is a simplistic approach. A better system wouldn't rely so heavily on global statics.
        setCollisionInfo(false, false, false, false);
        // m_bodyInfo might also need resetting or careful handling.

        for (int i = 0; i < numberOfPlatformObjects; ++i) {
            platformBody& currentPlatform = platformBodies[i]; // Work with a reference

            // Placeholder for actual collision info from the test
            collision individualCollisionData;

            if (movingToStaticIntersectionTest(PtrDynamicBody, currentPlatform, &individualCollisionData)) {
                // A collision was detected with currentPlatform
                // Update the global static information (this will be for the *last* one found if multiple hit)
                setBodyInfo(currentPlatform); // Stores info about 'currentPlatform' globally

                // Determine collision sides based on 'individualCollisionData' (e.g., normal or axis)
                // This is a highly simplified example:
                bool hitTop = false, hitBottom = false, hitLeft = false, hitRight = false;
                if (individualCollisionData.normal.y > 0.5f) hitBottom = true; // Player's bottom hit platform's top
                else if (individualCollisionData.normal.y < -0.5f) hitTop = true;    // Player's top hit platform's bottom
                if (individualCollisionData.normal.x > 0.5f) hitLeft = true;  // Player's left hit platform's right
                else if (individualCollisionData.normal.x < -0.5f) hitRight = true; // Player's right hit platform's left
                
                setCollisionInfo(hitTop, hitBottom, hitLeft, hitRight); // Updates global collision flags

                // Call the static resolution logic
                // The '1.0f' for timesliceRemaining is a placeholder.
                // In a real system, this would be the remaining fraction of the frame's timestep.
                resolveStaticCollision(PtrDynamicBody, individualCollisionData, 1.0f);

                // If you only want to process the first collision, you might break here.
                // Otherwise, it continues, and static info gets overwritten.
            }
        }
    }

    // --- Collision State Management Method Implementations ---
    void collisionSystem::setBodyInfo(const platformBody& platform) {
        // Copy relevant data from 'platform' to the static 'm_bodyInfo'
        // This requires platformbody.hpp to be included and for platformBody to have these members.
        m_bodyInfo.m_type = platform.m_type;
        m_bodyInfo.m_id = platform.m_id;
        m_bodyInfo.m_width = platform.m_width;
        m_bodyInfo.m_height = platform.m_height;
        m_bodyInfo.m_position = platform.m_position;
        m_bodyInfo.m_surfaceVelocity = platform.m_surfaceVelocity;
        m_bodyInfo.m_falling = platform.m_falling;
        // ... any other relevant fields ...
    }

    bodyInfo collisionSystem::getBodyInfo() const {
        return m_bodyInfo; // Returns the current state of the static member
    }

    void collisionSystem::setCollisionInfo(bool t, bool b, bool l, bool r) {
        // Update the static collisionInfo struct
        m_collisionInfo.m_collisionTop = t;
        m_collisionInfo.m_collisionBottom = b;
        m_collisionInfo.m_collisionLeft = l;
        m_collisionInfo.m_collisionRight = r;

        // Also update the individual static bools (if they are truly needed separately)
        m_collisionTop = t;
        m_collisionBottom = b;
        m_collisionLeft = l;
        m_collisionRight = r;
    }

    collisionInfo collisionSystem::getCollisionInfo() const {
        return m_collisionInfo; // Returns the current state of the static member
    }


    // --- Internal Static Method Implementations ---
    bool collisionSystem::movingToStaticIntersectionTest(dynamicBody* PtrDynamicBody, const platformBody& platform, collision* outCollision) {
        if (!PtrDynamicBody || !outCollision) return false;

        // TODO: Implement your actual AABB vs. AABB (or other shape) intersection test here.
        // This is a complex part. For now, a placeholder:
        // Example: Basic AABB check (assumes members like m_position, m_width, m_height exist)
        sf::FloatRect bodyRect(PtrDynamicBody->m_position, {PtrDynamicBody->m_width, PtrDynamicBody->m_height});
        sf::FloatRect platformRect(platform.m_position, {platform.m_width, platform.m_height});

        if (bodyRect.intersects(platformRect)) {
            // Crude way to determine collision normal/axis for placeholder
            // A real system would use sweep tests (e.g., separating axis theorem with movement vector)
            // to find time of impact and a more accurate normal.
            sf::Vector2f bodyCenter = PtrDynamicBody->m_position + sf::Vector2f(PtrDynamicBody->m_width / 2.f, PtrDynamicBody->m_height / 2.f);
            sf::Vector2f platformCenter = platform.m_position + sf::Vector2f(platform.m_width / 2.f, platform.m_height / 2.f);
            sf::Vector2f diff = bodyCenter - platformCenter;
            sf::Vector2f overlap;
            overlap.x = (PtrDynamicBody->m_width / 2.f + platform.m_width / 2.f) - std::abs(diff.x);
            overlap.y = (PtrDynamicBody->m_height / 2.f + platform.m_height / 2.f) - std::abs(diff.y);

            if (overlap.x < overlap.y) { // Horizontal collision is shallower
                outCollision->axis = 0; // X-axis
                outCollision->normal = sf::Vector2f( (diff.x < 0.f ? -1.f : 1.f), 0.f);
            } else { // Vertical collision is shallower
                outCollision->axis = 1; // Y-axis
                outCollision->normal = sf::Vector2f(0.f, (diff.y < 0.f ? -1.f : 1.f) );
            }
            outCollision->time = 0.f; // Placeholder, a sweep test would calculate this
            return true;
        }
        return false;
    }

    void collisionSystem::resolveStaticCollision(dynamicBody* PtrDynamicBody, const collision& colData, float timesliceRemaining) {
        if (!PtrDynamicBody) return;

        // TODO: Implement your actual collision response logic.
        // This typically involves adjusting PtrDynamicBody->m_position and PtrDynamicBody->m_velocity
        // based on colData.normal, colData.time, and the properties of the bodies.
        // Example: Simple position correction (very basic, doesn't handle velocity or true TOI)
        
        // This is a very naive penetration resolution.
        // A better system would use the time of impact (if available from a sweep test)
        // to move the dynamic body TO the point of collision, then adjust velocity.
        sf::FloatRect bodyRect(PtrDynamicBody->m_position, {PtrDynamicBody->m_width, PtrDynamicBody->m_height});
        // Need to know which platform it hit for its rect. This info isn't directly in `colData` from the
        // simplified `movingToStaticIntersectionTest`. This highlights a limitation of the current structure.
        // For now, let's assume `m_bodyInfo` (the static one) holds the platform info.

        sf::FloatRect platformRect(m_bodyInfo.m_position, {m_bodyInfo.m_width, m_bodyInfo.m_height});
        sf::FloatRect intersection;
        bodyRect.intersects(platformRect, intersection);


        if (colData.normal.x != 0) { // Horizontal collision
            PtrDynamicBody->m_position.x -= colData.normal.x * intersection.width;
            PtrDynamicBody->m_velocity.x = 0; // Simplistic: stop horizontal movement
        }
        if (colData.normal.y != 0) { // Vertical collision
            PtrDynamicBody->m_position.y -= colData.normal.y * intersection.height;
            PtrDynamicBody->m_velocity.y = 0; // Simplistic: stop vertical movement
            if (colData.normal.y > 0) { // Hit from below (landed on top)
                // Potentially set onGround flag for player
            }
        }
        // std::cout << "Resolved collision. Player new pos: " << PtrDynamicBody->m_position.x << ", " << PtrDynamicBody->m_position.y << std::endl;
    }

} // namespace phys