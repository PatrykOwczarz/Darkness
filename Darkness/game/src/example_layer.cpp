#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"

example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{

    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/abstract-background-music.wav", engine::sound_type::track, "music");		// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/laser-gun.wav", engine::sound_type::event, "laser");						// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/death-sound-male.wav", engine::sound_type::event, "player_death");		// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/death-sound-male.wav", engine::sound_type::event, "player_death");		// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/enem_crooked_att1.wav", engine::sound_type::event, "enemy_att1");			// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/enem_crooked_att2.wav", engine::sound_type::event, "enemy_att2");			// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/enem_crooked_hit1.wav", engine::sound_type::event, "enemy_hit1");			// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/enem_crooked_hit2.wav", engine::sound_type::event, "enemy_hit2");			// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/enem_crooked_dead.wav", engine::sound_type::event, "enemy_dead");			// Attribution to all sound assets in the report
	m_audio_manager->load_sound("assets/audio/sfx_tcell_blown.wav", engine::sound_type::event, "vial");					// Attribution to all sound assets in the report

	m_audio_manager->play("music");
	m_audio_manager->volume("music", 0.2f);

	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.01f;
	m_directionalLight.DiffuseIntensity = 0.01f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	m_pointLight1.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_pointLight1.AmbientIntensity = 0.1f;
	m_pointLight1.DiffuseIntensity = 0.2f;
	m_pointLight1.Position = glm::vec3(-20.f, 2.7f, 8.f); // positioned on the lamp

	m_pointLight2.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_pointLight2.AmbientIntensity = 0.1f;
	m_pointLight2.DiffuseIntensity = 0.3f;
	m_pointLight2.Position = glm::vec3(22.43f, 2.7f, 8.f); // positioned to the second lamp

	m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight.AmbientIntensity = 0.9f;
	m_spotLight.DiffuseIntensity = 0.6f;
	m_spotLight.Position = glm::vec3(-5.f, 2.f, 5.f);
	m_spotLight.Direction = glm::vec3(1.f, -1.f, 0.f);
	m_spotLight.Cutoff = 0.8f;
	m_spotLight.Attenuation.Constant = 1.0f;
	m_spotLight.Attenuation.Linear = 0.1f;
	m_spotLight.Attenuation.Exp = 0.01f;

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 0.1f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.1f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));

	m_material_blue = engine::material::create(1.0f, glm::vec3(0.f, 0.f, 0.8f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_material_orange = engine::material::create(1.0f, glm::vec3(0.75f, 0.75f, 0.f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_lightsource_material = engine::material::create(1.0f, m_pointLight1.Color,
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	// Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
	// Used the same website as above for the skybox. Skybox changed in this part ofthe code too.
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox2/SkyboxFront.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox2/SkyboxRight.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox2/SkyboxBack.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox2/SkyboxLeft.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox2/SkyboxTop.bmp", true),
		  engine::texture_2d::create("assets/textures/skybox2/SkyboxBottom.bmp", true)
		});

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(3.0f, 1.5f, -5.0f);
	mannequin_props.rotation_amount = engine::PI;
	mannequin_props.rotation_axis = glm::vec3(0.f, 1.f, 0.f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x / 4.f, m_skinned_mesh->size().y / 2.f, m_skinned_mesh->size().x / 4.f);
	mannequin_props.friction = 0.0f;

	m_mannequin = engine::game_object::create(mannequin_props);
	// enable this to make bullet handle player physics
	m_mannequin->set_offset(m_skinned_mesh->offset());

	m_player.initialise(m_mannequin);
	m_player.set_box(mannequin_props.bounding_shape.x * 2.f *
		mannequin_props.scale.x, mannequin_props.bounding_shape.y * 2.f *
		mannequin_props.scale.x, mannequin_props.bounding_shape.z * 2.f *
		mannequin_props.scale.x, mannequin_props.position);

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	// Changed terrain here, reference to the terrain is in the report
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/terrain2.bmp", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	// Load the cow model. Create a cow object. Set its properties
	engine::ref <engine::model> cow_model = engine::model::create("assets/models/static/cow4.3ds");
	engine::game_object_properties cow_props;
	cow_props.meshes = cow_model->meshes();
	cow_props.textures = cow_model->textures();
	float cow_scale = 1.f / glm::max(cow_model->size().x, glm::max(cow_model->size().y, cow_model->size().z));
	cow_props.position = { -2.f, 2.f, 10.f };
	cow_props.scale = glm::vec3(cow_scale);
	cow_props.bounding_shape = cow_model->size() / 2.f;
	cow_props.type = 0;

	m_cow = engine::game_object::create(cow_props);
	m_cow->set_offset(cow_model->offset());
	m_cow_box.set_box(cow_props.bounding_shape.x * 2.f * cow_scale,
		cow_props.bounding_shape.y * 2.f * cow_scale,
		cow_props.bounding_shape.z * 2.f* cow_scale,
		cow_props.position - glm::vec3(0.f, m_cow->offset().y, 0.f) * m_cow->scale());

	m_npc.initialise(m_cow, cow_props.position, glm::vec3(1.0f, 0.f, 0.f));

	// Load the first enemy model and set its properties
	engine::ref <engine::model> enemy_model = engine::model::create("assets/models/static/CH1.obj");
	engine::game_object_properties enemy_props;
	enemy_props.meshes = enemy_model->meshes();
	enemy_props.textures = enemy_model->textures();
	float enemy_scale = 1.f / glm::max(enemy_model->size().x, glm::max(enemy_model->size().y, enemy_model->size().z));
	enemy_props.position = { 4.f, 1.f, 2.f };
	enemy_props.scale = glm::vec3(enemy_scale);
	enemy_props.mass = 50.f;
	enemy_props.bounding_shape = enemy_model->size() / 2.f;
	enemy_props.type = 0;

	m_enemy_model1 = engine::game_object::create(enemy_props);
	m_enemy_model1->set_offset(enemy_model->offset());
	m_enemy_model_box1.set_box(enemy_props.bounding_shape.x * 2.f *
		enemy_props.scale.x, enemy_props.bounding_shape.y * 2.f *
		enemy_props.scale.x, enemy_props.bounding_shape.z * 2.f *
		enemy_props.scale.x, enemy_props.position);

	m_enemy.initialise(m_enemy_model1, enemy_props.position, glm::vec3(1.f, 0.f, 0.f), 1);

	// Load the second enemy model and set its properties
	engine::ref <engine::model> enemy_model2 = engine::model::create("assets/models/static/ch2.obj");
	engine::game_object_properties enemy_props2;
	enemy_props2.meshes = enemy_model2->meshes();
	enemy_props2.textures = enemy_model2->textures();
	float enemy_scale2 = 1.f / glm::max(enemy_model2->size().x, glm::max(enemy_model2->size().y, enemy_model2->size().z));
	enemy_props2.position = { -4.f, 1.f, 2.f };
	enemy_props2.scale = glm::vec3(enemy_scale2);
	enemy_props2.mass = 50.f;
	enemy_props2.bounding_shape = enemy_model2->size() / 2.f;
	enemy_props2.type = 0;
	m_enemy_model2 = engine::game_object::create(enemy_props2);
	m_enemy_model2->set_offset(enemy_model2->offset()+ glm::vec3(0.f,0.f,0.f));
	m_enemy_model_box2.set_box(enemy_props2.bounding_shape.x * 2.f *
		enemy_props2.scale.x, enemy_props2.bounding_shape.y * 2.f *
		enemy_props2.scale.x, enemy_props2.bounding_shape.z * 2.f *
		enemy_props2.scale.x, enemy_props2.position);

	m_enemy2.initialise(m_enemy_model2, enemy_props2.position, glm::vec3(1.f, 0.f, 0.f), 2);

	// Load the barrel model. Create a barrel object. Set its properties.
	engine::ref <engine::model> barrel_model = engine::model::create("assets/models/static/barrel.obj");
	engine::game_object_properties barrel_props;
	barrel_props.meshes = barrel_model->meshes();
	barrel_props.textures = barrel_model->textures();
	float barrel_scale = 0.125f;
	barrel_props.position = { -20.f, 0.5f, 5.f };
	barrel_props.scale = glm::vec3(barrel_scale);
	barrel_props.bounding_shape = barrel_model->size() / 2.f * barrel_scale;
	m_barrel = engine::game_object::create(barrel_props);

	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.25f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, -5.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.25f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.45f;
	m_ball = engine::game_object::create(sphere_props);

	m_ball2 = engine::game_object::create(sphere_props);

	std::vector<engine::ref<engine::texture_2d>> lamp_textures =
	{ engine::texture_2d::create("assets/textures/stone.bmp", false) };

	std::vector<engine::ref<engine::texture_2d>> vial_textures =
	{ engine::texture_2d::create("assets/textures/glass.bmp", false) };

	std::vector<engine::ref<engine::texture_2d>> cork_textures =
	{ engine::texture_2d::create("assets/textures/cork.bmp", false) };

	std::vector<engine::ref<engine::texture_2d>> electric_powerup_textures =
	{ engine::texture_2d::create("assets/textures/electric.bmp", false) };

	engine::ref<engine::cuboid> wall_shape = engine::cuboid::create(glm::vec3(25.f, 1.f, 1.f), false);
	engine::game_object_properties wall_props;
	wall_props.meshes = { wall_shape->mesh() };
	wall_props.position = glm::vec3(0.f, 0.5f, -10.f);
	wall_props.is_static = true;
	wall_props.type = 0;
	wall_props.bounding_shape = glm::vec3(25.f, 1.f, 1.f);
	wall_props.restitution = 0.92f;
	m_wall = engine::game_object::create(wall_props);

	// Create a tetrahedron
	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(0.f, 10.f, 0.f));//0
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 10.f));//1
	tetrahedron_vertices.push_back(glm::vec3(-10.f, 0.f, -10.f)); //2
	tetrahedron_vertices.push_back(glm::vec3(10.f, 0.f, -10.f)); //3
	engine::ref<engine::tetrahedron> tetrahedron_shape = engine::tetrahedron::create(tetrahedron_vertices);
	engine::game_object_properties tetrahedron_props;
	tetrahedron_props.position = { 0.f, 0.5f, -20.f };
	tetrahedron_props.meshes = { tetrahedron_shape->mesh() };
	tetrahedron_props.textures = lamp_textures;
	m_tetrahedron = engine::game_object::create(tetrahedron_props);

	// Create a vial
	std::vector<glm::vec3> vial_vertices;
	/*vial_vertices.push_back(glm::vec3(1.f, 0.f, 0.f)); //0
	vial_vertices.push_back(glm::vec3(0.f, 0.f, -1.f)); //1
	vial_vertices.push_back(glm::vec3(-1.f, 0.f, 0.f)); //2
	vial_vertices.push_back(glm::vec3(0.f, 0.f, 1.f)); //3
	vial_vertices.push_back(glm::vec3(0.5f, 1.f, 0.f)); //4
	vial_vertices.push_back(glm::vec3(0.f, 1.f, -0.5f)); //5
	vial_vertices.push_back(glm::vec3(-0.5f, 1.f, 0.f)); //6
	vial_vertices.push_back(glm::vec3(0.f, 1.f, 0.5f)); //7*/
	vial_vertices.push_back(glm::vec3(0.25f, 0.f, -0.25f)); //0
	vial_vertices.push_back(glm::vec3(-0.25f, 0.f, -0.25f)); //1
	vial_vertices.push_back(glm::vec3(-0.25f, 0.f, 0.25f)); //2
	vial_vertices.push_back(glm::vec3(0.25f, 0.f, 0.25f)); //3
	vial_vertices.push_back(glm::vec3(0.125f, 0.25f, -0.125f)); //4
	vial_vertices.push_back(glm::vec3(-0.125f, 0.25f, -0.125f)); //5
	vial_vertices.push_back(glm::vec3(-0.125f, 0.25f, 0.125f)); //6
	vial_vertices.push_back(glm::vec3(0.125f, 0.25f, 0.125f)); //7
	engine::ref<engine::vial> vial_shape = engine::vial::create(vial_vertices);
	engine::game_object_properties vial_props;
	vial_props.position = { -18.f, 1.f, 8.f };
	vial_props.meshes = { vial_shape->mesh() };
	vial_props.textures = vial_textures;
	vial_props.type = 0;
	vial_props.mass = 100.f;
	vial_props.bounding_shape = glm::vec3(0.25f,0.25f,0.25f);

	m_vial = engine::game_object::create(vial_props);
	m_vial->set_offset(glm::vec3(0.f,0.25f,0.f));
	m_vial_box.set_box(vial_props.bounding_shape.x * 2.f,
		vial_props.bounding_shape.y,
		vial_props.bounding_shape.z * 2.f,
		vial_props.position - glm::vec3(0.f, m_vial->offset().y, 0.f) );

	// Create a crystal
	std::vector<glm::vec3> crystal_vertices;
	crystal_vertices.push_back(glm::vec3(0.f, 1.f, 0.f)); //0
	crystal_vertices.push_back(glm::vec3(0.5f, 0.5f, 0.f)); //1
	crystal_vertices.push_back(glm::vec3(0.f, 0.5f, -0.5f)); //2
	crystal_vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.f)); //3
	crystal_vertices.push_back(glm::vec3(0.f, 0.5f, 0.5f)); //4
	crystal_vertices.push_back(glm::vec3(0.f, 0.f, 0.f)); //5
	engine::ref<engine::crystal> crystal_shape = engine::crystal::create(crystal_vertices);
	engine::game_object_properties crystal_props;
	crystal_props.position = { 0.f, 1.f, -30.f };
	crystal_props.meshes = { crystal_shape->mesh() };
	crystal_props.textures = electric_powerup_textures;
	crystal_props.type = 0;
	crystal_props.mass = 100.f;
	crystal_props.bounding_shape = glm::vec3(0.5f, 0.5f, 0.5f);
	m_electric_crystal = engine::game_object::create(crystal_props);

	// Create a lamp
	std::vector<glm::vec3> lamp_vertices;
	lamp_vertices.push_back(glm::vec3(1.f, 0.f, 0.f)); //0
	lamp_vertices.push_back(glm::vec3(0.f, 0.f, -1.f)); //1
	lamp_vertices.push_back(glm::vec3(-1.f, 0.f, 0.f)); //2
	lamp_vertices.push_back(glm::vec3(0.f, 0.f, 1.f)); //3
	lamp_vertices.push_back(glm::vec3(0.5f, 1.f, 0.f)); //4
	lamp_vertices.push_back(glm::vec3(0.f, 1.f, -0.5f)); //5
	lamp_vertices.push_back(glm::vec3(-0.5f, 1.f, 0.f)); //6
	lamp_vertices.push_back(glm::vec3(0.f, 1.f, 0.5f)); //7
	lamp_vertices.push_back(glm::vec3(0.5f, 4.f, 0.f)); //8
	lamp_vertices.push_back(glm::vec3(0.f, 4.f, -0.5f)); //9
	lamp_vertices.push_back(glm::vec3(-0.5f, 4.f, 0.f)); //10
	lamp_vertices.push_back(glm::vec3(0.f, 4.f, 0.5f)); //11
	engine::ref<engine::lamp> lamp_shape = engine::lamp::create(lamp_vertices);
	engine::game_object_properties lamp_props;
	lamp_props.position = { -20.f, 0.5f, 8.f };
	lamp_props.meshes = { lamp_shape->mesh() };
	lamp_props.textures = lamp_textures;
	lamp_props.type = 0;
	lamp_props.mass = 100.f;
	lamp_props.bounding_shape = glm::vec3(0.25f, 0.25f, 0.25f);

	m_lamp = engine::game_object::create(lamp_props);

	// Create a vial 2
	std::vector<glm::vec3> vial_vertices2;
	vial_vertices2.push_back(glm::vec3(0.5f, 0.f, 0.f)); //0
	vial_vertices2.push_back(glm::vec3(0.f, 0.f, -0.5f)); //1
	vial_vertices2.push_back(glm::vec3(-0.5f, 0.f, 0.f)); //2
	vial_vertices2.push_back(glm::vec3(0.f, 0.f, 0.5f)); //3
	vial_vertices2.push_back(glm::vec3(0.25f, 0.5f, 0.f)); //4
	vial_vertices2.push_back(glm::vec3(0.f, 0.5f, -0.25f)); //5
	vial_vertices2.push_back(glm::vec3(-0.25f, 0.5f, 0.f)); //6
	vial_vertices2.push_back(glm::vec3(0.f, 0.5f, 0.25f)); //7
	vial_shape = engine::vial::create(vial_vertices2);
	vial_props.position = { 0.f, 0.5f, -25.f };
	vial_props.meshes = { vial_shape->mesh() };
	m_vial2 = engine::game_object::create(vial_props);

	// added cork for the finishing touches on the potion
	engine::ref<engine::cuboid> cube_shape = engine::cuboid::create(glm::vec3(0.0625f, 0.0625f, 0.0625f), false);
	engine::game_object_properties cube_props;
	cube_props.position = m_vial->position() + glm::vec3(0.f, 0.0625f, 0.f);
	cube_props.meshes = { cube_shape->mesh() };
	cube_props.textures = cork_textures;
	m_cork = engine::game_object::create(cube_props);

	// added cork for the finishing touches on the potion
	engine::ref<engine::cuboid> cube_shape2 = engine::cuboid::create(glm::vec3(0.12f, 0.12f, 0.12f), false);
	engine::game_object_properties cube_props2;
	cube_props2.position = m_vial2->position() + glm::vec3(0.f, 0.625f, 0.f);
	cube_props2.meshes = { cube_shape2->mesh() };
	cube_props2.textures = cork_textures;
	m_cork2 = engine::game_object::create(cube_props2);

	// create a powerup
	//engine::ref<engine::cuboid> powerup_shape = engine::cuboid::create(glm::vec3(0.25f, 0.25f, 0.25f), false);
	engine::game_object_properties powerup_props;
	powerup_props.position = glm::vec3(20.f,1.f,8.f);
	powerup_props.meshes = { crystal_shape->mesh() };
	powerup_props.textures = electric_powerup_textures;
	powerup_props.type = 0;
	powerup_props.mass = 100.f;
	powerup_props.bounding_shape = glm::vec3(0.5f, 0.5f, 0.5f);
	m_powerup = engine::game_object::create(powerup_props);

	m_powerup->set_offset(glm::vec3(0.f, 0.25f, 0.f));
	m_powerup_box.set_box(powerup_props.bounding_shape.x * 2.f,
		powerup_props.bounding_shape.y * 2.f,
		powerup_props.bounding_shape.z * 2.f,
		powerup_props.position - glm::vec3(0.f, m_vial->offset().y, 0.f));

	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_cow);
	m_game_objects.push_back(m_enemy_model1);
	m_game_objects.push_back(m_enemy_model2);
	m_game_objects.push_back(m_ball);
	// enable this to make bullet handle player physics
	m_game_objects.push_back(m_player.object());
	m_game_objects.push_back(m_vial);
	m_game_objects.push_back(m_powerup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);

	m_projectile.initialise(m_ball);

	// A red screen pops up when the player takes damage.
	m_damage_fade = cross_fade::create("assets/textures/Red.bmp", 2.0f, 1.6f, 0.9f);

	// create explosion for weapon.
	m_explosion = electric_explosion::create(glm::vec3(1.f, 0.f, 1.0f), glm::vec3(0.f, 0.f, 1.f), true, 0.25f, 1.0f);

	// Initialise the start 
	m_start_screen = start_screen::create("assets/textures/Start.bmp", 1.6f, 0.9f);

	// activate the start screen at launch
	m_start_screen->activate();
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) 
{
    //m_3d_camera.on_update(time_step);

	if(!m_player.getDead())
	{
		m_player.update_camera(m_3d_camera, time_step);
		m_player.on_update(time_step);
	}

	m_projectile.on_update(time_step);

	m_enemy.on_update(time_step, m_player.object()->position());
	m_enemy2.on_update(time_step, m_player.object()->position());
	m_npc.on_update(time_step, m_player.object()->position(), m_enemy.object()->position());

	m_cow_box.on_update(m_cow->position() - glm::vec3(0.f, m_cow->offset().y, 0.f) *
		m_cow->scale(), m_cow->rotation_amount(), m_cow->rotation_axis());

	m_enemy_model_box1.on_update(m_enemy_model1->position() - glm::vec3(0.f, m_enemy_model1->offset().y, 0.f) *
		m_enemy_model1->scale(), m_enemy_model1->rotation_amount(), m_enemy_model1->rotation_axis());

	m_enemy_model_box2.on_update(m_enemy_model2->position() - glm::vec3(0.f, m_enemy_model2->offset().y, 0.f) *
		m_enemy_model2->scale(), m_enemy_model2->rotation_amount(), m_enemy_model2->rotation_axis());

	if(m_powerup->is_active())
	{
		m_powerup_box.on_update(m_powerup->position() - glm::vec3(0.f, m_powerup->offset().y + 0.25f, 0.f),
			m_powerup->rotation_amount(), m_powerup->rotation_axis());
	}

	if (m_vial->is_active())
	{
		m_cork->set_position(m_vial->position() + glm::vec3(0.f, 0.0625f, 0.f));
		m_vial_box.on_update(m_vial->position() - glm::vec3(0.f, m_vial->offset().y, 0.f),
			m_vial->rotation_amount(), m_vial->rotation_axis());
	}

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	// This code sets the spotlight to be in the same position as the player to simulate the player holding a flashlight.
	m_spotLight.Position = glm::vec3(m_player.object()->position().x, m_player.object()->position().y + 1.f, m_player.object()->position().z);
	m_spotLight.Direction = m_player.object()->forward() + glm::vec3(0.f,-0.3f,0.f);

	m_explosion->on_update(time_step);

	m_damage_fade->on_update(time_step);

	m_audio_manager->update_with_camera(m_3d_camera);

	// powerup for health
	if (m_vial->is_colliding())
	{
		if (m_vial->collision_objects().back() == m_player.object())
		{
			m_vial->set_active(false);

			// replenish health by 50, unless replenishing 50 would go over 100 health
			if ((m_player.getHealth() + 50) > 100)
			{
				m_player.setHealth(100);
				m_player.setArmour(50);
			}
			else
			{
				m_player.setHealth(50 + m_player.getHealth());
				m_player.setArmour(50);
			}

			m_audio_manager->play("vial");

			// if collision with player occurs erase the vial from the simulation
			// looked here for guidance on looping through vectors:
			// https://thispointer.com/removing-all-occurences-of-an-element-from-vector-in-on-complexity/ 
			auto it = m_game_objects.begin();
			while (it != m_game_objects.end())
			{
				if (*it == m_vial) {
					it = m_game_objects.erase(it);
				}
				else
					it++;
			}
			m_vial->set_collision_state(false);
		}
	}

	// collision when the projectile hits the enemy.
	if (m_ball->is_colliding())
	{
		// if the projectile collides with the first enemy
		if (m_ball->collision_objects().at(0) == m_enemy_model1)
		{
			m_projectile.set_destroyed(true);
			m_projectile.object()->set_position(m_projectile.object()->position() + glm::vec3(0.f, -5.0f, 0.f));

			// if powerup is active, create an electric explosion at the location.
			if (m_projectile.get_blue())
			{
				m_explosion->activate(0.25f, m_enemy_model1->position());
			}

			// if powerup is active do 2 damage, otherwise do 1
			if (!m_projectile.get_blue())
				m_enemy.set_health(m_enemy.get_health() -1);
			else if (m_projectile.get_blue())
				m_enemy.set_health(m_enemy.get_health() -2);

			// if enemy health reaches 0, the enemy dies and respawns.
			if (m_enemy.get_health() <= 0)
			{
				m_audio_manager->play("enemy_dead");
				m_player.setScore(50);
				int rand1 = rand() % 50 - 25;
				int rand2 = rand() % 35 - 10;
				m_enemy_model1->set_position(glm::vec3(rand1, 1.f, rand2));

				// if the player has less than 200 score, set the health to 2
				if (m_player.getScore() < 200)
					m_enemy.set_health(2);
				// else set the player health and speed based on the player score
				else
				{
					float max_speed = m_enemy.get_speed() + 0.5f;
					m_enemy.set_speed(max_speed);
					int max_health = m_player.getScore() / 100;
					m_enemy.set_health(max_health);
				}
			}
			else
				m_audio_manager->play("enemy_hit1");
		}

		// collision with second enemy
		else if (m_ball->collision_objects().at(0) == m_enemy_model2)
		{
			m_projectile.set_destroyed(true);
			m_projectile.object()->set_position(m_projectile.object()->position() + glm::vec3(0.f, -5.0f, 0.f));

			// if powerup is active, create an electric explosion at the location.
			if (m_projectile.get_blue())
			{
				m_explosion->activate(0.25f, m_enemy_model2->position());
			}

			// if powerup is active do 2 damage, otherwise do 1
			if (!m_projectile.get_blue())
				m_enemy2.set_health(m_enemy2.get_health() -1);
			else if (m_projectile.get_blue())
				m_enemy2.set_health(m_enemy2.get_health() -2);

			// if the enemy dies, play death sound and respawn the enemy in a random location
			if (m_enemy2.get_health() <= 0)
			{
				m_audio_manager->play("enemy_dead");
				m_player.setScore(50);
				int rand1 = rand() % 50 - 25;
				int rand2 = rand() % 35 - 10;
				m_enemy_model2->set_position(glm::vec3(rand1, 1.f, rand2));

				// if the player has less than 200 score, set the health to 2
				if(m_player.getScore() < 200)
					m_enemy2.set_health(2);
				// else set the player health and speed based on the player score
				else
				{
					float max_speed = m_enemy2.get_speed() + 0.5f;
					m_enemy2.set_speed(max_speed);
					int max_health = m_player.getScore() / 100;
					m_enemy2.set_health(max_health);
				}
			}
			else
				m_audio_manager->play("enemy_hit2");
		}
	}

	// collision when the player touches the powerup
	if (m_powerup->is_colliding())
	{
		if (m_powerup->collision_objects().back() == m_player.object())
		{
			m_powerup->set_active(false);

			if (!m_projectile.get_blue())
			{
				m_projectile.set_timer(20.f);
				m_projectile.set_blue(true);
			}

			// if collision with player occurs erase the vial from the simulation
			// looked here for guidance on looping through vectors:
			// https://thispointer.com/removing-all-occurences-of-an-element-from-vector-in-on-complexity/ 
			auto it = m_game_objects.begin();
			while (it != m_game_objects.end())
			{
				if (*it == m_powerup) {
					it = m_game_objects.erase(it);
				}
				else
					it++;
			}
			m_powerup->set_collision_state(false);
		}
	}

	// enemy collision with the player
	if (m_enemy.object()->is_colliding())
	{
		if (m_enemy.object()->collision_objects().back() == m_player.object())
		{
			if (!m_player.getDamaged())
			{
				if(m_player.getHealth() > 0)
				{
					m_audio_manager->play("enemy_att1");
				}
				int damage = 40;
				if (m_player.getArmour() > 0)
				{
					if (m_player.getArmour() - (damage / 2) < 0)
					{
						int health_damage = damage - m_player.getArmour();
						int armour_damage = damage - health_damage;
						m_player.setHealth(m_player.getHealth() - (health_damage));
						m_player.setArmour(-armour_damage);
					}
					else
					{
						m_player.setHealth(m_player.getHealth() - (damage / 2));
						m_player.setArmour(-(damage / 2));
					}

				}
				else if (m_player.getArmour() <= 0)
				{
					m_player.setHealth(m_player.getHealth() - damage);
					if (m_player.getHealth() <= 0)
					{
						m_audio_manager->play("player_death");
						m_player.setDead(true);
					}
				}
				m_damage_fade->activate();
				m_player.setDamaged(true);
				m_player.set_timer(5.f);
			}
		}
	}

	// second enemy collides with the enemy.
	if (m_enemy2.object()->is_colliding())
	{
		if (m_enemy2.object()->collision_objects().back() == m_player.object())
		{
			if (!m_player.getDamaged())
			{
				if (m_player.getHealth() > 0)
				{
					m_audio_manager->play("enemy_att2");
				}
				int damage = 40;
				if (m_player.getArmour() > 0)
				{
					if (m_player.getArmour() - (damage / 2) < 0)
					{
						int health_damage = damage - m_player.getArmour();
						int armour_damage = damage - health_damage;
						m_player.setHealth(m_player.getHealth() - (health_damage));
						m_player.setArmour(-armour_damage);
					}
					else
					{
						m_player.setHealth(m_player.getHealth() - (damage / 2));
						m_player.setArmour(-(damage / 2));
					}

				}
				else if (m_player.getArmour() <= 0)
				{
					m_player.setHealth(m_player.getHealth() - damage);
					if (m_player.getHealth() <= 0)
					{
						m_audio_manager->play("player_death");
						m_player.setDead(true);
					}
				}
				m_damage_fade->activate();
				m_player.setDamaged(true);
				m_player.set_timer(5.f);
			}
		}

	}

	check_bounce();
} 

