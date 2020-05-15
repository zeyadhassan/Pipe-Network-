//#include "PipeNetwork_header.h"

class Node 
{
public:
    Node(double x_, double y_, double flow_, int id_) {
        x = x_;
        y = y_;
        flow = flow_;
        id = id_;
    }
    double getx() {
        return x;
    }
    double gety() {
        return y;
    }
    double getflow() {
        return flow;
    }
    double getid() {
        return id;
    }

private:
    double x;
    double y;
    double flow;
    int id;

};
