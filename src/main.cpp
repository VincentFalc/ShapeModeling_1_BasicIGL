#include <iostream>
#include <igl/readOFF.h>
#include <igl/writeOFF.h>
#include <igl/viewer/Viewer.h>

#include "gui_utils.h"

/*** insert any libigl headers here ***/
#include <igl/facet_components.h>
#include <igl/is_edge_manifold.h>
#include <igl/is_vertex_manifold.h>
#include <igl/vertex_triangle_adjacency.h>
#include <igl/boundary_loop.h>
#include <igl/loop.h>
#include <igl/edge_topology.h>
#include <igl/barycenter.h>
#include <igl/triangle_triangle_adjacency.h>
#include <igl/adjacency_list.h>

/* Added inclusion for mesh processing */
#include <igl/jet.h>
#include <assert.h>     /* assert */

using namespace std;
using Viewer = igl::viewer::Viewer;

enum MouseMode { NONE, FACE_SELECT, VERTEX_SELECT, TRANSLATE, ROTATE };
MouseMode mouse_mode = NONE;
bool doit = false;
int down_mouse_x = -1, down_mouse_y = -1;
// Vertex array, #V x3
Eigen::MatrixXd V;
// Face array, #F x3
Eigen::MatrixXi F;
// Vectors of indices for adjacency relations
std::vector<std::vector<int> > VF, VFi, VV;

// Added structure : 
const double pi = std::acos(-1);

// Integer vector of component IDs per face, #F x1
Eigen::VectorXi cid;
// Per-face color array, #F x3
Eigen::MatrixXd colors_per_face;

std::set<int> selected_faces;
int selected_v = -1;
void update_display(igl::viewer::Viewer& viewer);

void printVectorOfVector(std::vector<std::vector<int> > &A){
    std::cout << "Printing ..." << endl;
    for(int i=0; i<A.size(); i++){
        std::cout << i << "\t\t";
        for(int j=0; j<A[i].size(); j++){
            std::cout << A[i][j] << " ";
        }
        std::cout << endl;
    }
}

void printSet(std::set<int> &S){
    std::cout << "Printing ..." << endl;
    for(std::set<int>::iterator iter=S.begin(); iter!=S.end();++iter){
        std::cout<<*iter<<endl;
    }
}

void printVector(std::vector<int> &S){
    std::cout << "Printing ..." << endl;
    for(std::vector<int>::iterator iter=S.begin(); iter!=S.end();++iter){
        std::cout<<*iter<<endl;
    }
}

bool personnalIsEdgeManifold(
  const  Eigen::MatrixXi& F)
{
  // List of edges (i,j,f,c) where edge i<j is associated with corner i of face
  // f
  std::vector<std::vector<int> > TTT;
  for(int f=0;f<F.rows();++f)
    for (int i=0;i<3;++i)
    {
      // v1 v2 f ei
      int v1 = F(f,i);
      int v2 = F(f,(i+1)%3);
      if (v1 > v2) std::swap(v1,v2);
      std::vector<int> r(4);
      r[0] = v1; r[1] = v2;
      r[2] = f;  r[3] = i;
      TTT.push_back(r);
    }
  // Sort lexicographically
  std::sort(TTT.begin(),TTT.end());

  for(int i=2;i<(int)TTT.size();++i)
  {
    // Check any edges occur 3 times
    std::vector<int>& r1 = TTT[i-2];
    std::vector<int>& r2 = TTT[i-1];
    std::vector<int>& r3 = TTT[i];
    if ( (r1[0] == r2[0] && r2[0] == r3[0])
        &&
        (r1[1] == r2[1] && r2[1] == r3[1]) )
    {
        std::cout << "Edge occur 3 times !" << endl;
    }
  }
  return true;
}

