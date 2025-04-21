

# 🔐 Cryptography in C++

This is a simple C++ program that demonstrates a hybrid cryptography system combining **RSA**, **AES-like XOR encryption**, **Hex encoding**, and **Diffie-Hellman key exchange**. It allows secure message transmission with encryption, digital signatures, and key exchange simulation.

---

## 📜 Features

- **Hex Encoding/Decoding**
- **XOR-based AES-style encryption**
- **RSA Key Generation**
- **Digital Signing & Signature Verification**
- **Diffie-Hellman Key Exchange**
- **Tampering Detection**

---

## 🛠 How It Works

1. **User inputs a message.**
2. **RSA keys are generated** using two primes provided by the user.
3. **Diffie-Hellman** is used to compute a shared AES encryption key.
4. The message is:
   - 🔒 **Encrypted using XOR-based AES**
   - 🖊️ **Digitally signed using RSA private key**
5. **Decryption and signature verification** is performed.
6. The program also includes a tampering test case to verify integrity checking.

---

## 🧪 Example Flow

```txt
Enter Message: Hello Crypto!
Enter prime p : 61
Enter prime q : 53

=== RSA Key Generation Steps ===
p: 61
q: 53
n = p * q: 3233
phi(n): 3120
Public Key (e, n): (65537, 3233)
Private Key (d, n): (2753, 3233)

=== Diffie-Hellman Key Exchange ===
Public base: 5
Public modulus: 104729
Alice sends A = ...
Bob sends B = ...
Alice computes shared key: ...
Bob computes shared key: ...

=== Transaction Details ===
🔒 Encrypted Message : <ciphertext>
🖊️  Digital Signature: <signature>

=== Bob's Verification ===
🔓 Decrypted Message: Hello Crypto!
Signature Valid: Yes

=== Test Case 2: Tampered Data ===
Tampered Message: Hfllo Crypto!
Signature Valid After Tampering: No
```

---

## 🧾 Requirements

- A C++ compiler (supports C++11 and above)
- Terminal/Command-line access

---

## 🚀 Compile & Run

```bash
g++ -o crypto_demo crypto_demo.cpp
./crypto_demo
```

---

## 📌 Notes

- Primes `p` and `q` must be greater than 50.
- The AES-style encryption here uses a simple XOR cipher, not real AES — for educational/demo purposes only.
- Hash used in digital signing is a basic sum of ASCII values (not cryptographically secure).

---

