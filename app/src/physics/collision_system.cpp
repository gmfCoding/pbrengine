#include "physics/collision_system.hpp"

CollisionSystem::CollisionSystem() : staticToBoxes{}, aabbToStaticbodies{}, alignedBoxes{}
{

}


void CollisionSystem::Add(Staticbody* body)
{
    aabbToStaticbodies.emplace(body->aabb, body);
    staticToBoxes.emplace(body, body->aabb);
    alignedBoxes.push_back(body->aabb);
}

void CollisionSystem::ApplyCollisions()
{
    int size = alignedBoxes.size();
    
    for (size_t i = 0; i < size; i++)
    {
        for (size_t n = i; n < size; n++)
        {
            if(i == n)
                continue;
            Collide(alignedBoxes[i], alignedBoxes[n]);
        }
    }
}

void CollisionSystem::Collide(AABB* first, AABB* second)
{

    if(!first->intersect(*second))
        return;
    
    Staticbody* sb_first = aabbToStaticbodies[first];
    Staticbody* sb_second = aabbToStaticbodies[second];
    if(sb_first->isStatic() && sb_second->isStatic())
        return;

    glm::vec3 direction = *sb_first->position - *sb_second->position;

    glm::vec3 normal = AABBCollisionNormal(*first, *second, direction);
	float penetration = normal.length();
	normal = glm::normalize(normal);
    glm::vec3 anitNorm = physutil::antivector(glm::abs(normal));

    float ratio;
    if(!sb_first->isStatic() && !sb_second->isStatic())
        ratio = 0.5f;
    else if(sb_first->isStatic())
        ratio = 0.0f;
    else
        ratio = 1.0f;

    // *sb_first->position += (peneration_norm * ratio);
    // *sb_second->position += (peneration_norm * (1.0f - ratio));
	glm::vec3 sb1vel = glm::vec3(0);
	glm::vec3 sb2vel = glm::vec3(0);
	if(!sb_first->isStatic())
		sb1vel = ((Rigidbody*)sb_first)->velocity;
	if(!sb_second->isStatic())
    	sb2vel = ((Rigidbody*)sb_second)->velocity;
	glm::vec3 relative = sb2vel - sb1vel;

	float velAlongNormal = glm::dot(relative, normal);
	// If velocities are separating
	if (velAlongNormal < 0) 
		return;

	float j = -1 * velAlongNormal;

	glm::vec3 impulse = j * normal;

    if(!sb_first->isStatic())
        ((Rigidbody*)sb_first)->velocity += impulse;

    if(!sb_second->isStatic())
        ((Rigidbody*)sb_second)->velocity += impulse;


	const float percentage = 0.001f;
	const float slop = 0.00f;
	float fcorr = glm::max(penetration - slop, 0.0f);
	glm::vec3 correction = glm::vec3(fcorr, fcorr, fcorr) * percentage * normal;
	if(!sb_first->isStatic())
		*sb_first->position -= correction;
	if(!sb_second->isStatic())
    	*sb_second->position -= correction;
}


glm::vec3 CollisionSystem::AABBCollisionNormal(AABB abox, AABB bbox, glm::vec3 direction)
{
    glm::vec3 normal {};
    float penetration = 0.0f;
    bool overlapped = false;
    // Calculate half extents along x axis for each object
    float aExtentX = (abox.max.x - abox.min.x) / 2;
    float bExtentX = (bbox.max.x - bbox.min.x) / 2;

    // Calculate overlap on x axis
    float xOverlap = aExtentX + bExtentX - std::abs(direction.x);

    // SAT test on x axis
    if (xOverlap > 0)
    {
        // Calculate half extents along y axis for each object
        float aExtentY = (abox.max.y - abox.min.y) / 2;
        float bExtentY = (bbox.max.y - bbox.min.y) / 2;

        // Calculate overlap on y axis
        float yOverlap = aExtentY + bExtentY - std::abs(direction.y);

        // SAT test on y axis
        if (yOverlap > 0)
        {
            // Calculate half extents along z axis for each object
            float aExtentZ = (abox.max.z - abox.min.z) / 2;
            float bExtentZ = (bbox.max.z - bbox.min.z) / 2;

            // Calculate overlap on z axis
            float zOverlap = aExtentZ + bExtentZ - std::abs(direction.z);

            // SAT test on z axis
            if (zOverlap > 0)
            {
                // Find out which axis is axis of least penetration
                if (xOverlap < yOverlap && xOverlap < zOverlap)
                {
                    // Point towards B knowing that n points from A to B
                    if (direction.x < 0)
                        normal = glm::vec3(1, 0, 0);
                    else
                        normal = glm::vec3(-1, 0, 0);

                    penetration = xOverlap;
                    overlapped = true;
                }
                else if (yOverlap < zOverlap)
                {
                    // Point towards B knowing that n points from A to B
                    if (direction.y < 0)
                        normal = glm::vec3(0, 1, 0);
                    else
                        normal = glm::vec3(0, -1, 0);

                    penetration = yOverlap;
                    overlapped = true;
                }
                else
                {
                    // Point towards B knowing that n points from A to B
                    if (direction.z < 0)
                        normal = glm::vec3(0, 0, 1);
                    else
                        normal = glm::vec3(0, 0, -1);

                    penetration = zOverlap;
                    overlapped = true;
                }
            }
        }
    }

    return normal * penetration; 
}