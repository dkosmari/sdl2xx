# SDL2XX - A modern C++ wrapper for SDL2

SDL2XX is a modern C++ wrapper for SDL2. Currently the target is C++23.

It's licensed under the Zlib license (same as SDL2).

> Note: this is still a work in progress.

> Documentation is still missing, but the C++ API maps naturally to the C API.


## Simple Example

```cpp
#include <exception>
#include <iostream>

#include <sdl2xx/sdl.hpp>

using sdl::vec2;
using namespace sdl::literals;

int main(int, char* [])
{
    try {
        sdl::init guard{sdl::init::video};

        sdl::window win{"Simple App",
                        sdl::window::pos::centered,
                        sdl::window::pos::centered,
                        1280, 720,
                        0};

        sdl::renderer rend{win,
                           -1,
                           sdl::renderer::flag::accelerated | sdl::renderer::flag::present_vsync};

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

                    case SDL_QUIT:
                        running = false;
                        break;

                }
            }
        }
    }
    catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return -1;
    }
}
```

See the [examples directory](examples) for more examples of the API usage.


## Key features

- Convenient short namespaces:

  - `sdl` for the top-level namespace.
  - `sdl::img` for `SDL_image`.
  - `sdl::ttf` for `SDL_ttf`.
  - `sdl::mix` for `SDL_mixer`.

- Entities that are opaque pointers are handled through movable RAII classes. For
  instance:
  
  - `SDL_Window*` → `sdl::window`
  - `SDL_Renderer*` → `sdl::renderer`
  - `SDL_Surface*` → `sdl::surface`
  - `SDL_Texture*` → `sdl::texture`
  
  Objects can be in an "invalid" state, where they just store a null pointer. In such a
  state, you can only construct, destroy or check for validity:
  
  ```cpp
  sdl::window win{/* ... */};
  // ...
  if (/* ... */)
      win.destroy(); // if this executes, win is now invalid
  // ...
  
  if (win)
      win.minimize();

  if (win.is_valid())
      win.maximize();
  
  if (win.data())
      use_raw_pointer(win.data());
  ```

- Lifetime guard types:

  - `sdl::init`: calls `SDL_Init()` and `SDL_Quit()`.
  - `sdl::img::init`: calls `IMG_Init()` and `IMG_Quit()`.
  - `sdl::ttf::init`: calls `TTF_Init()` and `TTF_Quit()`.

- Various utility types are extended, with extra methods and operators:

  - `SDL_Point` → `sdl::vec2` (or `sdl::point`)
  - `SDL_FPoint` → `sdl::vec2f` (or `sdl::pointf`)
  - `SDL_Rect` → `sdl::rect`
  - `SDL_FRect` → `sdl::rectf`
  - `SDL_Color` → `sdl::color`

- Additional types:

  - `sdl::degrees`, `sdl::degreesf`: represent angles in degrees.
  - `sdl::radians`, `sdl::radiansf`: represent angles in radians.
  - `sdl::unique_ptr<>`: uses `SDL_malloc()` and `SDL_free()` to manage memory allocations.
  - `sdl::blob`: represents raw byte blobs.

- Literals, found in `sdl::literals`:

  - `_deg` → `sdl::degrees`
  - `_degf` → `sdl::degreesf`
  - `_rad` → `sdl::radians`
  - `_radf` → `sdl::radiansf`
  - `_rgb` → `sdl::color` (with `.a = 0xff`)
  - `_rgba` → `sdl::color`
  
- Functions either throw `sdl::error` or return `std::expected<..., sdl::error>`.
