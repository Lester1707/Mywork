#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>
#include "ANN_Lib.hpp"
//#include <windows.h>

using namespace sf;
using namespace std;

const int h_snake = 20;

const int K_x = 16*5;
const int K_y = 9*5;
const int Xsize = K_x * h_snake;
const int Ysize = K_y * h_snake;

const int K_len = 4;

const int radius = 5;

int canvas[K_y][K_x];

class Snake {
public:
    int head_x, head_y;
    int tail_x, tail_y;
    int eat_x, eat_y;
    int size;
    int size_dont_change_way;
    int way;
    vector <vector <int>> points_break;
    vector <vector <int>> directions;
    Snake() {
        size_dont_change_way = 0;
        way = 0;
        size = 0;
        for (size_t i = 0; i < K_y; i++)
            for (size_t j = 0; j < K_x; j++)
                canvas[i][j] = 0;
        for (size_t i = 0; i < K_len; i++)
            canvas[K_y / 2][(K_x - K_len) / 2 + i] = 1;
        head_x = (K_x - K_len) / 2 + K_len - 1;
        head_y = K_y / 2;
        tail_x = (K_x - K_len) / 2;
        tail_y = head_y;
        directions.push_back({ 1, 0 });
        spawn_eat();
    }
    void print_snake() {
        for (size_t i = 0; i < K_y; i++) {
            for (size_t j = 0; j < K_x; j++)
                cout << canvas[i][j] << ' ';
            cout << endl;
        }
    }
    bool move() {
        head_x += directions[directions.size() - 1][0];
        head_y -= directions[directions.size() - 1][1];
        way++;
        size_dont_change_way++;
        if (head_x > K_x - 1 || head_y > K_y - 1 || head_x < 0 || head_y < 0 || canvas[head_y][head_x] == 1)
            return false;
        if (canvas[head_y][head_x] == 2) {
            size += h_snake;
            size_dont_change_way = 0;
            spawn_eat();
        }
        else {
            canvas[tail_y][tail_x] = 0;
            if (points_break.size())
                if (points_break[0][0] == tail_x && points_break[0][1] == tail_y) {
                    directions.erase(directions.begin());
                    points_break.erase(points_break.begin());
                }
            tail_x += directions[0][0];
            tail_y -= directions[0][1];
        }
        canvas[head_y][head_x] = 1;
        return true;
    }
    bool up_key(bool &rot) {
        if (directions[directions.size() - 1][1] != 0)
            return true;
        rot = true;
        directions.push_back({ 0, 1 });
        points_break.push_back({ head_x, head_y });
        return move();
    }
    bool down_key(bool &rot) {
        if (directions[directions.size() - 1][1] != 0)
            return true;
        rot = true;
        directions.push_back({ 0, -1 });
        points_break.push_back({ head_x, head_y });
        return move();
    }
    bool left_key(bool &rot) {
        if (directions[directions.size() - 1][0] != 0)
            return true;
        rot = true;
        directions.push_back({ -1, 0 });
        points_break.push_back({ head_x, head_y });
        return move();
    }
    bool right_key(bool &rot) {
        if (directions[directions.size() - 1][0] != 0)
            return true;
        rot = true;
        directions.push_back({ 1, 0 });
        points_break.push_back({ head_x, head_y });
        return move();
    }
    void draw_canvas(RenderWindow &win) {
        for (size_t i = 0; i < K_y; i++)
            for (size_t j = 0; j < K_x; j++) {
                if (canvas[i][j] == 1) {
                    RectangleShape rectangle;
                    rectangle.setFillColor(Color::White);
                    rectangle.setSize(Vector2f((float)h_snake, (float)h_snake));
                    rectangle.setPosition(Vector2f((float)h_snake*j, (float)h_snake*i));
                    win.draw(rectangle);
                }
                if (canvas[i][j] == 2) {
                    CircleShape point;
                    point.setFillColor(Color::Red);
                    point.setRadius(radius);
                    point.setPosition(Vector2f((float)(h_snake*j + radius), (float)(h_snake*i + radius)));
                    win.draw(point);
                }
            }
    }
    void spawn_eat() {
        do {
            eat_x = rand() % K_x;
            eat_y = rand() % K_y;
        } while (canvas[eat_y][eat_x] != 0);
        canvas[eat_y][eat_x] = 2;
    }

