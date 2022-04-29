#include "player.h"
#include "pch.h"
#include <engine.h>
#include "engine/core/input.h"
#include "engine/key_codes.h"


player::player() {
	m_timer = 0.0f;
	m_speed = 1.0f;

}

player::~player() {

}

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	m_object->animated_mesh()->set_default_animation(1);
}

void player::on_update(const engine::timestep& time_step)
{
	m_object->animated_mesh()->on_update(time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	m_object->set_right(glm::normalize(glm::cross(m_object->up(), m_object->forward())));

	// my implementation for movement
	// w sets forward_val to a positive value
	if (engine::input::key_pressed(engine::key_codes::KEY_W))
	{
		forward_val = 1.f;
	}
	// s sets forward_val to a negative value
	else if (engine::input::key_pressed(engine::key_codes::KEY_S))
	{
		forward_val = -1.f;
	}
	else
		forward_val = 0.f;

	// D sets forward_val to a negative value
	if (engine::input::key_pressed(engine::key_codes::KEY_D))
	{
		right_val = -1.f;
	}
	// A sets forward_val to a positive value
	else if (engine::input::key_pressed(engine::key_codes::KEY_A))
	{
		right_val = 1.f;
	}
	else
		right_val = 0.f;

	// takes the input from the keyboard input and calculates a vector that will cause the player to face in one of 8 positions
	// and move in that direction when the values for forward and right are not 0.
	if (forward_val != 0.f || right_val != 0.f) {
		glm::vec3 facingDir = glm::normalize((forward_val * glm::normalize(m_object->forward())) + (right_val * m_object->right()));
		// optional implementation where the player model turns to the movement direction every time, doesnt work with my flashlight well...
		//m_object->turn_towards(facingDir);
		//m_object->set_position(m_object->position() += facingDir * m_speed * (float)time_step);
		m_object->set_velocity(facingDir * m_speed);
	}
	else
	{
		m_object->set_velocity(glm::vec3(0.f));
		m_object->set_angular_velocity(glm::vec3(0.f));
	}
	
	// pressing left shift toggles sprint and regular walking speeds.
	if (engine::input::key_pressed(engine::key_codes::KEY_LEFT_SHIFT))
		m_speed = 4.f;
	else
		m_speed = 2.f;

	// when you right click you enter free_look mode
	if (engine::input::mouse_button_pressed(1)) {
		m_free_look = true;
	}
	else {
		m_free_look = false;
	}

	// a timer that controls how often the player can be damaged
	if (m_damage_timer > 0.0f)
	{
		m_damage_timer -= (float)time_step;
		if (m_damage_timer < 0.0f)
		{
			is_damaged = false;
		}
	}

	m_player_box.on_update(object()->position() - glm::vec3(0.f, object()->offset().y, 0.f) * object()->scale(), object()->rotation_amount(), object()->rotation_axis());
}

void player::turn(float angle)
{
	//m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
	m_object->set_angular_velocity(glm::vec3(0.f, angle*100.f, 0.f));

}


void player::update_camera(engine::perspective_camera& camera, const engine::timestep& time_step)
{
	
	float A = 2.f, B = 3.f, C = 6.f;
	// free look camera can be toggled by pressing the 
	if(m_free_look)
	{
		// a free look version of the below camera that only rotates the camera around the player
		// and the player's forward vector stays the same.
		camera.on_update(time_step);
		glm::vec3 direction = camera.front_vector();
		glm::vec3 cam_pos = m_object->position() - glm::normalize(direction) * B;
		cam_pos.y += A;
		glm::vec3 cam_look_at = m_object->position() + glm::normalize(direction) * C;
		cam_look_at.y = 0.f;
		camera.set_view_matrix(cam_pos, cam_look_at);
	}
	else
	{
		// processes the mouse and sets the player forward vector and the player's forward
		// vector also to make the player face where the camera is pointing
		camera.on_update(time_step);
		glm::vec3 direction = camera.front_vector();
		glm::vec3 cam_pos = m_object->position() - glm::normalize(direction) * B;
		cam_pos.y += A;
		glm::vec3 cam_look_at = m_object->position() + glm::normalize(direction) * C;
		cam_look_at.y = 0.f;
		camera.set_view_matrix(cam_pos, cam_look_at);

		glm::vec3 player_forward = camera.front_vector();
		player_forward.y = 0.f;

		// fixed a problem with physics here, credit goes to my colleague Alain Siow :)
		m_object->set_forward(player_forward);
		m_object->set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));
		//float angle = atan2(camera.front_vector().x, camera.front_vector().z);
		//m_object->set_rotation_amount(angle);
		

		// Old 3d camera code.
		/*glm::vec3 direction = m_object->forward();
		glm::vec3 cam_pos = m_object->position() - glm::normalize(direction) * B;
		cam_pos.y += A;
		glm::vec3 cam_look_at = m_object->position() + glm::normalize(direction) * C;
		cam_look_at.y = 0.f;
		camera.set_view_matrix(cam_pos, cam_look_at);*/
	}

}

void player::jump()
{
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);
	m_speed = 0.0f;
	m_timer = m_object->animated_mesh()->animations().at(3)->mDuration;
}
