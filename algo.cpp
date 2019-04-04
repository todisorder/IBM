////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <random>

#include "resources/parameters.cpp"
#include "resources/auxiliary_functions.cpp"

#define Here(a) std::cout << "output: " #a << '\n'
#define Here2(a,b) std::cout << (a) << (b) << '\n'

void print_this(float whatever)
{
    std::cout << whatever << '\n';
}

////////////////////////////////////////////////////////////
// Class for droplets
////////////////////////////////////////////////////////////

class Droplet : public sf::Drawable
{
public:
    static int number_of_droplets;

    Droplet()
    {
    }
    Droplet(float time, float x, float y)
    {
        number_of_droplets++;
        origin_time = time;
        pos.x = x;
        pos.y = y;
        float ballRadius = 2.f;
        body.setRadius(ballRadius);
        body.setOutlineThickness(0);
        body.setOutlineColor(sf::Color::Black);
        body.setFillColor(sf::Color::White);
        body.setOrigin(ballRadius / 2, ballRadius / 2);
        body.setPosition(x,y);
        //        std::cout << number_of_droplets << "\n";
        
    }
    sf::CircleShape body;
    float origin_time;
    sf::Vector2f pos;
    float elapsed_time(float time)
    {
        return time - origin_time;
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        
    }
    int static CountDroplets()
    {
        return number_of_droplets;
    }
    void initialize(float time, float x, float y)
    {
        origin_time = time;
        pos.x = x;
        pos.y = y;
        float ballRadius = 2.f;
        body.setRadius(ballRadius);
        body.setOutlineThickness(0);
        body.setOutlineColor(sf::Color::Black);
        body.setFillColor(sf::Color::White);
        body.setOrigin(ballRadius / 2, ballRadius / 2);
        body.setPosition(x,y);
    }
};

int Droplet::number_of_droplets = 0;




////////////////////////////////////////////////////////////
// Class for one ant
////////////////////////////////////////////////////////////
int what=1000;
class OneAnt : public sf::Drawable
{
public:
    
    OneAnt()
    {
    }
    
