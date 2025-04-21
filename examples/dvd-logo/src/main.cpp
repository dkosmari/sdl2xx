/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <iostream>
#include <numbers>
#include <random>
#include <stdexcept>
#include <vector>

#include <sdl2xx/sdl.hpp>
#include <sdl2xx/ttf.hpp>


using std::cout;
using std::endl;

using namespace sdl::literals;

using sdl::vec2;
using sdl::vec2f;
using sdl::rectf;


std::mt19937_64 rand_eng;

void
seed_random_engine()
{
    std::random_device dev;
    rand_eng.seed(dev());
}


vec2f
rand_position(const rectf& box)
{
    std::uniform_real_distribution<float> dist_x{box.x, box.x + box.w};
    std::uniform_real_distribution<float> dist_y{box.y, box.y + box.h};
    return {
        dist_x(rand_eng),
        dist_y(rand_eng)
    };
}


vec2f
rand_direction(float scale)
{
    std::uniform_real_distribution<float> dist{0, 2 * std::numbers::pi_v<float>};
    float angle = dist(rand_eng);
    return scale * vec2f{std::sin(angle), std::cos(angle)};
}


sdl::degreesf
rand_degree_away_from(sdl::degreesf old)
{
    std::uniform_real_distribution<float> dist{0.0f, 180.0f};
    return wrap_positive(old + sdl::degreesf{dist(rand_eng) + 90.0f});
}


struct Logo {

    sdl::color color;

    vec2f position; // center
    vec2f velocity;

    rectf box;
    sdl::texture texture;


    void
    set_text(sdl::renderer& ren,
             sdl::ttf::font& font,
             const std::string& text)
    {
        sdl::surface surf = font.render_blended(text, sdl::color::white);
        texture.create(ren, surf);
        box.set_size(vec2f{texture.get_size()});
    }


    void
    set_color(sdl::color c)
    {
        color = c;
        texture.set_color_mod(color);
    }


    void
    set_position(vec2f pos)
    {
        position = pos;
        box.set_center(position);
    }


    vec2f
    get_size()
        const noexcept
    {
        return box.get_size();
    }


    void
    step(float dt)
    {
        set_position(position + dt * velocity);
    }


    void
    draw(sdl::renderer& ren)
    {
        ren.set_color(color);
        ren.draw_box(box);

        ren.copy(texture, {}, box);
    }

};


struct App {

    sdl::init init{sdl::init::video | sdl::init::game_controller};
    sdl::window window{
        "DVD Logo",
        sdl::window::pos::centered, sdl::window::pos::centered,
        1280, 720,
        0
    };
    sdl::renderer renderer{
        window,
        -1,
        sdl::renderer::flag::accelerated | sdl::renderer::flag::present_vsync
    };

    sdl::ttf::init ttf_init;
    sdl::ttf::font font{"assets/LiberationSans-Regular.ttf", 96};

    sdl::color bg_color = sdl::color::black;

    rectf boundary;
    Logo logo;


    bool running = false;


    App()
    {
        using style = sdl::ttf::font::style;
        font.set_style(style::bold | style::italic);

        auto size = window.get_size();
        renderer.set_logical_size(size);

        logo.set_text(renderer, font, "DVD");
        logo.set_color(sdl::color::yellow);
        update_boundary(logo.get_size());

        logo.set_position(rand_position(boundary));
        logo.velocity = rand_direction(50);
    }


    void
    update_boundary(vec2f logo_size)
    {
        boundary.set_min_corner(logo_size / 2);
        auto ren_size = vec2f{renderer.get_logical_size()};
        boundary.set_size(ren_size - logo_size);
    }


    void
    run()
    {
        running = true;
        while (running) {
            animate();
            draw();
            process_events();
        }
    }


    void
    animate()
    {
        logo.step(0.1);

        unsigned bounced = 0;

        auto [min, max] = boundary.get_corners();

        if ((logo.position.x < min.x && logo.velocity.x < 0)
            ||
            (logo.position.x >= max.x && logo.velocity.x > 0)) {
            ++bounced;
            logo.velocity.x = -logo.velocity.x;
        }

        if ((logo.position.y < min.y && logo.velocity.y < 0)
             ||
            (logo.position.y >= max.y && logo.velocity.y > 0)) {
            ++bounced;
            logo.velocity.y = -logo.velocity.y;
        }

        if (bounced) {
            // Change logo's hue.
            auto [h, s, l] = logo.color.to_hsl();
            auto new_h = rand_degree_away_from(h);
            logo.set_color(sdl::color::from_hsl(new_h, s, l));

            // add a random perturbation to the velocity vector
            float speed = length(logo.velocity);
            float mag = speed / 10;
            vec2f delta = rand_position({
                    -mag,
                    -mag,
                    2 * mag,
                    2 * mag
                });
            vec2f new_vel = speed * normalized(logo.velocity + delta);
            logo.velocity = new_vel;

            // TODO: play sound

            // TODO: count bounces and corner bounces

        }

    }


    void
    draw()
    {
        renderer.set_color(bg_color);
        renderer.clear();

#if 0
        // Draw the real boundary
        renderer.set_color(0x00ff00_rgb);
        renderer.draw_box(boundary);
#endif
        logo.draw(renderer);

        renderer.present();
    }


    void
    process_events()
    {
        while (auto e = sdl::event::poll())
            handle(*e);
    }


    void
    handle(const sdl::event::event& e)
    {
        switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;

        }
    }

};


int
main()
{
    try {
        seed_random_engine();

        App app;
        app.run();
    }
    catch (std::exception& e) {
        cout << "Error: " << e.what() << endl;
        return -1;
    }

}
