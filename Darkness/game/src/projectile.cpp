#include "pch.h"
#include "projectile.h"

projectile::projectile()
{}

projectile::~projectile()
{}

void projectile::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_rotational_inertia = (2.f / 3.f) * object->mass() * (m_object->bounding_shape().y / 2.f) * (m_object->bounding_shape().y / 2.f);
	m_contact_time = 0.05f;
}

void projectile::shoot(glm::vec3 player_pos, glm::vec3 player_forward, float kick)
{
	// if the m_shot is not true, you can shoot a bullet, this prevents machine gun shooting
	if (!m_shot)
	{
		m_object->set_velocity(glm::vec3(0.f));
		m_object->set_acceleration(glm::vec3(0.f, 0.f, 0.f));
		m_object->set_torque(glm::vec3(0.f));
		m_object->set_rotation_amount(0.f);
		m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
		m_object->set_angular_velocity(glm::vec3(0.f));
		m_contact_time = 0.0f;

		// Set the ball to the current player position
		m_object->set_position(glm::vec3(player_pos.x, player_pos.y + 0.4f, player_pos.z));
		float acceleration_const = 3.0f * kick;
		m_object->set_acceleration(glm::normalize(player_forward) * acceleration_const);

		// Determine rotation angles of camera (from Lab 4)
		m_theta = engine::PI / 2.f - acos(player_forward.y);
		m_phi = atan2(player_forward.x, player_forward.z);
		m_timer = 1.5f;
		m_shot = true;
		m_destroyed = false;
	}
}

void projectile::on_update(const engine::timestep& time_step)
{
	// Update physical quanitities
	m_last_position = m_object->position();

	// A timer that resets the m_shot bool, allowing the player to shoot another projectile
	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			m_shot = false;
		}
	}
	// A timer that resets the m_blue bool, stopping the powerup for the player.
	if (m_electric_timer > 0.0f) {
		m_electric_timer -= (float)time_step;
		if (m_electric_timer < 0.0f)
		{
			m_blue = false;
		}
	}
}

void projectile::on_render(const engine::ref<engine::shader>& shader)
{
	// The bullet only renders if it has been shot and not destroyed
	if (!m_destroyed && m_shot)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_object->position());
		//transform = glm::rotate(transform, -m_theta, glm::vec3(1.f, 0.f, 0.f));
		//transform = glm::rotate(transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
		transform = glm::rotate(transform, m_object->rotation_amount(), m_object->rotation_axis());
		engine::renderer::submit(shader, transform, m_object);
	}
}

bool projectile::collision_detection(float y_plane)
{
	// Check for collision with the ground by looking at the y value of the ball's position
	if (m_object->position().y - m_object->bounding_shape().y < y_plane && m_object->velocity().y < 0) {
		return true;
	}
	return false;
}

void projectile::collision_response(float y_plane)
{
	float convergenceThreshold = 0.5f;
	if (glm::length(m_object->velocity()) > convergenceThreshold) {
		// The ball has bounced!  Implement a bounce by flipping the y velocity
		m_object->set_velocity(glm::vec3(m_object->velocity().x, -m_object->velocity().y, m_object->velocity().z));
	}
	else {
		// Velocity of the ball is below a threshold.  Stop the ball. 
		m_object->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
		m_object->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
		m_object->set_position(glm::vec3(m_object->position().x, m_object->bounding_shape().y + y_plane, m_object->position().z));
		m_object->set_angular_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}