    sf::CircleShape body;
    sf::Vector2f pos;
    sf::Vector2f pos_old;
    sf::Vector2f antenna_L;
    sf::Vector2f antenna_R;
    sf::CircleShape antenna_L_shape;
    sf::CircleShape antenna_R_shape;
    sf::Vector2f sting_pos;
    sf::Vector2f vel;
    sf::Vector2f vel_old;
    sf::Vector2f force;
    sf::Color ant_color;
    int ant_type;
    int near_neighbors;
    int near_neighbors_t1;
    int near_neighbors_t2;
    int near_neighbors_t3;
    int near_neighbors_t4;
//    float near_radius;
    
    
    bool load()
    {
        // Create an ant
        float ballRadius = 10.f;
        body.setRadius(ballRadius - 3);
        body.setOutlineThickness(3);
        body.setOutlineColor(sf::Color::Black);
        body.setFillColor(sf::Color::White);
        body.setOrigin(ballRadius / 2, ballRadius / 2);

        float antennaRadius = 3.f;
        antenna_L_shape.setRadius(antennaRadius - 0);
        antenna_L_shape.setOutlineThickness(2);
        antenna_L_shape.setOutlineColor(sf::Color::Black);
        antenna_L_shape.setFillColor(sf::Color::Yellow);
        antenna_L_shape.setOrigin(antennaRadius / 2, antennaRadius / 2);
        
        antenna_R_shape.setRadius(antennaRadius - 0);
        antenna_R_shape.setOutlineThickness(2);
        antenna_R_shape.setOutlineColor(sf::Color::Black);
        antenna_R_shape.setFillColor(sf::Color::Yellow);
        antenna_R_shape.setOrigin(antennaRadius / 2, antennaRadius / 2);

//        float x = static_cast<float>(std::rand() % 1000);       // just to see if I can define this outside the class. I can.
//        float y = static_cast<float>(std::rand() % 1000);
        float x = x_2*Uniform(generator) + x_1*(1.-Uniform(generator));
        float y = y_2*Uniform(generator) + y_1*(1.-Uniform(generator));
//        pos.x = x;
//        pos.y = y;
        pos.x = x/50. +x_2/2;
        pos.y = y/50. +y_2/2;

        pos_old.x = pos.x;
        pos_old.y = pos.y;
        body.setPosition(pos.x,pos.y);
        
//        x = static_cast<float>(std::rand() % 1000);
//        y = static_cast<float>(std::rand() % 1000);
        float iniangle = UniformAngle(generator);
        vel.x = cos(iniangle);
        vel.y = sin(iniangle);
        float norm = sqrt(vel.x*vel.x + vel.y*vel.y)/100.;
        vel.x /= norm;
        vel.y /= norm;
        vel_old.x = vel.x;
        vel_old.y = vel.y;
        
        float xl = pos.x + sensing_area_radius * cos(Angle(vel.x,vel.y)-sensing_area_half_angle);
        float yl = pos.y + sensing_area_radius * sin(Angle(vel.x,vel.y)-sensing_area_half_angle);
        float xr = pos.x + sensing_area_radius * cos(Angle(vel.x,vel.y)+sensing_area_half_angle);
        float yr = pos.y + sensing_area_radius * sin(Angle(vel.x,vel.y)+sensing_area_half_angle);
        antenna_L.x = xl;
        antenna_L.y = yl;
        antenna_R.x = xr;
        antenna_R.y = yr;
        antenna_L_shape.setPosition(xl,yl);
        antenna_R_shape.setPosition(xr,yr);
        
        near_neighbors = 0;
//        near_radius = 10.;

        return true;
    }
    
//    void update(float time, std::vector<Droplet>& pheromone)
    void update(float time)
    {
//        DetermineType(time);
//        ComputeForce(time, pheromone);
        pos.x = pos_old.x + delta_t * vel_old.x;
        pos.y = pos_old.y + delta_t * vel_old.y;
        vel.x = vel_old.x + delta_t * (1./tau) * (-vel_old.x + force.x);
        vel.y = vel_old.y + delta_t * (1./tau) * (-vel_old.y + force.y);
        Boundarify();
        pos_old.x = pos.x;
        pos_old.y = pos.y;
        vel_old.x = vel.x;
        vel_old.y = vel.y;
        antenna_L.x = pos.x + sensing_area_radius * cos(Angle(vel.x,vel.y)-sensing_area_half_angle);
        antenna_L.y = pos.y + sensing_area_radius * sin(Angle(vel.x,vel.y)-sensing_area_half_angle);
        antenna_R.x = pos.x + sensing_area_radius * cos(Angle(vel.x,vel.y)+sensing_area_half_angle);
        antenna_R.y = pos.y + sensing_area_radius * sin(Angle(vel.x,vel.y)+sensing_area_half_angle);
        
        body.setPosition(pos.x, pos.y);
        antenna_L_shape.setPosition(antenna_L.x,antenna_L.y);
        antenna_R_shape.setPosition(antenna_R.x,antenna_R.y);
        
        
        body.setFillColor(ant_color);
        

    }
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        
    }
    void ComputeForce(float time, std::vector<Droplet>& pheromone);
    void DetermineType(float time);
    void DetermineType2(float time);
    float FeltPheromone(float time, float x, float y, std::vector<Droplet>& pheromone);
    void Boundarify();
    void ShowPosition()
    {
        std::cout << "x = "<< pos.x << ", y = " << pos.y << "\n";
    }
    void Interaction(const OneAnt& two);
    void SetType(int type);

//private:
    
};

class Population {
public:
    Population(int num_ants)
    {
        m_ants.resize(num_ants);
    }
    
    std::vector<OneAnt> m_ants;
    
    void Step(float time);
    
    
    
    
    
    
};

////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////

