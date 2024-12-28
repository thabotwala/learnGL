#ifndef CAMERA_H
#define CAMERA_H
enum CAMERA_AXIS = { x = 0, y = 1, z = 2};
class Camera
{
  public: //comstructors
    Camera(glm::mat4 proj, float fov, float zoom)
  public:
      void lookAt(glm::vec3 target);
      void cameraLockAxis(int axis);
  private:
      glm::vec3 viewportCenter;
      glm::vec3 cameraXAxis; // camera right axis
      glm::vec3 cameraYAxis; // camera up axis
      glm::vec3 cameraZAxis; // camera forward/front axis
      glm::vec3 cameraPos;
      glm::vec3 cameraTarget;
      glm::cameraDirection;
      glm::vec3 worldUpAxis;
  private:
      glm::mat4 projection;
      glm::mat4 view;
      glm::mat4 model;
};
//<----------------------------- CAMERA_IMPLEMENTAION ---------------------------------------->
Camera::Camera(glm::mat4 proj, float fov, float zoom)
{
  cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraDirection = glm::normalize(cameraTarget - cameraPos);

}
#endif // !CAMERA_H
