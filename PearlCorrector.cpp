#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <iomanip>

using namespace std;


// 映射表
map<string, double> aim_height = {
    {"活板门", 0.03625},
    {"阳光传感器", 0.22375},
    {"营火", 0.28625},
    {"栅栏", 0.34874999523163},
    {"灯笼", 0.41125},
    {"锁链", 0.4425},
    {"横末地烛", 0.47375},
    {"附魔台", 0.59875},
    {"箱子", 0.72375},
    {"完整方块", 0.84875},
    {"地毯", 0.91125},
    {"中继器", 0.97375}
};

double g = 0.03;
double f = static_cast<double>(0.99f);
const double one_tick_vy = 0.9603000092506409;
const double down_one_tick_vy = -1.0197000098228455;




const int start_tick = 21;
double init_y = 72.01;
double init_vy = 1.0;


vector<tuple<int, double, int>> operation;
double min_error = numeric_limits<double>::max();
const double y_error_limit = 0.006;
const double vy_error_limit = 0.2;


void backtrack(int tick, double y, double vy, const string& choice = "栅栏", double height_limit=-999);
void print_res(int tick, double y, double vy, double y_error, const vector<tuple<int, double, int>>& operation);


int main() {
    backtrack(start_tick + 1, init_y, init_vy, "栅栏", -999);
    return 0;
}

void backtrack(int tick, double y, double vy, const string& choice, double height_limit) {
    static const double g = 0.03;
    static const double f = static_cast<double>(0.99f);

    if (tick > 120) return;

    vy = (vy - g) * f;
    y += vy;

    // 剪枝条件
    if (y <= init_y || operation.size() > 5) return;


    double integer_part;
    double fractional_part = modf(y, &integer_part);

    double y_error = abs(aim_height[choice] - fractional_part);

    // 输出条件
    if (height_limit < y && y <= 115 && -0.15 <= vy && vy <= 0.23 && y_error < 0.0000003) {
        print_res(tick, y, vy, y_error, operation);
    }

    // 自然运动
    backtrack(tick + 1, y, vy, choice, height_limit);

    if (69 <= y && y <= 73) return;

    height_limit = max(height_limit, y + 3);
    // ==================================== 1 =============================================
    if (0.75 < fractional_part && fractional_part <= 1) {
        operation.emplace_back(tick, y, 1);
        backtrack(tick + 1, y + 0.51, 1, choice, height_limit);
        operation.pop_back();
    }

    if (0.5 < fractional_part && fractional_part < 0.5396999900855124) {
        operation.emplace_back(tick, y, 2);
        backtrack(tick + 2, y + one_tick_vy + 0.51, 1, choice, height_limit);
        operation.pop_back();
    }

    if (0.75 < fractional_part && fractional_part < 1) {
        operation.emplace_back(tick, y, 3);
        backtrack(tick + 2, y + 0.51 + one_tick_vy + 0.51, 1, choice, height_limit);
        operation.pop_back();
    }
    // =================================================================================

    // ==================================== 2 =============================================
    if (0.25 < fractional_part && fractional_part <= 0.5) {
        double last_y = y - vy;
        if (last_y < integer_part - 1.25 or integer_part + 0.5 < last_y) {
            operation.emplace_back(tick, y, 4);
            backtrack(tick + 1, y + 0.51, 1, choice, height_limit);
            operation.pop_back();
        }
    }

    if (0 < fractional_part && fractional_part <= 0.25) {
        double last_y = y - vy;
        if (last_y < integer_part - 1.25 or integer_part + 0.5 < last_y) {
            operation.emplace_back(tick, y, 5);
            backtrack(tick + 1, y, 1, choice, height_limit);
            operation.pop_back();
        }
    }

    // if ( (vy < -1.5 || vy > 0.75) && 0.25 < fractional_part && fractional_part < 0.5) {
    //     operation.emplace_back(tick, y, 6);
    //     backtrack(tick + 1, y + 0.51, 1, choice);
    //     operation.pop_back();
    // }
    // =================================================================================


    // ==================================== 3 =============================================
    if (0.75 < fractional_part && fractional_part < 1) {
        operation.emplace_back(tick, y, 7);
        backtrack(tick + 1, y - 0.51, -1, choice, height_limit);
        operation.pop_back();
    }

    if (-down_one_tick_vy - 0.24 < fractional_part && fractional_part < 1) {
        operation.emplace_back(tick, y, 9);
        backtrack(tick + 2, y - 0.51 + down_one_tick_vy - 0.51 , -1, choice, height_limit);
        operation.pop_back();
    }
    // =================================================================================

    // ===================================== 4 ============================================
    if (0.25 < fractional_part && fractional_part < 0.5) {
        double last_y = y - vy;
        if (last_y < integer_part + 0.5 or integer_part + 2.25 < last_y) {
            operation.emplace_back(tick, y, 10);
            backtrack(tick + 1, y - 0.51 , -1, choice, height_limit);
            operation.pop_back();
        }
    }

    if (0.5 < fractional_part && fractional_part < 0.75) {
        double last_y = y - vy;
        if (last_y < integer_part + 0.5 or integer_part + 2.25 < last_y) {
            operation.emplace_back(tick, y, 11);
            backtrack(tick + 1, y , -1, choice, height_limit);
            operation.pop_back();
        }
    }
    // =================================================================================
}

void print_res(int tick, double y, double vy, double y_error, const vector<tuple<int, double, int>>& operation) {
    cout << fixed << setprecision(8);
    cout << "Tick: " << tick
         << ", y: " << round(y * 100000000) / 100000000
         << ", vy: " << round(vy * 100000000) / 100000000
         << ", Error: " << fixed << setprecision(15) << y_error
         << " (order of magnitude: " << scientific << setprecision(3) << y_error << ")"
         << endl
         << "Operation: [" << fixed << setprecision(6);

    for (const auto& op : operation) {
        cout << "(" << get<0>(op) << ", "
             << get<1>(op) << ", "
             << get<2>(op) << "), ";
    }
    cout << "]" << endl << endl;
}