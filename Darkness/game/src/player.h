#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"
#include "engine/entities/bounding_box_bullet.h"

class player
{
public:
	player();
	~player();

	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);
	engine::ref<engine::game_object> object() const { return m_object; }


	void turn(float angle);
	void update_camera(engine::perspective_camera& camera, const engine::timestep& time_step);

	void jump();

	void set_box(float width, float height, float depth, glm::vec3 position) { m_player_box.set_box(width, height, depth, position); }
	engine::bounding_box& getBox() { return m_player_box; };

	// setters and getters for health, armour and score.
	int getHealth() { return m_health; };
	void setHealth(int health) { m_health = health; };
	int getArmour() { return m_armour; };
	void setArmour(int armour) { m_armour += armour; };
	int getScore() { return m_score; };
	void setScore(int score) { m_score += score; };
	bool getDead() { return is_dead; };
	void setDead(bool dead) { is_dead = dead; };
	bool getDamaged() { return is_damaged; };
	void setDamaged(bool damaged) { is_damaged = damaged; };
	float get_timer() { return m_damage_timer; }
	void set_timer(float timer) { m_damage_timer = timer; }


private:
	float m_speed{ 0.f };
	float m_timer;
	bool m_free_look;
	float forward_val{ 0.f };
	float right_val{ 0.f };

	engine::bounding_box m_player_box;

	// player values that will affect the game
	int m_health{ 100 };
	int m_armour{ 0 };
	int m_score{ 0 };
	bool is_dead = false;
	bool is_damaged = false;
	float m_damage_timer = 0.f;

	engine::ref< engine::game_object> m_object;
};