    vector <double> to_NN() {
        int bord;
        vector <double> str;
        // str.push_back(directions[directions.size() - 1][0]);
        // str.push_back(directions[directions.size() - 1][1]);
        if (directions[directions.size() - 1][0] != -1) {
            bord = K_x - head_x - 2;
            for (int i = head_x + 1; i < K_x; i++)
                if (canvas[head_y][i] == 1)
                    bord = i - head_x - 1;
        }
        else
            bord = 0;
        str.push_back(bord);
        if (directions[directions.size() - 1][0] != 1) {
            bord = head_x - 1;
            for (int i = head_x - 1; i >= 0; i--)
                if (canvas[head_y][i] == 1)
                    bord = head_x - i - 1;
        }
        else
            bord = 0;
        str.push_back(bord);
        if (directions[directions.size() - 1][1] != 1) {
            bord = head_y - 1;
            for (int i = head_y - 1; i >= 0; i--)
                if (canvas[i][head_x] == 1)
                    bord = head_y - i - 1;
        }
        else
            bord = 0;
        str.push_back(bord);
        if (directions[directions.size() - 1][1] != -1) {
            bord = K_y - head_y - 2;
            for (int i = head_y + 1; i < K_y; i++)
                if (canvas[i][head_x] == 1)
                    bord = i - head_y - 1;
        }
        else
            bord = 0;
        str.push_back(bord);

        /*
        if (directions[directions.size() - 1][1] != 0)
            for (size_t i = 0; i <= 2 * radius_visio
            n; i++)
                for (size_t j = 0; j <= 2 * radius_vision; j++) {
                    x = head_x + (j - radius_vision)*directions[directions.size() - 1][1];
                    y = head_y + (i - 2 * radius_vision)*directions[directions.size() - 1][1];
                    if (y >= 0 && y < K_y && x >= 0 && x < K_x)
                        if (canvas[y][x] == 1 && (y != head_y || x != head_x))
                            str.push_back(-1);
                        else
                            str.push_back(canvas[y][x]);
                    else
                        str.push_back(-1);
                }
        if (directions[directions.size() - 1][0] != 0)
            for (size_t i = 0; i <= 2 * radius_vision; i++)
                for (size_t j = 0; j <= 2 * radius_vision; j++) {
                    x = head_x + (2 * radius_vision - i)*directions[directions.size() - 1][0];
                    y = head_y + (j - radius_vision)*directions[directions.size() - 1][0];
                    if (y >= 0 && y < K_y && x >= 0 && x < K_x)
                        if (canvas[y][x] == 1 && (y != head_y || x != head_x))
                            str.push_back(-1);
                        else
                            str.push_back(canvas[y][x]);
                    else
                        str.push_back(-1);
                }
        */
        str.push_back((double)(eat_x - head_x));
        str.push_back((double)(eat_y - head_y));
        // for (size_t i = 0; i < str.size(); i++)
        //     cout << str[i] << endl;
        // cout << endl;
        return str;
    }
};

void user_game() {
    RenderWindow window(VideoMode(Xsize, Ysize), "Snake");
    window.setFramerateLimit(30);
    Snake snake;
    Font font;
    Text score;
    Event event;
    font.loadFromFile("impact.ttf");
    score.setColor(Color::Green);
    score.setFont(font);
    score.setCharacterSize(12);
    score.setPosition(Vector2f(0, 0));
    score.setString("score:" + to_string(snake.size));
    bool rotate = false;
    bool snake_live = true;
    while (window.isOpen()) {
        window.clear();
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    snake_live = snake.up_key(rotate);
                if (event.key.code == Keyboard::Down)
                    snake_live = snake.down_key(rotate);
                if (event.key.code == Keyboard::Left)
                    snake_live = snake.left_key(rotate);
                if (event.key.code == Keyboard::Right)
                    snake_live = snake.right_key(rotate);
                break;
            }
        }
        if (!rotate)
            snake_live = snake.move();
        if (!snake_live) {
            cout << "LOSE!!!" << endl;
            window.close();
        }
        score.setString("score:" + to_string(snake.size));
        snake.draw_canvas(window);
        window.draw(score);
        rotate = false;
        window.display();
    }
}

