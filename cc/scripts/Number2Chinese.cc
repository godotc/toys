/**
 * 
 * 、在银行、单位和个人填写的各种票据和结算凭证中，常常需要把阿拉伯数字转化为中文的大写数字。请编写一个简单的控制台程序，该程序的要求如下：
接受用户输入整型的数字
用户按下回车后，程序在屏幕输出转化为【中文大写】的数字
示例：
要转化为：壹千贰佰叁拾肆
要转化为：壹万零壹拾
附：
阿拉伯数字	中文大写数字
	零
	壹
	贰
	叁
	肆
	伍
	陆
	柒
	捌
	玖
阿拉伯数字	中文大写数字
	拾
	贰拾
	叁拾
	肆拾
	壹佰
	壹仟
	壹万
	壹亿


、概述处理思路，以及需要注意问题：

读取用户输入的数字，首先排除掉异常输入（浮点数，负数溢出了咋办）
用除法判断有多少位，分割成每位数一组。
对于每一组排布数量如，肆佰零壹，肆拾，拾
将每一组拼凑起来，从大往小位数，大位数根据量级加后缀，如小组前面是，则补一个前缀零。

需要注意，

、具体代码（写在背面）

 */


#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <algorithm>

const std::unordered_map<int, std::string> digit_map = {
    {0, "零"}, {1, "壹"}, {2, "贰"}, {3, "叁"}, {4, "肆"},
    {5, "伍"}, {6, "陆"}, {7, "柒"}, {8, "捌"}, {9, "玖"}
};
const std::vector<std::string> units = {"", "拾", "佰", "仟"};
const std::vector<std::string> big_units = {"", "万", "亿", "兆"};

std::string process_group(const std::string& group) {
    std::string result;
    bool has_zero = false;
    
    for (int i = 0; i < group.size(); ++i) {
        int num = group[i] - '0';
        if (num == 0) {
            has_zero = true;
        } else {
            // 处理前导零
            if (has_zero && !result.empty()) {
                result += "零";
            }
            result += digit_map.at(num) + units[group.size() - 1 - i];
            has_zero = false;
        }
    }
    
    return result;
}


std::string number_to_chinese(const std::string& input) 
{
    // edge condition
    if (input == "0") return "零";
    if(input == "10"){ return "拾"; }
    
    std::string_view num = input;
    while (num.size() > 1 && num[0] == '0') {
        num.remove_prefix(1);
    }
    
    std::vector<std::string> groups;
    int len = num.length();
    // reverse split
    for (int i = len; i > 0; i -= 4) {
        int start = std::max(0, i - 4);
        groups.push_back(std::string( num.substr(start, i - start)));
    }
    // 
    std::reverse(groups.begin(), groups.end());

    std::string output;
    for (size_t i = 0; i < groups.size(); ++i) 
    {

        std::string group_result = process_group(groups[i]);

        if (!group_result.empty()) {
            output += group_result + big_units[groups.size() - 1 - i];
        }
        
        if (i < groups.size() - 1 && // not last one
	    !group_result.empty() && 
	    groups[i+1] != "0000" &&
	    groups[i+1][0] == '0')  // start with zero
	{
            output += "零";
        }
    }
    
    return output;
}

int main() {
    while(1){	
	    std::string input;
	    std::cout << "请输入正整数: ";
	    std::cin >> input;

	    if (input.find_first_not_of("0123456789") != std::string::npos) {
		std::cerr << "输入包含非法字符！" << std::endl;
		return 1;
	    }


	    auto v = std::string_view(input);
	    while (v.size() > 1 && v[0] == '0') {
		v.remove_prefix(1);
	    }

	    std::cout << "转换结果: " ;

	    std::string output = number_to_chinese(std::string(v));
     	    std::cout << output; 
            std::cout << "\n";
    }
    
    return 0;
}