void Population::Step(float time)
{
    for (size_t i=0; i<m_ants.size(); i++)
    {
        // Current ant
        OneAnt& ant = m_ants[i];
        ant.near_neighbors = -1;
        ant.near_neighbors_t1 = -1;
        ant.near_neighbors_t2 = -1;
        ant.near_neighbors_t3 = -1;
        ant.near_neighbors_t4 = -1;
//        for (size_t j=0; j<m_ants.size() && j != i; ++j)
//        {
//            // Other ant
//            const OneAnt& other_ant = m_ants[j];
////            ant.Interaction(other_ant);
////            print_this(ant.near_neighbors);
//        }
////        ant.DetermineType(time);
        ant.force.x = 0.;
        ant.force.y = 0.;

        for (size_t j=0; (j<m_ants.size()) ; j++)
        {
            // Other ant
            const OneAnt& other_ant = m_ants[j];
            
            float dx = ant.pos.x - other_ant.pos.x;
            float dy = ant.pos.y - other_ant.pos.y;
            float dvx = ant.vel.x - other_ant.vel.x;
            float dvy = ant.vel.y - other_ant.vel.y;
            float dist2 = (dx*dx + dy*dy);
            
            
            if (dist2 <= near_radius*near_radius)
            {
                ant.near_neighbors++;
                if (other_ant.ant_type == 1)
                {
                    ant.near_neighbors_t1++;
                }
                if (other_ant.ant_type == 2)
                {
                    ant.near_neighbors_t2++;
                }
                if (other_ant.ant_type == 3)
                {
                    ant.near_neighbors_t3++;
                }
                if (other_ant.ant_type == 4)
                {
                    ant.near_neighbors_t4++;
                }
            }
        }
//        ant.DetermineType(time);
//        ant.DetermineType2(time);
    }

    
    for (size_t i=0; i<m_ants.size(); i++)
    {
        // Current ant
        OneAnt& ant = m_ants[i];

        for (size_t j=0; j<m_ants.size(); j++)
        {
            // Other ant
            const OneAnt& other_ant = m_ants[j];
            float dx = ant.pos.x - other_ant.pos.x;
            float dy = ant.pos.y - other_ant.pos.y;
            float dvx = ant.vel.x - other_ant.vel.x;
            float dvy = ant.vel.y - other_ant.vel.y;
            float dist2 = (dx*dx + dy*dy);

            float a_number = 1.;
            
            //  > 0 attract, < 0 repels
            float a11 = -30.;
            float a12 = 10.;
            float a13 = -10.;
            float a14 = 10.;
            float a21 = -10.;
            float a22 = 30.;
            float a23 = -10.;
            float a24 = 10.;
            float a31 = -10.;
            float a32 = -10.;
            float a33 = -10.;
            float a34 = -10.;
            float a41 = 100.;
            float a42 = -50.;
            float a43 = -10.;
            float a44 = -10.;
            std::vector<int> types = {ant.ant_type, other_ant.ant_type};
            
            if ((types[0]== 1 && types[1]==1))
            {
                a_number = a11;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 1 && types[1]==2))
            {
                a_number = a12;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 1 && types[1]==3))
            {
                a_number = a13;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 1 && types[1]==4))
            {
                a_number = a14;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 2 && types[1]==1))
            {
                a_number = a21;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 2 && types[1]==2))
            {
                a_number = a22;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 2 && types[1]==3))
            {
                a_number = a23;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 2 && types[1]==4))
            {
                a_number = a24;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 3 && types[1]==1))
            {
                a_number = a31;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 3 && types[1]==2))
            {
                a_number = a32;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 3 && types[1]==3))
            {
                a_number = a33;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 3 && types[1]==4))
            {
                a_number = a34;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 4 && types[1]==1))
            {
                a_number = a41;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 4 && types[1]==2))
            {
                a_number = a42;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 4 && types[1]==3))
            {
                a_number = a43;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            if ((types[0]== 4 && types[1]==4))
            {
                a_number = a44;
                ant.force.x += a_number*Decay(dist2)*(dx);
                ant.force.y += a_number*Decay(dist2)*(dy);
            }
            
            
//            float a1 = 10.;
//            float a2 = -100.;
//            float a3 = 1000.;
//            float a4 = 10.;
//            switch (ant.ant_type)
//            {
//                    case 1:
//                    a_number = a1;
//                    ant.force.x += a_number*Decay(dist2)*(dx);
//                    ant.force.y += a_number*Decay(dist2)*(dy);
//                    break;
//                    case 2:
//                    a_number = a2;
//                    ant.force.x += a_number*Decay(dist2)*(dx);
//                    ant.force.y += a_number*Decay(dist2)*(dy);
//                    break;
//                    case 3:
//                    a_number = a3;
//                    ant.force.x += a_number*Decay(dist2)*(dx);
//                    ant.force.y += a_number*Decay(dist2)*(dy);
//                    break;
//                    case 4:
//                    a_number = a4;
//                    ant.force.x += a_number*Decay(dist2)*(dx);
//                    ant.force.y += a_number*Decay(dist2)*(dy);
//                    break;
//                default:
//                    break;
//            }
            
            
            
        }
        // Update
        ant.update(time);
            

    }
}



