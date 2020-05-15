#include "Node_header.h"
#include <cmath>


class Tube 
{
public:
    Tube(Node* n1_, Node* n2_, double diameter_) {
        n1 = n1_;
        n2 = n2_;
        diameter = diameter_;
    }

    Node* getn1()
    {
        return n1;
    }

    Node* getn2()
    {
        return n2;
    }

    double getdiameter()
    {
        return diameter;
    }
    double length() {
        // sqrt((n1.x-n2.x)**2 + (n1.y-n2.y)**2)
        // (*n1).x      n1->x
        double l = sqrt(((n1->getx() - n2->getx()) * (n1->getx() - n2->getx())) + ((n1->gety() - n2->gety()) * (n1->gety() - n2->gety())));
        return l;
    }
    double permeability() {
        double perm = (3.14 * 9.81 * pow(getdiameter(), 4)) / (128 * 1.002E-6 * length());
        return perm;
    }
private:
    Node* n1;
    Node* n2;
    double diameter;
};