bool callback_key_down(Viewer& viewer, unsigned char key, int modifiers) {
if (key == '1')
  {
    //SANITY CHECK : std::cout << "KEY 1 PRESSED" << endl;
    //add your code for computing vertex to face relations here
    igl::vertex_triangle_adjacency(V.rows(), F,VF,VFi);

    //Print in a beautiful way the results
    std::cout << "Vertex ID\t Faces associated" << endl;
    for(int i=0; i<VF.size(); i++){
        std::cout << i << "\t\t";
        for(int j=0; j<VF[i].size(); j++){
            std::cout << VF[i][j] << " ";
        }
        std::cout << endl;
    }
    //store in VF,VFi
  }
  if (key == '2')
  {
    //add your code for computing vertex to vertex relations here
    // SANITY CHECK : std::cout << "KEY 2 PRESSED" << endl;
    igl::adjacency_list(F,VV);

    //Print in a beautiful way the results
    std::cout << "Vertex ID\t Vertices associated" << endl;
    for(int i=0; i<VV.size(); i++){
        std::cout << i << "\t\t";
        for(int j=0; j<VV[i].size(); j++){
            std::cout << VV[i][j] << " ";
        }
        std::cout << endl;
    }
    
    //store in VV
  }

  if (key == '3')
  {
    viewer.data.clear();
    viewer.data.set_mesh(V, F);
    colors_per_face.setZero(F.rows(),3);

    //add your code for computing per-face connected components here
    igl::facet_components(F,cid);
    //store the component labels in cid

    //Print in a beautiful way the results + get the number of connected Components
    int max = cid[0];
    std::cout << "N° of Facet\t Component label" << endl;
    for(int i=0; i<cid.size(); i++){
        std::cout << i << "\t\t" << cid[i] << endl;
        if(max<cid[i]){
            max = cid[i];
        }
    }

    //compute colors for the faces based on components
    const bool normalize = true;
    igl::jet(cid,true,colors_per_face);
    //store the colors in component_colors_per_face

    //Calcul of nb of faces per component
    long long nbOfFacesPerComp[max];
    for(int i =0 ; i<=max ; i++){
        nbOfFacesPerComp[i]=0;
    }
    for(int i =0 ; i<cid.size() ; i++){
        nbOfFacesPerComp[cid[i]]++;
    }

    //Printing total number of components
    std::cout << "Number of connected Components : " << max << endl;

    //Printing nb of faces per components
    std::cout << "N° of Components\t Nb Of faces" << endl;
    for(int i =0 ; i<=max ; i++){
        std::cout << i << "\t\t\t\t" << nbOfFacesPerComp[i] << endl;
    }

    //set the viewer colors
    viewer.data.set_colors(colors_per_face);
  }

  if (key == '4') {

    // Add your code for sqrt(3) subdivision here.
    std::vector<std::vector<int> > A;
    Eigen::MatrixXd Vout=V;
    Eigen::MatrixXi Fout=F;
    Eigen::Array<bool, Eigen::Dynamic, 1> Vide;

    //** CREATION OF THE NEWS VERTICES */
    
    //Global informations
    int initialRowsNbV = V.rows();
    int initialRowsNbF = F.rows();
    
    // CREATION OF NEW VERTICES (BARYCENTERS)
    //Calculate barycenters of all faces
    Eigen::MatrixXd BC;
    igl::barycenter(V,F,BC);

    int createdRowsNbBarycenter = BC.rows();
    
    // CREATION OF NEW POSITIONS OF OLD VERTICES
    //Get the adjacency list of vertices for each vertice (old configuration)
    igl::adjacency_list(F, A, true); //F ? instead of Fout

    int createdRowsNbAdacencyList = A.size();
    
    //Calculate for each old vertex, his new coordinates
    for(int i=0; i<A.size(); i++){ //ROWS ? SURE ?  

        //calculate An
        int n = A[i].size(); //SIZE ? SURE ? 
        double an = (4-2*(std::cos((2*pi)/n)))/9;

        //calculate Sum of coord neighboor (matrix style)
        // = Sum coordinates of adjacent vertices
        Eigen::MatrixXd VTMP(1,3);
        VTMP << 0.0,0.0,0.0; //initialization

        for(int j = 0 ; j< n ; j++){
            VTMP.row(0) = VTMP.row(0) + V.row(A[i][j]);
        }

        //Calculate new coordinate (matrix style) = Final calculus
        VTMP.row(0) = (1-an)*V.row(i) + (an/n)*VTMP.row(0);

        //Replace in the new Vertice list
        Vout.row(i) = VTMP.row(0);

        //Checks
        assert (i<Vout.rows());
    }

    // CONCATENATE RESULT TO HAVE FINAL VERTICES LIST
    int oldSizeV = V.rows();

    //We make some free space in the new vertices matrix
    Vout.conservativeResize(V.rows()+BC.rows(), Eigen::NoChange); //Fout => B

    int createdRowsNbVout = Vout.rows();
    
    //Concat of old vertices and new barycenters vertices
    for(int i=0; i< BC.rows(); i++){
        Vout.row(oldSizeV+i) = BC.row(i) ;

        //Checks
        assert (oldSizeV+i<Vout.rows());
    }

    //** CREATION OF THE NEWS FACES */
    Eigen::MatrixXi TT;
    Eigen::MatrixXi TTi;
    igl::triangle_triangle_adjacency(F,TT,TTi);
    
    long nbOfNewFaces = 0 ;
    Fout.conservativeResize(3*F.rows(), Eigen::NoChange); // We can that the size of F triple.

    for(int i = 0 ; i<F.rows(); i++){
        for(int j = 0 ; j<3 ; j ++){
            //int currentVertice = F.row(i)[j]
            //int neighboorFaceOfTheCurrentFace = TT.row(i)[j];
            //int barycenterOfTheCurrentFace = BC.row(i);
            //int barycenterOfTheNeighboorFace = BC.row(neighboorFaceOfTheCurrentFace);
            //IndiceOfNewPointOfCurrentFace = oldSizeV+i
            //IndiceOfNewPointOfNeighboorFace = oldSizeV+neighboorFaceOfTheCurrentFace

            //In ordier : current vertex, newVertexOfTheFace, newVertexOfAdjacentFace
            Fout.row(nbOfNewFaces) << F.row(i)[j], oldSizeV+TT.row(i)[j], oldSizeV+i;
            nbOfNewFaces++;
        }
    }

    Fout.conservativeResize(nbOfNewFaces, Eigen::NoChange);
    
    bool isEdgeManifold = igl::is_edge_manifold(Fout);
    bool isVertexManifold = igl::is_vertex_manifold(Fout,Vide);

    std::cout<< "isEdgeManifold :" << isEdgeManifold << endl;
    std::cout<< "isVertexManifold :" << isVertexManifold << endl;
    
    V = Vout; 
    F = Fout;

    update_display(viewer);
    }
    
    return true;
}

