#pragma once
#include <engine.h>

class projectile
{
public:
	projectile();
	~projectile();

	void initialise(engine::ref<engine::game_object> object);
	void shoot(glm::vec3 player_pos, glm::vec3 player_forward, float kick);
	void on_update(const engine::timestep& time_step);
	void on_render(const engine::ref<engine::shader>& shader);

	glm::vec3 last_position() const { return m_last_position; }

	engine::ref<engine::game_object> object() const { return m_object; }

	// getters and setters for m_shot and m_destroyed
	bool get_shot() { return m_shot; }
	void set_shot(bool is_shot) { m_shot = is_shot; }

	bool get_destroyed() { return m_destroyed; }
	void set_destroyed(bool is_destroyed) { m_destroyed = is_destroyed; }

	bool get_blue() { return m_blue; }
	void set_blue(bool is_blue) { m_blue = is_blue; }

	float get_timer() { return m_electric_timer; }
	void set_timer(float timer) { m_electric_timer = timer; }

private:
	engine::ref<engine::game_object> m_object;

	glm::vec3 m_instantaneous_acceleration{ 0.f };
	glm::vec3 m_instantaneous_angular_acceleration{ 0.f };

	float m_contact_time = 0.0f;

	float m_rotational_inertia = 0.f;

	float m_phi, m_theta;

	glm::vec3 m_last_position{ 0.f };

	// bool that allows me to shoot a single bullet
	bool m_shot = false;
	// bool that controls if the bullet is destroyed as a result of a collision
	bool m_destroyed = false;
	// bool that controls the bullet color for powerup or no powerup
	bool m_blue = false;
	// timer to control the time between shots
	float m_timer = 0.f;
	// timer to control the time left on the electric powerup
	float m_electric_timer = 0.f;


	// Methods to handle collisions with the ground
	bool collision_detection(float y_plane);
	void collision_response(float y_plane);
};
