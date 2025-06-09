#include "crypto_utils.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>

namespace {
    /**
     * Получает криптографический ключ и вектор инициализации (IV) из заданного пароля.
     *
     * Эта функция использует метод OpenSSL `EVP_BytesToKey` для получения ключа и IV,
     * которые будут использоваться при шифровании/дешифровании с алгоритмом AES-256-CBC.
     *
     * @param password Строка с паролем, используемым для получения ключа и IV.
     * @param key Указатель на буфер, в который будет записан полученный ключ.
     *            Буфер должен быть достаточного размера (32 байта для AES-256).
     * @param iv Указатель на буфер, в который будет записан полученный вектор инициализации (IV).
     *           Буфер должен быть достаточного размера (16 байт для AES-256).
     * @return Возвращает true, если ключ и IV были успешно получены, иначе — false.
     */
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

/**
 * Шифрует содержимое входного файла и записывает зашифрованные данные в выходной файл.
 *
 * @param inputPath Путь к входному файлу, который нужно зашифровать.
 * @param outputPath Путь, по которому должен быть сохранён зашифрованный файл.
 * @param password Пароль для генерации ключа и вектора инициализации (IV) для шифрования.
 * @return true, если процесс шифрования прошёл успешно и выходной файл был записан; иначе — false.
 */
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

/**
 * Расшифровывает файл с использованием AES-256-CBC и ключа с IV, полученных из указанного пароля.
 *
 * Эта функция читает зашифрованный файл по указанному пути, расшифровывает его содержимое
 * и записывает результат в указанный выходной файл.
 *
 * @param inputPath Путь к зашифрованному входному файлу.
 * @param outputPath Путь для сохранения расшифрованного выходного файла.
 * @param password Пароль, используемый для получения ключа и вектора инициализации (IV) для расшифровки.
 * @return true, если файл был успешно расшифрован; иначе — false.
 *         Возможные причины ошибки: неверный пароль, ошибки ввода-вывода или повреждённые данные.
 */
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