std::set<int> get_v_from_faces_idx(const Eigen::MatrixXi& F, std::set<int>& face_idx) {
    std::set<int> v_set;
    for (auto f: face_idx) {
        v_set.insert(F(f,0)); v_set.insert(F(f,1)); v_set.insert(F(f,2));
    }
    return v_set;
}

void extrude(igl::viewer::Viewer& viewer) {
    Eigen::MatrixXd Vout=V;
    Eigen::MatrixXi Fout=F;

    // Get selected faces
    Eigen::MatrixXi sF(selected_faces.size(),3); int idx = 0;
    for (auto it=selected_faces.begin();it!=selected_faces.end();it++){sF.row(idx++) = F.row(*it);}

    //Sanity Check
    //DEBUG // std::cout<< "selected_faces\n";
    //DEBUG // printSet(selected_faces); 
    //DEBUG // std::cout<< "========================\n";

    // Assert selected faces are connected
    Eigen::VectorXi comp; igl::facet_components(sF,comp);
    if (comp.maxCoeff() != 0) { cout << "Error: Not a single connected component, #face_comp =  " << comp << endl; return;}

    // 1) Get the boundary vertices surrounding the selected faces
    std::vector<int> bnd_loop; 
    igl::boundary_loop(sF,bnd_loop);

    //DEBUG // std::cout<< "bnd_loop\n";
    //DEBUG // printVector(bnd_loop); 
    //DEBUG // std::cout<< "========================\n";

    // 2) Duplicate boundary vertices
    Vout.resize(V.rows()+bnd_loop.size(),3);
    for (int i = 0; i < V.rows(); i++) Vout.row(i)=V.row(i); // set vertices as old vertices
    for (int i = 0; i < bnd_loop.size(); i++) {Vout.row(V.rows()+i) = V.row(bnd_loop[i]);} // create new vertices as duplicates of the faces boundary

    // 3) Compute direction T: The average of selected face normals
    Eigen::RowVector3d T; T.setZero(); Eigen::MatrixXd FN;
    igl::per_face_normals(V,F,FN);
    for (auto it=selected_faces.begin();it!=selected_faces.end();it++){ T += FN.row(*it);}
    T.normalize(); T*=0.25*(V.row(bnd_loop[1])-V.row(bnd_loop[0])).norm();

    //DEBUG // std::cout << " Normal vector : " << T << endl;
    //DEBUG // std::cout<< "========================\n";

    // 4) Offset old vertices by T
    std::set<int> inner_v = get_v_from_faces_idx(F, selected_faces);;
    for (auto v: inner_v) {
        Vout.row(v) += T;
    }

    //DEBUG // std::cout<< "inner_v\n";
    //DEBUG // printSet(inner_v);
    //DEBUG // std::cout<< "========================\n";

    // 5) Update Fout 
    Fout.resize(F.rows()+2*bnd_loop.size(),3); // 2 new faces per new edge (= per new vertex)
    for (int i = 0; i < F.rows(); i++) Fout.row(i)=F.row(i); // set first 'F.rows()' faces as the old faces

    // Add your code for updating Fout here

    // 5.1) Get the set of faces containing the old boundary vertices (hint: call igl::vertex_triangle_adjacency on the old 'F')
    std::vector<std::vector<int>> VF;
    std::vector<std::vector<int>> VI;
    igl::vertex_triangle_adjacency(V.rows(),F,VF,VI);

    //Sanity check
    //DEBUG // printVectorOfVector(VF);
    //DEBUG // std::cout<< "//List of adjacent face per vertice (new)------------\n" << endl;

    // 5.2) Get the "outer" set of faces containing the boundary vertices 
    //      (hint: call std::set_difference to compute the difference between the previously computed set of faces, a
    // and the selected faces)

    //Construction of a set of all faces adjacent to boudaries vertices
    std::set<int> allAdjacentFacesOfBoundaries;

    for(int i=0 ; i<bnd_loop.size(); i++){
        //for each vertice of the boundary
        VF.at(bnd_loop.at(i)); //ListOfAdjacentFaces( indice of a vertice of the boudnari)

        //Ad this list of faces to the global set
        allAdjacentFacesOfBoundaries.insert(VF.at(bnd_loop.at(i)).begin(), VF.at(bnd_loop.at(i)).end());
    }

    //Sanity check
    //DEBUG // std::cout<< "allAdjacentFacesOfBoundaries\n";
    //DEBUG // printSet(allAdjacentFacesOfBoundaries);
    //DEBUG // std::cout<< "========================\n";

    std::set<int> diff;
 
    std::set_difference(allAdjacentFacesOfBoundaries.begin(), allAdjacentFacesOfBoundaries.end(), selected_faces.begin(), selected_faces.end(), 
                        std::inserter(diff, diff.begin()));

    //Sanity check
    //DEBUG // std::cout<< "diff\n";
    //DEBUG // printSet(diff);
    //DEBUG // std::cout<< "========================\n";

    // 5.3) Edit old outer faces indices, replacing the old vertices with the indices of the duplicated boundary vertices
    for(int i=0 ; i < Fout.rows() ; i++){
        //DEBUG // std::cout << F.row(i)[0] << " \t " << F.row(i)[1] << " \t " << F.row(i)[2] << " \t " << endl;

        //For each colums of F
        for(int j = 0 ; j <3 ; j++){
            //IF F.row(i)[0] is a vertex from the boundary ==> We change it.
            std::vector<int>::iterator it;
            it = find (bnd_loop.begin(), bnd_loop.end(), Fout.row(i)[j]);

            
            //Changement : Only if we have found that the current value of Fout (in one column) is in the boundary
            if (it != bnd_loop.end()){
                int posInTheBoundaryList = std::distance(bnd_loop.begin(), it);
                //DEBUG // std::cout << "F.row(i)[" << j << "] =  " << Fout.row(i)[j] << " is part of boundary : " << *it << " on pos " << posInTheBoundaryList << endl;

                std::set<int>::iterator isToChange;
                isToChange = find (diff.begin(), diff.end(), i);
                //and the current Face number is not part of the selected area
                if (isToChange != diff.end()){  
                    //DEBUG // std::cout << "Face N° " << i << " is not part of selected faces : we modify it" << endl;
                    //We change the value, with the new one : 
                    Fout.row(i)[j] = posInTheBoundaryList + V.rows();//where it is in the boundary list + offset due to non-changed initial vertices of V
            
                } else {
                    //DEBUG // std::cout << "Face N° " << i << " is part of selected faces : we Don't modify it" << endl;
                }

              }
        }
    }

    //DEBUG // std::cout << "F (avant modif) " << F << endl;
    //DEBUG // std::cout<< "========================\n";

    //DEBUG // std::cout << "V : " << V << endl;
    //DEBUG // std::cout<< "========================\n";

    //DEBUG // std::cout << "Vout : " << Vout << endl;
    //DEBUG // std::cout<< "========================\n";

    //DEBUG // std::cout << "F (après modif) " << Fout << endl;
    //DEBUG // std::cout<< "========================\n";


    // 5.4) Add new faces, 2 per edge
    int f_idx = F.rows();

    for (int i = 0; i < bnd_loop.size(); i++) {
        int v1 = bnd_loop.at(i);
        int v2 = bnd_loop.at((i+1)%(bnd_loop.size()));
        int v3 = V.rows()+i;
        int v4 = V.rows()+ ((i+1)%(bnd_loop.size()));

        //DEBUG // std::cout << "We bind : " <<  " bnd_loop.at(i) " << v1 << " bnd_loop.at(...); " << v2 << " V.rows()+i; " << v3 << endl;
        //DEBUG // std::cout << "We bind : " <<  " V.rows()+i; " << v3 << " V.rows()+ ((i+1 ... " << v4 << " bnd_loop.at(i) " << v1 << endl;

        // set v1,v2,v3,v4 correctly
        Fout.row(f_idx++) << v3,v2,v1;
        Fout.row(f_idx++) << v2,v3,v4;
    }

    // 6) Check that the new mesh is a manifold (call is_edge_manifold, is_vertex_manifold on Vout,Fout)
    Eigen::Array<bool, Eigen::Dynamic, 1> Vide;

    bool isEdgeManifold = igl::is_edge_manifold(Fout);
    bool isVertexManifold = igl::is_vertex_manifold(Fout,Vide);

    std::cout<< "isEdgeManifold :" << isEdgeManifold << endl;
    std::cout<< "isVertexManifold :" << isVertexManifold << endl;

    // 7) Update V,F
    V = Vout; // uncomment for your code to take effect
    F = Fout; // uncomment for your code to take effect

    // Update gui and move to edit-translate mode
    colors_per_face = Eigen::MatrixXd::Ones(F.rows(),3); // number of faces has changed
    viewer.data.clear();
    viewer.data.set_mesh(V,F);
    for (auto f: selected_faces) {colors_per_face.row(f) << 1,0,0;}
    viewer.data.set_colors(colors_per_face);
    mouse_mode = TRANSLATE;
}

