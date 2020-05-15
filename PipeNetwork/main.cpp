#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include "PipeNetwork_header.h"
#include "linalg.hpp"
using namespace std;


int main() {

	std::ifstream infile("input.txt");
	int nodes_number;
	int tubes_number;

	//inputting number of nodes and number of tubes from file
	infile >> nodes_number;
	infile >> tubes_number;

	//Creating the Vector of tubes and vector of nodes
	vector<Tube> tubes;
	vector<Node*> nodes;
	tubes.reserve(tubes_number);
	nodes.reserve(nodes_number);

	// Inputting the nodes data;
	for (int i = 0; i < nodes_number; i++) {
		double x, y, flow;
		infile >> x;
		infile >> y;
		infile >> flow;

		Node *n = new Node(x, y, flow, i);
		nodes.push_back(n);
	}

	//Inputting the tubes data
	for (int i = 0; i < tubes_number; i++) {
		int n1_idx, n2_idx;
		double diameter;
		infile >> n1_idx;
		infile >> n2_idx;
		infile >> diameter;
		Tube t = Tube(nodes[n1_idx], nodes[n2_idx], diameter);
		tubes.push_back(t);
	}

	//creating the permeability matrix
	cie::linalg::Matrix B(8, 8);
	
	//setting pipe values to zeros except the martix(0,0)
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			B(i, j) = 0.0;
		}
	}

	//Setting up permeability matrix
	for (int k=0; k < 11; k++)
	{

		Node *id1, *id2;
		double Permeability_Tube;
		id1 = tubes[k].getn1();
		id2 = tubes[k].getn2();
		Permeability_Tube = tubes[k].permeability();

		B(id1->getid(), id1->getid()) += Permeability_Tube;
		B(id2->getid(),id2->getid()) += Permeability_Tube;
		B(id1->getid(),id2->getid()) -= Permeability_Tube;
		B(id2->getid(), id1->getid()) -= Permeability_Tube;
		
	}


	//Setting up the load vector
	vector<double> FlowVector;
	FlowVector.reserve(nodes_number);
	
	for (int k = 0; k < 8; k++) {
	
		FlowVector.push_back(nodes[k]->getflow());
		if (FlowVector[k] != 0) {
			FlowVector[k] = FlowVector[k] * (-1);
		}
	}

	//imposing boundary conditions
	for (int i = 0; i < 8; i++) {

		B(i, 0) = 0;
		B(0, i) = 0;
	}
	B(0, 0) = 1.0;
	FlowVector[0] = 0;


	//Solving and creating the head vector
	vector<double> Head_Vector = cie::linalg::solve(B, FlowVector);

	//vector of q and initialization to zero
	vector<double> fluxes;
	for (int i = 0; i < tubes_number; i++) {
		fluxes.push_back(0);
	}

	//solving for q
	for (int k = 0; k < tubes_number; k++) {

		double h1, h2;
		h1 = Head_Vector[tubes[k].getn1()->getid()];
		h2 = Head_Vector[tubes[k].getn2()->getid()];
		double Permeability_Tube = tubes[k].permeability();
		fluxes[k] = Permeability_Tube * (h1 - h2);
	}

	for (int i = 0; i < 8;i++) {
		for (int j=0;j<8;j++){
			cout << B(i, j) << " \t";
		}
		cout << endl;
	}

	for (int i = 0; i < 11; i++) {
		std::cout << "The fluxes for the Tube ["<< i << "] ="<< fluxes[i] << endl;
	}

}
