#include "main.hpp"
#include <cstdlib>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gl_interface.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include "stdio.h"
#include <string>

void usage(void) 
{
  std::cerr<< "usage: "<<progname<<" [-h] -i bvhfile"<<std::endl<<std::endl;
  std::cerr<< "\t-h\t\t\tprint usage"<<std::endl;  
  std::cerr<< "\t-i bvhfile\t\tThe BVH filename\n";
  exit(0);
}

//-----------------------------------------------------------------------
void renderGL(bvh::bvh_t* bvh_fig,GLFWwindow* window)
{
  glScalef(0.05,0.05,0.05);
  bvh_fig->render_canonical_pose();
  int frames = (bvh_fig->get_motion())->get_frames();
  while(true)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    bvh_fig->render_frame(count);
    glfwSwapBuffers(window);
    glfwPollEvents();
    if(!pause_motion && count<frames) count++;
  }

}



int main(int argc, char **argv)
{
  progname = argv[0];

  bool bvhf_flag = false;

  char c;
  while ((c = getopt(argc, argv, "h?i:")) != EOF)
    {
      switch (c) 
        {
	case 'h':
        case '?':
          usage();
          break;
	case 'i':
	  bvhfilename =  optarg;   
          bvhf_flag = true;
          break;
	default:
	  usage();
	}
    }

  if (!(bvhf_flag)) usage();

  try 
    { 
      bvh_fig = new bvh::bvh_t(bvhfilename, true); 
      try 
	{ 
	  bvh_fig->print_hierarchy(std::cout);
    bvh_fig->print_motion(std::cout);
	}
      catch (util::common::error *e)
	{ util::common::error::halt_on_error(e); }

      /*!
       * CS 775:
       * CALL THE FUNCTION THAT STARTS YOUR PROGRAM HERE
       */
       //! The pointer to the GLFW window
      
      GLFWwindow* window;
      //! Setting up the GLFW Error callback
      glfwSetErrorCallback(error_callback);
      //! Initialize GLFW
      if (!glfwInit())
        return -1;
      
      int win_width=640;
      int win_height=480;
      
      //! Create a windowed mode window and its OpenGL context
      window = glfwCreateWindow(win_width, win_height, "Transformers", NULL, NULL);
      if (!window)
      {
        glfwTerminate();
        return -1;
        }
      
      //! Make the window's context current 
      glfwMakeContextCurrent(window);
      
      //Keyboard Callback
      glfwSetKeyCallback(window, key_callback);
      //Framebuffer resize callback
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      // Ensure we can capture the escape key being pressed below
      glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
      
      glfwGetFramebufferSize(window, &win_width, &win_height);
      framebuffer_size_callback(window, win_width, win_height);
      //Initialize GL state
      initGL();

      glfwSetTime(0);
      bool check = true;
      while (glfwWindowShouldClose(window) == 0)
      {
        if(check){
        renderGL(bvh_fig,window);
        check = false;
      }
        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
        //glfwSetWindowShouldClose(window, GL_TRUE);
      }
    }
  catch (util::common::error *e)
    { util::common::error::halt_on_error(e); }

  return 0;
}

//-----------------------------------------------------------------------
  