void OneAnt::Interaction(const OneAnt& two)
{
    float p1x = pos.x;
    float p1y = pos.y;
    float p2x = two.pos.x;
    float p2y = two.pos.y;
    float dx = p1x - p2x;
    float dy = p1y - p2y;
    float dist2 = (dx*dx + dy*dy);
    float dist = sqrt(dist2);
//    near_neighbors = 0;
    
    
    if (dist <= near_radius)
    {
        near_neighbors++;
    }
//    if (dist > near_radius)
//    {
//        near_neighbors--;
//    }
}

void OneAnt::DetermineType(float time)
{
//    if (near_neighbors == 0) {
//        ant_type = 1;
//        ant_color = sf::Color(255,255,255);
//    }
//    else if (near_neighbors == 1) {
//        ant_type = 2;
//        ant_color = sf::Color(255,155,155);
//    }
//    else if (near_neighbors == 2) {
//        ant_type = 3;
//        ant_color = sf::Color(155,255,155);
//    }
//    else if (near_neighbors >= 3) {
//        ant_type = 4;
//        ant_color = sf::Color(155,155,255);
//    }
}

void OneAnt::DetermineType2(float time)
{
//    if (near_neighbors_t2 > near_neighbors_t3)
//    {
//        ant_type = 3;
//        ant_color = sf::Color(155,255,155);
//    }
    
}

        
void OneAnt::SetType(int type)
{
    switch (type) {
            case 1:
//            std::cout << "111\n";
            ant_type = 1;
            ant_color = sf::Color(255,255,255);
            break;
            case 2:
//                        std::cout << "222\n";
            ant_type = 2;
            ant_color = sf::Color(255,100,100);
            break;
            case 3:
//                        std::cout << "333\n";
            ant_type = 3;
            ant_color = sf::Color(100,255,100);
            break;
            case 4:
//                        std::cout << "444\n";
            ant_type = 4;
            ant_color = sf::Color(100,100,255);
            break;
            
        default:
            break;
    }
}
        
void OneAnt::ComputeForce(float time, std::vector<Droplet>& pheromone)
{
    float ax = pos.x;
    float ay = pos.y;
    float alx = antenna_L.x;
    float aly = antenna_L.y;
    float arx = antenna_R.x;
    float ary = antenna_R.y;
    float fpl = FeltPheromone(time, antenna_L.x,antenna_L.y,pheromone);
    float fpr = FeltPheromone(time, antenna_R.x,antenna_R.y,pheromone);
    float denom = fpl + fpr;
    float numerx = (alx - ax)*fpl + (arx - ax)*fpr;
    float numery = (aly - ay)*fpl + (ary - ay)*fpr;
    force.x = numerx/denom;
    force.y = numery/denom;
}


float OneAnt::FeltPheromone(float time, float x, float y, std::vector<Droplet>& pheromone)
{
    float result = 0.;
    float dx, dy;
    float elapsed_time;
    // neighbor_squares is defined in parameters.cpp:
    // std::vector<std::vector<float>> neighbor_squares = {N,S,W,E,NW,NE,SW,SE};
    
    int NN = Droplet::CountDroplets();
    for (int i=0; i<NN; ++i)
    {
        // Current droplet
        Droplet& dd = pheromone[i];
        dx = x-dd.pos.x;
        dy = y-dd.pos.y;
        elapsed_time = time - dd.origin_time;
        if (BoundaryMethod=="periodic")
        {
            float dist = (dx*dx + dy*dy);
            if (dist*0 <= ignore_droplet_if_this_far2)    // Careful... this does not measure across periodic Bdry!
            {
                result += Heat(dx,dy,elapsed_time,Amount);
                for (int dir=0; dir<neighbor_squares.size(); dir++)
                {
                    result += Heat(dx+neighbor_squares[dir][0],dy+neighbor_squares[dir][1],elapsed_time,Amount);
                }
                
            }
        }
        if (BoundaryMethod=="reflective")
        {
            float dist = (dx*dx + dy*dy);
            if (dist*0 <= ignore_droplet_if_this_far2)    // Careful... this does not measure across periodic Bdry!
            {
                result += Heat(dx,dy,elapsed_time,Amount);
            }
        }
        
    }

//    print_this(result);
    return std::max(threshold,result);
}

