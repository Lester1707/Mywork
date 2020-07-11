#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

const double pi = 3.1415926535;
const double g = 9.8;
const double k_air = 0.25;
const double N_reaction = 2500;

using namespace sf;
using namespace std;

double macheps_d()
{
    double e = 1;
    while (1 + e / 2 > 1)
        e /= 2;
    return e;
}

const double eps_d = macheps_d();

double distance(const double &x1, const double &y1, const double &x2, const double &y2) {
    return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

class Space {
private:
    //class of phys objects
    class Body {
    public:
        size_t index;
        bool static_pos;          //options of class      
        int index_to_force;

        double x;
        double y;
        double velocity_x;
        double velocity_y;              //moving
        double acceleration_x;
        double acceleration_y;

        double mass;          //some paramters
        double lose_k;
        double radius;

        double w;
        double e;
        double x_p;                 //rotate
        double y_p;
        double ang;

        CircleShape circle;
        CircleShape point;        //draw

        const float r_sz = 1.f;                         //proportis of point rotation
        const float rad_point = (float)radius / 2.f;

        Body(const double &in_x, const double &in_y, const double &rad, const size_t &i, const double &m = 1,
            const double &vel_x = 0, const double &vel_y = 0, const double &w_r = 0, const double &lose = 1) {
            x = in_x;
            y = in_y;
            velocity_x = vel_x;
            velocity_y = vel_y;
            acceleration_x = 0;
            acceleration_y = 0;

            mass = m;
            radius = rad;
            lose_k = lose;
            index_to_force = -1;

            circle.setFillColor(Color::White);
            circle.setRadius((float)radius);
            circle.setOutlineThickness(1.f);
            circle.setOutlineColor(Color::Black);

            index = i;
            static_pos = false;

            x_p = x + radius;
            y_p = y + radius - rad_point;
            e = 0;
            w = w_r;
            ang = 0;

            point.setFillColor(Color::Black);
            point.setRadius(r_sz);
        }

        void operator = (const Body &body) {
            index = body.index;
            static_pos = body.static_pos;

            x = body.x;
            y = body.y;
            velocity_x = body.velocity_x;
            velocity_y = body.velocity_y;
            acceleration_x = body.acceleration_x;
            acceleration_y = body.acceleration_y;

            mass = body.mass;
            radius = body.radius;
            lose_k = body.lose_k;

            point = body.point;
            circle = body.circle;

            x_p = body.x_p;
            y_p = body.y_p;
            w = body.w;
            ang = body.ang;
            e = body.e;
        }

        void draw(RenderWindow &win) {
            circle.setPosition(Vector2f((float)x, (float)y));
            circle.setRadius((float)radius);
            win.draw(circle);                                               //draw circles
            if (w != 0) {
                point.setPosition(Vector2f((float)x_p - r_sz, (float)y_p - r_sz));
                win.draw(point);
            }
            if (index_to_force >= 0)
                circle.setOutlineColor(Color::Yellow);
            else
                circle.setOutlineColor(Color::Black);
        }

        void rotate(const float &Xlimit, const int &time = 1) {
            if (abs(e) / time < eps_d)
                e = 0;
            w += e / time;
            if (abs(w) / time < eps_d)
                w = 0;
            ang += w / time;
            if (ang > 2 * pi)                   //rotate moving
                ang = 2 * pi - ang;
            if (ang < -2 * pi)
                ang = 2 * pi + ang;
            x_p = x + radius + cos(ang)*rad_point;
            y_p = y + radius - rad_point + (1 - sin(ang))*rad_point;
        }

        void step_x(const float &Xlimit, const int &time = 1) {
            if (x + 2 * radius > Xlimit) {
                x = Xlimit - 2 * radius;
                velocity_x *= -lose_k;
            }
            if (x < 0) {
                x = 0;                              //step on x axis in form: dv = a*dt; dx = v*dt, so just kinematics
                velocity_x *= -lose_k;
            }
            if (abs(acceleration_x) / time < eps_d)
                acceleration_x = 0;
            velocity_x += acceleration_x / time;
            if (abs(velocity_x) / time < eps_d)
                velocity_x = 0;
            x += velocity_x / time;
        }

        void step_y(const float &Ylimit, const int &time = 1) {
            if ((y + 2 * radius) > Ylimit) {
                y = Ylimit - 2 * radius;
                velocity_y *= -lose_k;                          //step on y axis in form: dv = a*dt; dx = v*dt, so just kinematics
            }
            if (y < 0) {
                y = 0;
                velocity_y *= -lose_k;
            }
            if (abs(acceleration_y) / time < eps_d)
                acceleration_y = 0;
            velocity_y += acceleration_y / time;
            if (abs(velocity_y) / time < eps_d)
                velocity_y = 0;
            y += velocity_y / time;
        }

        bool smash(Body &a, const int &time) {
            double L = distance(0, 0, x + radius - a.x - a.radius, y + radius - a.y - a.radius);
            if (L <= (radius + a.radius)) {

                if (a.static_pos) {
                    velocity_x *= -1;
                    velocity_y *= -1;
                    return true;
                }
                if (static_pos) {
                    a.velocity_x *= -1;
                    a.velocity_y *= -1;
                    return true;
                }
                //fucntion of collisions of objects
                double cos_a = -(y + radius - a.y - a.radius) / L;
                double sin_a = -(x + radius - a.x - a.radius) / L;
                double N = N_reaction * ((radius + a.radius) - L);

                double w_new1 = ((mass*radius*radius - a.mass*a.radius*a.radius)*w + 2 * a.mass*a.radius*a.radius*a.w) /
                    (mass*radius*radius + a.mass*a.radius*a.radius);
                double w_new2 = ((a.mass*a.radius*a.radius - mass * radius*radius)*a.w + 2 * mass*radius*radius*w) /
                    (mass*radius*radius + a.mass*a.radius*a.radius);

                w = w_new1;
                a.w = w_new2;

                acceleration_x += N * -sin_a / mass;
                acceleration_y += N * -cos_a / mass;
                a.acceleration_x += N * sin_a / a.mass;
                a.acceleration_y += N * cos_a / a.mass;

                return true;
            }
            return false;
        }
    };
    //class of using forces
    class Force {
    public:
        char type;              //type of force

        double g;
        double r;           //parametrs
        double G;

        double value_x;     //another parametrs
        double value_y;

        int potencial_body;     //pointer to force object

        Force() {
            type = 'u';
            g = 0;
            r = 0;
            G = 0;
            value_x = 0;
            value_y = 0;
            potencial_body = -1;
        }

        Force(const string &&name, const double &a) : Force() {
            if (name == "gravity") {
                type = 'g';
                g = a;                      //constructor for default forces
            }
            if (name == "air") {
                type = 'a';
                r = a;
            }
        }

        Force(const double &x, const double &y) : Force() {
            type = 'c';
            value_x = x;            //constructor for some const vector force
            value_y = y;
        }

        Force(const double &K, int index, Body &body, const double &st) : Force() {
            type = 'p';
            potencial_body = index;
            body.circle.setFillColor(Color::Yellow);                  //constructor for potencial force
            body.static_pos = st;
            G = K;
        }

        Force(const double &K) : Force() {
            type = 's';         // constructor for some gas force
            r = K;
        }

        void force(Body &a, const vector <Body> &bods) {
            if (type == 'g')
                a.acceleration_y += g;
            if (type == 'a' || type == 's') {
                a.acceleration_x += -r * a.velocity_x / a.mass;
                a.acceleration_y += -r * a.velocity_y / a.mass;
            }
            if (type == 'p') {
                if (potencial_body == a.index) {                        //using some force
                    return;
                }
                double L = distance(0, 0, a.x + a.radius - bods[potencial_body].x - bods[potencial_body].radius,
                    a.y + a.radius - bods[potencial_body].y - bods[potencial_body].radius);
                double cos = (a.x + a.radius - bods[potencial_body].x - bods[potencial_body].radius) / L;
                double sin = (a.y + a.radius - bods[potencial_body].y - bods[potencial_body].radius) / L;
                a.acceleration_x -= G * cos / L / L / a.mass;
                a.acceleration_y -= G * sin / L / L / a.mass;
            }
            if (type == 'c') {
                a.acceleration_x += value_x / a.mass;
                a.acceleration_y += value_y / a.mass;
            }
        }
    };

    const int Xbord;
    const int Ybord;
    RenderWindow window;            //paramets for window
    Event event;
    RectangleShape square_stat;
    const float stat_k = 1.f / 5;

    const float to_size;      //scale 1 metr to pix
    float new_size;

    vector <Force> forces;      //objects
    vector <Body> bodyes;

    const int force_size = 3;      //work with forces
    vector <bool> colors;
    
    float size_ghost;       //spawn imaganary circle
    CircleShape ghost;

    VertexArray points;     //lines

    int stat_show;                 //for statistic
    
    Text stat_text;
    string str;         //work with text
    string str_scale;
    Font font;

    const int scale = 500;          //info for step
    const int frame_rate = 60;

    double time;   
    bool pause;    //time
    double seconds;

    Vector2i cord;      //help values
    bool new_smash;
    bool new_obj;
    const int next_power = 10;

    void move() {
        for (int i = 0; i < scale; i++) {
            object_dynamic();
            for (size_t j = 0; j < bodyes.size(); j++) {
                for (size_t k = j + 1; k < bodyes.size(); k++)        //step for each object
                    bodyes[j].smash(bodyes[k], scale);
                bodyes[j].rotate(Xbord - Xbord * stat_k, scale);
                bodyes[j].step_x(Xbord - Xbord * stat_k, scale);
                bodyes[j].step_y((float)Ybord, scale);
            }
        }
    }

    int get_index_body(Vector2i &v) {
        if (v.x < 0 || v.y < 0)
            return -1;
        for (size_t i = 0; i < bodyes.size(); i++)
            if (bodyes[i].radius > distance(v.x, v.y, bodyes[i].radius + bodyes[i].x, bodyes[i].radius + bodyes[i].y)) {
                stat_show = i;              //find object in v cordinates
                v = Vector2i(-1, -1);
                return stat_show;
            }
        return -1;
    }

    void draw_bond_lines() {
        points[0] = (Vertex(Vector2f((1 - stat_k)*Xbord, stat_k*Ybord), Color::Black));
        points[1] = (Vertex(Vector2f((float)Xbord, stat_k*Ybord), Color::Black));
        window.draw(points);                                                                    //draw lines in gray space
        points[0] = (Vertex(Vector2f((1 - stat_k)*Xbord, Ybord - stat_k * Ybord), Color::Black));
        points[1] = (Vertex(Vector2f((float)Xbord, (float)Ybord - stat_k * Ybord), Color::Black));
        window.draw(points);
    }

    void object_dynamic() {
        for (size_t i = 0; i < bodyes.size(); i++) {
            bodyes[i].acceleration_x = 0;
            bodyes[i].acceleration_y = 0;           //using dynamic for each object
            for (size_t j = 0; j < forces.size(); j++)
                forces[j].force(bodyes[i], bodyes);
        }
    }

    void statistic(Vector2i &v) {
        window.draw(square_stat);
        stat_text.setPosition(Vector2f((1 - stat_k)*Xbord + 2.f, 0));
        stat_text.setCharacterSize(7 + Xbord / 100);
        stat_text.setFillColor(Color::Black);
        str = "stats:\n";
        if (stat_show >= 0) {
            bodyes[stat_show].circle.setOutlineColor(Color::Red);
            str += "x = " + to_string((int)(bodyes[stat_show].x + bodyes[stat_show].radius));   //show statistic
            str += "\n";
            str += "y = " + to_string((int)(bodyes[stat_show].y + bodyes[stat_show].radius));
            str += "\n";
            str += "mass = " + to_string(bodyes[stat_show].mass);
            str += "\n";
            str += "speed = " + to_string(to_speed()*distance(0, 0, bodyes[stat_show].velocity_x, bodyes[stat_show].velocity_y));
            str += "\n";
            str += "force = " + to_string((new_size / to_size)*bodyes[stat_show].mass*to_acel()*distance(0, 0, bodyes[stat_show].acceleration_x, bodyes[stat_show].acceleration_y));
        }
        stat_text.setString(str);
        window.draw(stat_text);
    }

    void remove_object(const size_t &index) {
        if (index == stat_show)
            stat_show = -1;
        else if ((int)index < stat_show)
            stat_show--;
        for (size_t i = 0; i < forces.size(); i++)
            if (forces[i].potencial_body == index) {
                forces.erase(forces.begin() + i);
                break;
            }
        for (size_t i = 0; i < forces.size(); i++)
            if (forces[i].potencial_body > (int)index)       //removing object with some index
                forces[i].potencial_body--;
        bodyes.erase(bodyes.begin() + index);
        for (size_t i = index; i < bodyes.size(); i++)
            bodyes[i].index--;
    }


    void remove_force(const string &name) {
        if (name == "gravity")
            for (size_t i = 0; i < forces.size(); i++) {
                if (forces[i].type == 'g') {
                    forces.erase(forces.begin() + i);
                    colors[0] = false;
                    return;
                }
            }                                           //remove defaul force
        if (name == "air")
            for (size_t i = 0; i < forces.size(); i++) {
                if (forces[i].type == 'a') {
                    colors[1] = false;
                    forces.erase(forces.begin() + i);
                    return;
                }
            }
    }

    void draw_size_line() { // draw lines for scale
        stat_text.setString(str_scale);
        stat_text.setFillColor(Color(100, 100, 100));
        stat_text.setCharacterSize(9 + Xbord / 500);
        if (new_size >= stat_text.getLocalBounds().height) {
            stat_text.setPosition(Vector2f(Xbord / 50 - stat_text.getLocalBounds().width - Xbord / 200, Ybord / 50 - (stat_text.getLocalBounds().height - new_size) / 2));
            points[0] = (Vertex(Vector2f((float)Xbord / 50, (float)Ybord / 50), Color(100, 100, 100)));
            points[1] = (Vertex(Vector2f((float)Xbord / 50, (float)Ybord / 50 + new_size), Color(100, 100, 100)));
        }
        else {
            stat_text.setPosition(Vector2f((float)Xbord / 50 - stat_text.getLocalBounds().width - Xbord / 200, (float)Ybord / 50));
            points[0] = (Vertex(Vector2f((float)Xbord / 50, (float)Ybord / 50 + (stat_text.getLocalBounds().height - new_size) / 2), Color(100, 100, 100)));
            points[1] = (Vertex(Vector2f((float)Xbord / 50, (float)Ybord / 50 + (stat_text.getLocalBounds().height - new_size) / 2 + new_size), Color(100, 100, 100)));
        }
        window.draw(points);
        window.draw(stat_text);
    }

    void draw() {
        for (size_t i = 0; i < bodyes.size(); i++)
            bodyes[i].draw(window);
    }

    void get_time() {
        stat_text.setCharacterSize(5 + 3 * Xbord / 200);
        stat_text.setString("sec " + to_string(seconds));
        stat_text.setPosition((1 - stat_k)*Xbord, Ybord - 2*stat_text.getGlobalBounds().height);
        window.draw(stat_text);
    }

    void additional_funcs() {
        str = "|| R";
        stat_text.setString(str);
        stat_text.setCharacterSize(8 + 3 * Xbord / 200);
        stat_text.setPosition((1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2,
            (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2);
        str = "||";
        stat_text.setString(str);
        if (pause)
            stat_text.setFillColor(Color::Green);               // draw pause and restart
        else
            stat_text.setFillColor(Color::Black);
        window.draw(stat_text);
        stat_text.setFillColor(Color::Black);
        stat_text.setPosition(stat_text.getPosition().x + stat_text.getLocalBounds().width,
            stat_text.getPosition().y);
        str = " R";
        stat_text.setString(str);
        window.draw(stat_text);
    }

    void act_draw() { // draw buttons
        stat_text.setCharacterSize(5 + 3 * Xbord / 200);
        stat_text.setString("gravity");
        if (colors[0])
            stat_text.setFillColor(Color::Green);
        else
            stat_text.setFillColor(Color::Black);
        stat_text.setPosition(Vector2f((1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2,
            stat_k*Ybord + ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1)));
        window.draw(stat_text);
        stat_text.setString("air");
        if (colors[1])
            stat_text.setFillColor(Color::Green);
        else
            stat_text.setFillColor(Color::Black);
        stat_text.setPosition(Vector2f((1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2,
            stat_k*Ybord + stat_text.getLocalBounds().height + 2 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1)));
        window.draw(stat_text);
        stat_text.setString("object field");
        if (colors[2])
            stat_text.setFillColor(Color::Yellow);
        else
            stat_text.setFillColor(Color::Black);
        stat_text.setPosition(Vector2f((1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2,
            stat_k*Ybord + 2 * stat_text.getLocalBounds().height + 3 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1)));
        window.draw(stat_text);
        stat_text.setFillColor(Color::Black);
    }

    double to_metr() {
        return 1 / to_size;
    }
    double to_pix() {
        return to_size;
    }
    double to_frame() {
        return 1 / time;
    }
    double to_sec() {
        return time;                        //functions of translating computer varialbe in CI
    }
    double to_speed() {
        return to_metr() / to_sec();
    }
    double to_speed_pix() {
        return to_pix() / to_frame();
    }
    double to_acel() {
        return to_metr() / to_sec() / to_sec();
    }
    double to_acel_pix() {
        return to_pix() / to_frame() / to_frame();
    }
    double r_to_frame() {
        return 1 / to_frame();
    }
public:
    Space(const int &X, const int &Y, const float &sz) : Xbord(X), Ybord(Y), to_size(sz), cord(-1, -1), points(PrimitiveType::Lines, 2), colors(force_size, false), window(VideoMode(Xbord, Ybord), "Simulation") {
        window.setFramerateLimit(frame_rate);

        time = (double)1 / frame_rate;
        seconds = 0;
        pause = false;
        new_obj = false;

        square_stat.setFillColor(Color(128, 128, 128));
        square_stat.setPosition(Vector2f((1 - stat_k)*Xbord, 0));
        square_stat.setSize(Vector2f((float)stat_k*Xbord, (float)Ybord));

        font.loadFromFile("UbuntuMono-R.ttf");
        stat_text = Text("", font);
        stat_text.setFillColor(Color::Black);
        stat_show = -1;

        size_ghost = 0.f;

        new_size = to_size;
        str = "1\n";
        if (to_size < 1) {
            while (new_size < 1) {
                new_size *= next_power;
                str += "0\n";
            }
            if (str.size() >= 2 && str.size() < 8)
                str += "m\ne\nt\nr\n";
            else if (str.size() >= 8) {
                str.resize(str.size() - 6);
                str += "k\ni\nl\no\nm\ne\nt\nr\n";
            }
        } else if (to_size > Ybord - Ybord / 25) {
            while (new_size > Ybord - Ybord / 25) {
                new_size /= next_power;
                str += "0\n";
            }
                str += "^\n-\n1\nm\ne\nt\nr\n";
        }
        else
            str += "m\ne\nt\nr\n";
        str_scale = str;
    }

    void show() { //general function which controling all moving computer and user's
        while (window.isOpen()) {
            window.clear();
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                if (event.type == Event::MouseButtonPressed) {
                    cord = Mouse().getPosition(window);
                    if (event.mouseButton.button == Mouse().Left && cord.x > 0 && cord.y > 0 && cord.x < (1 - stat_k)*Xbord && get_index_body(cord) == -1) {
                        if (!colors[2])
                            add_body(cord.x, cord.y, size_ghost, size_ghost,
                            (cord.x - Mouse().getPosition(window).x) / 10, -(Mouse().getPosition(window).y - cord.y) / 10, 0, 0.9);
                        else
                            add_potencial_force(size_ghost*size_ghost, cord.x, cord.y, size_ghost,
                                size_ghost, (cord.x - Mouse().getPosition(window).x) / 10, -(Mouse().getPosition(window).y - cord.y) / 10, 0);
                        stat_show = bodyes.size() - 1;
                        new_obj = true;
                    }
                    if (event.mouseButton.button == Mouse().Right) {
                        for (size_t i = 0; i < bodyes.size(); i++)
                            if (distance(cord.x, cord.y, bodyes[i].x + bodyes[i].radius, bodyes[i].y + bodyes[i].radius) < bodyes[i].radius)
                                remove_object(i);
                    }
                    if (cord.x > (1 - stat_k)*Xbord) {
                        stat_text.setCharacterSize(5 + 3 * Xbord / 200);
                        stat_text.setString("gravity");
                        if (cord.y <= stat_k * Ybord + ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) +
                            stat_text.getLocalBounds().height && cord.y >= stat_k * Ybord +
                            ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) && !colors[0]) 
                            add_force_gravity();
                        else if (cord.y <= stat_k * Ybord + ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) +
                            stat_text.getLocalBounds().height && cord.y >= stat_k * Ybord +
                            ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) && colors[0])
                            remove_force("gravity");
                        stat_text.setString("air");
                        if (cord.y >= stat_k * Ybord + stat_text.getLocalBounds().height +
                            2 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) &&
                            cord.y <= stat_k * Ybord + stat_text.getLocalBounds().height +
                            2 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) + stat_text.getLocalBounds().height &&
                            !colors[1])
                            add_force_air();
                        else if (cord.y >= stat_k * Ybord + stat_text.getLocalBounds().height +
                            2 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) &&
                            cord.y <= stat_k * Ybord + stat_text.getLocalBounds().height +
                            2 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) + stat_text.getLocalBounds().height &&
                            colors[1])
                            remove_force("air");
                        stat_text.setString("object field");
                        if (cord.y >= stat_k * Ybord + 2 * stat_text.getLocalBounds().height +
                            3 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) &&
                            cord.y <= stat_k * Ybord + 2 * stat_text.getLocalBounds().height +
                            3 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) + stat_text.getLocalBounds().height &&
                            !colors[2])
                            colors[2] = true;
                        else if (cord.y >= stat_k * Ybord + 2 * stat_text.getLocalBounds().height +
                            3 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) &&
                            cord.y <= stat_k * Ybord + 2 * stat_text.getLocalBounds().height +
                            3 * ((1 - 2 * stat_k)*Ybord - force_size * stat_text.getLocalBounds().height) / (force_size + 1) + stat_text.getLocalBounds().height &&
                            colors[2])
                            colors[2] = false;
                        stat_text.setString("|| R");
                        stat_text.setCharacterSize(8 + 3 * Xbord / 200);
                        if (cord.x >= (1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2 && 
                            cord.x <= (1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2 + stat_text.getLocalBounds().width / 2 &&
                            cord.y >= (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2 &&
                            cord.y <= (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2 + stat_text.getLocalBounds().height &&
                            !pause)
                            pause = true;
                        else if (cord.x >= (1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2 &&
                            cord.x <= (1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2 + stat_text.getLocalBounds().width / 2 &&
                            cord.y >= (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2 &&
                            cord.y <= (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2 + stat_text.getLocalBounds().height &&
                            pause)
                            pause = false;
                        if (cord.x >= (1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2 + stat_text.getLocalBounds().width / 2 &&
                            cord.x <= (1 - stat_k)*Xbord + (stat_k*Xbord - stat_text.getLocalBounds().width) / 2 + stat_text.getLocalBounds().width &&
                            cord.y >= (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2 &&
                            cord.y <= (1 - stat_k)*Ybord + (stat_k*Ybord - stat_text.getLocalBounds().height) / 2 + stat_text.getLocalBounds().height) {
                            while (bodyes.size() != 0)
                                remove_object(0);
                        }
                    }
                }
                if (event.type == Event::MouseWheelScrolled) {
                    if (!(size_ghost == 0 && event.mouseWheelScroll.delta < 0)) {
                        if (cord.x < size_ghost || cord.y < size_ghost || cord.x + size_ghost >
                            (1-stat_k)*Xbord || cord.y + size_ghost > Ybord)
                            new_smash = true;
                        for (size_t i = 0; i < bodyes.size() - 1; i++)
                            if (distance(cord.x, cord.y, bodyes[i].x + bodyes[i].radius, bodyes[i].y + bodyes[i].radius) <
                                size_ghost + bodyes[i].radius)
                                new_smash = true;
                        if (event.mouseWheelScroll.delta > 0)
                            size_ghost += !new_smash*event.mouseWheelScroll.delta;
                        else
                            size_ghost += event.mouseWheelScroll.delta;
                        if (size_ghost < 0)
                            size_ghost = 0;
                        new_smash = false;
                    }
                }
            }
            draw();
            statistic(cord);
            act_draw();
            draw_size_line();
            draw_bond_lines();
            additional_funcs();
            get_time();
            if (Mouse().isButtonPressed(Mouse().Left) && cord.x > 0 && cord.y > 0 && cord.x < (1 - stat_k)*Xbord && new_obj) {
                points[0] = (Vertex(Vector2f(cord), Color::White));
                points[1] = (Vertex(Vector2f((float)Mouse().getPosition(window).x, (float)Mouse().getPosition(window).y), Color::White));
                bodyes[bodyes.size() - 1].x = cord.x - size_ghost;
                bodyes[bodyes.size() - 1].y = cord.y - size_ghost;
                bodyes[bodyes.size() - 1].velocity_x = (cord.x - Mouse().getPosition(window).x) / 10;
                bodyes[bodyes.size() - 1].velocity_y = -(Mouse().getPosition(window).y - cord.y) / 10;
                bodyes[bodyes.size() - 1].radius = size_ghost;
                bodyes[bodyes.size() - 1].mass = size_ghost;
                if (bodyes[bodyes.size() - 1].index_to_force != -1)
                    forces[bodyes[bodyes.size() - 1].index_to_force].G = size_ghost * size_ghost;
                window.draw(points);
                window.display();
                continue;
            }
            if (new_obj && colors[2])
                colors[2] = false;
            if (new_obj && size_ghost == 0) {
                remove_object(bodyes.size() - 1);
                stat_show = -1;
            }
            new_obj = false;
            size_ghost = 0.f;
            if (!pause) {
                move();
                seconds += time;
            }
            window.display();
        }
    }

    void add_body(const double &in_x, const double &in_y, const float &rad, const double &m, 
        const double &vel_x = 0, const double &vel_y = 0, const double &w = 0, const double &lose = 1) {
        bodyes.push_back(Body(in_x - rad, in_y - rad, rad, bodyes.size(), m, vel_x, vel_y, w, lose));
    }
    void add_force_gravity() {
        colors[0] = true;
        forces.push_back(Force("gravity", g*(to_size/new_size)*to_acel_pix()));
    }
    void add_force_air() {
        colors[1] = true;
        forces.push_back(Force("air", k_air*r_to_frame()));                  //functions which add some force
    }
    void add_potencial_static_force(const double &K, const double &x_pos, const double &y_pos, const double &radius, 
        const double &mass, const double &vel_x = 0, const double &vel_y = 0) {
        bodyes.push_back(Body(x_pos - radius, y_pos - radius, radius, bodyes.size(), mass, vel_x, vel_y, 0));
        forces.push_back(Force(K, bodyes.size() - 1, bodyes[bodyes.size() - 1], true));
        bodyes[bodyes.size() - 1].index_to_force = forces.size() - 1;
    }
    void add_potencial_force(const double &K, const double &x_pos, const double &y_pos, const double &radius, 
        const double &mass, const double &vel_x = 0, const double &vel_y = 0, const double &w = 0) {
        bodyes.push_back(Body(x_pos - radius, y_pos - radius, radius, bodyes.size(), mass, vel_x, vel_y, w));
        forces.push_back(Force(K, bodyes.size() - 1, bodyes[bodyes.size() - 1], false));
        bodyes[bodyes.size() - 1].index_to_force = forces.size() - 1;
    }
    void some_const_force(const double &x, const double &y) {
        forces.push_back(Force(x, y));
    }
    void add_some_gas(const double &K) {
        forces.push_back(Force(K*r_to_frame()));
    }
    void use_pause() {
        pause = !pause;
    }
};