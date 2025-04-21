#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cctype>

using namespace std;

// Hex encoding
string hex_encode(const string& input) {
    const char hex_chars[] = "0123456789ABCDEF";
    string output;
    for (unsigned char c : input) {
        output += hex_chars[c >> 4];
        output += hex_chars[c & 0xF];
    }
    return output;
}

// Hex decoding
string hex_decode(const string& input) {
    string output;
    for (size_t i = 0; i < input.length(); i += 2) {
        unsigned char high = input[i];
        unsigned char low = input[i + 1];
        high = (high <= '9') ? high - '0' : toupper(high) - 'A' + 10;
        low = (low <= '9') ? low - '0' : toupper(low) - 'A' + 10;
        output += (high << 4) | low;
    }
    return output;
}

// Modular exponentiation
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2)
            result = (result * base) % mod;
        exp /= 2;
        base = (base * base) % mod;
    }
    return result;
}

// GCD
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Modular inverse
long long mod_inverse(long long a, long long m) {
    long long m0 = m, x = 1, y = 0;
    while (a > 1) {
        long long q = a / m;
        long long t = m;
        m = a % m, a = t;
        t = y, y = x - q * y, x = t;
    }
    return (x + m0) % m0;
}

// AES-like XOR + hex encoding
string aes_encrypt(const string& data, const string& key) {
    string result = data;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] ^= key[i % key.length()];
    return hex_encode(result);
}

string aes_decrypt(const string& enc, const string& key) {
    string raw = hex_decode(enc);
    for (size_t i = 0; i < raw.size(); ++i)
        raw[i] ^= key[i % key.length()];
    return raw;
}

// RSA Key Structure
struct RSAKey {
    long long n;
    long long e_or_d;
};

// RSA Key Generation
void generate_rsa_keys(long long p, long long q, RSAKey& pub, RSAKey& priv) {
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);
    long long e = 65537;
    while (gcd(e, phi) != 1) e += 2;
    long long d = mod_inverse(e, phi);

    pub = { n, e };
    priv = { n, d };

    // Display RSA steps
    cout << "\n=== RSA Key Generation Steps ===\n";
    cout << "p: " << p << "\n";
    cout << "q: " << q << "\n";
    cout << "n = p * q: " << n << "\n";
    cout << "phi(n): " << phi << "\n";
    cout << "Public Key (e, n): (" << e << ", " << n << ")\n";
    cout << "Private Key (d, n): (" << d << ", " << n << ")\n";
}

// RSA Signing
long long sign_data(const string& data, const RSAKey& priv) {
    long long hash = 0;
    for (char c : data) hash += c;
    return mod_exp(hash, priv.e_or_d, priv.n);
}

bool verify_signature(const string& data, long long sig, const RSAKey& pub) {
    long long hash = 0;
    for (char c : data) hash += c;
    long long decrypted = mod_exp(sig, pub.e_or_d, pub.n);
    return decrypted == hash;
}

// Diffie-Hellman Key Exchange
long long diffie_hellman_key_exchange(long long base, long long mod, long long a_priv, long long b_priv) {
    long long A = mod_exp(base, a_priv, mod);
    long long B = mod_exp(base, b_priv, mod);

    cout << "\n=== Diffie-Hellman Key Exchange ===\n";
    cout << "Public base: " << base << "\n";
    cout << "Public modulus: " << mod << "\n";
    cout << "Alice private key: " << a_priv << "\n";
    cout << "Bob private key: " << b_priv << "\n";
    cout << "Alice sends A = base^a mod p: " << A << "\n";
    cout << "Bob sends B = base^b mod p: " << B << "\n";

    long long shared_secret_a = mod_exp(B, a_priv, mod);
    long long shared_secret_b = mod_exp(A, b_priv, mod);

    cout << "Alice computes shared key: " << shared_secret_a << "\n";
    cout << "Bob computes shared key: " << shared_secret_b << "\n";

    return shared_secret_a;
}

// Main Program
int main() {
    string message;
    cout << "Enter Message: ";
    getline(cin >> ws, message);  // ✅ FIXED: Preserve the first character

    long long p, q;
    do {
        cout << "Enter prime p : ";
        cin >> p;
        cout << "Enter prime q : ";
        cin >> q;
        if (p <= 50 || q <= 50)
            cout << "Both p and q must be > 50. Try again.\n";
    } while (p <= 50 || q <= 50);

    // Generate RSA keys (only Alice)
    RSAKey alice_pub, alice_priv;
    generate_rsa_keys(p, q, alice_pub, alice_priv);

    // Diffie-Hellman
    long long shared_key = diffie_hellman_key_exchange(5, 104729, 1234, 5678);
    string aes_key = to_string(shared_key);

    // Encrypt message
    string encrypted = aes_encrypt(message, aes_key);
    long long signature = sign_data(message, alice_priv);

    cout << "\n=== Transaction Details ===\n";
    cout << "🔒 Encrypted Message : " << encrypted << "\n";
    cout << "🖊️  Digital Signature: " << signature << "\n";

    // Verification
    cout << "\n=== Bob's Verification ===\n";
    string decrypted = aes_decrypt(encrypted, aes_key);
    cout << "🔓 Decrypted Message: " << decrypted << "\n";
    cout << "Signature Valid: " << (verify_signature(decrypted, signature, alice_pub) ? "Yes" : "No") << "\n";

    // Tampered test
    cout << "\n=== Test Case 2: Tampered Data ===\n";
    if (!encrypted.empty()) encrypted[0] = (encrypted[0] != 'F') ? 'F' : 'E'; // tamper
    string tampered = aes_decrypt(encrypted, aes_key);
    cout << "Tampered Message: " << tampered << "\n";
    cout << "Signature Valid After Tampering: " << (verify_signature(tampered, signature, alice_pub) ? "Yes" : "No") << "\n";

    return 0;
}
