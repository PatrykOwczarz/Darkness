#include "pch.h"
#include "friend_npc.h"


friend_npc::friend_npc() {}
friend_npc::~friend_npc() {}
void friend_npc::initialise(engine::ref<engine::game_object> object, glm::vec3 position,
	glm::vec3 forward)
{
	m_object = object;
	m_object->set_forward(forward);
	m_object->set_position(position);

}
void friend_npc::on_update(const engine::timestep& time_step, const glm::vec3& player_position, const glm::vec3& enemy_position)
{
	float distance_to_player = glm::distance(m_object->position(), player_position);
	float distance_to_enemy = glm::distance(m_object->position(), enemy_position);

	// check which state is the npc in, then execute the matching behaviour
	if (m_state == state::following)
	{
		if(distance_to_player > 2.0f)
			chase_player(time_step, player_position);

		// check whether the condition has been met to switch to the on_guard state
		if (distance_to_enemy < m_detection_radius)
			m_state = state::on_guard;
	}
	else if (m_state == state::on_guard)
	{
		face_enemy(time_step, player_position);
		// check whether the condition has been met to switch back to following state
		if (distance_to_enemy > m_detection_radius)
			m_state = state::following;
		// check whether the condition has been met to switch to the chasing state
		else if (distance_to_enemy < m_trigger_radius)
			m_state = state::chasing;
	}
	else
	{
		if(distance_to_enemy > 2.0f)
			chase_enemy(time_step, enemy_position);
		if (distance_to_enemy < 2.0f)
			keep_distance(time_step, enemy_position);

		// check whether the condition has been met to switch back to folowing state
		if (distance_to_enemy > m_detection_radius)
			m_state = state::following;
	}

}


// stop and look at the enemy
void friend_npc::face_enemy(const engine::timestep& time_step, const glm::vec3&
	player_position)
{
	m_object->set_forward(player_position - m_object->position());
	m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}




// move forwards in the direction of the player
void friend_npc::chase_player(const engine::timestep& time_step, const glm::vec3&
	player_position)
{
	m_object->set_forward(player_position - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

// move forwards in the direction of the enemy
void friend_npc::chase_enemy(const engine::timestep& time_step, const glm::vec3&
	enemy_position)
{
	m_object->set_forward(enemy_position - m_object->position());
	m_object->set_position(m_object->position() + m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}


// the npc moves back to keep a distance from the enemy
void friend_npc::keep_distance(const engine::timestep& time_step, const glm::vec3&
	enemy_position)
{
	m_object->set_forward(enemy_position - m_object->position());
	m_object->set_position(m_object->position() - m_object->forward() * m_speed * (float)time_step);
	m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}
