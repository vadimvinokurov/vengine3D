//
// Created by Boris on 10/18/2021.
//

#ifndef VENGINE_MATRIX22_HPP
#define VENGINE_MATRIX22_HPP

namespace VE {

    class Matrix22 {
    public:
        Matrix22() {
        };

        Matrix33(float a00, float a01,
                 float a10, float a11
        ) {
            a[0][0] = a00;
            a[0][1] = a01;

            a[1][0] = a10;
            a[1][1] = a11;
        }

        Matrix22(const float initValue[2][2]) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] = initValue[i][j];
                }
            }
        }

        float &operator()(size_t row, size_t col) {
            return a[row][col];
        }

        float operator()(size_t row, size_t col) const {
            return a[row][col];
        }

        void setZero() {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] = 0.0f;
                }
            }
        }

        void setIdentity() {
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;

            a[1][0] = 0.0f;
            a[1][1] = 1.0f;
        }

        Matrix22 operator+(float v) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    res(i, j) = a[i][j] + v;
                }
            }
            return res;
        }

        Matrix22 operator-(float v) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    res(i, j) = a[i][j] - v;
                }
            }
            return res;
        }

        Matrix22 operator*(float v) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    res(i, j) = a[i][j] * v;
                }
            }
            return res;
        }

        Matrix22 operator/(float v) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    res(i, j) = a[i][j] / v;
                }
            }
            return res;
        }

        Matrix22 operator+(const Matrix22 &b) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    res(i, j) = a[i][j] + b(i, j);
                }
            }
            return res;
        }

        Matrix22 operator-(const Matrix22 &b) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    res(i, j) = a[i][j] - b(i, j);
                }
            }
            return res;
        }

        Matrix22 operator*(const Matrix22 &b) const {
            Matrix22 res;
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    for (size_t k = 0; k < 2; k++) {
                        res(i, j) += a[i][k] * b(k, j);
                    }
                }
            }
            return res;
        }

        Vector operator*(const Vector &b) const {
            Vector res;
            res.setX(a[0][0] * b.x() + a[0][1] * b.y());
            res.setY(a[1][0] * b.x() + a[1][1] * b.y());
            res.setZ(0);
            return res;
        }

        Matrix22 &operator+=(float v) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] += v;
                }
            }
            return *this;
        }

        Matrix22 &operator-=(float v) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] -= v;
                }
            }
            return *this;
        }

        Matrix22 &operator*=(float v) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] *= v;
                }
            }
            return *this;
        }

        Matrix22 &operator/=(float v) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] /= v;
                }
            }
            return *this;
        }

        Matrix22 &operator+=(const Matrix22 &b) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] += b(i, j);
                }
            }
            return *this;
        }

        Matrix22 &operator-=(const Matrix22 &b) {
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    a[i][j] -= b(i, j);
                }
            }
            return *this;
        }

        Matrix22 &operator*=(const Matrix22 &b) {
            *this = *this * b;
            return *this;
        }

        Matrix22 getTranspose() const {
            Matrix22 res;

            res(0, 0) = a[0][0];
            res(0, 1) = a[1][0];

            res(1, 0) = a[0][1];
            res(1, 1) = a[1][1];

            return res;
        }

        float getDeterminant() const {
            return a[0][0] * a[1][1] - a[1][0] * a[0][1];
        }

        Matrix22 getInverse() const {
            Matrix22 res;
            float det = getDeterminant();
            if (det == 0.0f) return res;
            float invDet = 1.0f / det;

            res(0, 0) = a[1][1];
            res(1, 0) = -a[1][0];

            res(0, 1) = -a[0][1];
            res(1, 1) = a[0][0];

            res *= invDet;

            return res;
        }

        void print() {
            std::cout << "\n";
            for (size_t i = 0; i < 2; i++) {
                for (size_t j = 0; j < 2; j++) {
                    std::cout << a[i][j] << "\t";
                }
                std::cout << "\n";
            }
        }

        virtual ~Matrix22() = default;

    private:
        float a[2][2]{};
    };
}


#endif //VENGINE_MATRIX22_HPP
