#include "PipeNetwork.hpp"
#include "Node.hpp"
#include "Tube.hpp"

#include "catch.hpp"

#include <vector>
#include <array>
#include <fstream>
#include <cmath>
#include <type_traits>

namespace cie
{
namespace pipenetwork
{

TEST_CASE( "Node_test" )
{
    Node node( 0.3, 2.2, -0.1, 7 );

    const Node& constNode = node;

    double tolerance = 1e-14;

    CHECK( constNode.x( ) == Approx( 0.3 ).epsilon( tolerance ) );
    CHECK( constNode.y( ) == Approx( 2.2 ).epsilon( tolerance ) );
    CHECK( constNode.flow( ) == Approx( -0.1 ).epsilon( tolerance ) );
    CHECK( constNode.id( ) == 7 );
}

TEST_CASE( "Tube_test" )
{
    Node node1( 4.3,  0.2, -0.5, 4 );
    Node node2( 5.8, -1.8,  0.5, 6 );

    Tube tube( &node1, &node2, 0.3 );

    const Tube& constTube = tube;

    auto tubeNode1 = constTube.node1( );
    auto tubeNode2 = constTube.node2( );

    // Check if node1( ) and node2( ) return a const Node*
    static_assert( std::is_same<decltype( tubeNode1 ), const Node*>::value,
    		       "Return value of Tube::node1 must be const Node*");

    static_assert( std::is_same<decltype( tubeNode2 ), const Node*>::value,
    		       "Return value of Tube::node1 must be const Node*");

    double tolerance = 1e-10;

    CHECK( tubeNode1->x( )    == Approx( 4.3 ).epsilon( tolerance ) );
    CHECK( tubeNode1->y( )    == Approx( 0.2 ).epsilon( tolerance ) );
    CHECK( tubeNode1->flow( ) == Approx(-0.5 ).epsilon( tolerance ) );
    CHECK( tubeNode1->id( )   == Approx( 4 ).epsilon( tolerance ) );

    CHECK( tubeNode2->x( )    == Approx( 5.8 ).epsilon( tolerance ) );
    CHECK( tubeNode2->y( )    == Approx(-1.8 ).epsilon( tolerance ) );
    CHECK( tubeNode2->flow( ) == Approx( 0.5 ).epsilon( tolerance ) );
    CHECK( tubeNode2->id( )   == Approx( 6 ).epsilon( tolerance ) );

    CHECK( constTube.length( ) == Approx( 2.5 ).epsilon( tolerance ) );

	double pi = 3.14159265358979323846;

    double B = pi * 9.81 * std::pow( 0.3, 4 ) / ( 128 * 1e-6 * 2.5 );

    CHECK( constTube.permeability( ) == Approx( B ).epsilon( tolerance ) );
}

TEST_CASE( "PipeNetwork_test" )
{
    std::string filename = "testinput.txt";

    std::ofstream outfile( filename );

    REQUIRE( outfile.is_open( ) );

    outfile <<
        "9              \n"
        "15             \n"
        "1000    0 -100 \n"
        "1000 2500  -50 \n"
        "   0 1000 -200 \n"
        "1300 1500    0 \n"
        "1300 2000    0 \n"
        "2500 2000    0 \n"
        "1500  500    0 \n"
        "2000 -500    0 \n"
        "3000    0  350 \n"
        "2 1 0.3        \n"
        "2 3 0.1        \n"
        "2 0 0.6        \n"
        "0 4 0.4        \n"
        "0 6 0.4        \n"
        "0 7 0.5        \n"
        "7 8 0.3        \n"
        "3 8 0.6        \n"
        "5 8 0.5        \n"
        "4 5 0.7        \n"
        "1 5 0.7        \n"
        "6 5 0.5        \n"
        "4 3 0.7        \n"
        "1 4 0.3        \n"
        "6 7 0.4        \n";

    outfile.close( );

    PipeNetwork network( filename );

    std::vector<double> fluxes = network.computeFluxes( );

    std::array<double, 15> expectedFluxes
    {
        33.7095714610594,
        0.626574546787126,
        165.663853992154,
        83.0196645235276,
        96.2163366793506,
        86.4278527892752,
        60.9761468868924,
        173.956788866264,
        115.067064246844,
       -78.3132246132872,
        71.7122462783971,
        121.668042581733,
        173.330214319477,
        11.9973251826624,
       -25.4517059023828
    };

    double tolerance = 1e-10;

    for( int i = 0; i < 15; ++i )
    {
        CHECK( fluxes[i] == Approx( expectedFluxes[i] ).epsilon( tolerance ) );
    }

    REQUIRE_NOTHROW( PipeNetwork( "testinput.txt" ).computeFluxes( ) );
}

} // pipenetwork
} // cie
