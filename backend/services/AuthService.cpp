#include "AuthService.h"
#include <array>
#include <cstdint>
#include <iomanip>
#include <random>
#include <sstream>
#include <vector>

AuthService::AuthService() {
    currentUser = nullptr;
}

namespace {
constexpr int kHashIterations = 10000;
constexpr std::array<uint32_t, 64> kConstants = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

constexpr uint32_t rotateRight(uint32_t value, uint32_t bits) {
    return (value >> bits) | (value << (32 - bits));
}

std::string sha256(const std::string& input) {
    std::vector<uint8_t> data(input.begin(), input.end());
    uint64_t bitLength = static_cast<uint64_t>(data.size()) * 8;
    data.push_back(0x80);
    while (data.size() % 64 != 56) {
        data.push_back(0x00);
    }
    for (int i = 7; i >= 0; --i) {
        data.push_back(static_cast<uint8_t>(bitLength >> (i * 8)));
    }

    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    for (size_t offset = 0; offset < data.size(); offset += 64) {
        uint32_t w[64];
        for (size_t i = 0; i < 16; ++i) {
            size_t index = offset + i * 4;
            w[i] = (static_cast<uint32_t>(data[index]) << 24) |
                   (static_cast<uint32_t>(data[index + 1]) << 16) |
                   (static_cast<uint32_t>(data[index + 2]) << 8) |
                   (static_cast<uint32_t>(data[index + 3]));
        }
        for (size_t i = 16; i < 64; ++i) {
            uint32_t s0 = rotateRight(w[i - 15], 7) ^ rotateRight(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rotateRight(w[i - 2], 17) ^ rotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        for (size_t i = 0; i < 64; ++i) {
            uint32_t S1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + kConstants[i] + w[i];
            uint32_t S0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    std::ostringstream output;
    output << std::hex << std::setfill('0')
           << std::setw(8) << h0
           << std::setw(8) << h1
           << std::setw(8) << h2
           << std::setw(8) << h3
           << std::setw(8) << h4
           << std::setw(8) << h5
           << std::setw(8) << h6
           << std::setw(8) << h7;
    return output.str();
}
}

std::string AuthService::hashPassword(const std::string& pass, const std::string& salt) const {
    std::string hashed = salt + "::" + pass;
    for (int i = 0; i < kHashIterations; ++i) {
        hashed = sha256(hashed);
    }
    return hashed;
}

std::string AuthService::generateSalt(size_t length) const {
    std::random_device device;
    std::uniform_int_distribution<int> distribution(0, 255);
    std::ostringstream output;
    output << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        output << std::setw(2) << distribution(device);
    }
    return output.str();
}

bool AuthService::isRegistrationValid(const std::string& uname, const std::string& pass, const std::string& role) const {
    if (uname.empty() || pass.empty() || role.empty()) {
        return false;
    }
    if (isUsernameTaken(uname)) {
        return false;
    }
    return true;
}

bool AuthService::isUsernameTaken(const std::string& uname) const {
    for (const auto& user : users) {
        if (user.username == uname) {
            return true;
        }
    }
    return false;
}

int AuthService::getNextUserId() const {
    int maxId = 0;
    for (const auto& user : users) {
        if (user.userId > maxId) {
            maxId = user.userId;
        }
    }
    return maxId + 1;
}

bool AuthService::registerUser(int id, const std::string& uname, const std::string& pass, const std::string& role) {
    if (!isRegistrationValid(uname, pass, role)) {
        return false;
    }
    std::string salt = generateSalt();
    users.push_back(User(id, uname, hashPassword(pass, salt), role, salt));
    return true;
}

bool AuthService::registerUser(const std::string& uname, const std::string& pass, const std::string& role) {
    if (!isRegistrationValid(uname, pass, role)) {
        return false;
    }
    std::string salt = generateSalt();
    users.push_back(User(getNextUserId(), uname, hashPassword(pass, salt), role, salt));
    return true;
}

bool AuthService::login(const std::string& uname, const std::string& pass) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == uname) {
            if (users[i].password == hashPassword(pass, users[i].salt)) {
                currentUser = &users[i];
                return true;
            }
            return false;
        }
    }
    return false;
}

User* AuthService::getCurrentUser() {
    return currentUser;
}

void AuthService::logout() {
    currentUser = nullptr;
}