void OneAnt::Boundarify()
{
    if (BoundaryMethod == "reflective")
    {
        float newposx = std::max(x_1,std::min(x_2,pos.x));
        if (newposx >= x_2 || newposx <= x_1)
        {
            vel.x = -vel.x;
        }
        float newposy = std::max(y_1,std::min(y_2,pos.y));
        if (newposy >= y_2 || newposy <= y_1)
        {
            vel.y = -vel.y;
        }
    }
    if (BoundaryMethod == "periodic")
    {
        Periodify(pos.x,pos.y);
        Periodify(antenna_L.x,antenna_L.y);
        Periodify(antenna_R.x,antenna_R.y);
    }
    
}

////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////

int main()
{
    // Define some constants
    const float pi = 3.14159f;
//    float Lx = x_2-x_1;
//    float Ly = y_2-y_1;
    const int windowWidth = Lx;
    const int windowHeight = Ly;
    float ballRadius = 10.f;
//    int number_of_ants = 300;
//    number_of_ants -= 1;
    
    // Create the Ants
    Population pop = Population(number_of_ants);
//    std::vector<OneAnt*> all_the_ants;
//    for (int i = 0; i<number_of_ants; i++) {
//        all_the_ants.push_back(& pop.m_ants[i]);
//    }

    
    // Create the Ants
//    std::vector<OneAnt*> all_the_ants;
//    for (int i = 0; i<=number_of_ants; i++) {
//        all_the_ants.push_back(new OneAnt());
//    }
    std::size_t current = 0;    // ?????

    for (std::size_t i = 0; i < pop.m_ants.size(); ++i)
    {
        pop.m_ants[i].load();
//        std::cout << all_the_ants[i]->pos.y << "\t";
    }
    // Set types if they are not to change
    for (std::size_t i = 0; i < pop.m_ants.size(); ++i)
    {
        if (i<pop.m_ants.size()/4)
        {
            pop.m_ants[i].SetType(1);
        }
        if (i>=pop.m_ants.size()/4 && i<pop.m_ants.size()/2)
        {
            pop.m_ants[i].SetType(2);
        }
        if (i>=pop.m_ants.size()/2 && i<3*pop.m_ants.size()/4)
        {
            pop.m_ants[i].SetType(3);
        }
        if (i>=3*pop.m_ants.size()/4)
        {
            pop.m_ants[i].SetType(4);
        }

        //        std::cout << all_the_ants[i]->pos.y << "\t";
    }
    for (std::size_t i = 0; i < pop.m_ants.size(); ++i)
    {
//        std::cout<< "type = " << pop.m_ants[i].ant_type << "\n";
        //        std::cout << all_the_ants[i]->pos.y << "\t";
    }


    // Create the pheromone vector
//    std::vector<float> droplet_centers_x;
//    std::vector<float> droplet_centers_y;
//    std::vector<float> droplet_origin_times;

    
    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "SFML Ants",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    
    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
    return EXIT_FAILURE;
    
    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(170.f, 150.f);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("SFML Ants.\nPress space to start.");

    sf::Clock clock;
    sf::Clock pause_clock;
    
    sf::Time re_start_time;
    sf::Time I_have_been_paused_this_time;
    
    // I need -std=c++11 for the following:
    std::vector<Droplet> all_droplets = {};
    
    bool I_will_drop = false;
    float last_drop_time = 0.;
    float last_drop_iter = 0.;
    float drop_every_sec = 1.;
//    float drop_every_iter = 10.;
    int iteration = 0;
    bool isPaused = true;
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
            // Space key pressed: play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                if (isPaused)
                {
                    // (re)start the game
                    isPaused = false;
                    re_start_time = clock.getElapsedTime();
                    I_have_been_paused_this_time += pause_clock.getElapsedTime();
//                    clock.restart();
                }
                else
                {
                    isPaused = true;
                    pause_clock.restart();
//                    window.draw(pauseMessage);
                }
            }
            // Insert pheromone with mouse and keys
            if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
            {
                all_droplets.push_back(Droplet(clock.getElapsedTime().asSeconds(),event.mouseButton.x,event.mouseButton.y));
            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::D))
            {
                all_droplets.push_back(Droplet(clock.getElapsedTime().asSeconds(),sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y));
            }
            //
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R))
            {
                float mouse_x = sf::Mouse::getPosition(window).x;
                float mouse_y = sf::Mouse::getPosition(window).y;
                float eraser_size = 10.;
                float eraser_size2 = eraser_size*eraser_size;
                float rightnow = clock.getElapsedTime().asSeconds();
                for (int ii=0; ii<Droplet::CountDroplets(); ii++)
                {
                    Droplet& dd = all_droplets[ii];
                    float dx = dd.pos.x - mouse_x;
                    float dy = dd.pos.y - mouse_y;
                    float dist2 = (dx*dx + dy*dy);
                    
                    float elt = rightnow-all_droplets[ii].origin_time;
                    if (dist2 <= eraser_size2)
                    {
                        all_droplets.erase(all_droplets.begin()+ii);
                        Droplet::number_of_droplets--;
                    }
                }
            }


        }
        if (!isPaused)
        {
//            std::cout << "this = " <<clock.getElapsedTime().asSeconds() - last_drop_time << "\n";
            float rightnow = clock.getElapsedTime().asSeconds();
            sf::Time exec_time = clock.getElapsedTime() - I_have_been_paused_this_time;
//            std::cout << iteration << " iterations in " << exec_time.asSeconds() << "sec." << "\r";
            //
            //
            //
            pop.Step(rightnow);
            //
            //
            //
            //
            for (std::size_t i = 0; i < pop.m_ants.size(); ++i)
            {
                if (iteration - last_drop_iter >= drop_every_iter)
                {
                    I_will_drop = true;
                }
                //
                //  Update
                //
                //
//                pop.m_ants[i].update(rightnow, all_droplets);
                //
                //
                //
                //
                // Drop pheromone
//                if (I_will_drop)
//                {
//
//                    for (int ii=0; ii<Droplet::CountDroplets(); ii++)
//                    {
//                        // Erase some droplets
//                        float elt = rightnow-all_droplets[ii].origin_time;
//                        if (exp(-Evaporation * elt)*Amount <= .01 || rightnow - all_droplets[ii].origin_time > droplet_too_old)
//                        {
//                            all_droplets.erase(all_droplets.begin()+ii);
//                            Droplet::number_of_droplets--;
//                        }
//                    }
//                    // There is just a global pheromone:
//                    all_droplets.push_back(Droplet(rightnow,pop.m_ants[i].pos.x,pop.m_ants[i].pos.y));
//                    last_drop_time = clock.getElapsedTime().asSeconds();
//                    last_drop_iter = iteration;
//                }
            }
            iteration++;
            I_will_drop = false;
        }
        // Clear the window
        window.clear(sf::Color(20, 20, 30));
        
        //  Drawing
        // Draw the ants
        for (std::size_t i = 0; i < pop.m_ants.size(); ++i)
        {
            window.draw(pop.m_ants[i].body);
            window.draw(pop.m_ants[i].antenna_L_shape);
            window.draw(pop.m_ants[i].antenna_R_shape);
        }
        // Draw the pheromone
        int N = Droplet::CountDroplets();
//            print_this(N-all_droplets.size());
        
        for (int i=0; i<N; ++i)
        {
            // Current droplet
//            Droplet& dd = all_droplets[i];
//            window.draw(dd.body);
//            if (!isPaused)
//            {
//                dd.body.setFillColor(sf::Color(255,255,255,255*exp(.05*(dd.origin_time - clock.getElapsedTime().asSeconds()))));
//            }
        }
        


//        else
//        {
//            // Draw the pause message
//            window.draw(pauseMessage);
//        }
        
        // Display things on screen
        window.display();
//        print_this(iteration);
    }

    return EXIT_SUCCESS;

}