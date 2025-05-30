#include <exception>
#include <iostream>

#include <sdl2xx/sdl.hpp>

using sdl::vec2;
using namespace sdl::literals;
using std::cout;
using std::endl;


int main(int, char* [])
{
    try {
        sdl::init guard{sdl::init::flag::video};

        sdl::window win{"Simple App",
                        sdl::window::pos_centered,
                        {1280, 720}};

        sdl::renderer rend{win,
                           -1,
                           sdl::renderer::flag::accelerated,
                           sdl::renderer::flag::present_vsync};

        auto box = sdl::rect::from_corners({300, 200},
                                           win.get_size() - vec2{300, 200});

        bool running = true;

        while (running) {

            // Clear window to a navy blue background.
            rend.set_color(0x000080_rgb);
            rend.clear();

            // Draw yellow box.
            rend.set_color(sdl::color::yellow);
            rend.fill_box(box);

            rend.present();

            // Process events.
            while (auto event = sdl::events::poll()) {
                switch (event->type) {

                    case sdl::events::type::e_quit:
                        running = false;
                        break;

                }
            }
        }
    }
    catch (std::exception& e) {
        cout << "Error: " << e.what() << endl;
        return -1;
    }
}
