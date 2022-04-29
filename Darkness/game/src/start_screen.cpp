#include "pch.h"
#include "start_screen.h"
#include "quad.h"

// This class is based on the cross_fade code from lab 5
start_screen::start_screen(const std::string& path, float width, float height)
{
	m_texture = engine::texture_2d::create(path, true);

	m_transparency = 0.0f;

	m_quad = quad::create(glm::vec2(width, height));
	s_active = false;
}

start_screen::~start_screen()
{}

// removed the on_update code from cross_fade in the fx example because I did not need the start screen to change based on time
void start_screen::on_update(const engine::timestep& time_step)
{

}

void start_screen::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.1f));

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}

void start_screen::activate()
{
	s_active = true;
	m_transparency = 1.0f;
}

// added a deactivate function which is going to be called when the player presses the 'X' key
void start_screen::deactivate()
{
	s_active = false;
	m_transparency = 0.f;
}

engine::ref<start_screen> start_screen::create(const std::string& path, float width, float height)
{
	return std::make_shared<start_screen>(path, width, height);
}
