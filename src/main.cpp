#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>






#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <iostream>

using namespace std;

// ----------------
// Regular C++ code
// ----------------

// multiply all entries by 2.0
// input:  std::vector ([...]) (read only)
// output: std::vector ([...]) (new copy)
std::vector<double> modify(std::vector<double>& input)
{
  input[0] = 42;
  return input;
}

class Test
{
public:
  Test(int i)
    : i(i)
  {}

  void p()
  {
    cout << i << endl;
  }

  int i;
};

void test()
{
sf::ContextSettings settings;
    // settings.depthBits = 24;
    // settings.stencilBits = 8;
    // settings.antialiasingLevel = 4;
    // settings.majorVersion = 3;
    // settings.minorVersion = 0;
    
    // create the window
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...
        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0);
        glVertex2f(1, 0);
        glVertex2f(1, 1);
        glEnd();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }
}

// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(luigi,m)
{
  // m.doc() = "pybind11 example plugin";

  m.def("modify", &modify, "Multiply all entries of a list by 2.0");
  m.def("test", &test, "tst");

  py::class_<Test>(m, "Test")
    .def_readwrite("i", &Test::i)
    .def(py::init<int>())
    .def("p", &Test::p);
}