void clear_selection(igl::viewer::Viewer& viewer) {
    selected_faces.clear();
    selected_v = -1;
    colors_per_face = Eigen::MatrixXd::Ones(F.rows(),3);
    viewer.data.clear();
    viewer.data.set_mesh(V,F);
    viewer.data.set_colors(colors_per_face);
}

void export_mesh() {
    std::string f = igl::file_dialog_save();
    igl::writeOFF(f,V,F);
}

bool callback_init(igl::viewer::Viewer& viewer) {
    viewer.ngui->addVariable("Mouse Mode", mouse_mode, true)->setItems({ "None","Faces Select","Vertex Select", "Translate", "Rotate"});
    viewer.ngui->addButton("Extrude",[&](){extrude(viewer);});
    viewer.ngui->addButton("Clear selection",[&](){clear_selection(viewer);});
    viewer.ngui->addButton("Export mesh",[&](){export_mesh();});

    viewer.screen->performLayout();
    return true;
}


bool callback_mouse_down(igl::viewer::Viewer& viewer, int button, int modifier) {
    down_mouse_x = viewer.current_mouse_x;
    down_mouse_y = viewer.current_mouse_y;

    if (mouse_mode == FACE_SELECT) {
        int f = pick_face(viewer, down_mouse_x, down_mouse_y,V,F);
        if (f !=-1)  {
            selected_faces.insert(f);
            selected_v = -1;
            // update face colors
            //colors_per_face.setConstant(colors_per_face.rows(), colors_per_face.cols(), 0);
            colors_per_face.row(f) << 1,0,0;
            viewer.data.set_colors(colors_per_face);
        }
        
    } else if (mouse_mode == VERTEX_SELECT) {
        int v = pick_vertex(viewer, down_mouse_x, down_mouse_y,V,F);
        if (v !=-1) {
            selected_v = v;
            selected_faces.clear(); 
            update_display(viewer);
            viewer.data.set_points(V.row(selected_v),Eigen::RowVector3d(1,0,0));
        }
    } else if ((mouse_mode == TRANSLATE) || (mouse_mode == ROTATE)) {
        if (!selected_faces.empty()) {
            int f = pick_face(viewer, down_mouse_x, down_mouse_y,V,F);
            if (std::find(selected_faces.begin(),selected_faces.end(),f)!= selected_faces.end()) {
                doit = true;
            }
        } else if (selected_v != -1) {
            int v = pick_vertex(viewer, down_mouse_x, down_mouse_y,V,F);
            if (v == selected_v) {
                viewer.data.set_points(V.row(selected_v),Eigen::RowVector3d(1,0,0));
                doit = true;
            }
        }
    }
    return false;
}

