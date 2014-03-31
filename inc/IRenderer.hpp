#ifndef _R_IRENDERER_HPP_
#define _R_IRENDERER_HPP_

#include  <string>
#include  <opencv2/opencv.hpp>


class   IRenderer
{
public:
  virtual ~IRenderer() {}

  // General scene functions
  virtual void  createEntity(std::string _name, std::string _mesh) = 0;
  virtual void  createLight(std::string _name) = 0;
  virtual void  moveEntity(std::string _name, float x, float y, float z) = 0;
  virtual void  newPosEntity(std::string _name, float x, float y, float z)= 0;
  virtual void  rotateEntity(std::string _name, float x, float y, float z) = 0;
  virtual void  moveLight(std::string _name, float x, float y, float z) = 0;

  // Functions related to assets loading
  virtual void  loadMesh(std::string _name, std::string _file) = 0;
  virtual void  loadTexture(std::string _name, std::string _file) = 0;

  // Other functions
  virtual void  loadCam(cv::Mat left, cv::Mat right) = 0;
  virtual void  render() = 0;
  virtual bool  isAlive() = 0;
  virtual bool  getShutDown() = 0;

protected:
  bool      alive; // Indicate status
  bool      ShutDown;
};

#endif
