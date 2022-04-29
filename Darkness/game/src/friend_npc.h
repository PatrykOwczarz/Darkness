#pragma once
#include <engine.h>

class friend_npc
{
	enum class state
	{
		following,
		on_guard,
		chasing,
	};

public:
	friend_npc();
	~friend_npc();
	// set the parameters for the npc
	void initialise(engine::ref<engine::game_object> object, glm::vec3 position,
		glm::vec3 forward);
	// update the npc
	void on_update(const engine::timestep& time_step, const glm::vec3& player_position, const glm::vec3& enemy_position);
	// methods controlling the enemy’s behaviour in a certain state

	void face_enemy(const engine::timestep& time_step, const glm::vec3&
		player_position);
	void chase_player(const engine::timestep& time_step, const glm::vec3&
		player_position);
	void chase_enemy(const engine::timestep& time_step, const glm::vec3&
		enemy_position);
	void keep_distance(const engine::timestep& time_step, const glm::vec3&
		enemy_position);


	// game object bound to the npc
	engine::ref<engine::game_object> object() const { return m_object; }


private:
	// enemy's speed
	float m_speed{ 0.5f };
	// timer controlling the direction switch and the reset value for this timer
	float m_default_time{ 4.f };
	float m_switch_direction_timer = m_default_time;

	// threshold distances
	float m_detection_radius{ 6.f };
	float m_trigger_radius{ 4.f };

	// game object bound to the npc
	engine::ref< engine::game_object> m_object;
	// npc health,
	int m_health = 2;
	//current state of the enemy's state machine
	state m_state = state::following;
};
