#include "crypto_utils.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>

namespace {
    bool deriveKeyAndIV(const std::string& password, unsigned char* key, unsigned char* iv) {
        const EVP_CIPHER* cipher = EVP_aes_256_cbc();
        const EVP_MD* digest = EVP_sha256();

        return EVP_BytesToKey(
            cipher,
            digest,
            nullptr,
            reinterpret_cast<const unsigned char*>(password.c_str()),
            password.length(),
            1,
            key,
            iv
        ) > 0;
    }
}

bool encryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    unsigned char key[32], iv[16];
    if (!deriveKeyAndIV(password, key, iv)) return false;

    std::ifstream in(inputPath, std::ios::binary);
    std::ofstream out(outputPath, std::ios::binary);
    if (!in || !out) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    std::vector<unsigned char> buffer(4096);
    std::vector<unsigned char> outbuf(4096 + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    int outlen;

    while (in.good()) {
        in.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        std::streamsize readBytes = in.gcount();

        if (readBytes > 0) {
            EVP_EncryptUpdate(ctx, outbuf.data(), &outlen, buffer.data(), static_cast<int>(readBytes));
            out.write(reinterpret_cast<char*>(outbuf.data()), outlen);
        }
    }

    if (EVP_EncryptFinal_ex(ctx, outbuf.data(), &outlen)) {
        out.write(reinterpret_cast<char*>(outbuf.data()), outlen);
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool decryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    unsigned char key[32], iv[16];
    if (!deriveKeyAndIV(password, key, iv)) return false;

    std::ifstream in(inputPath, std::ios::binary);
    std::ofstream out(outputPath, std::ios::binary);
    if (!in || !out) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    std::vector<unsigned char> buffer(4096);
    std::vector<unsigned char> outbuf(4096 + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
    int outlen;

    while (in.good()) {
        in.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        std::streamsize readBytes = in.gcount();

        if (readBytes > 0) {
            EVP_DecryptUpdate(ctx, outbuf.data(), &outlen, buffer.data(), static_cast<int>(readBytes));
            out.write(reinterpret_cast<char*>(outbuf.data()), outlen);
        }
    }

    if (!EVP_DecryptFinal_ex(ctx, outbuf.data(), &outlen)) {
        std::cerr << "Ошибка при расшифровке (возможно, неверный пароль)\n";
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    out.write(reinterpret_cast<char*>(outbuf.data()), outlen);
    EVP_CIPHER_CTX_free(ctx);
    return true;
}