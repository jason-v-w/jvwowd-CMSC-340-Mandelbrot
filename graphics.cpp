#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sstream>


/* Main method */
int main() {
  using namespace std;

  double aspect_ratio = 16.0/9.0;
  int width = 900;
  int height = width/aspect_ratio;
  int image_width = 1920;
  int image_height = (int)(image_width/aspect_ratio);

  double def_real_min = -2.5;
  double def_real_max = 1;
  double def_imag_max = 1;
  double def_imag_min = -1;

  double real_min = def_real_min;
  double real_max = def_real_max;
  double imag_max = def_imag_max;
  double imag_min = def_imag_min;

  sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot", sf::Style::Close|sf::Style::Titlebar);

  // Generate system call		
  std::ostringstream strs;
	strs.precision(100); // Set too high
  strs << "./generate " 
       << real_min << " " 
       << imag_min << " " 
       << real_max << " " 
       << imag_max << " "
       << image_width << " "
       << image_height;
  std::string str = strs.str();
  std::cout << str << std::endl;
  std::cout.flush();
  system(str.c_str());

  // Create texture containing image
  sf::Texture texture;
  if (!texture.loadFromFile("mandelbrot.jpg")) std::cout << "ERROR";

  // Create sprite to hold image data
  sf::Sprite image;
  image.setTexture(texture);
  double scale = width*1.0/image_width;
  image.setScale(sf::Vector2f(scale, scale));

  // Boolean to monitor when mouse is pressed and moving selector box
  bool mouse_pressed = false;

  int select_x = 0;
  int select_y = 0;
  int select_width = 0;
  int select_height = 0;
  int x1,x2,y1,y2=0;

  sf::RectangleShape selector(sf::Vector2f(select_width, select_height));
  selector.setOutlineThickness(1);
  selector.setOutlineColor(sf::Color(128, 0, 128));
  selector.setFillColor(sf::Color(0, 0, 0, 0));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseButtonPressed) {
	if (event.mouseButton.button == sf::Mouse::Left) {
	  mouse_pressed = true;
	  x1 = event.mouseButton.x;
	  y1 = event.mouseButton.y;
	  selector.setPosition(select_x, select_y);
	} else if (event.mouseButton.button == sf::Mouse::Right) {
		// Reset bounds
	  real_min = def_real_min;
	  real_max = def_real_max;
	  imag_max = def_imag_max;
	  imag_min = def_imag_min;

		// Generate system call		
		std::ostringstream strs;
		strs.precision(100); // Set too high
		strs << "./generate " 
		     << real_min << " " 
		     << imag_min << " " 
		     << real_max << " " 
		     << imag_max << " "
		     << image_width << " "
		     << image_height;
		std::string str = strs.str();
		std::cout << str << std::endl;
		std::cout.flush();
		system(str.c_str());
	  if (!texture.loadFromFile("mandelbrot.jpg")) std::cout << "ERROR";
	  image.setTexture(texture);
	}
      } else if (event.type == sf::Event::MouseButtonReleased) {
	if (event.mouseButton.button == sf::Mouse::Left) {
	  mouse_pressed = false;
	  
	  int select_x2 = select_x + select_width;
	  double temp_real_min = (1.0*select_x/width)*(real_max - real_min)+real_min;
	  double temp_real_max = (1.0*select_x2/width)*(real_max - real_min)+real_min;
	  
	  int select_y2 = select_y + select_height;
	  double temp_imag_min = (-1.0*select_y2/height)*(imag_max - imag_min)+imag_max;
	  double temp_imag_max = (-1.0*select_y/height)*(imag_max - imag_min)+imag_max;

	  real_min = temp_real_min;
	  real_max = temp_real_max;
	  imag_min = temp_imag_min;
	  imag_max = temp_imag_max;

	  int rel_width = select_width;
	  int rel_height = select_height * aspect_ratio;

	  bool use_width = rel_width >= rel_height;

	  if (use_width) {
	    double imag_center = (imag_max+imag_min)/2;
	    double new_height = (real_max - real_min) / aspect_ratio;
	    imag_max = imag_center + new_height/2;
	    imag_min = imag_center - new_height/2;
	    
	    int pix_height = (int)(select_width*1.0/aspect_ratio);
	    int pix_y = (int)(select_y + select_height/2.0 - pix_height/2.0);
	    
	    selector.setSize(sf::Vector2f(select_width, pix_height));
	    selector.setPosition(select_x, pix_y);
	  } else {
	    double real_center = (real_max+real_min)/2;
	    double new_width = aspect_ratio*(imag_max - imag_min);
	    real_max = real_center + new_width/2;
	    real_min = real_center - new_width/2;
	    
	    int pix_width = (int)(select_height*aspect_ratio);
	    int pix_x = (int)(select_x + select_width/2.0 - pix_width/2.0);
	    
	    selector.setSize(sf::Vector2f(pix_width, select_height));
	    selector.setPosition(pix_x, select_y);
	  }
	  
	  window.clear();
	  window.draw(image);
	  window.draw(selector);
	  window.display();

	  //generate system call		
	  std::ostringstream strs;
		strs.precision(100); // Set too high
	  strs << "./generate " 
	       << real_min << " " 
	       << imag_min << " " 
	       << real_max << " " 
	       << imag_max << " "
	       << image_width << " "
	       << image_height;
	  std::string str = strs.str();
	  std::cout << str << std::endl;
	  std::cout.flush();
	  system(str.c_str());
	  
	  // Reload texture
	  if (!texture.loadFromFile("mandelbrot.jpg")) std::cout << "ERROR";
	  image.setTexture(texture);

	  // Reset selector variables
	  select_x = 0;
	  select_y = 0;
	  select_width = 0;
	  select_height = 0;
	  selector.setSize(sf::Vector2f(select_width, select_height));
	  selector.setPosition(select_x, select_y);
	}
      } else if (event.type == sf::Event::MouseMoved) {
	if (mouse_pressed) {
	  x2 = event.mouseMove.x;
	  y2 = event.mouseMove.y;

	  select_x = x1 < x2 ? x1:x2;
	  select_y = y1 < y2 ? y1:y2;
	  select_width = abs(x1-x2);
	  select_height = abs(y1-y2);

	  selector.setSize(sf::Vector2f(select_width, select_height));
	  selector.setPosition(select_x, select_y);
	}
      }
    }

    window.clear();
    window.draw(image);
    window.draw(selector);
    window.display();
  }
  
  return 0;
}