bool do_rotate(Snake &snake, FeedforwardANN &NN, bool &rotate) {
    vector <double> rotatets = NN.process(snake.to_NN());
    // for(int i = 0; i < rotatets.size(); i++) {
    //     cout << rotatets[i] << endl;
    // }
    // cout << endl;
    size_t max = 0;
    for (size_t i = 1; i < rotatets.size(); i++)
        if (rotatets[i] > rotatets[max])
            max = i;
    if (max == 0)
        if (snake.directions[snake.directions.size() - 1][1] != 0)
            return snake.right_key(rotate);
    if (max == 1)
        if (snake.directions[snake.directions.size() - 1][0] != 0)
            return snake.up_key(rotate);
    if (max == 2)
        if (snake.directions[snake.directions.size() - 1][1] != 0)
            return snake.left_key(rotate);
    if (max == 3)
        if (snake.directions[snake.directions.size() - 1][0] != 0)
            return snake.down_key(rotate);
    //cout << max << "\n\n";
    // if (snake.directions[snake.directions.size() - 1][0] == 1)
    //     if (max == 0)
    //         return snake.up_key();
    //     else if (max == 2) 
    //         return snake.down_key();
    // if (snake.directions[snake.directions.size() - 1][0] == -1)
    //     if (max == 0)
    //         return snake.down_key();
    //     else if (max == 2)
    //         return snake.up_key();
    // if (snake.directions[snake.directions.size() - 1][1] == 1)
    //     if (max == 0)
    //         return snake.left_key();
    //     else if (max == 2)
    //         return snake.right_key();
    // if (snake.directions[snake.directions.size() - 1][1] == -1)
    //     if (max == 0)
    //         return snake.right_key();
    //     else if (max == 2)
    //         return snake.left_key();
    rotate = false;
    return true;
}

double II_game(FeedforwardANN &NN) {
    srand(0);
    Snake snake;
    bool rotate = false;
    bool snake_live = true;
    while (true) {
        if (snake.size_dont_change_way > 10 / 2 * K_x*K_y)
            return 0;
        snake_live = do_rotate(snake, NN, rotate);
        if (!rotate)
            snake_live = snake.move();
        if (!snake_live)
            return (double)(((double)(snake.size + 1) / h_snake) * log10(snake.way));
    }
}

double II_game_win(FeedforwardANN &NN) {
    srand(0);
    RenderWindow window(VideoMode(Xsize, Ysize), "Snake");
    window.setFramerateLimit(50);
    Snake snake;
    Font font;
    Text score;
    Event event;
    font.loadFromFile("impact.ttf");
    score.setColor(Color::Green);
    score.setFont(font);
    score.setCharacterSize(12);
    score.setPosition(Vector2f(0, 0));
    score.setString("score:" + to_string(snake.size));
    bool rotate = false;
    bool snake_live = true;
    vector <double> grid;
    while (window.isOpen()) {
        window.clear();
        if (snake.size_dont_change_way > 10 / 2 * K_x*K_y)
            return 0;
        snake_live = do_rotate(snake, NN, rotate);
        while (window.pollEvent(event))
            if (event.type == Event::Closed) {
                window.close();
                snake_live = false;
            }
        if (!rotate)
            snake_live = snake.move();
        if (!snake_live)
            return (double)(((double)(snake.size + 1) / h_snake) * log10(snake.way));
        snake.draw_canvas(window);
        score.setString("score:" + to_string(snake.size));
        window.draw(score);
        window.display();
    }
    return -1;
}

int main() {
    //FeedforwardANN NN({ 6, 10, 10, 4 });
    //NN.load("Autosave.ann");
    //FitnessRandomHillClimbing(NN, II_game, 100, 1, false);
    user_game();
     return 0;
}
