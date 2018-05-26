#include "gui_utils.h"

int pick_face(igl::viewer::Viewer& viewer, int mouse_x, int mouse_y, const Eigen::MatrixXd& V, const Eigen::MatrixXi& F) {
  // Cast a ray in the view direction starting from the mouse position
  double x = mouse_x;
  double y = viewer.core.viewport(3) - mouse_y;
  
  Eigen::RowVector3d pt;
  
  Eigen::Matrix4f modelview = viewer.core.view * viewer.data.model;
  int vi = -1;
  
  std::vector<igl::Hit> hits;

  igl::unproject_in_mesh(Eigen::Vector2f(x,y), viewer.core.view * viewer.data.model,
      viewer.core.proj, viewer.core.viewport, V, F, pt,hits);

  int fi = -1;
  if (hits.size()> 0) {
    fi = hits[0].id;
  }
  return fi;
}

int pick_vertex(igl::viewer::Viewer& viewer, int mouse_x, int mouse_y, const Eigen::MatrixXd& V, const Eigen::MatrixXi& F) {
  // Cast a ray in the view direction starting from the mouse position
  double x = mouse_x;
  double y = viewer.core.viewport(3) - mouse_y;
  
  Eigen::RowVector3d pt;
  
  Eigen::Matrix4f modelview = viewer.core.view * viewer.data.model;
  int vi = -1;
  
  std::vector<igl::Hit> hits;
  /*
  igl::unproject_in_mesh(Eigen::Vector2f(x,y),
                         modelview,
                         viewer.core.proj,
                         viewer.core.viewport,
                         ei,pt,hits);
  */

  igl::unproject_in_mesh(Eigen::Vector2f(x,y), viewer.core.view * viewer.data.model,
      viewer.core.proj, viewer.core.viewport, V, F, pt,hits);

  if (hits.size()> 0) {
    int fi = hits[0].id;
    Eigen::RowVector3d bc;
    bc << 1.0-hits[0].u-hits[0].v, hits[0].u, hits[0].v;
    bc.maxCoeff(&vi);
    vi = F(fi,vi);
  }
  return vi;
}

//computes translation for the vertices of the moving handle based on the mouse motion
Eigen::Vector3f computeTranslation (igl::viewer::Viewer& viewer,
                                    int mouse_x,
                                    int from_x,
                                    int mouse_y,
                                    int from_y,
                                    Eigen::RowVector3d pt3D) {
    Eigen::Matrix4f modelview = viewer.core.view * viewer.data.model;
    //project the given point (typically the handle centroid) to get a screen space depth
    Eigen::Vector3f proj = igl::project(pt3D.transpose().cast<float>().eval(),
                                                                            modelview,
                                                                            viewer.core.proj,
                                                                            viewer.core.viewport);
    float depth = proj[2];
    
    double x, y;
    Eigen::Vector3f pos1, pos0;
    
    //unproject from- and to- points
    x = mouse_x;
    y = viewer.core.viewport(3) - mouse_y;
    pos1 = igl::unproject(Eigen::Vector3f(x,y,depth),
                                                modelview,
                                                viewer.core.proj,
                                                viewer.core.viewport);
    
    
    x = from_x;
    y = viewer.core.viewport(3) - from_y;
    pos0 = igl::unproject(Eigen::Vector3f(x,y,depth),
                                                modelview,
                                                viewer.core.proj,
                                                viewer.core.viewport);
    
    //translation is the vector connecting the two
    Eigen::Vector3f translation;
    translation = pos1 - pos0;
    
    return translation;
}

Eigen::Vector4f computeRotation(igl::viewer::Viewer& viewer,
                                int mouse_x,
                                int from_x,
                                int mouse_y,
                                int from_y,
                                Eigen::RowVector3d pt3D) {

  Eigen::Vector4f rotation;
  rotation.setZero();
  rotation[3] = 1.;

  Eigen::Matrix4f modelview = viewer.core.view * viewer.data.model;

  //initialize a trackball around the handle that is being rotated
  //the trackball has (approximately) width w and height h
  double w = viewer.core.viewport[2]/8;
  double h = viewer.core.viewport[3]/8;

  //the mouse motion has to be expressed with respect to its center of mass
  //(i.e. it should approximately fall inside the region of the trackball)

  //project the given point on the handle(centroid)
  Eigen::Vector3f proj = igl::project(pt3D.transpose().cast<float>().eval(),
                                      modelview,
                                      viewer.core.proj,
                                      viewer.core.viewport);
  proj[1] = viewer.core.viewport[3] - proj[1];

  //express the mouse points w.r.t the centroid
  from_x -= proj[0]; mouse_x -= proj[0];
  from_y -= proj[1]; mouse_y -= proj[1];

  //shift so that the range is from 0-w and 0-h respectively (similarly to a standard viewport)
  from_x += w/2; mouse_x += w/2;
  from_y += h/2; mouse_y += h/2;

  //get rotation from trackball
  Eigen::Vector4f drot = viewer.core.trackball_angle.coeffs();
  Eigen::Vector4f drot_conj;
  igl::quat_conjugate(drot.data(), drot_conj.data());
  igl::trackball(w, h, float(1.), rotation.data(), from_x, from_y, mouse_x, mouse_y, rotation.data());

  //account for the modelview rotation: prerotate by modelview (place model back to the original
  //unrotated frame), postrotate by inverse modelview
  Eigen::Vector4f out;
  igl::quat_mult(rotation.data(), drot.data(), out.data());
  igl::quat_mult(drot_conj.data(), out.data(), rotation.data());
  return rotation;
}