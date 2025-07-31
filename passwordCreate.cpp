#include <string>
#include <random>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <iostream>

class PasswordCreater {
public:
    static std::string create(std::string& input) {
        if (input.empty()) {
            throw std::invalid_argument("Input string cannot be empty");
        }

        // 1. 使用FNV-1a 64位哈希算法生成确定性种子
        const uint64_t seed = fnv1a_64(input);
        
        // 2. 初始化随机数生成器（确保相同种子产生相同序列）
        std::mt19937_64 rng(seed);
        
        // 3. 定义密码字符集（70个安全字符）
        const std::string charset = 
            "abcdefghijklmnopqrstuvwxyz"  // 小写字母
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  // 大写字母
            "0123456789"                  // 数字
            "!@#$%^&*";                    // 安全特殊字符
        
        // 4. 生成满足复杂度要求的12位密码
        std::string password;
        password.reserve(12);
        
        // 确保包含所有必需字符类型
        password += getCharFromSet(rng, "abcdefghijklmnopqrstuvwxyz");
        password += getCharFromSet(rng, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        password += getCharFromSet(rng, "0123456789");
        password += getCharFromSet(rng, "!@#$%^&*");
        
        // 生成剩余8个随机字符
        std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);
        for (int i = 0; i < 8; ++i) {
            password += charset[dist(rng)];
        }
        
        // 5. 随机打乱密码顺序（避免固定模式）
        std::shuffle(password.begin(), password.end(), rng);
        
        return password;
    }

private:
    // FNV-1a 64位哈希算法（确定性、跨平台）
    static uint64_t fnv1a_64(const std::string& str) {
        const uint64_t prime = 1099511628211ULL;
        const uint64_t offset_basis = 14695981039346656037ULL;
        uint64_t hash = offset_basis;
        
        for (char c : str) {
            hash ^= static_cast<uint8_t>(c);
            hash *= prime;
        }
        return hash;
    }

    // 从指定字符集获取随机字符
    static char getCharFromSet(std::mt19937_64& rng, const std::string& set) {
        std::uniform_int_distribution<size_t> dist(0, set.size() - 1);
        return set[dist(rng)];
    }
};






int main() {
    try {
    	std::string input;
		std::cout<<"input a string to make password\n";
		std::cin>>input;
		std::cout<<"password of "<<input<<" is "<<PasswordCreater::create(input)<<std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    system("pause");
    return 0;
}
