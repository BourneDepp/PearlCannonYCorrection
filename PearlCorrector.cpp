#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// 全局变量声明
double g = 0.03; // 原始Python代码中通过np.float64(0.03)创建
double f = static_cast<double>(0.99f); // 模拟np.float64(np.float32(0.99))

const double one_tick_vy = 0.9603000092506409;
const double available_gap = 1 - one_tick_vy;

// 目标高度映射表
map<string, double> aim_height = {
    {"活板门", 0.03625},
    {"阳光传感器", 0.22375},
    {"营火", 0.28625},
    {"栅栏", 0.34874999},
    {"灯笼", 0.41125},
    {"锁链", 0.4425},
    {"横末地烛", 0.47375},
    {"附魔台", 0.59875},
    {"箱子", 0.72375},
    {"完整方块", 0.84875},
    {"地毯", 0.91125},
    {"中继器", 0.97375}
};

// 初始参数
const int start_tick = 22;
double init_y = 69.01;
double init_vy = 1.0;

// 全局状态变量
vector<tuple<int, double, int>> operation;
double min_error = numeric_limits<double>::max();
const double y_error_limit = 0.006;
const double vy_error_limit = 0.2;

void backtrack(int tick, double y, double vy, const string& choice = "栅栏");

int main() {
    backtrack(start_tick + 1, init_y, init_vy, "栅栏");
    return 0;
}

void backtrack(int tick, double y, double vy, const string& choice) {
    static const double g = 0.03;
    static const double f = static_cast<double>(0.99f);

    if (tick > 130) return;

    // 物理模拟
    vy = (vy - g) * f;
    y += vy;

    // 剪枝条件
    if (y <= init_y - 3 || vy < -3 || operation.size() > 6) return;

    // 计算小数部分
    double integer_part;
    double fractional_part = modf(y, &integer_part);

    // 检查高度误差
    double y_error = abs(aim_height[choice] - fractional_part);
    if (y <= 115 && abs(vy) <= 0.35 && y_error < 0.0000002) {
        cout << fixed << setprecision(8);
        cout << "Tick: " << tick 
             << ", y: " << round(y * 100000000) / 100000000
             << ", vy: " << round(vy * 100000000) / 100000000
             << ", Error: " << y_error
             << ", Operation: [";
        for (const auto& op : operation) {
            cout << "(" << get<0>(op) << ", " 
                 << get<1>(op) << ", " 
                 << get<2>(op) << "), ";
        }
        cout << "]" << endl;
    }

    // 自然运动分支
    backtrack(tick + 1, y, vy, choice);

    // 各种操作分支
    if (0.75 < fractional_part && fractional_part <= 1) {
        operation.emplace_back(tick, y, 0);
        backtrack(tick + 1, y + 0.51, 1, choice);
        operation.pop_back();
    }

    if (0.5 < fractional_part && fractional_part < 0.5396999900855124) {
        operation.emplace_back(tick, y, 1);
        backtrack(tick + 2, y + one_tick_vy + 0.51, 1, choice);
        operation.pop_back();
    }

    if (0.75 < fractional_part && fractional_part < 1) {
        operation.emplace_back(tick, y, 2);
        backtrack(tick + 2, y + 0.51 + one_tick_vy + 0.51, 1, choice);
        operation.pop_back();
    }

    if (vy < -0.1 && 0.25 < fractional_part && fractional_part <= 0.5) {
        operation.emplace_back(tick, y, 3);
        backtrack(tick + 1, y + 0.51, 1, choice);
        operation.pop_back();
    }

    if (vy < -0.25 && 0 < fractional_part && fractional_part <= 0.25) {
        operation.emplace_back(tick, y, 4);
        backtrack(tick + 1, y, 1, choice);
        operation.pop_back();
    }

    if (0 < fractional_part && fractional_part < 0.25) {
        operation.emplace_back(tick, y, 6);
        backtrack(tick + 1, y - 0.51, -1, choice);
        operation.pop_back();
    }
}