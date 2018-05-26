
##### TL;DR
This project was linked to a lecture about Shape Modeling. This project presents primitives to manipulate data with the IGL and Eigen libraries.
These librairies are not well documented and therefore understand the implementation of some trivial operations might be time-consuming. This project try to show some basics with the library. It leads to a very basic blender-like real-time mesh editor.
<br/>

##### Table of Contents  
[1. Neighboorhood computations](#Neighboorhood)  <br/>
[1.1. Vertex To Face relations](#Vertex2Face)  <br/>
[1.2. Vertex To Vertex relations](#Vertex2Vertex)  <br/>
[2. Connected components](#Connected)  <br/>
[3. A simple subdivision scheme](#subdivision)  <br/>
[4. Mesh extrude](#extrusion)  <br/>
[5. My wonderful design](#designMaster)  <br/>

<br/>

<a name="Neighboorhood"/>

#### 1. Neighboorhood computations

<a name="Vertex2Face"/>

#### 1.1 Vertex To Face relations

V is the matrix representing the vertices of the mesh
F is the matrix representing the triangular faces of the mesh, as references to V.

As a first step, we can represent the relation between vertices and faces as an adjacency list.
By pressing '1' we get a text output of the vertex-to-face relation

###### Documentation informations

Information about function of IGL Library can be found directly in the header files of the git repository.

We can use the following function : 
* __igl::vertex_triangle_adjacency()__
This function construct the vertx-face topology of a mesh defined by (V,F) (described below). <br/>
Informations about the function : [-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/vertex_triangle_adjacency.h)

An over-view on how to use this function is following.


**Inputs** :
* (Implicit :  V = a 3-list that represent vertex coordinates)
* n = number of vertices of V (here, `V.rows()`)
* F = a 3-list that represent mesh faces (has to be triangles)

**Outputs** :
* VF = An adjacency list of incident faces
* VI = An adjacency list of index of incidence (related by index to VF adajcency list)


###### Usage
We want, for each vertex, the adjacent faces. We call the function that build the adjacent list and print the VF list.

```C++
igl::vertex_triangle_adjacency(V.rows(), F,VF,VFi);
```

###### Result
<div style="text-align:center">
<p align="center">
  <img style="height : 600px;" src="/results/capture1_1.png">
</p>
</div>

```
cube.off
Vertex ID	 Faces associated
0		0 1 4 5 10 
1		0 5 6 7 
2		0 1 7 8 9 
3		1 9 10 11 
4		2 3 4 10 11 
5		3 4 5 6 
6		2 3 6 7 8 
7		2 8 9 11 
```

__________________________________

<a name="Vertex2Vertex"/>

#### 1.2. Vertex To Vertex relations

As a second step, we can represent the relation between vertices as an adjacency list, as we did between vertices and faces.
By pressing '2' we get a text output of the vertex-to-vertex relation

###### Documentation informations

We can use the following function : 
* __igl::adjacency list__
    
This function construct the graph adjacency list of a mesh defined by (V,F). <br/> Informations about the function : 
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/adjacency_list.h)

**Inputs** :
* F = a 3-list that represent mesh faces (has to be triangles)

**Outputs** :
* A = An adjacency list of incident vertices

###### Usage
We want, for each vertex, the adjacent vertices. We call the function that build the adjacent list and print the VV list.
```C++
igl::adjacency_list(F,VV);
```

###### Result
<div style="text-align:center">
<p align="center">
  <img style="height : 600px;" src="/results/capture2_1.png">
</p>
</div>

```
cube.off
Vertex ID	 Vertices associated
0		1 2 3 4 5 
1		0 2 5 6 
2		0 1 3 6 7 
3		0 2 4 7 
4		0 3 5 6 7 
5		0 1 4 6 
6		1 2 4 5 7 
7		2 3 4 6 
```

__________________________________

<a name="Connected"/>

### 2.1 Connected components

A mesh can be divided in independants smaller meshes, not connected, that we call separated connected components. Each mesh face only belongs to a single component. We can color differently each component.

By pressing '3' we get a text output of the connected components list.

###### Documentation informations

We can use the following function : 
* __igl::facet components()__
This function compute a list of the connected components, based on adjacency of edges.<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/facet_components.h)
* __igl::jet()__
This function compute a list colors [r,g,b] from a list of factor [0-1]. <br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/jet.h)

For __igl::facet components()__ we have : 


**Inputs** :
* F = a 3-list that represent mesh faces (has to be triangles)

**Outputs** :
* C = A list of connected component ids
* (overload : Cnb = A list of number of facets in each components )

For __igl::jet()__ we have : 


**Inputs** :
* Z  = list of factors 
* const bool normalize = jet normalize Z to fit into [0-1]

**Outputs** :
* C = A 3-list of colors as [r,g,b]

###### Usage
We compute each component by a call to IGL library. Then we compute a color for each component. Then we extract the number of component. Then we calcul the number of faces for each component.
```C++
igl::facet_components(F,cid);
igl::jet(cid,true,colors_per_face);

//Calcul of nb of faces per component
long long nbOfFacesPerComp[max];
for(int i =0 ; i<=max ; i++){
  nbOfFacesPerComp[i]=0;
}
for(int i =0 ; i<cid.size() ; i++){
  nbOfFacesPerComp[cid[i]]++;
}

(...)
```

###### Result
<div style="text-align:center">
<p align="center">
  <img style="height : 300px;" src="/results/cube_1.png">
  <img style="height : 300px;" src="/results/coffe_1.png">
  <img style="height : 300px;" src="/results/honda_1.png">
</p>
</div>


<div style="text-align:center">
<p align="center">
  <img style="width : 300px;" src="/results/cube_2.png">
  <img style="width : 300px;" src="/results/coffe_2.png">
  <img style="width : 300px;" src="/results/honda_2.png"></p>
</div>


__________________________________

<a name="subdivision"/>

### 3. A simple subdivision scheme

We can implement a subdivision scheme to iteratively create a denser grid of triangle representing the same mesh. The subdivision scheme used is described there : 
[-> Description](https://www.graphics.rwth-aachen.de/media/papers/sqrt31.pdf)

<p align="center">
<img style="height : 400px;" src="/results/subdivision.png"><br/>
Extracted from the cited paper - RWTH Aachen
</p>

This operation shrink and smooth the mesh during the process.

###### Documentation informations

We can use the following function : 
* __igl::adjacency list__
Give adjacent vertices.<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/adjacency_list.h)
* __igl::triangle triangle adjacency__
Give a list for topological adjacency of triangles.<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/triangle_triangle_adjacency.h)
* __igl::edge topology__
Give topological relations of edges .<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/edge_topology.h)
* __igl::barycenter__
Computes the barycenter of every simplex.<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/barycenter.h)
* __igl::viewer.data.clear()__
Clean the data displayed on the screen<br/>

###### Usage
We create new vertices, we create new faces, we put these new data in the final V,F.
(Only essentials parts are displayed, such code doesn't compile)

```C++
    // CREATION OF NEW VERTICES (BARYCENTERS)
    igl::barycenter(V,F,BC);

    // CREATION OF NEW POSITIONS OF OLD VERTICES
    igl::adjacency_list(Fout, A, true);

    //Calculate for each old vertex, his new coordinates
    for(int i=0; i<A.size(); i++){

        //calculate n & An
        int n = A[i].size();
        double an = (4-2*(std::cos((2*pi)/n)))/9;
       
        //calculate Sum of coord neighboor (matrix style)
        for(int j = 0 ; j< n ; j++){
          VTMP.row(0) = VTMP.row(0) + Vout.row(A[i][j]); //sum
        }

        //Calculate new coordinate (matrix style) = Final calculus
        VTMP.row(0) = (1-an)*Vout.row(i) + (an/n)*VTMP.row(0);

        //Replace in the new Vertice list
        Vout.row(i) = VTMP.row(0);
    }

    // CONCATENATE RESULT TO HAVE FINAL VERTICES LIST
  ...

    //** CREATION OF THE NEWS FACES */
    igl::triangle_triangle_adjacency(F,TT,TTi);
    
    long nbOfNewFaces = 0 ;
    Fout.conservativeResize(3*F.rows(), Eigen::NoChange);

    for(int i = 0 ; i<F.rows(); i++){
        for(int j = 0 ; j<3 ; j ++){
            //In ordier : current vertex, newVertexOfTheFace, newVertexOfAdjacentFace
            Fout.row(nbOfNewFaces) << F.row(i)[j], oldSizeV+TT.row(i)[j], oldSizeV+i;
            nbOfNewFaces++;
        }
    }

    Fout.conservativeResize(nbOfNewFaces, Eigen::NoChange);
    
    V = Vout; 
    F = Fout;

    update_display(viewer);
    }
    
```

###### Result
<div style="text-align:center">
<p align="center">
  <img style="height : 500px;" src="/results/CubeSpeed.gif">
  <img style="height : 500px;" src="/results/CoffeSpeed.gif">
  <img style="height : 500px;" src="/results/hondaSpeed.gif">
</p>
</div>

__________________________________

<a name="extrusion"/>

### 4. Mesh extrude

A basic mesh editing operation are implemented : selecting faces, translating faces, ... Here is the code concerning the extrusion.

###### Documentation informations

We can use the following function : 
* __igl::vertex triangle adjacency__
Constructs the vertex-face topology of a given mesh<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/vertex_triangle_adjacency.h)
* __igl::is edge manifold__
Return if mesh is edge manifold<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/is_edge_manifold.h)
* __igl::is vertex manifold__
Checks whether the faces incident on each vertex form exactly one connected component.<br/>
[-> Documentation](https://github.com/libigl/libigl/blob/master/include/igl/is_vertex_manifold.h)

std::set, std::set difference are part of the STL lib.<br/>

###### Usage

We select a set of faces, and we want to extrude the mesh from these faces.

We get a list of the faces having at least on vertice of the set of faces we want to extrude. 
We update these vertices, as new faces are created during the process to make a link between the "old boundary" (vertices which didn't move) and the "new boundary" (vertices newly created, which will be moved). 
We then create the new faces generated due to the extrusion, that we link in a consistent way between the old and new vertices.
(Only essentials parts are displayed, such code doesn't compile)

```C++
    // 5) Update Fout 
    ...

    // 5.1) Get the set of faces containing the old boundary vertices 
    igl::vertex_triangle_adjacency(V.rows(),F,VF,VI);

    // 5.2) Get the "outer" set of faces containing the boundary vertices 
    for(int i=0 ; i<bnd_loop.size(); i++){
        //Add this list of faces to the global set
        allAdjacentFacesOfBoundaries.insert(VF.at(bnd_loop.at(i)).begin(), VF.at(bnd_loop.at(i)).end());
    }

    ...
    std::set_difference(allAdjacentFacesOfBoundaries.begin(), allAdjacentFacesOfBoundaries.end(), selected_faces.begin(), selected_faces.end(),std::inserter(diff, diff.begin()));

    // 5.3) Edit old outer faces indices, replacing the old vertices with the indices of the duplicated boundary vertices
    for(int i=0 ; i < Fout.rows() ; i++){

        //For each colums of F
        for(int j = 0 ; j <3 ; j++){
            it = find (bnd_loop.begin(), bnd_loop.end(), Fout.row(i)[j]);

            //Changement : Only if we have found that the current value of Fout (in one column) is in the boundary
            if (it != bnd_loop.end()){
                int posInTheBoundaryList = std::distance(bnd_loop.begin(), it);
                isToChange = find (diff.begin(), diff.end(), i);

                //and the current Face number is not part of the selected area
                if (isToChange != diff.end()){  
                    //We change the value, with the new one : 
                    Fout.row(i)[j] = posInTheBoundaryList + V.rows();//where it is in the boundary list + offset due to non-changed initial vertices of V
                }
              }
        }
    }


    // 5.4) Add new faces, 2 per edge
    int f_idx = F.rows();

    for (int i = 0; i < bnd_loop.size(); i++) {
        int v1 = bnd_loop.at(i);
        int v2 = bnd_loop.at((i+1)%(bnd_loop.size()));
        int v3 = V.rows()+i;
        int v4 = V.rows()+ ((i+1)%(bnd_loop.size()));

        // set v1,v2,v3,v4 correctly
        Fout.row(f_idx++) << v3,v2,v1;
        Fout.row(f_idx++) << v2,v3,v4;
    }

    // 6) Check that the new mesh is a manifold (call is_edge_manifold, is_vertex_manifold on Vout,Fout)
    bool isEdgeManifold = igl::is_edge_manifold(Fout);
    bool isVertexManifold = igl::is_vertex_manifold(Fout,Vide);

    std::cout<< "isEdgeManifold :" << isEdgeManifold << endl;
    std::cout<< "isVertexManifold :" << isVertexManifold << endl;

    // 7) Update V,F
    V = Vout;
    F = Fout;
  ...
    
```

###### Result
<div style="text-align:center">
<p align="center">
  <img style="width : 600px;" src="/results/EasyExtrusionCoffe.gif">
  <img style="width : 600px;" src="/results/EasyExtrusionCube.gif">
</p>
</div>
<div style="text-align:center">
<p align="center">
  <img style="width : 600px;" src="/results/HardExtrusionCoffee.gif">
  <img style="width : 600px;" src="/results/HardExtrusionCube.gif">
</p>
</div>
<div style="text-align:center">
<p align="center">
  <img style="width : 600px;" src="/results/HondaExtrusion.gif">
</p>
</div>

Note that the code is not optimized by thoughtful laziness: this code is not made to be reused as is in other applications, and if this is the case, the code will be reviewed and optimized. Thanks for your understanding.