void example_layer::on_render() 
{
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f}); 
    engine::render_command::clear();

    //const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
    //engine::renderer::begin_scene(m_3d_camera, textured_shader);

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(textured_lighting_shader, m_terrain);

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
	m_pointLight1.submit(textured_lighting_shader, 0);
	m_pointLight2.submit(textured_lighting_shader, 1);

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(textured_lighting_shader, 0);

	m_player.getBox().on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
	m_cow_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
	m_enemy_model_box1.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
	m_enemy_model_box2.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

	engine::renderer::submit(textured_lighting_shader, m_tetrahedron);

	// rendered the vials and the cork for the vial.

	if(m_powerup->is_active())
	{
		engine::renderer::submit(textured_lighting_shader, m_powerup);
		m_powerup_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
	}

	if (m_vial->is_active())
	{
		engine::renderer::submit(textured_lighting_shader, m_vial);
		engine::renderer::submit(textured_lighting_shader, m_cork);
		m_vial_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
	}

	engine::renderer::submit(textured_lighting_shader, m_vial2);

	engine::renderer::submit(textured_lighting_shader, m_cork2);

	engine::renderer::submit(textured_lighting_shader, m_electric_crystal);

	glm::mat4 lamp_transform(1.0f);
	lamp_transform = glm::translate(lamp_transform, m_lamp->position());
	lamp_transform = glm::rotate(lamp_transform, glm::pi<float>() / 4, glm::vec3(0.f, 1.f,0.f));
	lamp_transform = glm::scale(lamp_transform, m_lamp->scale() * 0.5f);
	engine::renderer::submit(textured_lighting_shader, lamp_transform, m_lamp);

	lamp_transform = glm::translate(lamp_transform, glm::vec3(60.f, 0.f, 60.f));
	engine::renderer::submit(textured_lighting_shader, lamp_transform, m_lamp);
	
	glm::mat4 cow_transform(1.0f);
	cow_transform = glm::translate(cow_transform, m_cow->position() - m_cow->offset() * m_cow->scale());
	// places the cow in the tetrahedron
	//cow_transform = glm::translate(cow_transform, glm::vec3(0.f, 0.5f, -20.f));
	cow_transform = glm::rotate(cow_transform, m_cow->rotation_amount(), m_cow->rotation_axis());
	cow_transform = glm::scale(cow_transform, m_cow->scale());
	engine::renderer::submit(textured_lighting_shader, cow_transform, m_cow);

	engine::renderer::submit(textured_lighting_shader, m_enemy_model1);

	engine::renderer::submit(textured_lighting_shader, m_enemy_model2);

	// rendering of the barrel into the world.
	glm::mat4 barrel_transform(1.0f);
	barrel_transform = glm::translate(barrel_transform, m_barrel->position());
	engine::renderer::submit(textured_lighting_shader, barrel_transform, m_barrel);
	
	barrel_transform = glm::translate(barrel_transform, glm::vec3(0.f, 0.f, 0.6f));
	barrel_transform = glm::scale(barrel_transform, m_barrel->scale() * 10.f);
	engine::renderer::submit(textured_lighting_shader, barrel_transform, m_barrel);

	barrel_transform = glm::translate(barrel_transform, glm::vec3(0.4f, 0.2f, -0.1f));
	barrel_transform = glm::rotate(barrel_transform, glm::pi<float>() / 2, glm::vec3(m_barrel->position().x, 0.f, 0.f));
	barrel_transform = glm::scale(barrel_transform, m_barrel->scale() * 7.f);
	engine::renderer::submit(textured_lighting_shader, barrel_transform, m_barrel);

    engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, material_shader);

	if (m_projectile.get_blue())
		m_material_blue->submit(material_shader);
	else
		m_material_orange->submit(material_shader);

	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
	m_pointLight1.submit(material_shader, 0);
	m_pointLight2.submit(material_shader, 1);

	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(material_shader, 0);

	// testing positioning of lights code.
	//std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);
	//m_lightsource_material->submit(material_shader);
	//engine::renderer::submit(material_shader, m_ball2->meshes().at(0), glm::translate(glm::mat4(1.f), m_pointLight1.Position));
	//engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_pointLight2.Position));
	//engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_spotLight.Position));
	//std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

	//engine::renderer::submit(material_shader, m_ball);

	m_projectile.on_render(material_shader);

	m_explosion->on_render(material_shader);

	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_3d_camera, material_shader);

	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);
	m_lightsource_material->submit(material_shader);
	engine::renderer::submit(material_shader, m_ball2->meshes().at(0), glm::translate(glm::mat4(1.f), m_pointLight1.Position));
	engine::renderer::submit(material_shader, m_ball2->meshes().at(0), glm::translate(glm::mat4(1.f), m_pointLight2.Position));
	//engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_spotLight.Position));
	std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", true);

	engine::renderer::end_scene();


	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
	m_pointLight1.submit(animated_mesh_shader, 0);
	m_pointLight2.submit(animated_mesh_shader, 1);

	// making my flashlight render the 
	//std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	//m_spotLight.submit(animated_mesh_shader, 0);

	glm::mat4 aniTransform = glm::mat4(1.0f);

	engine::renderer::submit(animated_mesh_shader, m_player.object());


	engine::renderer::end_scene();

	// Render the hud. Renders the health, armour, score and position of the player (for testing purposes).
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	std::string health_text = "Health: " + std::to_string(m_player.getHealth());;
	std::string armour_text = "Armour: " + std::to_string(m_player.getArmour());;
	std::string score_text = "Score: " + std::to_string(m_player.getScore());;
	std::string electric_timer = "Electric powerup time left: " + std::to_string(m_projectile.get_timer()) + "s";
	std::string positionTest = "x: " + std::to_string(m_player.object()->position().x) + ", y: " + std::to_string(m_player.object()->position().y) + ", z: " + std::to_string(m_player.object()->position().z);
	m_text_manager->render_text(text_shader, health_text, 10.f, 40.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, armour_text, 10.f, 10.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, score_text, (float)engine::application::window().width()/2 -50.f,
		(float)engine::application::window().height() - 25.f , 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, positionTest, (float)engine::application::window().width() / 2 - 50.f, 10.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));

	if (m_projectile.get_blue())
	{
		m_text_manager->render_text(text_shader, electric_timer, (float)engine::application::window().width() / 2 - 175.f,
			(float)engine::application::window().height() - 75.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}

	if (m_player.getDead())
	{
		m_text_manager->render_text(text_shader, "Dead", (float)engine::application::window().width() / 2,
			(float)engine::application::window().height() / 2, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
	}

	// Render start screen onto the screen
	engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
	m_start_screen->on_render(textured_lighting_shader);

	// Render damage feedback onto the screen
	m_damage_fade->on_render(textured_lighting_shader);

	engine::renderer::end_scene();
} 

void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe();
        }
		// if the player presses 'X' the start screen deactivates
		if(e.key_code() == engine::key_codes::KEY_X)
		{
			m_start_screen->deactivate();
		}
		if (e.key_code() == engine::key_codes::KEY_9)
		{
			// testing HUD by setting the player variables.
			m_player.setHealth(m_player.getHealth() + 20);
			m_player.setArmour(20);
			m_player.setScore(20);
		}
		// testing input for explosion
		if(e.key_code() == engine::key_codes::KEY_MINUS)
		{
			m_explosion->activate(0.5f, m_player.object()->position());
		}
		if (e.key_code() == engine::key_codes::KEY_EQUAL)
		{
			m_damage_fade->activate();
		}
    }
	if (engine::input::mouse_button_pressed(0))
	{
		if (!m_player.getDead())
		{
			if (m_projectile.get_blue() && !m_projectile.get_shot()) {
				m_audio_manager->play("laser");
			}
			m_projectile.shoot(m_player.object()->position(), m_player.object()->forward(), 300.0f);
		}
	}
}

void example_layer::check_bounce()
{
	//if (m_prev_sphere_y_vel < 0.f && m_game_objects.at(1)->velocity().y > 0.f)
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}
