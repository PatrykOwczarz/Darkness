
#pragma once
#include <engine.h>

class quad;

// code based on the cross fade code in lab 5
class start_screen
{
public:
	start_screen(const std::string& path, float width, float height);
	~start_screen();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	void activate();
	void deactivate();

	static engine::ref<start_screen> create(const std::string& path, float width, float height);

private:
	bool s_active;

	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;
};
