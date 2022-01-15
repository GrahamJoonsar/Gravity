#include <iostream>
#include "tigr.h"
#include <vector>
#include <cmath>
#include <random>
#include <ctime>



const TPixel BLACK = tigrRGB(0, 0, 0);
const TPixel WHITE = tigrRGB(255, 255, 255);

constexpr int SCREEN_WIDTH = 750;
constexpr int SCREEN_HEIGHT = 750;

double distSquared(double x1, double y1, double x2, double y2){
    return (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2);
}

constexpr int radius = 2;
constexpr int rad_2 = radius*radius;
constexpr int rad_4 = rad_2*rad_2;

constexpr double GRAV_CONSTANT = 2;


class Object{
    public:
        double mass, xVel = 0, yVel = 0;
        double x, y;
        
        Object(int _x, int _y, double _mass){
            x = _x;
            y = _y;
            mass = _mass;
        }

        Object(){}
        
        void move(){
            x += xVel;
            y += yVel;
        }

        void changeVelFromOtherObj(double ox, double oy, double omass, Tigr * screen){
            double dist = distSquared(x, y, ox, oy);
            if (dist > rad_4){
                double gravForce = ((mass + omass) / dist) * GRAV_CONSTANT;
                double angle = atan2(oy - y, ox - x);
                xVel += cos(angle) * gravForce;
                yVel += sin(angle) * gravForce;
            }
        }

        void draw(Tigr * screen){
            int y_p_rad = y + radius;
            int x_m_rad = x - radius;
            int x_p_rad = x + radius;
            for (int i = y - radius; i < y_p_rad; i++){
            for (int j = x_m_rad; j < x_p_rad; j++){
                if (distSquared(x, y, j, i) <= rad_2){
                    tigrPlot(screen, j, i, WHITE);
                }
            }}
        }
};

constexpr int num_bodies = 400;
constexpr double pi = 3.1415926;

Object bodies[num_bodies];


int main(){
    Tigr *screen = tigrWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Gravity", 0);
    std::srand(std::time(nullptr));
    for (int i = 0; i < num_bodies-1; i++){
        bodies[i] = (Object{std::rand()%SCREEN_WIDTH, std::rand()%SCREEN_HEIGHT, 1});
        double angle = atan2(375 - bodies[i].y, 375 - bodies[i].x);
        bodies[i].xVel = cos(angle + pi/4) * 2;
        bodies[i].yVel = sin(angle + pi/4) * 2;
    }
    bodies[num_bodies-1] = (Object{375, 375, 1000});



    while (!tigrClosed(screen)){
        tigrClear(screen, BLACK);

        tigrRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

        for (int i = 0; i < num_bodies; i++){
            for (int j = 0; j < num_bodies; j++){
                if (i != j){
                    bodies[i].changeVelFromOtherObj(bodies[j].x, bodies[j].y, bodies[j].mass, screen);
                }
            }
            bodies[i].draw(screen);
            bodies[i].move();
        }

        bodies[num_bodies-1].x = 375;
        bodies[num_bodies-1].y = 375;

        tigrUpdate(screen);
    }

    return 0;
}