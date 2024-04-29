#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>

// 定义struct存储每个symbol对应的：
// last_timestamp：上次出现的时间戳（若第一次出现则为此次出现的时间戳）
// max_gap：目前为止的max_timestamp_gap
// total_quantity：目前为止的total_quantity
// total_pq：目前为止的price*quantity之和
// max_price：目前为止的max_price
// 不断读入input.csv的每一行时同步更新该symbol对应的struct
struct SymbolStats {
    double last_timestamp;
    double max_gap;
    double total_quantity;
    double total_pq;
    double max_price;
};



int main()
{
    std::map<std::string, SymbolStats> symbol_stats;

    // 读取input.csv
    std::ifstream file("../input.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream line_stream(line);
        std::string token;
        std::getline(line_stream, token, '\n');

        // 提取每一行中的对应元素
        std::stringstream ss(token);
        double timestamp;
        double quantity;
        double price;
        std::string symbol;
        std::string element1, element2, element3, element4;

        std::getline(ss, element1, ',');
        std::getline(ss, element2, ',');
        std::getline(ss, element3, ',');
        std::getline(ss, element4, ',');
        timestamp = std::stoi(element1);
        symbol = element2;
        quantity = std::stoi(element3);
        price = std::stoi(element4);

        if (symbol_stats.find(symbol) == symbol_stats.end()) {
            // 若该symbol为第一次出现
            symbol_stats[symbol].last_timestamp = timestamp;
            symbol_stats[symbol].max_gap = 0;
            symbol_stats[symbol].total_quantity = quantity;
            symbol_stats[symbol].total_pq = price * quantity;
            symbol_stats[symbol].max_price = price;
        } else {
            // 若该symbol此前已经出现过
            symbol_stats[symbol].max_gap = std::max(symbol_stats[symbol].max_gap,
                timestamp-symbol_stats[symbol].last_timestamp);
            symbol_stats[symbol].last_timestamp = timestamp;
            symbol_stats[symbol].total_quantity += quantity;
            symbol_stats[symbol].total_pq += price * quantity;
            symbol_stats[symbol].max_price = std::max(symbol_stats[symbol].max_price, price);
        }


    }

    // 打开输出文件
    std::ofstream outfile("../output.csv");
    if (!outfile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
    // 将结果写入输出文件
    for (const auto& entry : symbol_stats) {
        outfile << entry.first;
        outfile << ","; // separator
        outfile << entry.second.max_gap;
        outfile << ","; // separator
        outfile << entry.second.total_quantity;
        outfile << ","; // separator
        outfile << std::floor(entry.second.total_pq/entry.second.total_quantity);
        outfile << ","; // separator
        outfile << entry.second.max_price;
        outfile << std::endl; // newline
    }
    // 关闭文件
    outfile.close();
    file.close();
    return 0;
}
