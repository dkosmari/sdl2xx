/*
 * SDL2XX - a C++23 wrapper for SDL2.
 *
 * Copyright 2025  Daniel K. O. <dkosmari>
 *
 * SPDX-License-Identifier: Zlib
 */

#include <exception>
#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>
#include <utility>
#include <vector>
#include <type_traits>

#include <sdl2xx/sdl.hpp>
#include <sdl2xx/img.hpp>
#include <sdl2xx/mix.hpp>
#include <sdl2xx/ttf.hpp>


using std::cout;
using std::endl;

using namespace sdl::literals;

using sdl::radiansf;
using sdl::rectf;
using sdl::vec2;
using sdl::vec2f;


const std::filesystem::path assets_path = "assets/";


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


sdl::radiansf
rand_angle(sdl::radiansf mean,
           sdl::radiansf dev)
{
    std::normal_distribution<float> dist{mean.value(), dev.value()};
    return radiansf{dist(rand_eng)};
}


vec2f
rotated(vec2f v,
        radiansf a)
{
    auto [s, c] = sincos(a);
    return {
         c * v.x - s * v.y,
         s * v.x + c * v.y
    };
}


template<typename T,
         typename... Ts>
std::vector<std::remove_cvref_t<T>>
make_vector(T&& first, Ts&&... tail)
{
    std::vector<std::remove_cvref_t<T>> result;
    result.reserve(1 + sizeof...(Ts));
    result.push_back(std::forward<T>(first));
    (result.push_back(std::forward<Ts>(tail)), ...);
    return result;
}


struct Logo {

    sdl::color color;

    vec2f position; // center
    vec2f velocity;

    rectf box;
    sdl::texture* texture = nullptr;


    void
    set_texture(sdl::texture* tex)
    {
        texture = tex;
        if (texture)
            box.set_size(vec2f{texture->get_size()});
        else
            box.clear();
        set_color(color);
    }


    void
    set_color(sdl::color c)
    {
        color = c;
        if (texture)
            texture->set_color_mod(color);
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

#if 0
        ren.draw_box(box);
#endif

        if (texture)
            ren.copy(*texture, {}, box);
        else
            ren.draw_point(position);
    }

};


struct App {

    sdl::init sdl_init{
        sdl::init::flag::video,
        sdl::init::flag::game_controller,
        sdl::init::flag::audio
    };
    sdl::img::init img_init;
    sdl::mix::init mix_init{sdl::mix::init::flag::ogg};
    sdl::ttf::init ttf_init;


    sdl::window window{
        "DVD Logo",
        sdl::window::pos_centered,
        {1280, 720},
        0
    };
    sdl::renderer renderer{
        window,
        -1,
        sdl::renderer::flag::accelerated | sdl::renderer::flag::present_vsync
    };


    sdl::mix::device mix_dev;
    sdl::mix::chunk bounce_sound{assets_path / "wood.ogg"};
    sdl::mix::chunk corner_sound{assets_path / "bell.ogg"};

    sdl::ttf::font font{assets_path / "LiberationSans-Regular.ttf", 24};

    sdl::color bg_color = sdl::color::black;

    rectf boundary;
    Logo logo;

    float logo_speed = 15;

    std::vector<sdl::texture> logo_textures =
        make_vector(sdl::img::load_texture(renderer, assets_path / "dvd-logo.svg"),
                    sdl::img::load_texture(renderer, assets_path / "blu-ray-logo.svg"));
    std::size_t current_texture = 0;

    sdl::texture status_texture;
    unsigned total_bounces = 0;
    unsigned total_corner_bounces = 0;

    bool running = false;


    App()
    {
        auto size = window.get_size();
        renderer.set_logical_size(size);

        set_logo(0);
        logo.set_color(sdl::color::yellow);
        update_boundary(logo.get_size());

        logo.set_position(rand_position(boundary));
        logo.velocity = rand_direction(logo_speed);

        update_status_text();

        sdl::mix::allocate_channels(4);
    }


    ~App()
    {
        sdl::mix::halt();
    }


    void
    update_boundary(vec2f logo_size)
    {
        boundary.set_min_corner(logo_size / 2);
        auto ren_size = vec2f{renderer.get_logical_size()};
        boundary.set_size(ren_size - logo_size);
    }


    void
    set_logo(std::size_t idx)
    {
        if (idx >= logo_textures.size())
            idx = 0;
        current_texture = idx;
        if (!logo_textures.empty()) {
            logo.set_texture(&logo_textures[current_texture]);
            update_boundary(logo.get_size());
        }
    }


    void
    update_status_text()
    {
        std::ostringstream out;
        out << "Bounces: " << total_bounces << "\n"
            << "Corner bounces: " << total_corner_bounces;

        auto status_surf = font.render_blended(out.str(), sdl::color::white, 0);
        status_texture.create(renderer, status_surf);
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
        logo.step(1.0);

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

            // add a random perturbation to the velocity vector after a bounce
            radiansf da = rand_angle(0_radf, 2_degf);
            logo.velocity = with_length(rotated(logo.velocity, da), logo_speed);

            ++total_bounces;
            if (bounced == 2) {
                corner_sound.play();
                ++total_corner_bounces;
            } else {
                bounce_sound.play();
            }

            update_status_text();
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

        if (status_texture) {
            auto tex_size = status_texture.get_size();
            rectf dst = rectf::from_corners(vec2f{0, 0},
                                            vec2f{tex_size});
            renderer.copy(status_texture, {}, dst);
        }

        renderer.present();
    }


    void
    process_events()
    {
        while (auto e = sdl::events::poll())
            handle(*e);
    }


    void
    handle(const sdl::events::event& e)
    {
        using sdl::events::type;

        switch (e.type) {
            case type::e_quit:
                running = false;
                break;

            case type::e_key_down:
                handle_key_down(e.key);
                break;
        }
    }


    void
    handle_key_down(const sdl::events::keyboard& e)
    {
        switch (e.keysym.sym) {

            case SDLK_SPACE:
                set_logo(current_texture + 1);
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