Eigen::RowVector3d get_face_avg(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, std::set<int>& selected_faces) {
    
    Eigen::RowVector3d avg; avg << 0,0,0;
    std::set<int> v_set = get_v_from_faces_idx(F, selected_faces);
    for (auto v: v_set) {
        avg += V.row(v);
    }
    avg/= v_set.size();
    
    return avg;
}

bool callback_mouse_move(igl::viewer::Viewer& viewer, int mouse_x, int mouse_y) {
    if (!doit)
        return false;
    if ((mouse_mode == TRANSLATE) || (mouse_mode == ROTATE))  {
        if (!selected_faces.empty()) {
            Eigen::RowVector3d face_avg_pt = get_face_avg(V,F,selected_faces);
            std::set<int> v_idx = get_v_from_faces_idx(F,selected_faces);
            if (mouse_mode == TRANSLATE) {
                Eigen::Vector3f translation = computeTranslation(viewer,
                                             mouse_x,
                                             down_mouse_x,
                                             mouse_y,
                                             down_mouse_y,
                                             face_avg_pt);

                for (auto v_i : v_idx) {V.row(v_i) += translation.cast<double>();}
            } else { // ROTATE
                Eigen::Vector4f rotation = computeRotation(viewer,
                                 mouse_x,
                                 down_mouse_x,
                                 mouse_y,
                                 down_mouse_y,
                                 face_avg_pt);
                for (auto v_i : v_idx) {
                    Eigen::RowVector3f goalPosition = V.row(v_i).cast<float>();
                    goalPosition -= face_avg_pt.cast<float>();
                    igl::rotate_by_quat(goalPosition.data(), rotation.data(), goalPosition.data());
                    goalPosition += face_avg_pt.cast<float>();
                    V.row(v_i) = goalPosition.cast<double>();
                }
            }
            viewer.data.set_mesh(V,F);
            down_mouse_x = mouse_x;
            down_mouse_y = mouse_y;
            return true;    
        } else if ((selected_v!=-1) && (mouse_mode == TRANSLATE)) {
            Eigen::Vector3f translation = computeTranslation(viewer,
                                             mouse_x,
                                             down_mouse_x,
                                             mouse_y,
                                             down_mouse_y,
                                             V.row(selected_v));
            V.row(selected_v) += translation.cast<double>();
            viewer.data.set_mesh(V,F);
            viewer.data.set_points(V.row(selected_v),Eigen::RowVector3d(1,0,0));
            down_mouse_x = mouse_x;
            down_mouse_y = mouse_y;
            return true;
        }
    }
    return false;
}

bool callback_mouse_up(igl::viewer::Viewer& viewer, int button, int modifier) {
    doit = false;
    return false;
}

void update_display(igl::viewer::Viewer& viewer) {
    viewer.data.clear();
    viewer.data.set_mesh(V, F);
    colors_per_face = Eigen::MatrixXd::Ones(F.rows(),3);
    viewer.data.set_colors(colors_per_face);
}

int main(int argc, char *argv[]) {
    // Show the mesh
    Viewer viewer;
    viewer.callback_key_down = callback_key_down;
    
    if (argc == 2)
    {
      // Read mesh
      igl::readOFF(argv[1],V,F);
      
    }
    else
    {
      // Read mesh
      igl::readOFF("../data/cube.off",V,F);
    }

    viewer.data.set_mesh(V,F);
    viewer.data.compute_normals();
    viewer.core.align_camera_center(V,F);
    viewer.callback_init = callback_init;
    viewer.callback_mouse_down = callback_mouse_down;
    viewer.callback_mouse_up = callback_mouse_up;
    viewer.callback_mouse_move = callback_mouse_move;

    //callback_key_down(viewer, ' ', 0);
    update_display(viewer);
    viewer.launch();
}
