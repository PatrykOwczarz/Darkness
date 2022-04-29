#pragma once
#include <engine.h>
#include "player.h"
#include "enemy.h"
#include "friend_npc.h"
#include "start_screen.h"
#include "electric_explosion.h"
#include "cross_fade.h"
#include "projectile.h"
#include "engine/entities/bounding_box_bullet.h"

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
	void check_bounce();

	player								m_player{};
	enemy								m_enemy{};
	enemy								m_enemy2{};
	friend_npc							m_npc{};

	engine::bounding_box				m_cow_box;
	engine::bounding_box				m_enemy_box1;
	engine::bounding_box				m_vial_box;
	engine::bounding_box				m_powerup_box;
	engine::bounding_box				m_enemy_model_box1;
	engine::bounding_box				m_enemy_model_box2;

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_wall{};
	engine::ref<engine::game_object>	m_cow{};

	engine::ref<engine::game_object>	m_powerup{};
	
	// my imported mesh objects:
	engine::ref<engine::game_object>	m_barrel{};
	engine::ref<engine::game_object>	m_enemy_model1{};
	engine::ref<engine::game_object>	m_enemy_model2{};

	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_ball2{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_tetrahedron{};

	// my primitive:
	engine::ref<engine::game_object>	m_vial{};
	engine::ref<engine::game_object>	m_vial2{};
	engine::ref<engine::game_object>	m_cork{};
	engine::ref<engine::game_object>	m_cork2{};

	engine::ref<engine::game_object>    m_electric_crystal{};

	engine::ref<engine::game_object>    m_lamp{};

	engine::ref<engine::material>		m_material_blue{};
	engine::ref<engine::material>		m_material_orange{};
	engine::ref<engine::material>		m_lightsource_material{};
	engine::ref<engine::material>		m_tetrahedron_material{};

	engine::DirectionalLight            m_directionalLight;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera			m_2d_camera; 
    engine::perspective_camera			m_3d_camera;

	// adding my spotlight for my flashlight here
	engine::SpotLight					m_spotLight;
	uint32_t							num_spot_lights = 1;

	engine::PointLight					m_pointLight1;
	engine::PointLight					m_pointLight2;
	uint32_t							num_point_lights = 2;

	engine::ref<start_screen>			m_start_screen;
	engine::ref<electric_explosion>		m_explosion;
	engine::ref<cross_fade>				m_damage_fade;

	projectile							m_projectile;
	projectile							m_enemy_projectile;
};
