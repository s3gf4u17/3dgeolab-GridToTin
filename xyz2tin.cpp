#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/IO/OBJ.h>
#include <CGAL/boost/graph/graph_traits_Delaunay_triangulation_2.h>
#include <CGAL/boost/graph/copy_face_graph.h>
#include <CGAL/Polygon_mesh_processing/transform.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <boost/property_map/vector_property_map.hpp>
#include <CGAL/Polygon_mesh_processing/remesh_planar_patches.h>
#include <CGAL/Polygon_mesh_processing/region_growing.h>

using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point_3 = Kernel::Point_3;
using Point_set = CGAL::Point_set_3<Point_3>;

using Projection_traits = CGAL::Projection_traits_xy_3<Kernel>;
using TIN = CGAL::Delaunay_triangulation_2<Projection_traits>;

int main(int argc, char **argv) {
    float COSINE_MAX_ANGLE=std::stof(argv[3]); // 0.98
    float MAX_DISTANCE=std::stof(argv[4]); // 0.011
    auto t_start = std::chrono::high_resolution_clock::now();
    std::ifstream input(argv[1]);
    CGAL::Point_set_3<Point_3> points;
    input >> points;
    std::cerr << points.size() << " points read" << std::endl;
    TIN dsm (points.points().begin(), points.points().end());
    std::cerr << "dsm generated" << std::endl;

    using Mesh = CGAL::Surface_mesh<Point_3>;
    Mesh dsm_mesh;
    CGAL::copy_face_graph(dsm,dsm_mesh);
    std::cerr << "graph copied v" << dsm_mesh.number_of_vertices() << " f" << dsm_mesh.number_of_faces() << std::endl;
    
    Mesh::Vertex_index vi=*(dsm_mesh.vertices_begin());
    Kernel::Point_3 tv=dsm_mesh.point(vi);
    CGAL::Polygon_mesh_processing::transform(CGAL::Aff_transformation_3<Kernel>(CGAL::Translation(),Kernel::Vector_3(-tv.x(),-tv.y(),-tv.z())),dsm_mesh);
    std::cerr << "mesh transformed" << dsm_mesh.number_of_vertices() << " f" << dsm_mesh.number_of_faces() << std::endl;

    std::cerr << "declare vectors to store mesh properties" << std::endl;
    std::vector<std::size_t> region_ids(num_faces(dsm_mesh));
    std::vector<std::size_t> corner_id_map(num_vertices(dsm_mesh),-1);
    std::vector<bool> ecm(num_edges(dsm_mesh),false);
    boost::vector_property_map<CGAL::Epick::Vector_3> normal_map;

    std::cerr << "detect planar regions in the mesh ";
    std::size_t nb_regions = CGAL::Polygon_mesh_processing::region_growing_of_planes_on_faces(
        dsm_mesh,CGAL::make_random_access_property_map(region_ids),
        CGAL::parameters::cosine_of_maximum_angle(COSINE_MAX_ANGLE).region_primitive_map(normal_map).maximum_distance(MAX_DISTANCE)
    );
    std::cerr << nb_regions << std::endl;

    std::cerr << "detect corner vertices on the boundary of planar regions ";
    std::size_t nb_corners = CGAL::Polygon_mesh_processing::detect_corners_of_regions(
        dsm_mesh,CGAL::make_random_access_property_map(region_ids),
        nb_regions,CGAL::make_random_access_property_map(corner_id_map),
        CGAL::parameters::cosine_of_maximum_angle(COSINE_MAX_ANGLE).maximum_distance(MAX_DISTANCE).edge_is_constrained_map(CGAL::make_random_access_property_map(ecm))
    );
    std::cerr << nb_corners << std::endl;

    std::cerr << "run the remeshing algorithm using filled properties" << std::endl;
    Mesh finalmesh;
    CGAL::Polygon_mesh_processing::remesh_almost_planar_patches(
        dsm_mesh,finalmesh,nb_regions,nb_corners,
        CGAL::make_random_access_property_map(region_ids),
        CGAL::make_random_access_property_map(corner_id_map),
        CGAL::make_random_access_property_map(ecm),
        CGAL::parameters::patch_normal_map(normal_map)
    );
    std::cerr << "remeshed v" << finalmesh.number_of_vertices() << " f" << finalmesh.number_of_faces() << std::endl;

    std::ofstream result(argv[2]);
    CGAL::IO::write_OBJ(result,finalmesh,CGAL::parameters::stream_precision(6));
    result.flush();
    result.close();
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    std::cerr << elapsed_time_ms << "ms\n";
}